#ifndef CONNECTION_HH
#define CONNECTION_HH

struct Component {
    uint8_t* data;
    uint8_t* pins;
};

struct Connection {
};

struct CompilationResult {
    std::vector<Component> components;
    std::vector<Connection> connections;
};

#endif //CONNECTION_HH
