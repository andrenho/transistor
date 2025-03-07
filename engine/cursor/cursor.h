#ifndef CURSOR_H
#define CURSOR_H
#include "util/result.h"

#include <stdbool.h>
#include <stdint.h>

#include "basic/position.h"
#include "board/wire.h"
#include "component/component.h"

typedef struct ts_Board ts_Board;

typedef struct ts_Cursor {
    ts_Board*              board;
    bool                   in_bounds;
    ts_Position            pos;
    ts_ComponentDef const* selected_def;
    ts_Direction           selected_direction;
    ts_Wire                selected_wire;
    struct {
        bool                 drawing;
        ts_Position          starting_pos;
        bool                 orientation_defined;
        ts_Orientation       orientation;
    }                      wire;
    bool                   erasing;
} ts_Cursor;

typedef enum ts_CursorButton {
    TS_BUTTON_LEFT, TS_BUTTON_MIDDLE, TS_BUTTON_RIGHT
} ts_CursorButton;

// initialization
ts_Result ts_cursor_init(ts_Cursor* cursor, ts_Board* board);
ts_Result ts_cursor_finalize(ts_Cursor* cursor);

// user actions
ts_Result ts_cursor_set_pointer(ts_Cursor* cursor, ts_Position pos);
ts_Result ts_cursor_set_pointer_out_of_bounds(ts_Cursor* cursor);
ts_Result ts_cursor_click(ts_Cursor* cursor, ts_CursorButton button);
ts_Result ts_cursor_release(ts_Cursor* cursor, uint8_t button);
ts_Result ts_cursor_key_press(ts_Cursor* cursor, char key, uint8_t keymod);
ts_Result ts_cursor_key_release(ts_Cursor* cursor, char key);
ts_Result ts_cursor_select_component_def(ts_Cursor* cursor, const char* name);

#endif //CURSOR_H
