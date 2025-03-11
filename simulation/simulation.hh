#ifndef SIMULATION_HH
#define SIMULATION_HH

#include <vector>
#include <lua.h>

struct Connection {
};

class Simulation {
public:
    explicit Simulation(lua_State* L) : L(L) {}

    void start();

    void update_connections(std::vector<Connection> connections);

    void pause();
    void resume();

private:
    lua_State* L;
    std::vector<Connection> connections_;
};

#endif //SIMULATION_HH
