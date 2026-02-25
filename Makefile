CXX = g++
EMXX = em++
OUTPUT_DIR = html
SRC_DIR = src
OBJS = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = hello
CC = -g -w -std=c++20

all : $(OBJS)
	mkdir -p $(OUTPUT_DIR)
	$(EMXX) $(OBJS) $(CC) -o $(OUTPUT_DIR)/index.html --shell-file custom.html -sUSE_SDL=2
	$(CXX) $(OBJS) $(CC) -o $(OBJ_NAME) -lSDL2