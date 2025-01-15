#
# project definitions
#

cmake_minimum_required(VERSION 3.28)

project(transistor
    VERSION 0.0.4
    DESCRIPTION "A game to learn and create circuits"
    LANGUAGES C CXX ASM
)

include(FetchContent)
FetchContent_Declare(
        battery-embed
        GIT_REPOSITORY https://github.com/batterycenter/embed.git
        GIT_TAG        v1.2.19
)
FetchContent_MakeAvailable(battery-embed)

#
# configuration
#

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set_property(GLOBAL PROPERTY CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
set_property(GLOBAL PROPERTY LINK_WHAT_YOU_USE TRUE)

# warnings / flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(warnings -Wall -Wextra -Wformat-nonliteral -Wundef -Wshadow -Wwrite-strings -Wfloat-equal -Wswitch-default -Wmissing-format-attribute -Wswitch-enum -Wmissing-noreturn -Wno-unused-parameter -Wno-unused)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(warnings ${warnings} -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=malloc -Wsuggest-attribute=format -Wsuggest-attribute=cold)
    endif()
endif()

# try to use ccache, if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

add_compile_definitions(
        PROJECT_NAME=\"${CMAKE_PROJECT_NAME}\"
        PROJECT_VERSION=\"${CMAKE_PROJECT_VERSION}\"
        PROJECT_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
        PROJECT_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
        PROJECT_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
)

#
# libraries
#

include(FetchContent)

find_package(PkgConfig)
find_package(SDL2 REQUIRED)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(LUA REQUIRED luajit)
#pkg_search_module(LUA REQUIRED lua5.4)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

#
# executable
#

set(ENGINE_SRC
        src/lua/luaengine.cc
        src/lua/luaengine.hh
        src/engine/util/types.hh
        src/engine/geometry/direction.hh
        src/engine/geometry/position.hh
        src/engine/geometry/position.cc
        src/engine/componentdb/componentdb.hh
        src/engine/componentdb/componentdb.cc
        src/engine/componentdb/componentdef.hh
        src/engine/componentdb/components.hh
        src/engine/componentdb/components.cc
        src/engine/editor/editor.hh
        src/engine/editor/editor.cc
        src/engine/editor/board.hh
        src/engine/editor/board.cc
        src/engine/editor/component.hh
        src/engine/editor/component.cc
        src/engine/editor/wire.hh
        src/engine/editor/wiremanagement.hh
        src/engine/editor/wiremanagement.cc
        src/engine/layout/layout.hh
        src/engine/layout/pin.hh
        src/engine/connections/connection.hh
        src/engine/sandbox/sandbox.hh
        src/engine/sandbox/sandbox.cc
        src/engine/sandbox/compiler.cc
        src/engine/sandbox/compiler.hh
        src/util/visitor.hh
        src/util/exceptions.hh
        src/engine/game/game.cc
        src/engine/game/game.hh
        src/engine/game/game_commands.hh
        src/engine/game/gamefilemanager.cc
        src/engine/game/gamefilemanager.hh
)

set(UI_SRC
        src/ui/ui.hh
        src/ui/ui.cc
        src/ui/scene.hh
        src/ui/events.hh
        src/ui/resources/resourcemanager.cc
        src/ui/resources/resourcemanager.hh
        src/ui/resources/resource.hh
        src/ui/devices/deviceeditor.hh
        src/ui/devices/board/boardeditor.cc
        src/ui/devices/board/boardeditor.hh
        src/ui/devices/board/circuit_atlas.hh
        src/ui/gui/gui.cc
        src/ui/gui/gui.hh
        src/ui/ui_commands.hh
)

set(IMGUI_SRC
        contrib/imgui/imgui.cpp
        contrib/imgui/imgui_demo.cpp
        contrib/imgui/imgui_draw.cpp
        contrib/imgui/imgui_tables.cpp
        contrib/imgui/imgui_widgets.cpp
        contrib/imgui/backends/imgui_impl_sdl2.cpp
        contrib/imgui/backends/imgui_impl_sdlrenderer2.cpp
        contrib/ImGuiFileDialog/ImGuiFileDialog.h
        contrib/ImGuiFileDialog/ImGuiFileDialog.cpp
        contrib/ImGuiFileDialog/ImGuiFileDialogConfig.h
        src/engine/sandbox/recompilation.hh
)

add_executable(${CMAKE_PROJECT_NAME}
        ${ENGINE_SRC}
        ${UI_SRC}
        ${IMGUI_SRC}
        src/main.cc
)

# ignore warnings in imported files
set_source_files_properties(${IMGUI_SRC} PROPERTIES COMPILE_FLAGS "-w")

b_embed(${CMAKE_PROJECT_NAME} resources/images/circuit.png)
b_embed(${CMAKE_PROJECT_NAME} resources/images/bg.jpg)
b_embed(${CMAKE_PROJECT_NAME} resources/scripts/hello.lua)
b_embed(${CMAKE_PROJECT_NAME} resources/fonts/Pixellari.ttf)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-ggdb -O0)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(DEF B_PRODUCTION_MODE=ON)
    add_compile_options(-Ofast -flto)
