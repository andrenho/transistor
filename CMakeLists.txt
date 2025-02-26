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

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-ggdb -O0)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(DEF B_PRODUCTION_MODE=ON)
    add_compile_options(-Ofast -flto)
endif()


#
# libraries
#

add_subdirectory(contrib/pastel2d)
add_subdirectory(contrib/transistor-sandbox)

if (EMBED_SDL)
    add_subdirectory(contrib/SDL EXCLUDE_FROM_ALL)
else()
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

#
# executable
#

add_executable(transistor src/main.c)

target_link_libraries(transistor PRIVATE SDL3::SDL3 pastel2d transistor-sandbox)