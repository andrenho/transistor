#include <cstdio>

#include "doctest.h"

#include <stb_ds.h>
#include <string>

extern "C" {
#include "board/board.h"
#include "sandbox/sandbox.h"
#include "basic/pos_ds.h"

#include "components.hh"

extern ts_HashPosComponentPtr ts_board_component_tiles(ts_Board const* board);
}

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

TEST_SUITE("Placement")
{
    TEST_CASE("Wire placement")
    {
        SUBCASE("Don't place wire outside of circuit bounds")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_wire(&sb.boards[0], { 20, 1, TS_E }, { TS_WIRE_1, TS_LAYER_TOP });
            CHECK(ts_board_wire(&sb.boards[0], { 20, 1, TS_E }) == NULL);
            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Don't wire place on top of ICs")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_component(&sb.boards[0], "__or_2i", { 1, 1 }, TS_N);
            ts_board_add_wire(&sb.boards[0], { 2, 2, TS_N }, { TS_WIRE_1, TS_LAYER_TOP });
            CHECK(ts_board_wire(&sb.boards[0], { 2, 2, TS_N }) == NULL);
            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Remove wire")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_wires(&sb.boards[0], { 1, 1 }, { 3, 1 }, TS_HORIZONTAL, { TS_WIRE_1, TS_LAYER_TOP });
            ts_board_clear_tile(&sb.boards[0], { 2, 1 });
            CHECK(ts_board_wire(&sb.boards[0], { 1, 1, TS_E }) != NULL);
            CHECK(ts_board_wire(&sb.boards[0], { 2, 1, TS_W }) == NULL);
            CHECK(ts_board_wire(&sb.boards[0], { 2, 1, TS_E }) == NULL);
            CHECK(ts_board_wire(&sb.boards[0], { 3, 1, TS_W }) != NULL);
            ts_sandbox_finalize(&sb);
        }
    }

    TEST_CASE("Single-tile placement")
    {
        SUBCASE("Don't place outside of circuit bounds")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_component(&sb.boards[0], "__button", { 10, 1 }, TS_N);
            CHECK(ts_board_component(&sb.boards[0], { 20, 1 }) == NULL);
            ts_board_add_component(&sb.boards[0], "__or_2i", { 0, 0 }, TS_N);
            CHECK(ts_board_component(&sb.boards[0], { 0, 0 }) == NULL);
            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Don't place single-tile component on top of another single-tile component")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_component(&sb.boards[0], "__button", { 1, 1 }, TS_N);
            ts_board_add_component(&sb.boards[0], "__led", { 1, 1 }, TS_N);
            CHECK(std::string(ts_board_component(&sb.boards[0], { 1, 1 })->def->key) == "__button");
            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Don't place single-tile component on top of IC component")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_component(&sb.boards[0], "__or_2i", { 1, 1 }, TS_N);
            ts_board_add_component(&sb.boards[0], "__button", { 2, 2 }, TS_N);
            CHECK(std::string(ts_board_component(&sb.boards[0], { 2, 2 })->def->key) == "__or_2i");
            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Remove component")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_component(&sb.boards[0], "__button", { 1, 1 }, TS_N);
            ts_board_clear_tile(&sb.boards[0], { 1, 1 });
            CHECK(ts_board_component(&sb.boards[0], { 1, 1 }) == NULL);
            ts_sandbox_finalize(&sb);
        }
    }

    TEST_CASE("IC placement")
    {
        SUBCASE("Occupied tiles")
        {
            ts_PositionHash p;
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);

            ts_board_add_component(&sb.boards[0], "__button", { 0, 0 }, TS_N);
            ts_board_add_component(&sb.boards[0], "__or_2i", { 2, 2 }, TS_N);

            ts_HashPosComponentPtr tiles = ts_board_component_tiles(&sb.boards[0]);
            CHECK(hmlen(tiles) == 13);
            p = ts_pos_hash({ 0, 0 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 1, 1 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 1, 2 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 1, 3 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 1, 4 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 2, 1 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 2, 2 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 2, 3 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 2, 4 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 3, 1 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 3, 2 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 3, 3 }); CHECK(hmgeti(tiles, p) >= 0);
            p = ts_pos_hash({ 3, 4 }); CHECK(hmgeti(tiles, p) >= 0);
            hmfree(tiles);

            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Query IC in a different tile")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_component(&sb.boards[0], "__or_2i", { 2, 2 }, TS_N);
            CHECK(ts_board_component(&sb.boards[0], { 1, 1 }) != NULL);
            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Don't place outside of circuit bounds")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_component(&sb.boards[0], "__or_2i", { 11, 1 }, TS_N);
            CHECK(ts_board_component(&sb.boards[0], { 21, 1 }) == NULL);
            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Don't place any part of IC outside of circuit bounds")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_component(&sb.boards[0], "__or_2i", { 1, 9 }, TS_N);
            CHECK(ts_board_component(&sb.boards[0], { 1, 9 }) == NULL);
            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Don't place IC over single-tile component")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_component(&sb.boards[0], "__button", { 2, 2 }, TS_N);
            ts_board_add_component(&sb.boards[0], "__or_2i", { 1, 1 }, TS_N);
            CHECK(ts_board_component(&sb.boards[0], { 1, 1 }) == NULL);
            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Don't place IC over another IC")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_component(&sb.boards[0], "__or_2i", { 2, 2 }, TS_N);
            ts_board_add_component(&sb.boards[0], "__or_2i", { 1, 1 }, TS_N);
            CHECK(ts_board_component(&sb.boards[0], { 0, 0 }) == NULL);
            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Overwrite wire on placement")
        {
            ts_Position p;

            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_wires(&sb.boards[0], { 0, 1 }, { 4, 1 }, TS_HORIZONTAL, { TS_WIRE_1, TS_LAYER_TOP });
            ts_board_add_component(&sb.boards[0], "__or_2i", { 2, 1 }, TS_N);

            CHECK(hmlen(sb.boards[0].wires) == 4);
            p = { 0, 1, TS_E }; CHECK(ts_board_wire(&sb.boards[0], p));
            p = { 1, 1, TS_W }; CHECK(ts_board_wire(&sb.boards[0], p));
            p = { 3, 1, TS_E }; CHECK(ts_board_wire(&sb.boards[0], p));
            p = { 4, 1, TS_W }; CHECK(ts_board_wire(&sb.boards[0], p));

            ts_sandbox_finalize(&sb);
        }

        SUBCASE("Remove IC")
        {
            ts_Sandbox sb; ts_sandbox_init(&sb);
            ts_add_lua_components(&sb);
            ts_board_add_component(&sb.boards[0], "__or_2i", { 1, 1 }, TS_N);
            ts_board_clear_tile(&sb.boards[0], { 2, 2 });
            CHECK(ts_board_component(&sb.boards[0], { 1, 1 }) == NULL);
            CHECK(ts_board_component(&sb.boards[0], { 2, 2 }) == NULL);
            ts_sandbox_finalize(&sb);
        }
    }
}
