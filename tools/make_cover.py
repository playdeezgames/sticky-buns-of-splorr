#!/usr/bin/env python3
"""Generates the itch.io art from the real game: assets/screenshots/*.png and assets/cover.png (630x500).

The frames come from `odin/sbos --shots` (the game's own renderer, so every pixel is the shipped font and palette).
The cover text is drawn with the same DINOBYTE sheet (odin/assets/dinobyte/dbyte_1x.png, CC BY 4.0 by mby, unmodified).
Usage: make odin-native && python3 tools/make_cover.py     (needs Pillow)"""
import os, subprocess, tempfile
from PIL import Image

root = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
sheet = Image.open(os.path.join(root, "odin/assets/dinobyte/dbyte_1x.png")).convert("RGBA")

def text(canvas, s, x, y, scale, rgb):
    """Draws s with the DINOBYTE glyphs (code page 437, 6x8 cells) at an integer scale."""
    for ch in s:
        code = ord(ch)
        cx, cy = (code % 16) * 6, (code // 16) * 8
        glyph = sheet.crop((cx, cy, cx + 6, cy + 8))
        mask = glyph.split()[3].resize((6 * scale, 8 * scale), Image.NEAREST)
        canvas.paste(Image.new("RGB", mask.size, rgb), (x, y), mask)
        x += 6 * scale

shots = os.path.join(root, "assets/screenshots")
os.makedirs(shots, exist_ok=True)
with tempfile.TemporaryDirectory() as tmp:
    subprocess.run([os.path.join(root, "odin/sbos"), "--shots", tmp], check=True, stdout=subprocess.DEVNULL)
    frames = {}
    for name in ("01-play", "02-menu", "03-shoppe"):
        im = Image.open(os.path.join(tmp, name + ".ppm")).convert("RGB")
        frames[name] = im
        im.resize((im.width * 4, im.height * 4), Image.NEAREST).save(os.path.join(shots, name + ".png"))

# ---- cover: 630x500 ----
W, H = 630, 500
BLUE, LIGHT = (0, 0, 170), (85, 85, 255)
cover = Image.new("RGB", (W, H), (0, 0, 0))
for y in range(0, H, 50):                      # the game's own checkerboard, behind everything
    for x in range(0, W, 50):
        cover.paste(BLUE if (x // 50 + y // 50) % 2 else (0, 0, 120), (x, y, x + 50, y + 50))

play = frames["01-play"]
board = play.crop((0, 0, 24 * 6, 8 * 24)).resize((24 * 6 * 2, 8 * 24 * 2), Image.NEAREST)   # 288x384
hud = play.crop((24 * 6, 0, 40 * 6, 9 * 8)).resize((16 * 6 * 2, 9 * 8 * 2), Image.NEAREST)  # 192x144
bx, by = W - board.width - 24, (H - board.height) // 2
cover.paste(Image.new("RGB", (board.width + 8, board.height + 8), (255, 255, 255)), (bx - 4, by - 4))
cover.paste(board, (bx, by))

lx = 24
text(cover, "STICKY", lx + 4, 36 + 4, 8, (0, 0, 0))
text(cover, "STICKY", lx, 36, 8, (255, 255, 85))
text(cover, "BUNS", lx + 4, 108 + 4, 8, (0, 0, 0))
text(cover, "BUNS", lx, 108, 8, (255, 255, 255))
text(cover, "of SPLORR!!", lx + 3, 192 + 3, 4, (0, 0, 0))
text(cover, "of SPLORR!!", lx, 192, 4, (255, 85, 255))
cover.paste(hud, (lx, 262))
text(cover, "a knight's tale", lx, 424, 3, (85, 255, 255))
text(cover, "eat buns. check buttholes.", lx, 452, 2, (170, 170, 170))
os.makedirs(os.path.join(root, "assets"), exist_ok=True)
cover.save(os.path.join(root, "assets/cover.png"))
print("wrote assets/cover.png and assets/screenshots/")
