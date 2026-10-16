# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

"Sticky Buns of SPLORR!!" — a small chess-themed roguelike (a knight moves on an 8x8 board, eats sticky buns, fights pawns/bishop, checks buttholes, shops). C++20 on SDL2, built both natively and to the web via Emscripten. `README.md` is the feature checklist (shoppe items still unchecked). `love/main.lua` is an untracked Love2D hello-world stub, not part of the game.

## Build / run

```bash
make    # builds BOTH targets: em++ -> html/index.html (+ .js/.wasm/.data), and g++ -> ./hello
./hello # run native build (must run from repo root: romfont8x8.png is loaded by relative path)
```

- Requires `em++` (Emscripten with SDL2/SDL2_image ports) and `g++` with `libsdl2-dev`/`libsdl2-image-dev`. The Makefile has no separate targets, so there is no way to build only one half.
- `make` globs `src/*.cpp`; new files are picked up automatically. Flags: `-g -std=c++20 -Wall -Wextra -Wpedantic`.
- Web build preloads `romfont8x8.png` into the virtual FS and uses `custom.html` as the shell page. `html/*.{js,wasm,html,data}` and `hello` are gitignored build output.
- `shippit.sh` pushes `html/` to itch.io via `butler`.
- No tests, no linter.

## Architecture

**Rendering is a text-mode framebuffer.** The window is 320x200 logical pixels (`VIEW_WIDTH/HEIGHT` in `state.h`) = 40x25 cells of 8x8. States only write `FrameBuffer` cells (character, fg color, bg color, 16-color CGA-style palette in `FrameBufferCellColor`); `Application::Loop` then blits each cell twice from the `romfont8x8.png` 16x16 glyph sheet — glyph 219 (solid block) tinted with the background, then the glyph tinted with the foreground, via `SDL_SetTextureColorMod`. Characters are CP437 codes (e.g. `0xc9`/`0xcd` box drawing for the cursor frame, `\x9c` for the shoppe).

**Input is a command queue.** `Application::Loop` maps SDL keycodes (`keycodeCommands` table: WASD/ZQSD/arrows, space=GREEN, backspace/esc=RED, tab=YELLOW, enter=BLUE) to abstract `CommandType`s pushed into `CommandBuffer`. States consume commands in `Update()`; they never see SDL events.

**State machine.** `State` (`state.h`) has `Draw()` and `std::optional<GameState> Update()`; returning the next `GameState` switches state, returning `nullopt` quits. `Application` owns all states in a `map<GameState, unique_ptr<State>>` (ROOM, GAME_MENU, SHOPPE) and the single shared `FrameBuffer`, `CommandBuffer`, `WorldData`, `World`. `RoomState` (`roomstate.cpp`, ~750 lines) holds nearly all gameplay: move validation, combat, butthole outcomes, XP/leveling, streaks, plus all HUD drawing. `Application` is all-static, and `main.cpp` uses `emscripten_set_main_loop` under `__EMSCRIPTEN__`, otherwise a plain `Run()` loop.

**World model: data/handle split.** `WorldData` holds flat `vector`s of `BoardData`, `LocationData`, `CharacterData` (with a recycled-index set for characters) plus the avatar index and message log. `Board`, `Location`, `Character` are thin handle objects `(WorldData&, size_t index)` with the behavior. Everything cross-references by index, not pointer. Locations link to up to 8 knight-move neighbors (`KnightMoveType`, `KnightWalker`); "valid move" = a location with a living knight a knight's move away. Characters (including the avatar knight, items, shoppe, blocks) are all `CharacterType`s placed on locations, with per-character `StatisticType` maps (value/min/max) — `SUPPLIES` ("Buns") ticks down per move and is the hunger clock. `World::PopulateBoard` sets avatar starting stats and the initial spawn.

**Randomness** goes through `RNG::FromRange` (mt19937, `rng.cpp`) and the weighted `RNG::FromGenerator(map<T,size_t>)` helper used for butthole/jools outcome tables.

## Gotchas

- `State`, `Board`, etc. have private declared-but-undefined default constructors to forbid default construction; don't "fix" them.
- `src/state.cpp` is intentionally empty.
- `.vscode/launch.json` hardcodes the absolute path to `hello` and `tasks.json` runs `make`.

## Odin port (in progress)

`odin/` is a port-in-progress (package `sbos`), independent of the C++ build; the C++ code stays the reference for behaviour. Done: data model + world init (`types/world/rng.odin`), text framebuffer + platform-neutral `render_to_rgba` (`framebuffer/render.odin`; glyphs are the DINOBYTE font packed into `font_data.odin` by `odin/tools/gen_font.py` from `odin/assets/dinobyte/dbyte_1x.png`, CC BY 4.0 by mby, credit in `odin/assets/dinobyte/NOTICE.txt` — ship that notice with any release; cells are 6x8 so the frame is 240x200, not 320x200; the C++ build still uses `romfont8x8.png`, whose provenance is unresolved), `RoomState` (`room.odin`), command queue/state machine (`game.odin`) and the native SDL2 loop (`platform_sdl.odin`, `#+build !js`). Game menu and shoppe live in `menus.odin` (one shared `Menu_State` widget). All gameplay is ported, and the web build works: `platform_js.odin` (`#+build js`) exports `step` for odin.js's rAF loop and blits `render_to_rgba` output through a custom `sbos.present` import defined in `odin/web/index.html` (plain 2D canvas, no WebGL/SDL; ~320 KB wasm). `check.odin`/`main_native.odin`/`platform_sdl.odin` are `#+build !js`. Handles are `distinct int` ids into flat arrays in `World`; stats are `[Statistic_Type]Stat`.

```bash
make odin-web      # builds odin/web/sbos.wasm + copies odin.js; serve odin/web over http and open index.html
make odin-dist     # release bundle in dist/web (page, odin.js, wasm, NOTICES.txt)
bash shippit.sh    # = tools/ship.sh: test, build, zip; pushes to itch ONLY with --push (never run --push unless the user says so)
python3 tools/make_cover.py   # regenerates assets/cover.png (630x500) and assets/screenshots/ from the game's own renderer
make odin-native   # ./odin/sbos;  make odin-check runs the headless bot
~/ODIN/odin run odin -vet -strict-style -out:<somewhere>             # play (SDL2)
~/ODIN/odin run odin -vet -strict-style -o:speed -out:<somewhere> -- --check [frame.ppm]   # headless: world checks + 5000-tick random bot, optional PPM dump
```

Preserved C++ quirks: `AddXP` adds the XP goal to itself (goal doubles per level). Deliberate deviations: menu/shoppe items are cleared on every exit and after each purchase (C++ kept stale offers, so the shoppe could sell things the avatar could no longer afford); `world_clear` clears everything (C++ only cleared boards), and out-of-range framebuffer writes are clipped.

Release layout follows the vault's Release standard: page copy `ITCH_DESCRIPTION.md`, art in `assets/`, ship logic in `tools/ship.sh`. Provenance of every shipped asset is in `PROVENANCE.md`; the shipped notice is `odin/NOTICES.txt`.
