package sbos

CELL_WIDTH   :: 6 // DINOBYTE glyph cells are 6x8
CELL_HEIGHT  :: 8
VIEW_COLUMNS :: 40
VIEW_ROWS    :: 25
VIEW_WIDTH   :: VIEW_COLUMNS * CELL_WIDTH  // 240
VIEW_HEIGHT  :: VIEW_ROWS * CELL_HEIGHT    // 200

Cell :: struct {
	ch:     u8,
	fg, bg: Color,
}

// Fixed 40x25 text-mode grid; zero value is char 0, black on black, as in the C++ version.
Frame_Buffer :: struct {
	cells: [VIEW_ROWS][VIEW_COLUMNS]Cell,
}

// Optional args mirror the std::optional parameters of FrameBuffer::SetCell/Fill/WriteText.
fb_set_cell :: proc(fb: ^Frame_Buffer, column, row: int, ch: Maybe(u8) = nil, fg: Maybe(Color) = nil, bg: Maybe(Color) = nil) {
	if column < 0 || column >= VIEW_COLUMNS || row < 0 || row >= VIEW_ROWS do return // clip
	cell := &fb.cells[row][column]
	if v, ok := ch.?; ok do cell.ch = v
	if v, ok := fg.?; ok do cell.fg = v
	if v, ok := bg.?; ok do cell.bg = v
}

fb_fill :: proc(fb: ^Frame_Buffer, column, row, columns, rows: int, ch: Maybe(u8) = nil, fg: Maybe(Color) = nil, bg: Maybe(Color) = nil) {
	for c in column ..< column + columns {
		for r in row ..< row + rows {
			fb_set_cell(fb, c, r, ch, fg, bg)
		}
	}
}

fb_write_text :: proc(fb: ^Frame_Buffer, column, row: int, text: string, fg: Maybe(Color) = nil, bg: Maybe(Color) = nil) {
	c := column
	for i in 0 ..< len(text) {
		fb_set_cell(fb, c, row, text[i], fg, bg)
		c += 1
	}
}
