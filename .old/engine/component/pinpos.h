#ifndef PINPOS_H
#define PINPOS_H

#include <stdint.h>

#include "basic/position.h"

typedef struct ts_Component ts_Component;

typedef struct ts_PinPos {
    uint8_t             pin_no;
    ts_Position         pos;
} ts_PinPos;

#endif //PINPOS_H
