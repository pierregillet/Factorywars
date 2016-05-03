OBJS = src/main.cpp src/gui.cpp src/game_mechanics.cpp src/save.c src/utils.c src/items.cpp src/player.cpp
CC = g++
COMPILER_FLAGS = -std=c++11 -Wall
LINKER_FLAGS = -lSDL2 -lSDL2_image
OBJ_NAME = factorywars

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

documentation :
	doxygen doc/config

clean :
	rm -rf doc/html
	rm -rf doc/latex
	rm -f factorywars
