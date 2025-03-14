#include "board.hh"

#include <unordered_map>

#include "resources.hh"

static constexpr int BOARD_ON_TOP = 1;
static constexpr int TILE_SIZE = 16;

struct BoardDef {
    int x;
    int y;
    int z_order;
    float zoom;
};
static std::unordered_map<BoardId, BoardDef> board_defs {};
static std::optional<BoardId> board_moving {};
static std::optional<BoardId> topmost_board {};

//
// board def
//

static void check_for_new_board(Snapshot::Board const& board)
{
    auto it = board_defs.find(board.id);
    if (it == board_defs.end()) {
        board_defs[board.id] = {
            .x = ((int) board.id + 3) * 20,
            .y = ((int) board.id + 3) * 20,
            .z_order = BOARD_ON_TOP,
            .zoom = 2.f,
        };
        topmost_board = board.id;
    }
}

static void move_board(Snapshot::Board const& board, int xrel, int yrel)
{
    BoardDef& board_def = board_defs.at(*board_moving);

    int scr_w, scr_h;
    SDL_GetWindowSize(ps::graphics::window(), &scr_w, &scr_h);

    const int w = board.w * TILE_SIZE * board_def.zoom;
    const int h = board.h * TILE_SIZE * board_def.zoom;

    const int min_x = -w + 56 * board_def.zoom;
    const int min_y = -h + 56 * board_def.zoom;
    const int max_x = scr_w - 56 * board_def.zoom;
    const int max_y = scr_h - 56 * board_def.zoom;

    board_def.x = fmin(fmax(board_def.x + xrel, min_x), max_x);
    board_def.y = fmin(fmax(board_def.y + yrel, min_y), max_y);
}

//
// events
//

static Direction tile_direction(float x, float y)
{
    x = x - floor(x);
    y = y - floor(y);

    float s = 1.f - y, n = y, e = 1.f- x, w = x;
    if (n <= s && n <= e && n <= w) return Direction::N;
    if (s <= n && s <= e && s <= w) return Direction::S;
    if (e <= s && e <= n && e <= w) return Direction::E;
    return Direction::W;
}

void board_events(Engine& engine, SDL_Event const* e, Snapshot::Board const& board)
{
    if (!topmost_board)
        return;

    BoardDef const& board_def = board_defs.at(*topmost_board);

    float mx, my;
    SDL_GetMouseState(&mx, &my);
    int tile_x = (mx - board_def.x) / board_def.zoom / TILE_SIZE;
    int tile_y = (my - board_def.y) / board_def.zoom / TILE_SIZE;
    Direction tile_dir = tile_direction((mx - (float) board_def.x) / board_def.zoom / TILE_SIZE, (my - (float) board_def.y) / board_def.zoom / TILE_SIZE);

    switch (e->type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (e->button.button == SDL_BUTTON_RIGHT)
                board_moving = topmost_board;
            engine.cursor_click(*topmost_board, (Engine::Button) (e->button.button - 1));
            break;

        case SDL_EVENT_MOUSE_MOTION:
            if (board_moving >= 0)
                move_board(board, e->motion.xrel, e->motion.yrel);
            engine.cursor_set_pointer(*topmost_board, tile_x, tile_y, tile_dir);
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (e->button.button == SDL_BUTTON_RIGHT)
                board_moving = {};
            engine.cursor_button_release(*topmost_board, (Engine::Button) (e->button.button - 1));
            break;

        case SDL_EVENT_KEY_DOWN:
            if (!e->key.repeat) {
                engine.cursor_key_press(*topmost_board, (char) e->key.key);
                /*
                if (e->key.key == SDLK_D) {
                    ts_serialize_to_file(T, stdout);
                    printf("-----------------------\n");
                }
                */
            }
            break;

        case SDL_EVENT_KEY_UP:
            engine.cursor_key_release(*topmost_board, (char) e->key.key);
            break;

        default: break;
    }
}

//
// rendering
//

#define ADD_IMAGE(img, x, y, ...) scene.add_image(img, (SDL_Rect) { (int)(x) * TILE_SIZE, (int)(y) * TILE_SIZE, 0, 0 }, ##__VA_ARGS__)

static void render_board(Snapshot::Board const& board, ps::Scene& scene)
{
    ADD_IMAGE(rs_board_top_left, -2, -2);
    ADD_IMAGE(rs_board_top_right, board.w, -2);
    ADD_IMAGE(rs_board_bottom_left, -2, board.h);
    ADD_IMAGE(rs_board_bottom_right, board.w, board.h);

    for (size_t x = 0; x < board.w; ++x) {
        ADD_IMAGE(rs_board_top, x, -2);
        ADD_IMAGE(rs_board_bottom, x, board.h);
    }

    for (size_t y = 0; y < board.h; ++y) {
        ADD_IMAGE(rs_board_left, -2, y);
        ADD_IMAGE(rs_board_right, board.w, y);
    }

    for (size_t x = 0; x < board.w; ++x)
        for (size_t y = 0; y < board.h; ++y)
            ADD_IMAGE(rs_tile, x, y);
}

static void render_wires(Snapshot::Board const& board, ps::Scene& scene)
{
    for (auto const& wire: board.wires)
        ADD_IMAGE(rs_wire_top_1[(int) wire.direction][wire.value ? 1 : 0], wire.x, wire.y, { .opacity = wire.is_cursor ? .5f : 1.f });
}

#undef ADD_IMAGE

ps::Scene board_scene(Snapshot::Board const& board)
{
    check_for_new_board(board);
    BoardDef const& board_def = board_defs.at(board.id);

    ps::Scene scene;
    scene.set_z_order(board_def.z_order);
    scene.push_context(ps::Context {
        .position = { .x = (int) (board_def.x / board_def.zoom), .y = (int) (board_def.y / board_def.zoom), .w=0, .h=0 },
        .zoom = board_def.zoom,
    });
    render_board(board, scene);
    render_wires(board, scene);

    return scene;
}
