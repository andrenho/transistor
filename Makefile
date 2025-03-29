#
# setup
#

PROJECT_NAME = transistor
PROJECT_VERSION = 0.2.1

all: $(PROJECT_NAME)

#
# configuration
#

include contrib/pastel-base/mk/config.mk

CPPFLAGS += -Wno-shadow

CPPFLAGS += -isystem contrib -isystem contrib/pastel2d/src -isystem contrib/pastel-base/pl_log -isystem contrib/pastel2d/contrib/pocketmod \
            -isystem contrib/pastel2d/contrib/stb -isystem contrib/imgui -Icontrib/SDL/include -isystem contrib/luaw
LDFLAGS += -lpthread

ifdef RELEASE
	LIB_DEPS = libSDL3.a libluajit.a
else
	CPPFLAGS += $(shell pkg-config --cflags sdl3 luajit)
	LDFLAGS += $(shell pkg-config --libs sdl3 luajit)
endif

ifdef APPLE  # SDL requirements for mac
	LDFLAGS += -framework CoreFoundation -framework AVFoundation -framework CoreMedia -framework CoreVideo \
			   -framework CoreAudio -framework AudioToolbox -framework CoreHaptics -framework GameController \
			   -framework CoreServices -framework Cocoa -framework Carbon -framework IOKit \
			   -framework ForceFeedback -framework Metal -framework MetalKit -framework OpenGL \
			   -framework QuartzCore -framework UniformTypeIdentifiers -liconv
endif

TL_CONFIG = -Iengine -Iengine/decl --gen-target 5.1 --global-env-def decl

#
# object files
#

OBJ = \
	main.o \
	implementation.o \
	luaenv/array.o \
	luaenv/hotreload.o \
	mappers/engine.o \
	ui/ui.o \
	ui/gui/gui.o

ifdef RELEASE
	OBJ += luaenv/require.o
endif

CONTRIB_OBJ = \
	contrib/luaw/luaw.o \
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

libpastel2d-cc.a:
	$(MAKE) -C contrib/pastel2d
	cp contrib/pastel2d/libpastel2d-cc.a .

# transistor executable

$(PROJECT_NAME): $(OBJ) $(CONTRIB_OBJ) libpastel2d-cc.a $(LIB_DEPS) | $(EXTRA_DEPS)
	$(CXX) -o $@ $^ $(LDFLAGS)
ifdef RELEASE
	strip $@
endif

release:
	make RELEASE=1

#
# leaks
#

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
	$(MAKE) -C contrib/pastel2d clean
	rm -rf build-sdl3 libSDL3.a libluajit.a libpastel2d-cc.a

FORCE: ;
