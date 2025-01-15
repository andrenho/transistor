#ifndef SANDBOX_HH
#define SANDBOX_HH

#include <list>

#include "engine/board/board.hh"
#include "engine/componentdb/componentdb.hh"
#include "engine/connections/connection.hh"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Sandbox {
public:
    Sandbox();
    explicit Sandbox(json const& content);

    [[nodiscard]] ComponentDatabase& component_db() { return component_db_; }

    [[nodiscard]] std::list<Board>& boards() { return boards_; }
    [[nodiscard]] std::list<Board> const& boards() const { return boards_; }
    Board& board(size_t);

    void recompile();
    void simulate();

    [[nodiscard]] bus_data_t wire_value(Position const& pos) const;

    [[nodiscard]] json serialize() const;

    friend bool operator==(Sandbox const& lhs, Sandbox const& rhs) { return lhs.boards_ == rhs.boards_; }
    friend bool operator!=(Sandbox const& lhs, Sandbox const& rhs) { return !(lhs == rhs); }

private:
    ComponentDatabase component_db_;
    std::list<Board>  boards_;

    Connections                              connections_cache_;
    std::vector<Component*>                  components_cache_;
    std::unordered_map<Position, bus_data_t> wire_value_cache_;
};

#endif //SANDBOX_HH
