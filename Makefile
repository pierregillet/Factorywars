OBJS = src/main.cpp
CC = g++
COMPILER_FLAGS = 
LINKER_FLAGS = -lSDL2
OBJ_NAME = factorywars

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)