#include "transistor.hh"

#include <array>
#include <cstdio>
#include <memory>
#include <string>
#include <stdexcept>
using namespace std::string_literals;

#define DMON_IMPL
#include "../contrib/dmon.h"

extern void setup_require(lua_State* L);
extern void setup_array(lua_State* L);

Transistor::Transistor()
{
    setup();
#ifdef DEV
    setup_autoreload();
#endif
}

Transistor::~Transistor()
{
#ifdef DEV
    dmon_deinit();
#endif
}

static void lua_execute(lua_State* L, int n_pars, int n_results=0)
{
    if (lua_pcall(L, n_pars, n_results, 0) != LUA_OK)
        throw std::runtime_error("Error executing Lua command: "s + lua_tostring(L, -1));
}

void Transistor::init()
{
    return lua_.with_lua([](lua_State* L) {
        lua_getglobal(L, "init");
        lua_execute(L, 0);
    });
}

std::pair<bool, std::string> Transistor::run_tests()
{
    return lua_.with_lua<std::pair<bool, std::string>>([](lua_State* L) -> std::pair<bool, std::string> {
        lua_getglobal(L, "run_tests");
        if (lua_pcall(L, 0, 0, 0) != LUA_OK)
            return { false, lua_tostring(L, -1) };
        return { true, "" };
    });
}

Render Transistor::render() const
{
    Render render;
    render.engine_compilation = engine_compilation_;
    lua_.with_lua([&render](lua_State* L) {
        lua_getglobal(L, "render");
        lua_execute(L, 0, 1);
        render.load_from_lua(L);
        lua_pop(L, 1);
    });
    return render;
}

//
// PRIVATE
//

void Transistor::setup()
{
#ifdef DEV
    {
        auto [success, msg] = check_engine();
        engine_compilation_.success = success;
        engine_compilation_.compilation_messages = msg;
    }
#endif

    lua_.with_lua([](lua_State* L) {
        luaL_openlibs(L);
        setup_array(L);
    #ifdef DEV
        if (luaL_dostring(L, R"(
            package.path = package.path .. ';./backend/?.lua;./engine/?.lua;./backend/engine/?.lua;./engine/?.d.tl;./backend/engine/?.d.tl'
            package.cpath = package.cpath .. ';./sim/?.so'
            serpent = require 'contrib.serpent'
            local tl = require 'contrib.tl'
            tl.loader()
        )") != LUA_OK)
            throw std::runtime_error("Error initializing tl: "s + lua_tostring(L, -1));
        luaL_dostring(L, "require 'api'");  // errors are ignored and displayed on the screen
    #else
        setup_require(L);
        if (luaL_dostring(L, "require 'api'") != LUA_OK)
            throw std::runtime_error("Error loading Transistor API lua file: "s + lua_tostring(L, -1));
    #endif
    });

    {
        auto [success, err] = run_tests();
        if (success)
            engine_compilation_.test_errors = {};
        else
            engine_compilation_.test_errors = err;
    }

    if (engine_compilation_.success)
        init();
}

void Transistor::setup_autoreload()
{
    // watch for files and restart simulation
    dmon_init();
    dmon_watch("./backend/engine",
        [](dmon_watch_id watch_id, dmon_action action, const char* dirname, const char* filename, const char* oldname, void* user) {
            if (std::string(filename).ends_with(".tl")) {
                auto self = (Transistor *) user;
                self->lua_.restart();
                // TODO - restart simulation
                self->setup();
            }
        }, DMON_WATCHFLAGS_RECURSIVE | DMON_WATCHFLAGS_FOLLOW_SYMLINKS, this);
}

std::pair<bool, std::string> Transistor::check_engine()
{
    std::array<char, 128> buffer{};
    std::string result;
    FILE* pipe = popen("make -C backend check_tl 2>&1 >/dev/null", "r");
    if (!pipe)
        return { false, "Failed to get compilation result: "s + strerror(errno) };
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    int status = pclose(pipe);
    return { WEXITSTATUS(status) == 0, result };
}
