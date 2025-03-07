#include "doctest.h"

#include <stb_ds.h>
#include <string>

extern "C" {
#include "sandbox/sandbox.h"
#include "compiler/compiler.h"
extern ts_Pin* ts_compiler_find_all_pins(ts_Sandbox const* sb);
}

#include "components.hh"

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

TEST_SUITE("Compilation")
{
    auto has_wire = [](ts_Connection* conn, ts_Position pos) {
        for (int i = 0; i < arrlen(conn->wires); ++i)
            if (ts_pos_equals(conn->wires[i], pos))
                return true;
        return false;
    };

    auto has_pin = [](ts_Connection* conn, uint8_t pin_no, std::string const& comp) {
        for (int i = 0; i < arrlen(conn->pins); ++i)
            if (pin_no == conn->pins[i].pin_no && comp == conn->pins[i].component->def->key)
                return true;
        return false;
    };

    TEST_CASE("Basic circuit")
    {
        struct CompFixture {
            ts_Sandbox sb {};

            CompFixture() {
                ts_sandbox_init(&sb);
                ts_add_lua_components(&sb);
                ts_board_add_component(&sb.boards[0], "__button", { 1, 1 }, TS_N);
                ts_board_add_component(&sb.boards[0], "__led", { 3, 1 }, TS_N);
                ts_board_add_wires(&sb.boards[0], { 1, 1 }, { 3, 1 }, TS_HORIZONTAL, { TS_WIRE_1, TS_LAYER_TOP });
            }

            ~CompFixture() {
                ts_sandbox_finalize(&sb);
            }
        };

        SUBCASE("Board elements")
        {
            CompFixture f;
            ts_Board* board = &f.sb.boards[0];

            CHECK(hmlen(board->components) == 2);
            CHECK(std::string(ts_board_component(board, { 1, 1 })->def->key) == "__button");
            CHECK(std::string(ts_board_component(board, { 3, 1 })->def->key) == "__led");

            CHECK(hmlen(board->wires) == 4);
            CHECK(ts_board_wire(board, { 1, 1, TS_E }));
            CHECK(ts_board_wire(board, { 2, 1, TS_W }));
            CHECK(ts_board_wire(board, { 2, 1, TS_E }));
            CHECK(ts_board_wire(board, { 3, 1, TS_W }));
        }

        SUBCASE("Find all pins")
        {
            CompFixture f;

            ts_Pin* pins = ts_compiler_find_all_pins(&f.sb);

            auto get = [&](ts_Position pos, uint8_t pin_no) -> ts_Component* {
                for (int i = 0; i < arrlen(pins); ++i)
                    if (ts_pos_equals(pins[i].pos, pos) && pins[i].pin_no == pin_no)
                        return pins[i].component;
                return nullptr;
            };

            CHECK(arrlen(pins) == 8);
            CHECK(std::string(get({ 1, 1, TS_N }, 0)->def->key) == "__button");
            CHECK(std::string(get({ 1, 1, TS_W }, 1)->def->key) == "__button");
            CHECK(std::string(get({ 1, 1, TS_S }, 2)->def->key) == "__button");
            CHECK(std::string(get({ 1, 1, TS_E }, 3)->def->key) == "__button");
            CHECK(std::string(get({ 3, 1, TS_N }, 0)->def->key) == "__led");
            CHECK(std::string(get({ 3, 1, TS_W }, 1)->def->key) == "__led");
            CHECK(std::string(get({ 3, 1, TS_S }, 2)->def->key) == "__led");
            CHECK(std::string(get({ 3, 1, TS_E }, 3)->def->key) == "__led");

            arrfree(pins);
        }

        SUBCASE("Compile")
        {
            CompFixture f;

            ts_Connection* connections = ts_compiler_compile(&f.sb);

            CHECK(arrlen(connections) == 1);
            CHECK(arrlen(connections[0].pins) == 2);
            CHECK(arrlen(connections[0].wires) == 4);

            CHECK(has_wire(&connections[0], { 1, 1, TS_E }));
            CHECK(has_wire(&connections[0], { 2, 1, TS_W }));
            CHECK(has_wire(&connections[0], { 2, 1, TS_E }));
            CHECK(has_wire(&connections[0], { 3, 1, TS_W }));

            CHECK(has_pin(&connections[0], 3, "__button"));
            CHECK(has_pin(&connections[0], 1, "__led"));

            for (int i = 0; i < arrlen(connections); ++i)
                ts_connection_finalize(&connections[i]);
            arrfree(connections);
        }

        SUBCASE("Multiple connections to the same component")
        {
            CompFixture f;
            ts_board_add_wires(&f.sb.boards[0], { 1, 0 }, { 1, 1 }, TS_VERTICAL, { TS_WIRE_1, TS_LAYER_TOP });

            ts_Connection* connections = ts_compiler_compile(&f.sb);
            CHECK(arrlen(connections) == 2);

            for (int i = 0; i < arrlen(connections); ++i)
                ts_connection_finalize(&connections[i]);
            arrfree(connections);
        }

        SUBCASE("Single-tile component rotation")
        {
            CompFixture f;
            ts_board_rotate_tile(&f.sb.boards[0], { 1, 1 });

            ts_Connection* connections = ts_compiler_compile(&f.sb);

            CHECK(arrlen(connections) == 1);
            CHECK(arrlen(connections[0].pins) == 2);

            CHECK(has_pin(&connections[0], 0, "__button"));
            CHECK(has_pin(&connections[0], 1, "__led"));

            for (int i = 0; i < arrlen(connections); ++i)
                ts_connection_finalize(&connections[i]);
            arrfree(connections);
        }
    }

    TEST_CASE("IC")
    {
        struct CompFixture {
            ts_Sandbox sb {};

            CompFixture(ts_Direction ic_dir) {
                ts_sandbox_init(&sb);
                ts_add_lua_components(&sb);
                ts_board_add_component(&sb.boards[0], "__button", { 1, 1 }, TS_N);
                ts_board_add_component(&sb.boards[0], "__or_2i", { 3, 1 }, ic_dir);
                ts_board_add_wires(&sb.boards[0], { 1, 1 }, { 3, 1 }, TS_HORIZONTAL, { TS_WIRE_1, TS_LAYER_TOP });
            }

            ~CompFixture() {
                ts_sandbox_finalize(&sb);
            }
        };

        SUBCASE("Compile")
        {
            CompFixture f(TS_N);

            ts_Connection* connections = ts_compiler_compile(&f.sb);

            CHECK(arrlen(connections) == 1);
            CHECK(arrlen(connections[0].pins) == 2);
            CHECK(arrlen(connections[0].wires) == 2);

            CHECK(has_wire(&connections[0], { 1, 1, TS_E }));
            CHECK(has_wire(&connections[0], { 2, 1, TS_W }));

            CHECK(has_pin(&connections[0], 3, "__button"));
            CHECK(has_pin(&connections[0], 0, "__or_2i"));

            for (int i = 0; i < arrlen(connections); ++i)
                ts_connection_finalize(&connections[i]);
            arrfree(connections);
        }

        SUBCASE("Compile rotated IC")
        {
            CompFixture f(TS_S);

            ts_Connection* connections = ts_compiler_compile(&f.sb);

            CHECK(arrlen(connections) == 1);
            CHECK(arrlen(connections[0].pins) == 2);
            CHECK(arrlen(connections[0].wires) == 2);

            CHECK(has_wire(&connections[0], { 1, 1, TS_E }));
            CHECK(has_wire(&connections[0], { 2, 1, TS_W }));

            CHECK(has_pin(&connections[0], 3, "__button"));
            CHECK(has_pin(&connections[0], 2, "__or_2i"));

            for (int i = 0; i < arrlen(connections); ++i)
                ts_connection_finalize(&connections[i]);
            arrfree(connections);
        }
    }
}
