#
# setup
#

PROJECT_NAME = transistor
PROJECT_VERSION = 0.0.6

all: $(PROJECT_NAME)

#
# configuration
#

CPPFLAGS += -std=c++20 -I. -isystem contrib/imgui -isystem contrib/luaw
CPPFLAGS += -MMD -MP   # generate dependencies
CPPFLAGS += -DPROJECT_VERSION=\"$(PROJECT_VERSION)\"

LDFLAGS += -lpthread

ifdef RELEASE
	CPPFLAGS += -Ofast -flto -fdata-sections -ffunction-sections -flto -DNDEBUG -DRELEASE
	ifeq ($(CXX),g++)
		LDFLAGS += -flto -Wl,--gc-sections
	else
		LDFLAGS += -flto -Wl,-dead_strip
	endif
else
	CPPFLAGS += -ggdb -O0 -DDEV
	CPPFLAGS += -Wall -Wextra -Wformat-nonliteral -Wshadow -Wwrite-strings -Wmissing-format-attribute -Wswitch-enum -Wmissing-noreturn
	ifeq ($(CXX),g++)
		CPPFLAGS += -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=malloc -Wsuggest-attribute=format -Wsuggest-attribute=cold
	endif
	WARNINGS += -Wno-unused-parameter -Wno-unused -Wno-unknown-warning-option -Wno-sign-compare  # ignore these warnings
endif

ifdef APPLE  # SDL requirements for mac
	LDFLAGS += -framework CoreFoundation -framework AVFoundation -framework CoreMedia -framework CoreVideo \
			   -framework CoreAudio -framework AudioToolbox -framework CoreHaptics -framework GameController \
			   -framework CoreServices -framework Cocoa -framework Carbon -framework IOKit \
			   -framework ForceFeedback -framework Metal -framework MetalKit -framework OpenGL \
			   -framework QuartzCore -framework UniformTypeIdentifiers -liconv
endif

CPPFLAGS += -Wno-unknown-pragmas

TL_CONFIG = -Iengine -Iengine/decl --gen-target 5.1 --global-env-def decl

#
# object files
#

OBJ = \
	main.o \
	implementation.o \
	simulator/native.o \
	simulator/simulator.o \
	luaenv/array.o \
	luaenv/hotreload.o \
	luaenv/cache.o \
	mappers/engine.o \
	mappers/in/scenemapper.o \
	mappers/in/compiled_circuit.o \
	ui/ui.o \
	ui/gui/gui.o \
	ui/gui/dialog.o \
	ui/gui/infobox.o \
	ui/gui/menu.o \
	ui/gui/toolbox.o

ifdef RELEASE
	OBJ += luaenv/require.o
endif

CONTRIB_OBJ = \
	contrib/luaw/luaw.o #\
	contrib/imgui/imgui.o \
	contrib/imgui/imgui_demo.o \
	contrib/imgui/imgui_draw.o \
	contrib/imgui/imgui_tables.o \
	contrib/imgui/imgui_widgets.o \
	contrib/imgui/backends/imgui_impl_sdl3.o \
	contrib/imgui/backends/imgui_impl_sdlrenderer3.o

