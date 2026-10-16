CXX = g++
EMXX = em++
OUTPUT_DIR = html
SRC_DIR = src
OBJS = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = hello
CC = -g -std=c++20 -Wall -Wextra -Wpedantic

all : $(OBJS)
	mkdir -p $(OUTPUT_DIR)
	$(EMXX) $(OBJS) $(CC) -o $(OUTPUT_DIR)/index.html --shell-file custom.html -sUSE_SDL=2 --preload-file romfont8x8.png -sUSE_SDL_IMAGE=2 -sALLOW_MEMORY_GROWTH -sSDL2_IMAGE_FORMATS='["png"]'
	$(CXX) $(OBJS) $(CC) -o $(OBJ_NAME) -lSDL2 -lSDL2_image

ODIN = $(HOME)/ODIN/odin
ODIN_FLAGS = -vet -strict-style -o:speed

# Odin port (see odin/): `make odin-web` -> odin/web/ (serve with: python3 -m http.server -d odin/web)
.PHONY: odin-native odin-web odin-check odin-dist
odin-native:
	$(ODIN) build odin $(ODIN_FLAGS) -out:odin/sbos
odin-web:
	$(ODIN) build odin $(ODIN_FLAGS) -target:js_wasm32 -out:odin/web/sbos.wasm
	cp $(HOME)/ODIN/core/sys/wasm/js/odin.js odin/web/odin.js
odin-check: odin-native
	odin/sbos --check

# Release bundle for itch.io (HTML5): dist/web/ holds index.html, odin.js, sbos.wasm and the font credit.
odin-dist: odin-web
	rm -rf dist/web && mkdir -p dist/web
	cp odin/web/index.html odin/web/odin.js odin/web/sbos.wasm dist/web/
	cp odin/NOTICES.txt dist/web/NOTICES.txt
