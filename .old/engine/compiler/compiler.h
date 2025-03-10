#ifndef COMPILER_H
#define COMPILER_H

#include "basic/position.h"

typedef struct ts_Board ts_Board;
typedef struct ts_Component ts_Component;
typedef struct ts_Sandbox ts_Sandbox;

typedef struct ts_Pin {
    ts_Board*     board;
    ts_Component* component;
    ts_Position   pos;
    uint8_t       pin_no;
} ts_Pin;

typedef struct ts_Connection {
    ts_Pin*      pins;
    ts_Position* wires;
    uint8_t      value;
} ts_Connection;

ts_Connection* ts_compiler_compile(ts_Sandbox const* sb);
ts_Result      ts_connection_finalize(ts_Connection* connection);

#endif //COMPILER_H
