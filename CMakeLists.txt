#
# project definitions
#

cmake_minimum_required(VERSION 3.28)

project(template-cpp-sdl-lua
    VERSION 0.0.1
    DESCRIPTION "Template for C++/SDL/Lua projects"
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

#
# libraries
#

find_package(PkgConfig)
find_package(SDL2 REQUIRED)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(LUA REQUIRED luajit)
#pkg_search_module(LUA REQUIRED lua5.4)

#
# executable
#

set(ENGINE_SRC
        src/lua/luaengine.cc
        src/lua/luaengine.hh
)

set(IMGUI_SRC
        contrib/imgui/imgui.cpp
        contrib/imgui/imgui_demo.cpp
        contrib/imgui/imgui_draw.cpp
        contrib/imgui/imgui_tables.cpp
        contrib/imgui/imgui_widgets.cpp
        contrib/imgui/backends/imgui_impl_sdl2.cpp
        contrib/imgui/backends/imgui_impl_sdlrenderer2.cpp
)

# ignore warnings in imported files
set_source_files_properties(${IMGUI_SRC} PROPERTIES COMPILE_FLAGS "-w")

add_executable(${CMAKE_PROJECT_NAME}
    ${IMGUI_SRC}
    ${ENGINE_SRC}
    src/main.cc
    src/ui/ui.hh
    src/ui/ui.cc
)

b_embed(${CMAKE_PROJECT_NAME} resources/images/face.png)
b_embed(${CMAKE_PROJECT_NAME} resources/scripts/hello.lua)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-ggdb -O0)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(DEF B_PRODUCTION_MODE=ON)
    add_compile_options(-Ofast -flto)
endif()

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC PROJECT_NAME=\"${CMAKE_PROJECT_NAME}\" ${DEF})
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${warnings})
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PUBLIC . src contrib/imgui contrib/glad/include ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${LUA_INCLUDE_DIRS})
target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${SDL2_TTF_LIBRARY_DIRS} ${SDL2_IMAGE_LIBRARY_DIRS} ${LUA_LIBRARY_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} LINK_PUBLIC ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${LUA_LIBRARIES})

# check for leaks

add_custom_target(leaks)
add_custom_command(TARGET leaks
    POST_BUILD
    COMMENT "Check for leaks using valgrind."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=${CMAKE_SOURCE_DIR}/valgrind.supp ./${PROJECT_NAME}
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
        tests/tests.cc
)
target_compile_definitions(${PROJECT_NAME}-tests PUBLIC PROJECT_NAME=\"${PROJECT_NAME}-tests\" ${DEF})
target_compile_options(${PROJECT_NAME}-tests PRIVATE ${warnings})
target_include_directories(${PROJECT_NAME}-tests SYSTEM PUBLIC . src ${LUA_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME}-tests PUBLIC ${LUA_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME}-tests LINK_PUBLIC ${LUA_LIBRARIES})

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
