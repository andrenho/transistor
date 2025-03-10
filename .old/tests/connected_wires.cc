#include "doctest.h"

#include <stb_ds.h>
#include <string>


extern "C" {
#include "transistor-sandbox.h"
#include "compiler/connectedwires.h"
}

static bool contains(PositionArray array, ts_Position pos)
{
    for (int i = 0; i < arrlen(array); ++i)
        if (ts_pos_equals(array[i], pos))
            return true;
    return false;
}

TEST_SUITE("Connected wires")
{
    TEST_CASE("Simple wire")
    {
        ts_PosSet* positions = nullptr;
        ts_Position p;

        p = { 1, 1, TS_E }; psetput(positions, p);
        p = { 2, 1, TS_W }; psetput(positions, p);
        p = { 2, 1, TS_E }; psetput(positions, p);
        p = { 3, 1, TS_W }; psetput(positions, p);

        PositionArray* groups = ts_compiler_find_connected_wires(positions, nullptr);
        CHECK(arrlen(groups) == 1);

        PositionArray ps = groups[0];
        CHECK(arrlen(ps) == 4);
        CHECK(contains(ps, { 1, 1, TS_E }));
        CHECK(contains(ps, { 2, 1, TS_W }));
        CHECK(contains(ps, { 2, 1, TS_E }));
        CHECK(contains(ps, { 3, 1, TS_W }));

        for (int i = 0; i < arrlen(groups); ++i)
            arrfree(groups[i]);
        arrfree(groups);
        psetfree(positions);
    }

    TEST_CASE("Two separate wires")
    {
        ts_PosSet* positions = nullptr;
        ts_Position p;

        p = { 1, 1, TS_E }; psetput(positions, p);
        p = { 2, 1, TS_W }; psetput(positions, p);
        p = { 2, 1, TS_E }; psetput(positions, p);
        p = { 3, 1, TS_W }; psetput(positions, p);

        p = { 1, 2, TS_E }; psetput(positions, p);
        p = { 2, 2, TS_W }; psetput(positions, p);
        p = { 2, 2, TS_E }; psetput(positions, p);
        p = { 3, 2, TS_W }; psetput(positions, p);

        PositionArray* groups = ts_compiler_find_connected_wires(positions, nullptr);
        CHECK(arrlen(groups) == 2);

        PositionArray ps = groups[0];
        CHECK(arrlen(ps) == 4);
        CHECK(contains(ps, { 1, 1, TS_E }));
        CHECK(contains(ps, { 2, 1, TS_W }));
        CHECK(contains(ps, { 2, 1, TS_E }));
        CHECK(contains(ps, { 3, 1, TS_W }));

        ps = groups[1];
        CHECK(arrlen(ps) == 4);
        CHECK(contains(ps, { 1, 2, TS_E }));
        CHECK(contains(ps, { 2, 2, TS_W }));
        CHECK(contains(ps, { 2, 2, TS_E }));
        CHECK(contains(ps, { 3, 2, TS_W }));

        for (int i = 0; i < arrlen(groups); ++i)
            arrfree(groups[i]);
        arrfree(groups);
        psetfree(positions);
    }

    TEST_CASE("Crossing wires")
    {
        ts_PosSet* positions = nullptr;
        ts_Position p;

        p = { 1, 1, TS_E }; psetput(positions, p);
        p = { 2, 1, TS_W }; psetput(positions, p);
        p = { 2, 1, TS_E }; psetput(positions, p);
        p = { 3, 1, TS_W }; psetput(positions, p);

        p = { 2, 1, TS_N }; psetput(positions, p);
        p = { 2, 0, TS_S }; psetput(positions, p);
        p = { 2, 0, TS_E }; psetput(positions, p);
        p = { 3, 0, TS_W }; psetput(positions, p);

        p = { 2, 1, TS_S }; psetput(positions, p);
        p = { 2, 2, TS_N }; psetput(positions, p);
        p = { 2, 2, TS_E }; psetput(positions, p);
        p = { 3, 2, TS_W }; psetput(positions, p);

        PositionArray* groups = ts_compiler_find_connected_wires(positions, nullptr);
        CHECK(arrlen(groups) == 1);

        PositionArray ps = groups[0];
        CHECK(arrlen(ps) == 12);
        CHECK(contains(ps, { 2, 1, TS_W }));
        CHECK(contains(ps, { 2, 1, TS_E }));
        CHECK(contains(ps, { 2, 1, TS_N }));
        CHECK(contains(ps, { 2, 1, TS_S }));

        for (int i = 0; i < arrlen(groups); ++i)
            arrfree(groups[i]);
        arrfree(groups);
        psetfree(positions);
    }

    TEST_CASE("Single-tile component in middle of connection")
    {
        ts_PosSet* positions = nullptr;
        ts_Position p;

        p = { 1, 1, TS_W }; psetput(positions, p);
        p = { 1, 1, TS_S }; psetput(positions, p);

        ts_Position* single_tile_pins = nullptr;
        p = { 1, 1, TS_CENTER }; arrpush(single_tile_pins, p);

        PositionArray* groups = ts_compiler_find_connected_wires(positions, single_tile_pins);
        CHECK(arrlen(groups) == 2);

        PositionArray ps = groups[0];
        CHECK(arrlen(ps) == 1);
        CHECK(contains(ps, { 1, 1, TS_W }));

        ps = groups[1];
        CHECK(arrlen(ps) == 1);
        CHECK(contains(ps, { 1, 1, TS_S }));

        arrfree(single_tile_pins);
        for (int i = 0; i < arrlen(groups); ++i)
            arrfree(groups[i]);
        arrfree(groups);
        psetfree(positions);
    }
}