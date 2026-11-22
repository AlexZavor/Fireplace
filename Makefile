# Install
BIN = Fireplace

# Flags
CFLAGS += -Wall -Wextra
#CFLAGS += -O2
CFLAGS += `sdl2-config --cflags`

SRC = src/main.c
OBJ = $(SRC:.c=.o)

LIBS += -lm -ldl `sdl2-config --libs`

INC += -Iinclude

$(BIN):
	@mkdir -p bin
	rm -f bin/$(BIN) $(OBJS)
	$(CC) $(SRC) $(CFLAGS) -o bin/$(BIN) $(LIBS) $(INC)
