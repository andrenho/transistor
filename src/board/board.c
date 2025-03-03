#include "board.h"

#include <stdlib.h>

#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_ds.h>

#include "components.h"
#include "../resources.h"

static const int BOARD_ON_TOP = 1;

typedef struct BoardDef {
    int x;
    int y;
    int z_order;
    double zoom;
} BoardDef;

static BoardDef* boards_def = NULL;
static int board_moving = -1;

//
// board ordering / positioning
//

static void check_for_new_board(size_t i)
{
    while (i >= arrlen(boards_def)) {
        BoardDef def = { .x = (i + 3) * 20, .y = (i + 3) * 20, .z_order = BOARD_ON_TOP, .zoom = 2.0 };
        arrpush(boards_def, def);
    }
}

static int topmost_board()
{
    return 0;  // TODO
}

static int board_at_pos(ts_TransistorSnapshot const* snap, int x, int y)
{
    return 0;  // TODO
}

static void bring_board_to_top(int i)
{
    // TODO
}

static void move_board(ts_TransistorSnapshot const* snap, int xrel, int yrel)
{
    int scr_w, scr_h;
    SDL_GetWindowSize(ps_graphics_window(), &scr_w, &scr_h);

    const double zoom = boards_def[board_moving].zoom;
    const int w = snap->boards[board_moving].w * TILE_SIZE * zoom;
    const int h = snap->boards[board_moving].h * TILE_SIZE * zoom;

    const int min_x = -w + 56 * zoom;
    const int min_y = -h + 56 * zoom;
    const int max_x = scr_w - 56 * zoom;
    const int max_y = scr_h - 56 * zoom;

    boards_def[board_moving].x = fmin(fmax(boards_def[board_moving].x + xrel, min_x), max_x);
    boards_def[board_moving].y = fmin(fmax(boards_def[board_moving].y + yrel, min_y), max_y);
}

//
// events
//

void board_update(ts_Transistor* T, ts_TransistorSnapshot const* snap, SDL_Event* e)
{
    int i = topmost_board();
    check_for_new_board(i);

    float mx, my;
    SDL_GetMouseState(&mx, &my);
    int tile_x = (mx - boards_def[i].x) / boards_def[i].zoom / TILE_SIZE;
    int tile_y = (my - boards_def[i].y) / boards_def[i].zoom / TILE_SIZE;
    // TODO - what is the direction?

    switch (e->type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (e->button.button == SDL_BUTTON_RIGHT)
                board_moving = topmost_board();
            ts_transistor_cursor_click(T, i, e->button.button - 1);
            break;

        case SDL_EVENT_MOUSE_MOTION:
            if (board_moving >= 0)
                move_board(snap, e->motion.xrel, e->motion.yrel);
            ts_transistor_cursor_set_pointer(T, i, (ts_Position) { tile_x, tile_y });
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (e->button.button == SDL_BUTTON_RIGHT)
                board_moving = -1;
            ts_transistor_cursor_release(T, e->button.button - 1);
            break;

        case SDL_EVENT_KEY_DOWN:
            if (!e->key.repeat) {
                ts_transistor_cursor_key_press(T, i, e->key.key, 0);
                if (e->key.key == SDLK_D) {
                    ts_transistor_serialize_to_file(T, stdout);
                    printf("-----------------------\n");
                }
            }
            break;

        case SDL_EVENT_KEY_UP:
            ts_transistor_cursor_key_release(T, (char) e->key.key);
            break;

        default: break;
    }
}

//
// rendering
//

#define ADD_IMAGE(img, x, y, ...) ps_scene_add_image_with(scene, img, (SDL_Rect) { x * TILE_SIZE, y * TILE_SIZE }, ##__VA_ARGS__, CTX_END)

static void render_board(ts_BoardSnapshot const* board, BoardDef const* board_def, ps_Scene* scene)
{
    ps_scene_push_context(scene, ps_create_context_with(CTX_ZOOM, boards_def->zoom,
        CTX_POSITION, (SDL_Rect) { boards_def->x / boards_def->zoom, boards_def->y / boards_def->zoom }, CTX_END));

    ADD_IMAGE(rs_board_top_left, -2, -2);
    ADD_IMAGE(rs_board_top_right, board->w, -2);
    ADD_IMAGE(rs_board_bottom_left, -2, board->h);
    ADD_IMAGE(rs_board_bottom_right, board->w, board->h);

    for (size_t x = 0; x < board->w; ++x) {
        ADD_IMAGE(rs_board_top, x, -2);
        ADD_IMAGE(rs_board_bottom, x, board->h);
    }

    for (size_t y = 0; y < board->h; ++y) {
        ADD_IMAGE(rs_board_left, -2, y);
        ADD_IMAGE(rs_board_right, board->w, y);
    }

    for (size_t x = 0; x < board->w; ++x)
        for (size_t y = 0; y < board->h; ++y)
            ADD_IMAGE(rs_tile, x, y);
}

static void render_wires(ts_WireSnapshot const* wire, BoardDef* board_def, ps_Scene* scene)
{
    ADD_IMAGE(rs_wire_top_1[wire->pos.dir][wire->value ? 1 : 0], wire->pos.x, wire->pos.y, CTX_OPACITY, wire->cursor ? .5f : 1.f);
}

static void render_component(ts_Transistor const* T, ts_ComponentSnapshot const* component, BoardDef* board_def, ps_Scene* scene)
{
    component_render(T, component, scene);
}

#undef ADD_IMAGE

size_t board_create_scenes(ts_Transistor const* T, ts_TransistorSnapshot const* snap, ps_Scene* scenes, size_t n_scenes)
{
    for (size_t i = 0; i < snap->n_boards; ++i) {
        check_for_new_board(i);

        ps_Scene* scene = &scenes[n_scenes++];
        ps_scene_init(scene);
        scene->z_order = boards_def[i].z_order;

        render_board(&snap->boards[i], &boards_def[i], scene);

        for (size_t j = 0; j < snap->boards[i].n_wires; ++j)
            render_wires(&snap->boards[i].wires[j], &boards_def[i], scene);
        for (size_t j = 0; j < snap->boards[i].n_components; ++j)
            render_component(T, &snap->boards[i].components[j], &boards_def[i], scene);
    }

    return n_scenes;
}