RESOURCES = \
	$(filter-out %.h, $(wildcard ui/resources/fonts/*)) \
	$(filter-out %.h, $(wildcard ui/resources/images/*))

$(OBJ): $(RESOURCES:=.h)

#
# generate rule dependencies
#

DEPENDS = $(shell find . -type f -name '*.d')
-include $(DEPENDS)

#
# generate embedded files (requires LuaJIT)
#

GENHEADER := mk/genheader.lua

%.png.h: %.png; $(GENHEADER) $^ > $@
%.jpg.h: %.jpg; $(GENHEADER) $^ > $@
%.wav.h: %.wav; $(GENHEADER) $^ > $@
%.mod.h: %.mod; $(GENHEADER) $^ > $@
%.ttf.h: %.ttf; $(GENHEADER) $^ > $@
%.txt.h: %.txt; $(GENHEADER) $^ > $@
%.bin.h: %.bin; $(GENHEADER) $^ > $@

%.lua.h: %.lua
ifdef RELEASE
	$(GENHEADER) $^ > $@ lua-strip
else
	$(GENHEADER) $^ > $@ lua
endif

.DELETE_ON_ERROR=%.h

#
# engine
#

ENGINE_SRC = $(shell find engine -type f -name '*.tl')
ENGINE_SRC_LUA = $(ENGINE_SRC:.tl=.lua)

EMBEDDED_HH = embedded.hh

%.lua: %.tl
	tl gen -c ${TL_CONFIG} $^ -o $@

ifdef RELEASE
luaenv/require.o: $(ENGINE_SRC_LUA:=.h) $(EMBEDDED_HH)   # embed Lua files into the application
endif

$(EMBEDDED_HH): luaenv/generate_embedded.lua
	luajit $^ > $@

check_tl:
	tl check ${TL_CONFIG} engine/*.tl engine/**/*.tl

#
# executable
#

# special rule to avoid warnings in contributed files

contrib/%.cc: contrib/%.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNINGS) -c

# libraries

LUAJIT_PATH = mk/LuaJIT
RAYLIB_PATH = mk/raylib
CPPFLAGS += -I$(LUAJIT_PATH)/src

libluajit.a:
	mkdir -p $(LUAJIT_PATH)
	git clone --depth=1 https://github.com/LuaJIT/LuaJIT.git $(LUAJIT_PATH) || true
	rm -rf !$/.git
	$(MAKE) -C $(LUAJIT_PATH)/src MACOSX_DEPLOYMENT_TARGET=$(MACOS_VERSION) libluajit.a
	cp $(LUAJIT_PATH)/src/libluajit.a .

libraylib.a:
	mkdir -p $(RAYLIB_PATH)
	git clone --depth=1 https://github.com/raysan5/raylib.git $(RAYLIB_PATH) || true
	rm -rf !$/.git
	$(MAKE) -C $(RAYLIB_PATH)/src MACOSX_DEPLOYMENT_TARGET=$(MACOS_VERSION)
	cp $(RAYLIB_PATH)/src/libraylib.a .

# transistor executable

$(OBJ): libluajit.a libraylib.a

$(PROJECT_NAME): $(OBJ) $(CONTRIB_OBJ) libluajit.a libraylib.a
	$(CXX) -o $@ $^ $(LDFLAGS)
ifdef RELEASE
	strip $@
endif

release:
	make RELEASE=1

#
# leaks
#

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)  # Apple
	APPLE := 1
	LEAKS_CMD := MallocStackLogging=1 leaks --atExit --
	MACOS_VERSION := $(shell cut -d '.' -f 1,2 <<< $$(sw_vers -productVersion))
	CFLAGS += -std=c2x
	CPPFLAGS += -mmacosx-version-min=$(MACOS_VERSION)
	export MACOSX_DEPLOYMENT_TARGET=$(MACOS_VERSION)
else
	CFLAGS += -std=c23
	LEAKS_CMD := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --fair-sched=yes
	LEAKS_SUPP := --suppressions=valgrind.supp
endif

leaks: $(PROJECT_NAME)
	$(LEAKS_CMD) $(LEAKS_SUPP) ./$^

helgrind: $(PROJECT_NAME)
	valgrind --tool=helgrind --fair-sched=yes ./$^

drd: $(PROJECT_NAME)
	valgrind --tool=drd --fair-sched=yes ./$^

#
# cleanup
#

.PHONY: softclean
softclean:
	rm -f $(PROJECT_NAME) $(OBJ) $(CONTRIB_OBJ) $(CLEANFILES) $(RESOURCES:=.h) $(EMBEDDED_HH) $(ENGINE_SRC_LUA:=.h) *.d **/*.d

.PHONY: clean
clean: softclean
	rm -rf $(LUAJIT_PATH) $(RAYLIB_PATH) libraylib.a libluajit.a libpastel2d-cc.a

FORCE: ;

#
# special rules
#

deepclean:
	git clean -fdx

update:
	git submodule update --init --remote --merge --recursive

compile_commands: clean
	bear -- $(MAKE)
