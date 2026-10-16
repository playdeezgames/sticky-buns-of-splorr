# Itch.io page text

Page: https://thegrumpygamedev.itch.io/sticky-buns-of-splorr
Pasted in by hand; nothing here is published by the repo.

## Short description

A knight, an 8 by 8 board, and a constant need for sticky buns. Eat. Fight pawns. Check yer butthole.

## Paste this as the page description

You are a KNIGHT on a chessboard, and you are hungry.

**How to play**

Move the cursor with WASD (or ZQSD, or the arrow keys) and press Space to move there. You move like a knight, in an L, so only the cyan squares are legal. Every move costs a bun. When yer out of buns, every move costs health instead. The square you just left is blocked for one move.

**What's on the board**

STICKY BUNS give you +5 buns. Eat them. They come back.

BUTTHOLES are worth checking. You might find jools, armour, a potion, or a flogger. You might also find a trap, or get teleported, or find nothing at all. A new butthole always turns up.

PAWNS are the enemy. Each one hurts a little (armour soaks it up first) and gives you XP. Collect enough XP and you level up: one more max health, and a heal. Hit things on consecutive moves for a streak bonus.

Clear every pawn and the BISHOP arrives, along with a SHOPPE. The bishop hits hard unless you have a flogger to flog it with. Beat the bishop and the pawns come back.

The SHOPPE sells things for jools: more max buns, potions, armour, floggers and anti-trap spray. Prices go up as you level.

**Controls:** WASD / ZQSD / arrows to move the cursor, Space to move or choose, Tab for the menu (continue, restart, use a potion), Tab again to leave the menu or the shoppe.

Needs a keyboard. Nothing is saved: close the tab and the knight is gone.

**Credits**

Game and code by TheGrumpyGameDev. The original was written in C++ earlier this year; the browser version was rebuilt in Odin in October 2026, and that port was written with Claude Code (generative AI).

Text drawn with the DINOBYTE bitmap font by mby, https://mby.itch.io/dinobyte, CC BY 4.0 (https://creativecommons.org/licenses/by/4.0/). The font is used unmodified. Runs on the Odin runtime. Full notices are in NOTICES.txt inside the game files.

## Notes (not for the page)

- Page slug is `sticky-buns-of-splorr`; `tools/ship.sh --push` sends `dist/web` to `thegrumpygamedev/sticky-buns-of-splorr:html`, the same channel the old Emscripten build used, so it replaces that build. The channel needs "This file will be played in the browser" ticked and "Kind of project" set to HTML; check that no old native upload is left on the page.
- Embed: the canvas is 240 by 200 (6:5). 480 by 400 or any larger 6:5 size scales cleanly (the page letterboxes anything else).
- Cover: `assets/cover.png` (630 by 500). Screenshots, in page order: `assets/screenshots/01-play.png`, `02-menu.png`, `03-shoppe.png` (960 by 800 each). All are generated from the game's own renderer by `tools/make_cover.py`.
- If the edit screen has a generative-AI disclosure field, fill it in to match the Credits paragraph.
- Provenance of every shipped asset: `PROVENANCE.md`. The old cover (`ss/cover.png`) was drawn with the C++ build's font, whose provenance is unresolved; do not upload it.
- Ship order: commit, `bash tools/ship.sh` (builds and zips, does not push), look at `build/sticky-buns-html5.zip`, then `bash tools/ship.sh --push` only when you say so.
