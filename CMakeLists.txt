#
# project definitions
#

cmake_minimum_required(VERSION 3.28)
project(transistor
        VERSION 0.1.0
        DESCRIPTION "A game to learn and create circuits"
        LANGUAGES C)

#
# configuration
#

option(EMBED_SDL "Embed SDL into executable" ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# warnings / flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(warnings -Wall -Wextra -Wformat-nonliteral -Wundef -Wshadow -Wwrite-strings -Wfloat-equal -Wswitch-default -Wmissing-format-attribute -Wswitch -Wmissing-noreturn -Wno-unused-parameter -Wno-unused)
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

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-ggdb -O0)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(GLOBAL PROPERTY LINK_WHAT_YOU_USE TRUE)
    add_compile_options(-Ofast -flto=auto)
endif()


#
# libraries
#

add_subdirectory(contrib/pastel2d)
add_subdirectory(contrib/transistor-sandbox)

if (EMBED_SDL)
    set(SDL_STATIC ON)
    set(SDL_SHARED OFF)
    set(SDL_TARGET SDL3::SDL3-static)
    add_subdirectory(contrib/SDL EXCLUDE_FROM_ALL)
else()
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
    set(SDL_TARGET SDL3::SDL3-shared)
endif()

#
# executable
#

add_executable(transistor ui/main.c
        ui/board/board.c
        ui/board/board.h
        ui/resources.c
        ui/resources.h
        ui/gui/gui.cc
        ui/gui/gui.hh
        ui/board/components.c
        ui/board/components.h)

#target_include_directories(transistor PRIVATE src)
target_link_libraries(transistor PRIVATE ${SDL_TARGET} pastel2d transistor-sandbox)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(transistor PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

# embed images
embed(transistor resources/images/bg.jpg)
embed(transistor resources/images/circuit.png)
embed(transistor resources/images/circuit.tileset.lua lua-strip)

# embed components
embed(transistor components/basic/button.lua)
embed(transistor components/basic/led.lua)
embed(transistor components/basic/npn.lua)
embed(transistor components/basic/pnp.lua)
embed(transistor components/basic/vcc.lua)
embed(transistor components/gates/or_2i.lua)

# check for leaks

add_custom_target(transistor-leaks)
if (APPLE)
    add_custom_command(TARGET transistor-leaks
            POST_BUILD
            COMMENT "Check for leaks using leaks."
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS ${PROJECT_NAME}-tests
            COMMAND MallocStackLogging=1 leaks --atExit -- ./transistor
    )
else()
    add_custom_command(TARGET transistor-leaks
            POST_BUILD
            COMMENT "Check for leaks using valgrind."
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS transistor
            COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --fair-sched=yes --suppressions=${CMAKE_SOURCE_DIR}/valgrind.supp ./transistor
    )
endif()
