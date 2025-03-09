#include <pl_log.h>
#include <string>

#include "doctest.h"

#include <stb_ds.h>
#include <unistd.h>

extern "C" {
#include "transistor-sandbox.h"
}

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

struct Fixture {
    Fixture(bool multithreaded)
    {
        pl_init();
        ts_init(&t, { multithreaded, TS_CPU_NORMAL }, nullptr);
        ts_add_board(&t, 20, 10);

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

    void serialize_and_deserialize()
    {
        FILE* f = fopen("/tmp/transistor.trn", "w");
        ts_serialize_to_file(&t, f);
        fclose(f);

        ts_clear(&t);

        f = fopen("/tmp/transistor.trn", "r");
        ts_unserialize_from_file(&t, f);
        fclose(f);

        unlink("/tmp/transistor.trn");
    }

    ~Fixture()
    {
        ts_finalize(&t);
    }

    ts_Transistor t;
};

TEST_SUITE("Wrapper")
{
#if 0
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
#endif

    TEST_CASE("Single threaded - unserialize")
    {
        pl_init();
        Fixture f(false);

        ts_Component* button = ts_board_component(&f.t.sandbox.boards[0], { 1, 1 });
        CHECK(std::string(button->def->key) == "__button");

        f.serialize_and_deserialize();

        button = ts_board_component(&f.t.sandbox.boards[0], { 1, 1 });
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

    TEST_CASE("Multithreaded - unserialize")
    {
        Fixture f(true);

        ts_lock(&f.t);
        ts_Component* button = ts_board_component(&f.t.sandbox.boards[0], { 1, 1 });
        CHECK(std::string(button->def->key) == "__button");
        ts_unlock(&f.t);

        f.serialize_and_deserialize();

        ts_lock(&f.t);
        button = ts_board_component(&f.t.sandbox.boards[0], { 1, 1 });
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
