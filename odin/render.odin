package sbos

// The 16-colour palette, indexed by Color (same values as Application::palette).
PALETTE := [Color][3]u8 {
	.Black = {0, 0, 0},         .Blue = {0, 0, 170},         .Green = {0, 170, 0},       .Cyan = {0, 170, 170},
	.Red = {170, 0, 0},         .Magenta = {170, 0, 170},    .Brown = {170, 85, 0},      .Light_Gray = {170, 170, 170},
	.Dark_Gray = {85, 85, 85},  .Light_Blue = {85, 85, 255}, .Light_Green = {85, 255, 85}, .Light_Cyan = {85, 255, 255},
	.Light_Red = {255, 85, 85}, .Light_Magenta = {255, 85, 255}, .Yellow = {255, 255, 85}, .White = {255, 255, 255},
}

// Rasterises the frame buffer into RGBA8 (VIEW_WIDTH x VIEW_HEIGHT, row-major) using GLYPHS
// (font_data.odin, generated from the DINOBYTE sheet by tools/gen_font.py).
// This is the whole renderer as far as the platform layers are concerned:
// SDL uploads it to a texture, the web host puts it on a canvas.
render_to_rgba :: proc(fb: ^Frame_Buffer, out: []u8) {
	assert(len(out) == VIEW_WIDTH * VIEW_HEIGHT * 4)
	for row in 0 ..< VIEW_ROWS {
		for column in 0 ..< VIEW_COLUMNS {
			cell := fb.cells[row][column]
			bg := PALETTE[cell.bg]
			fg := PALETTE[cell.fg]
			glyph := GLYPHS[cell.ch]
			for y in 0 ..< CELL_HEIGHT {
				base := ((row * CELL_HEIGHT + y) * VIEW_WIDTH + column * CELL_WIDTH) * 4
				for x in 0 ..< CELL_WIDTH {
					c := fg if glyph & (1 << uint(y * CELL_WIDTH + x)) != 0 else bg
					i := base + x * 4
					out[i], out[i + 1], out[i + 2], out[i + 3] = c.r, c.g, c.b, 255
				}
			}
		}
	}
}
