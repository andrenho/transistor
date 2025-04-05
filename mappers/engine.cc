#include "engine.hh"

#include <cerrno>
#include <cstring>

#include <array>
#include <string>
using namespace std;

#include "luaenv/array.hh"
#include "luaenv/cache.hh"

Engine::Engine(Lua& lua)
    : lua_(lua)
{
    lua.with_lua([](lua_State* L) {
        setup_array(L);
        setup_cache(L);
    });
    setup();
}

void Engine::setup()
{
#ifdef DEV
    setup_dev();
#else
    setup_release();
#endif
    init();
}

std::optional<luaobj::CompiledCircuit> Engine::events(std::vector<luaobj::Event> const& events)
{
    return lua_.with_lua<std::optional<luaobj::CompiledCircuit>>([&](lua_State* L) {
        return luaw_call_global<std::optional<luaobj::CompiledCircuit>>(L, "do_events", events);
    });
}

luaobj::Render Engine::render(std::unordered_map<uint32_t, uint8_t> const& wires_values) const
{
    return lua_.with_lua<luaobj::Render>([&](lua_State *L) {
        return luaw_call_global<luaobj::Render>(L, "render", wires_values);
    });
}

void Engine::save_in_progress()
{
    lua_.with_lua([&](lua_State* L) {
        luaw_call_global(L, "save_in_progress");
    });
}

void Engine::init()
{
    return lua_.with_lua([this](lua_State* L) {
        luaw_call_global(L, "init");
    });
}

void Engine::setup_dev()
{
    // run compiler and check for errors
    auto [success, msg] = check_engine();
    compilation_successful_ = success;
    compilation_messages_ = msg;

    // load/reload engine from files
    lua_.with_lua([](lua_State*L) {
        if (luaL_dostring(L, R"(
            if not initialized then
                original_loaded = {}
                for k,v in pairs(package.loaded) do original_loaded[k] = true end
            else
                print("Reloading engine...")
                for k,v in pairs(package.loaded) do if not original_loaded[k] then unload_package(k) end end
            end
            package.path = package.path .. ';./backend/?.lua;./engine/?.lua;./backend/engine/?.lua;./engine/?.d.tl;./backend/engine/?.d.tl'
            serpent = require 'contrib.serpent'
            local tl = require 'contrib.tl'
            tl.loader()
            initialized = true
        )") != LUA_OK)
            throw std::runtime_error("Error initializing tl: "s + lua_tostring(L, -1));
        luaL_dostring(L, "require 'api'");  // errors are ignored and displayed on the screen
    });

    // run tests
    auto [tsuccess, err] = run_tests();
    test_errors_ = success ? std::optional<std::string> {} : err;
}

void Engine::setup_release()
{
    lua_.with_lua([](lua_State*L) {
#ifdef DEV
        auto setup_require = [](lua_State* L) {};
#endif
        setup_require(L);
        if (luaL_dostring(L, "require 'api'") != LUA_OK)
            throw std::runtime_error("Error loading Transistor API lua file: "s + lua_tostring(L, -1));
    });
}

std::pair<bool, std::string> Engine::check_engine()
{
    std::array<char, 128> buffer{};
    std::string result;
    FILE* pipe = popen("make check_tl 2>&1 >/dev/null", "r");
    if (!pipe)
        return { false, "Failed to get compilation result: "s + strerror(errno) };
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    int status = pclose(pipe);
    return { WEXITSTATUS(status) == 0, result };
}

std::pair<bool, std::string> Engine::run_tests() const
{
    return lua_.with_lua<std::pair<bool, std::string>>([](lua_State* L) -> std::pair<bool, std::string> {
        lua_getglobal(L, "run_tests");
        if (lua_pcall(L, 0, 0, 0) != LUA_OK)
            return { false, lua_tostring(L, -1) };
        return { true, "" };
    });
}
