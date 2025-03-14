#ifndef SNAPSHOT_HH
#define SNAPSHOT_HH

#include <lua.hpp>
#include <vector>

#include "compilation.hh"

enum class Direction { Center=0, N=1, S=2, E=3, W=4 };

struct Snapshot {

    enum class Width { W1, W8 };
    enum class Layer { Top, Bottom };

    struct Component {
        int                  x, y;
        Direction            direction;
        std::string          key;
        ComponentId          id;
        size_t               data_size;
        std::vector<uint8_t> data;
    };

    struct Wire {
        int         x, y;
        Direction   direction;
        Width       width;
        Layer       layer;
        WireId      id;
        uint8_t     value;
    };

    struct Board {
        int                    w, h;
        std::vector<Component> components;
        std::vector<Wire>      wires;
    };

    std::vector<Board> boards;
};

Snapshot parse_snapshot(lua_State* L);
void     hydrate_snapshot_with_values(Snapshot& snapshot, CompilationResult const& result);

#endif //SNAPSHOT_HH
