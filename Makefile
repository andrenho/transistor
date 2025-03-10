#
# setup
#

PROJECT_NAME = transistor
PROJECT_VERSION = 0.1.1

all: $(PROJECT_NAME) $(PROJECT_NAME)-tests

#
# configuration
#

include contrib/pastel-base/mk/config.mk

CPPFLAGS += -I. -Iengine -Iresources/fonts -Iresources/images \
			-Icontrib/pastel2d/src -Icontrib/pastel-base/pl_log -Icontrib/pastel2d/contrib/pocketmod \
			-isystem contrib/imgui -isystem contrib/doctest \
            -isystem contrib/pastel2d/contrib/stb

ifdef RELEASE
	EMBED_LIBS ?= 1    # determine if libraries are embedded of linked
endif

ifdef EMBED_LIBS
	LIB_DEPS = libSDL3.a libluajit.a
else
	CPPFLAGS += $(shell pkg-config --cflags sdl3 luajit)
	LDFLAGS += $(shell pkg-config --libs sdl3 luajit)
endif

ifdef APPLE  # SDL requirements for mac
	LDFLAGS += -framework AVFoundation -framework CoreMedia -framework CoreVideo \
			   -framework CoreAudio -framework AudioToolbox -framework CoreHaptics -framework GameController \
			   -framework CoreServices -framework Cocoa -framework Carbon -framework IOKit \
			   -framework ForceFeedback -framework Metal -framework MetalKit -framework OpenGL \
			   -framework QuartzCore -framework UniformTypeIdentifiers -liconv
endif

#
# object files
#

ENGINE_OBJ = \
	engine/transistor-sandbox.o \
	engine/basic/pos_ds.o \
	engine/basic/rect.o \
	engine/basic/position.o \
	engine/basic/direction.o \
	engine/board/board.o \
	engine/board/wire.o \
	engine/compiler/compiler.o \
	engine/compiler/connectedwires.o \
	engine/component/componentdb.o \
	engine/component/component.o \
	engine/component/componentdef.o \
	engine/component/component_mt.o \
	engine/component/component.o \
	engine/component/included.o \
	engine/cursor/cursor.o \
	engine/sandbox/sandbox.o \
	engine/simulation/simulation.o \
	engine/simulation/componentsim.o

UI_OBJ = \
	ui/main.o \
	ui/common.o \
	ui/resources.o \
	ui/gui/gui.o \
	ui/gui/dialog.o \
	ui/gui/mainmenu.o \
	ui/gui/toolbox.o \
	ui/board/board.o \
	ui/board/components.o

IMGUI_OBJ = \
	contrib/imgui/imgui.o \
	contrib/imgui/imgui_demo.o \
	contrib/imgui/imgui_draw.o \
	contrib/imgui/imgui_tables.o \
	contrib/imgui/imgui_widgets.o \
	contrib/imgui/backends/imgui_impl_sdl3.o \
	contrib/imgui/backends/imgui_impl_sdlrenderer3.o

TEST_OBJ = \
	tests/_implementation.o \
	tests/wrapper.o \
	tests/position.o \
	tests/pinpositions.o \
	tests/connected_wires.o \
	tests/placement.o \
	tests/compilation.o \
	tests/serialization.o \
	tests/simulation.o \
	tests/custom_ic.o \
	tests/componentdb.o

CIRCUITS = \
	$(filter-out %.h, $(wildcard components/basic/*)) \
	$(filter-out %.h, $(wildcard components/gates/*))

EMBED = \
	$(filter-out %.h, $(wildcard resources/fonts/*)) \
	$(filter-out %.h, $(wildcard resources/images/*))

$(ENGINE_OBJ): $(CIRCUITS:=.h)

$(UI_OBJ): $(EMBED:=.h)

#
# executable
#

# special rule to avoid warnings in contributed files

contrib/imgui/%.cc: contrib/imgui/%.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNINGS) -c

# pastel2d library

libpastel2d.a:
	$(MAKE) -C contrib/pastel2d
	cp contrib/pastel2d/libpastel2d.a .

# transistor executable

$(PROJECT_NAME): $(UI_OBJ) $(ENGINE_OBJ) $(IMGUI_OBJ) libpastel2d.a $(LIB_DEPS)
	$(CXX) -o $@ $^ $(LDFLAGS)
ifdef RELEASE
	strip $@
endif

#
# tests
#

$(PROJECT_NAME)-tests: $(ENGINE_OBJ) $(TEST_OBJ) $(LIB)
	$(CXX) -o $@ $^ $(LDFLAGS)

check: $(PROJECT_NAME)-tests
	./$^

#
# leaks
#

leaks: $(PROJECT_NAME)
	$(LEAKS_CMD) $(LEAKS_SUPP) ./$^

leaks-tests: $(PROJECT_NAME)-tests
	$(LEAKS_CMD) ./$^

#
# cleanup
#

.PHONY: softclean
softclean:
	rm -f $(PROJECT_NAME) $(UI_OBJ) $(ENGINE_OBJ) $(TEST_OBJ) $(IMGUI_OBJ) $(CLEANFILES) $(EMBED:=.h) $(CIRCUITS:=.h) libpastel2d.a

.PHONY: clean
clean: softclean
	$(MAKE) -C contrib/pastel2d clean
	rm -rf build-sdl3 libSDL3.a libluajit.a