endif()

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${warnings})
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PUBLIC . src contrib/imgui contrib/glad/include ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${LUA_INCLUDE_DIRS})
target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${SDL2_TTF_LIBRARY_DIRS} ${SDL2_IMAGE_LIBRARY_DIRS} ${LUA_LIBRARY_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} LINK_PUBLIC ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${LUA_LIBRARIES} nlohmann_json::nlohmann_json)

# check for leaks

add_custom_target(leaks)
add_custom_command(TARGET leaks
    POST_BUILD
    COMMENT "Check for leaks using valgrind."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${PROJECT_NAME}
    COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=${CMAKE_SOURCE_DIR}/valgrind.supp ./${PROJECT_NAME}
)

add_custom_target(leaks-test)
add_custom_command(TARGET leaks-test
    POST_BUILD
    COMMENT "Check for leaks using valgrind (tests)."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${PROJECT_NAME}-tests
    COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=${CMAKE_SOURCE_DIR}/valgrind.supp ./${PROJECT_NAME}-tests
)


add_custom_target(leaks-gen-suppressions)
add_custom_command(TARGET leaks-gen-suppressions
    POST_BUILD
    COMMENT "Check for leaks using valgrind. See valgrind-gen.supp when finished."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=all --log-file=${CMAKE_SOURCE_DIR}/valgrind-gen.supp --suppressions=${CMAKE_SOURCE_DIR}/valgrind.supp ./${PROJECT_NAME}
)

add_executable(${PROJECT_NAME}-tests
        ${ENGINE_SRC}
        tests/doctest.h
        tests/main.cc
        tests/compilation.cc
        tests/connected_wires.cc
)
target_compile_definitions(${PROJECT_NAME}-tests PUBLIC ${DEF})
target_compile_options(${PROJECT_NAME}-tests PRIVATE ${warnings})
target_include_directories(${PROJECT_NAME}-tests SYSTEM PUBLIC . src ${LUA_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME}-tests PUBLIC ${LUA_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME}-tests LINK_PUBLIC ${LUA_LIBRARIES} nlohmann_json::nlohmann_json)

b_embed(${CMAKE_PROJECT_NAME}-tests resources/scripts/hello.lua)

enable_testing()
add_test(${PROJECT_NAME}-tests ${PROJECT_NAME}-tests)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${PROJECT_NAME}-tests)

# installation

install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION bin)

# packaging

set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_SOURCE_IGNORE_FILES .git/ .github/ .idea/ _CPack_Packages/ ${CMAKE_BINARY_DIR}/ ${PROJECT_BINARY_DIR}/)
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION})
include(CPack)
