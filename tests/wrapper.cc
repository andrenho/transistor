#include <string>

#include "doctest.h"

#include <stb_ds.h>
#include <unistd.h>

extern "C" {
#include "transistor-sandbox.h"
}

#include "components.hh"

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

struct Fixture {
    Fixture(bool multithreaded)
    {
        ts_init(&t, { multithreaded, multithreaded });
        ts_component_db_add_from_lua(&t, button, -1);
        ts_component_db_add_from_lua(&t, led, -1);
        ts_component_db_add_from_lua(&t, or_2i, -1);

        // add wire
        ts_on_cursor_set_position(&t, 0, { 1, 1 });
        ts_on_cursor_key_press(&t, 0, 'w', 0);
        ts_on_cursor_set_position(&t, 0, { 3, 1 });
        ts_on_key_release(&t, 'w');

        // add components
        ts_on_cursor_set_position(&t, 0, { 1, 1 });
        ts_on_cursor_key_press(&t, 0, 'b', 0);
        ts_on_cursor_set_position(&t, 0, { 3, 1 });
        ts_on_cursor_key_press(&t, 0, 'l', 0);

        ts_run(&t, 10000);
    }

    ~Fixture()
    {
        ts_finalize(&t);
    }

    ts_Transistor t;
};

TEST_SUITE("Wrapper")
{
    TEST_CASE("Build circuit")
    {
        Fixture f(false);

        ts_TransistorSnapshot snap;
        ts_take_snapshot(&f.t, &snap);

        CHECK(snap.n_boards == 1);
        CHECK(snap.boards[0].n_components == 2);
        CHECK(snap.boards[0].n_wires == 4);

        ts_snapshot_finalize(&snap);
    }

    TEST_CASE("Single threaded")
    {
        Fixture f(false);

        ts_Component* button = f.t.sandbox.boards[0].components[0].value;
        CHECK(std::string(button->def->key) == "__button");
        CHECK(button->data[0] == 0);

        ts_on_cursor_set_position(&f.t, 0, { 1, 1 });
        ts_on_cursor_click(&f.t, 0, TS_BUTTON_LEFT);
        ts_run(&f.t, 10000);
        CHECK(button->data[0] != 0);

        ts_TransistorSnapshot snap;
        ts_take_snapshot(&f.t, &snap);
        CHECK(snap.boards[0].wires[0].value != 0);
        ts_snapshot_finalize(&snap);

        ts_on_cursor_set_position(&f.t, 0, { 1, 1 });
        ts_on_cursor_click(&f.t, 0, TS_BUTTON_LEFT);
        ts_run(&f.t, 10000);
        CHECK(button->data[0] == 0);
    }

    TEST_CASE("Multithreaded")
    {
        Fixture f(true);

        ts_lock(&f.t);
        ts_Component* button = f.t.sandbox.boards[0].components[0].value;
        CHECK(std::string(button->def->key) == "__button");
        CHECK(button->data[0] == 0);
        ts_unlock(&f.t);

        ts_on_cursor_set_position(&f.t, 0, { 1, 1 });
        ts_on_cursor_click(&f.t, 0, TS_BUTTON_LEFT);
        usleep(10000);
        ts_lock(&f.t);
        CHECK(button->data[0] != 0);
        ts_unlock(&f.t);

        ts_TransistorSnapshot snap;
        ts_take_snapshot(&f.t, &snap);
        CHECK(snap.boards[0].wires[0].value != 0);
        ts_snapshot_finalize(&snap);

        ts_on_cursor_set_position(&f.t, 0, { 1, 1 });
        ts_on_cursor_click(&f.t, 0, TS_BUTTON_LEFT);
        usleep(10000);
        ts_lock(&f.t);
        CHECK(button->data[0] == 0);
        ts_unlock(&f.t);

        usleep(10000);
    }

}
