OBJS = src/main.cpp src/gui.cpp src/save.c src/utils.c src/items.cpp src/XMLParser.c src/player.cpp src/config.c src/display_map.cpp src/display_item.cpp
CC = g++
COMPILER_FLAGS = -std=c++11 -Wall `xml2-config --cflags`
LINKER_FLAGS = -lSDL2 -lSDL2_image `xml2-config --libs`
OBJ_NAME = factorywars

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -O3 $(LINKER_FLAGS) -o $(OBJ_NAME)

fast : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -O0 $(LINKER_FLAGS) -o $(OBJ_NAME)

debug : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -g $(LINKER_FLAGS) -o $(OBJ_NAME)

documentation :
	doxygen doc/config

clean :
	rm -rf doc/html
	rm -rf doc/latex
	rm -f factorywars
