OBJ_NAME = main
CC = g++
COMPILER_FLAGS = -w

ifeq ($(OS),Windows_NT)
	OBJS = Windows.cpp
	LINKER_FLAGS =
else
	OBJS = Ncurses.cpp
	LINKER_FLAGS = -lncursesw
endif

$(OBJ_NAME): $(OBJS)
	$(CC) $^ $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@

run: $(OBJ_NAME)
	./$(OBJ_NAME)

