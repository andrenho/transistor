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
			-Icontrib/pastel2d/src -Icontrib/pastel-base/pl_log -Icontrib/pastel2d/contrib/pocketmod \
			-isystem contrib/imgui

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

OBJ = \
	main.o \
	backend/engine.o \
	backend/tests.o \
	simulation/simulation.o #\
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

ENGINE = \
	$(wildcard engine/*.lua) \
	$(wildcard engine/tests/*.lua) \
	$(wildcard engine/util/*.lua) \
	$(wildcard components/basic/*.lua) \
	$(wildcard components/gates/*.lua)

EMBED = \
	$(filter-out %.h, $(wildcard resources/fonts/*)) \
	$(filter-out %.h, $(wildcard resources/images/*))

$(OBJ): $(ENGINE:=.h)
$(OBJ): $(EMBED:=.h)

#
# executable
#

# special rule to avoid warnings in contributed files

contrib/imgui/%.cc: contrib/imgui/%.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNINGS) -c

# pastel2d library

libpastel2d-cc.a:
	$(MAKE) -C contrib/pastel2d
	cp contrib/pastel2d/libpastel2d-cc.a .

# transistor executable

$(PROJECT_NAME): $(OBJ) $(IMGUI_OBJ) libpastel2d-cc.a $(LIB_DEPS)
	$(CXX) -o $@ $^ $(LDFLAGS)
ifdef RELEASE
	strip $@
endif

#
#
#

check:
	./run_tests.sh

#
# leaks
#

leaks: $(PROJECT_NAME)
	$(LEAKS_CMD) $(LEAKS_SUPP) ./$^

#
# cleanup
#

.PHONY: softclean
softclean:
	rm -f $(PROJECT_NAME) $(OBJ) $(TEST_OBJ) $(IMGUI_OBJ) $(CLEANFILES) $(EMBED:=.h) $(ENGINE:=.h) libpastel2d.a

.PHONY: clean
clean: softclean
	$(MAKE) -C contrib/pastel2d clean
	rm -rf build-sdl3 libSDL3.a libluajit.a

