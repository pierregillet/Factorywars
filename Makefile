OBJS = src/main.cpp src/gui.cpp src/game_mechanics.cpp src/items.cpp src/save.cpp src/utils.cpp
CC = g++
COMPILER_FLAGS = -std=c++11
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
