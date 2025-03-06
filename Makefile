PROJECT_NAME = transistor
PROJECT_VERSION = 1.0.0

all: $(PROJECT_NAME)

include contrib/pastel-base/mk/config.mk

CPPFLAGS += -I. -Icontrib/pastel2d/src -Icontrib/pastel-base/pl_log -Icontrib/SDL/include -Icontrib/transistor-sandbox/src \
            -Iresources/fonts -Iresources/images -Icontrib/pastel2d/contrib/pocketmod -Icontrib/pastel2d/contrib/stb

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

$(PROJECT_NAME): $(OBJ) libluajit.a
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f $(PROJECT_NAME) $(OBJ) $(CLEANFILES) $(EMBED:=.h)