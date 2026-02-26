CXX = g++
EMXX = em++
OUTPUT_DIR = html
SRC_DIR = src
OBJS = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = hello
CC = -g -std=c++20 -Wall

all : $(OBJS)
	mkdir -p $(OUTPUT_DIR)
	$(EMXX) $(OBJS) $(CC) -o $(OUTPUT_DIR)/index.html --shell-file custom.html -sUSE_SDL=2 --preload-file romfont8x8.png -sUSE_SDL_IMAGE=2 -sALLOW_MEMORY_GROWTH -sSDL2_IMAGE_FORMATS='["png"]'
	$(CXX) $(OBJS) $(CC) -o $(OBJ_NAME) -lSDL2 -lSDL2_image