#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stdio.h>

#define SR_INIT(s)              (vspace ? fprintf(f, "%*c" s "\n", vspace, ' ') : fprintf(f, s "\n"));
#define SR_INIT_INLINE(s)       fprintf(f, s "\n");
#define SR_ADD_INLINE(s, ...)   fprintf(f, s, ##__VA_ARGS__)
#define SR_CONT_INLINE(s, ...)  (vspace ? fprintf(f, "%*c" s, vspace, ' ', ##__VA_ARGS__) : fprintf(f, s, ##__VA_ARGS__))
#define SR_CONT(s, ...)         SR_CONT_INLINE(s, ##__VA_ARGS__); fprintf(f, "\n");

#define SR_CALL(fn, par, v)     fn(par, vspace + v, f)
#define SR_CALL_INLINE(fn, par) fn(par, 0, f)

#endif //SERIALIZE_H
