#OBJS specifies which files to compile as part of the project
OBJS = sbos.cpp thing.cpp board.cpp boardcell.cpp token.cpp rng.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = sbos

CC = -g -w

#This is the target that compiles our executable
all : $(OBJS)
	g++ $(OBJS) $(CC) -o $(OBJ_NAME) -std=c++20