#include "doctest.h"

#include <stb_ds.h>

extern "C" {
#include "sandbox/sandbox.h"
}

static bool pos_list_contains(ts_Position const* list, int list_sz, ts_Position pos)
{
    for (int i = 0; i < list_sz; ++i)
        if (ts_pos_equals(list[i], pos))
            return true;
    return false;
}

TEST_SUITE("Position & Wires")
{
    TEST_CASE("Position hashing")
    {
        ts_Position pos = { 1492, 328, TS_W };
        ts_Position pos2 = ts_pos_unhash(ts_pos_hash(pos));
        CHECK(pos.x == pos2.x);
        CHECK(pos.y == pos2.y);
        CHECK(pos.dir == pos2.dir);
    }

    TEST_CASE("Wire placement")
    {
        ts_Sandbox sb;
        ts_sandbox_init(&sb);
        ts_board_add_wire(&sb.boards[0], { 1, 1, TS_S }, { TS_WIRE_1, TS_LAYER_TOP });

        ts_Result response = ts_board_add_wire(&sb.boards[0], { 1, 11, TS_S }, { TS_WIRE_1, TS_LAYER_TOP });
        CHECK(response == TS_CANNOT_PLACE);

        CHECK(ts_board_wire(&sb.boards[0], { 1, 1, TS_S })->layer == TS_LAYER_TOP);
        CHECK(ts_board_wire(&sb.boards[0], { 1, 2, TS_S }) == NULL);

        ts_sandbox_finalize(&sb);
    }

    TEST_CASE("A to B: horizontal")
    {
        ts_Position pos[20];
        size_t sz = ts_pos_a_to_b({ 1, 1, TS_CENTER }, { 2, 3, TS_CENTER }, TS_HORIZONTAL, pos, 20);
        CHECK(sz == 6);
        CHECK(pos_list_contains(pos, 20, { 1, 1, TS_E }));
        CHECK(pos_list_contains(pos, 20, { 2, 1, TS_W }));
        CHECK(pos_list_contains(pos, 20, { 2, 1, TS_S }));
        CHECK(pos_list_contains(pos, 20, { 2, 2, TS_N }));
        CHECK(pos_list_contains(pos, 20, { 2, 2, TS_S }));
        CHECK(pos_list_contains(pos, 20, { 2, 3, TS_N }));
    }

    TEST_CASE("A to B: vertical")
    {
        ts_Position pos[20];
        size_t sz = ts_pos_a_to_b({ 1, 1, TS_CENTER }, { 2, 3, TS_CENTER }, TS_VERTICAL, pos, 20);
        CHECK(sz == 6);
        CHECK(pos_list_contains(pos, 20, { 1, 1, TS_S }));
        CHECK(pos_list_contains(pos, 20, { 1, 2, TS_N }));
        CHECK(pos_list_contains(pos, 20, { 1, 2, TS_S }));
        CHECK(pos_list_contains(pos, 20, { 1, 3, TS_N }));
        CHECK(pos_list_contains(pos, 20, { 1, 3, TS_E }));
        CHECK(pos_list_contains(pos, 20, { 2, 3, TS_W }));
    }
}
