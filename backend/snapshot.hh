#ifndef SNAPSHOT_HH
#define SNAPSHOT_HH

#include <lua.hpp>
#include <vector>

struct Snapshot {

    enum class Direction { N, S, E, W, Center };

    enum class Width { W1, W8 };
    enum class Layer { Top, Bottom };

    struct Component {
        int         x, y;
        Direction   direction;
        std::string key;
    };

    struct Wire {
        int         x, y;
        Direction   direction;
        Width       width;
        Layer       layer;
    };

    struct Board {
        int                    w, h;
        std::vector<Component> components;
        std::vector<Wire>      wire;
    };

    std::vector<Board> boards;
};

Snapshot parse_snapshot(lua_State* L);

#endif //SNAPSHOT_HH
