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

CPPFLAGS += -Ifrontend -Ibackend/interface -Iresources \
			-Icontrib/pastel2d/src -Icontrib/pastel-base/pl_log -Icontrib/pastel2d/contrib/pocketmod -Icontrib/pastel2d/contrib/stb \
			-isystem contrib/imgui -Icontrib/SDL/include

ifdef RELEASE
	LIB_DEPS = backend/transistor.a libSDL3.a libluajit.a
else
	CPPFLAGS += $(shell pkg-config --cflags sdl3 luajit)
	LDFLAGS += $(shell pkg-config --libs sdl3 luajit) -Wl,-rpath,. -L. -ltransistor
	EXTRA_DEPS = libtransistor.so
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
	frontend/main.o \
	frontend/implementation.o

IMGUI_OBJ = \
	contrib/imgui/imgui.o \
	contrib/imgui/imgui_demo.o \
	contrib/imgui/imgui_draw.o \
	contrib/imgui/imgui_tables.o \
	contrib/imgui/imgui_widgets.o \
	contrib/imgui/backends/imgui_impl_sdl3.o \
	contrib/imgui/backends/imgui_impl_sdlrenderer3.o

RESOURCES = \
	$(filter-out %.h, $(wildcard resources/fonts/*)) \
	$(filter-out %.h, $(wildcard resources/images/*))

$(OBJ): $(RESOURCES:=.h)

#
# executable
#

# special rule to avoid warnings in contributed files

contrib/imgui/%.cc: contrib/imgui/%.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNINGS) -c

# libraries

libpastel2d-cc.a:
	$(MAKE) -C contrib/pastel2d
	cp contrib/pastel2d/libpastel2d-cc.a .

backend/transistor.a:
	$(MAKE) -C backend release

libtransistor.so:
	$(MAKE) -C backend
	cp backend/libtransistor.so .

# transistor executable

$(PROJECT_NAME): $(OBJ) $(IMGUI_OBJ) libpastel2d-cc.a $(LIB_DEPS) | $(EXTRA_DEPS)
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
	$(MAKE) -C backend clean
	rm -f $(PROJECT_NAME) $(OBJ) $(IMGUI_OBJ) $(CLEANFILES) $(RESOURCES:=.h) **/*.d libtransistor.so backend/transistor.a

.PHONY: clean
clean: softclean
	$(MAKE) -C contrib/pastel2d clean
	rm -rf build-sdl3 libSDL3.a libluajit.a libpastel2d-cc.a
