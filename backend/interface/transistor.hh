#ifndef TRANSISTOR_H_
#define TRANSISTOR_H_

#include "lua.hh"
#include "render.hh"

class Transistor {
public:
    Transistor();
    ~Transistor();

    void                         init();
    std::pair<bool, std::string> run_tests();

    [[nodiscard]] Render render() const;

private:
    Lua lua_;
    EngineCompilation engine_compilation_;

    void setup();
    std::pair<bool, std::string> check_engine();
    void setup_autoreload();
};

#endif
