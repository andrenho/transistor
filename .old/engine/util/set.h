#ifndef SET_H
#define SET_H

#define SET(Type) struct { Type key; bool value; }

#define setlen hmlen
#define setput(collection, item) hmput(collection, item, true)
#define setdel hmdel
#define setcontains(a, b) (hmgeti(a, b) > 0)

#endif //SET_H
