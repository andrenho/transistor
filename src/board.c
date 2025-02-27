#include "board.h"

#include <stdlib.h>

#include "stb_ds.h"

#include "resources.h"

typedef struct BoardDef {
    int x;
    int y;
    int z_order;
    double zoom;
} BoardDef;

static BoardDef* boards_def = NULL;
static const int BOARD_ON_TOP = 1;
static const int TILE_SIZE = 16;

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

//
// events
//

void board_update(ts_Transistor* T, ts_TransistorSnapshot const* snap, SDL_Event* e)
{

}

//
// rendering
//

#define ADD_IMAGE(img, x, y) ps_scene_add_image(scene, img, (SDL_Rect) { x * TILE_SIZE, y * TILE_SIZE }, NULL)

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

    for (size_t y = 0; y < board->w; ++y) {
        ADD_IMAGE(rs_board_left, -2, y);
        ADD_IMAGE(rs_board_right, board->w, y);
    }

    for (size_t x = 0; x < board->w; ++x)
        for (size_t y = 0; y < board->h; ++y)
            ADD_IMAGE(rs_tile, x, y);
}

#undef ADD_IMAGE

size_t board_create_scenes(ts_TransistorSnapshot const* snap, ps_Scene** scenes)
{
    *scenes = calloc(snap->n_boards, sizeof(ps_Scene));

    for (size_t i = 0; i < snap->n_boards; ++i) {
        check_for_new_board(i);

        ps_Scene* scene = &(*scenes)[i];
        ps_scene_init(scene);
        scene->z_order = boards_def[i].z_order;
        render_board(&snap->boards[i], &boards_def[i], scene);
    }

    return snap->n_boards;
}
