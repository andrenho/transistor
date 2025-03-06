PROJECT_NAME = transistor
PROJECT_VERSION = 1.0.0

all: $(PROJECT_NAME)

include contrib/pastel-base/mk/config.mk

CPPFLAGS += -I. -Icontrib/pastel2d/src -Icontrib/pastel-base/pl_log -Icontrib/SDL/include -Icontrib/transistor-sandbox/src \
            -Iresources/fonts -Iresources/images -Icontrib/pastel2d/contrib/pocketmod -Icontrib/pastel2d/contrib/stb \
            -Icontrib/pastel-base/mk/LuaJIT/src

ifdef APPLE  # SDL requirements for mac
	LDFLAGS += -framework AVFoundation -framework CoreMedia -framework CoreVideo \
			   -framework CoreAudio -framework AudioToolbox -framework CoreHaptics -framework GameController \
			   -framework CoreServices -framework Cocoa -framework Carbon -framework IOKit \
			   -framework ForceFeedback -framework Metal -framework MetalKit -framework OpenGL \
			   -framework QuartzCore -framework UniformTypeIdentifiers -liconv
endif

OBJ = \
	src/main.o \
	src/resources.o \
	src/gui/gui.o \
	src/board/board.o \
	src/board/components.o

EMBED = \
	$(filter-out %.h, $(wildcard resources/fonts/*)) \
	$(filter-out %.h, $(wildcard resources/images/*)) \
	$(filter-out %.h, $(wildcard components/basic/*)) \
	$(filter-out %.h, $(wildcard components/gates/*))

$(OBJ): $(EMBED:=.h)

libpastel2d.a:
	$(MAKE) -C contrib/pastel2d
	cp contrib/pastel2d/libpastel2d.a .

libtransistor.a:
	$(MAKE) -C contrib/transistor-sandbox
	cp contrib/transistor-sandbox/libtransistor.a .

libSDL3.a:
	cmake -B build-sdl3 -S contrib/SDL -DCMAKE_OSX_DEPLOYMENT_TARGET=${MACOS_VERSION} -DCMAKE_BUILD_TYPE=Release -DSDL_SHARED=OFF -DSDL_STATIC=ON && cd ..
	$(MAKE) -C build-sdl3
	cp build-sdl3/libSDL3.a .

$(PROJECT_NAME): $(OBJ) libluajit.a libpastel2d.a libtransistor.a libSDL3.a
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf build-sdl3
	rm -f $(PROJECT_NAME) $(OBJ) $(CLEANFILES) $(EMBED:=.h) libpastel2d.a libtransistor.a libSDL3.a