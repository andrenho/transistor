#include "simulation.hh"

void Simulation::start()
{

}

void Simulation::update_connections(std::vector<Connection> connections)
{
    pause();
    connections_ = std::move(connections);
    resume();
}

void Simulation::pause()
{

}

void Simulation::resume()
{

}