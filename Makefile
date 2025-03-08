#
# setup
#

PROJECT_NAME = transistor
PROJECT_VERSION = 0.1.1

all: $(PROJECT_NAME)

#
# configuration
#

include contrib/pastel-base/mk/config.mk

CPPFLAGS += -I. -Iengine -Iresources/fonts -Iresources/images \
			-Icontrib/imgui -Icontrib/doctest \
			-Icontrib/pastel2d/src -Icontrib/pastel-base/pl_log -Icontrib/pastel2d/contrib/pocketmod \
            -Icontrib/pastel2d/contrib/stb

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
	engine/cursor/cursor.o \
	engine/sandbox/sandbox.o \
	engine/simulation/simulation.o \
	engine/transistor-sandbox.o \
	engine/component/component_mt.o \
	engine/simulation/componentsim.o

UI_OBJ = \
	ui/main.o \
	ui/common.o \
	ui/resources.o \
	ui/gui/gui.o \
	ui/gui/dialog.o \
	ui/gui/mainmenu.o \
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

EMBED = \
	$(filter-out %.h, $(wildcard resources/fonts/*)) \
	$(filter-out %.h, $(wildcard resources/images/*)) \
	$(filter-out %.h, $(wildcard components/basic/*)) \
	$(filter-out %.h, $(wildcard components/gates/*))

$(UI_OBJ): $(EMBED:=.h)

#
# executable
#

libpastel2d.a:
	$(MAKE) -C contrib/pastel2d
	cp contrib/pastel2d/libpastel2d.a .

$(PROJECT_NAME): $(UI_OBJ) $(ENGINE_OBJ) $(IMGUI_OBJ) libpastel2d.a $(LIB_DEPS)
	$(CXX) -o $@ $^ $(LDFLAGS)
ifdef RELEASE
	strip $@
endif

#
# tests
#

TEST_OBJ = \
	tests/position.o \
	tests/pinpositions.o \
	tests/connected_wires.o \
	tests/placement.o \
	tests/compilation.o \
	tests/serialization.o \
	tests/simulation.o \
	tests/wrapper.o \
	tests/custom_ic.o \
	tests/_implementation.o

transistor-tests: $(ENGINE_OBJ) $(TEST_OBJ) $(LIB)
	$(CXX) -o $@ $^ $(LDFLAGS)

check: transistor-tests
	./$^

#
# cleanup
#

.PHONY: softclean
softclean:
	rm -f $(PROJECT_NAME) $(UI_OBJ) $(ENGINE_OBJ) $(TEST_OBJ) $(IMGUI_OBJ) $(CLEANFILES) $(EMBED:=.h) libpastel2d.a

.PHONY: clean
clean: softclean
	rm -rf build-sdl3 libSDL3.a libluajit.a

