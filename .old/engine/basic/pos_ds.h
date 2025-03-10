#ifndef POS_DS_H
#define POS_DS_H

#include "position.h"

//
// position set
//

typedef struct { ts_PositionHash key; bool value; } ts_PosSet;

// hashed
#define psetlen                       hmlen
#define psetput(collection, pos)      { ts_PositionHash __p = ts_pos_hash(pos); hmput((collection), __p, true); }
#define psetcontains(collection, pos) (hmgeti((collection), ts_pos_hash(pos)) >= 0)
#define psetdel(collection, pos)      hmdel((collection), ts_pos_hash(pos))
#define psetget(collection, idx)      (ts_pos_unhash(collection[idx].key))
#define psetfirst(collection)         (psetget(collection, 0))
#define psetfree                      hmfree

// non-hashed
bool arrcontains(ts_Position* haystack, ts_Position needle);

//
// position hash
//

#define POS_HASH(TYPE, ...) struct __VA_ARGS__ { ts_PositionHash key; TYPE value; }*

#define phfree               hmfree
#define phlen                hmlen
#define phput(hsh, pos, obj) hmput(hsh, ts_pos_hash(pos), obj)
#define phdel(hsh, pos)      hmdel(hsh, ts_pos_hash(pos))
#define phgeti(hsh, pos)     hmgeti(hsh, ts_pos_hash(pos))

#endif //POS_DS_H
