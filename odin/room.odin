package sbos

import "core:fmt"

BOARD_CELL_WIDTH  :: 3
BOARD_CELL_HEIGHT :: 3

Room_State :: struct {
	x, y: int, // cursor
}

// ---- drawing ---------------------------------------------------------

board_cell_glyph :: proc(w: ^World, l: Location_Id) -> u8 {
	c := location_character(w, l).? or_else Character_Id(-1)
	if c < 0 do return 0
	switch w.characters[c].type {
	case .Shoppe:      return 0x9c
	case .Flogger:     return '!'
	case .Potion:      return '&'
	case .Butthole:    return '*'
	case .Bishop:      return 'B'
	case .Pawn:        return 'p'
	case .Sticky_Buns: return '@'
	case .Knight:      return 'N'
	case .Block:       return 'X'
	}
	return 0
}

board_cell_fg :: proc(w: ^World, l: Location_Id) -> Color {
	c := location_character(w, l).? or_else Character_Id(-1)
	if c < 0 do return .Black
	switch w.characters[c].type {
	case .Shoppe:           return .Light_Green
	case .Flogger:          return .Dark_Gray
	case .Potion:           return .Light_Red
	case .Sticky_Buns, .Pawn, .Bishop: return .White
	case .Knight, .Block:   return .Black
	case .Butthole:         return .Brown
	}
	return .Black
}

board_cell_bg :: proc(light, valid: bool) -> Color {
	if valid do return .Light_Cyan if light else .Cyan
	return .Light_Blue if light else .Blue
}

is_valid_move_destination :: proc(w: ^World, l: Location_Id) -> bool {
	for m in Knight_Move {
		n := location_neighbor(w, l, m).? or_continue
		c := location_character(w, n).? or_continue
		if w.characters[c].type == .Knight {
			if hp, ok := get_stat(w, c, .Health); ok && hp > 0 do return true
		}
	}
	return false
}

room_draw :: proc(g: ^Game) {
	room_draw_board(g)
	room_draw_cursor_frame(g)
	room_draw_stats(g)
	fb_write_text(&g.fb, 0, VIEW_ROWS - 1, " WASD:navigate \xb3 SPACE:move \xb3 TAB:menu  ", .Black, .White)
}

room_draw_board :: proc(g: ^Game) {
	w := &g.world
	b := character_board(w, w.avatar.?)
	bd := w.boards[b]
	for column in 0 ..< bd.columns {
		for row in 0 ..< bd.rows {
			loc := board_location(w, b, column, row).?
			valid := false
			if c, ok := location_character(w, loc).?; !ok || w.characters[c].type != .Block {
				valid = is_valid_move_destination(w, loc)
			}
			fb_fill(&g.fb, column * BOARD_CELL_WIDTH, row * BOARD_CELL_HEIGHT, BOARD_CELL_WIDTH, BOARD_CELL_HEIGHT,
				0, .Black, board_cell_bg(w.locations[loc].light, valid))
			fb_set_cell(&g.fb, column * BOARD_CELL_WIDTH + 1, row * BOARD_CELL_HEIGHT + 1,
				board_cell_glyph(w, loc), board_cell_fg(w, loc))
		}
	}
}

room_draw_cursor_frame :: proc(g: ^Game) {
	x, y := g.room.x * BOARD_CELL_WIDTH, g.room.y * BOARD_CELL_HEIGHT
	fb_set_cell(&g.fb, x, y, 0xc9)
	fb_set_cell(&g.fb, x + 1, y, 0xcd)
	fb_set_cell(&g.fb, x + 1, y + 2, 0xcd)
	fb_set_cell(&g.fb, x, y + 1, 0xba)
	fb_set_cell(&g.fb, x + 2, y + 1, 0xba)
	fb_set_cell(&g.fb, x + 2, y, 0xbb)
	fb_set_cell(&g.fb, x + 2, y + 2, 0xbc)
	fb_set_cell(&g.fb, x, y + 2, 0xc8)
}

room_draw_stats :: proc(g: ^Game) {
	w := &g.world
	av := w.avatar.?
	b := character_board(w, av)
	text_column := w.boards[b].columns * BOARD_CELL_WIDTH
	fb_fill(&g.fb, text_column, 0, VIEW_COLUMNS - text_column, VIEW_ROWS - 1, 0, nil, Color.Black)
	row := 0
	line :: proc(g: ^Game, col: int, row: ^int, text: string, fg: Color) {
		fb_write_text(&g.fb, col, row^, text, fg)
		row^ += 1
	}

	if streak := stat(w, av, .Streak); streak > 0 {
		line(g, text_column, &row, fmt.tprintf("Streak: %d", streak), .Light_Green)
	}
	line(g, text_column, &row, fmt.tprintf("Buns: %d/%d", stat(w, av, .Supplies), stat_max(w, av, .Supplies)), .Magenta)
	line(g, text_column, &row, fmt.tprintf("Armour: %d", stat(w, av, .Armour)), .Yellow)
	if n := stat(w, av, .Antitrap_Spray); n > 0 {
		line(g, text_column, &row, fmt.tprintf("Sprays: %d", n), .Light_Blue)
	}
	if n := stat(w, av, .Floggers); n > 0 {
		line(g, text_column, &row, fmt.tprintf("Floggers: %d", n), .Dark_Gray)
	}
	if n := stat(w, av, .Potions); n > 0 {
		line(g, text_column, &row, fmt.tprintf("Potions: %d", n), .Red)
	}
	line(g, text_column, &row, fmt.tprintf("Health: %d/%d", stat(w, av, .Health), stat_max(w, av, .Health)), .Light_Red)
	line(g, text_column, &row, fmt.tprintf("XP: %d/%d", stat(w, av, .XP), stat(w, av, .XP_Goal)), .Cyan)
	line(g, text_column, &row, fmt.tprintf("Level: %d", stat(w, av, .XP_Level)), .Light_Cyan)
	line(g, text_column, &row, fmt.tprintf("Jools: %d", stat(w, av, .Jools)), .Light_Green)
	line(g, text_column, &row, fmt.tprintf("Position: %c%c", u8('A' + g.room.x), u8('1' + g.room.y)), .Cyan)

	loc := board_location(w, b, g.room.x, g.room.y).?
	if is_valid_move_destination(w, loc) {
		line(g, text_column, &row, "Valid Move", .Green)
	}
	if c, ok := location_character(w, loc).?; ok {
		switch w.characters[c].type {
		case .Block:
			line(g, text_column, &row, "Blocked!", .Red)
		case .Butthole:
			line(g, text_column, &row, "Butthole", .Brown)
			line(g, text_column, &row, "(check it!)", .Brown)
		case .Knight:
			line(g, text_column, &row, "Knight(you)", .Dark_Gray)
		case .Sticky_Buns:
			line(g, text_column, &row, "+5 Buns", .White)
		case .Pawn:
			line(g, text_column, &row, "Pawn(enemy)", .White)
		case .Bishop:
			line(g, text_column, &row, "Bishop(elite)", .White)
		case .Shoppe:
			line(g, text_column, &row, "Shoppe", .Light_Green)
		case .Flogger, .Potion:
		}
	}
	for m in w.messages {
		fb_write_text(&g.fb, text_column, row, m.text, m.fg, m.bg)
		row += 1
	}
}

// ---- input / update --------------------------------------------------

room_update :: proc(g: ^Game) -> Game_State {
	next := Game_State.Room
	for next == .Room {
		cmd := command_read(g) or_break
		room_handle_command(g, cmd, &next)
	}
	return next
}

room_handle_command :: proc(g: ^Game, cmd: Command, next: ^Game_State) {
	w := &g.world
	r := &g.room
	#partial switch cmd {
	case .Up:    r.y = max(r.y - 1, 0);                clear_messages(w)
	case .Down:  r.y = min(r.y + 1, BOARD_ROWS - 1);   clear_messages(w)
	case .Left:  r.x = max(r.x - 1, 0);                clear_messages(w)
	case .Right: r.x = min(r.x + 1, BOARD_COLUMNS - 1); clear_messages(w)
	case .Green: room_attempt_move(g, next)
	case .Yellow: next^ = .Game_Menu
	}
}

msg :: proc(w: ^World, fg, bg: Color, format: string, args: ..any) {
	add_message(w, fmt.tprintf(format, ..args), fg, bg)
}

room_attempt_move :: proc(g: ^Game, next: ^Game_State) {
	w := &g.world
	av := w.avatar.?
	from := w.characters[av].location
	b := w.locations[from].board
	cursor := board_location(w, b, g.room.x, g.room.y).?
	for m in Knight_Move {
		dest := location_neighbor(w, from, m).? or_continue
		if dest != cursor do continue
		clear_messages(w)
		other, has_other := location_character(w, dest).?
		if hp, _ := get_stat(w, av, .Health); hp <= 0 {
			msg(w, .Red, .Black, "Yer Dead!")
		} else if has_other && w.characters[other].type == .Block {
			msg(w, .Black, .Red, "Blocked!")
		} else {
			room_move(g, next, from, cursor)
		}
	}
}

room_move :: proc(g: ^Game, next: ^Game_State, from, cursor: Location_Id) {
	w := &g.world
	room_remove_blocks(w)
	av := w.locations[from].character.?
	if supplies := stat(w, av, .Supplies); supplies > 0 {
		set_stat(w, av, .Supplies, supplies - 1)
	} else {
		set_stat(w, av, .Health, stat(w, av, .Health) - 1)
	}
	create_character(w, .Block, from) // leaves a block behind
	other, has_other := location_character(w, cursor).?
	w.characters[av].location = cursor
	w.locations[cursor].character = av
	if has_other {
		change_stat(w, av, .Streak, 1)
		#partial switch w.characters[other].type {
		case .Sticky_Buns: room_consume_sticky_buns(w, av, other)
		case .Butthole:    room_check_butthole(w, av, other)
		case .Pawn:        room_attack_pawn(w)
		case .Bishop:      room_attack_bishop(w)
		case .Shoppe:      next^ = .Shoppe
		}
	} else {
		if streak := stat(w, av, .Streak); streak > 1 {
			msg(w, .Cyan, .Black, "Streak Bonus!")
			room_add_xp(w, streak - 1)
		}
		set_stat(w, av, .Streak, 0)
	}
}

room_remove_blocks :: proc(w: ^World) {
	b := character_board(w, w.avatar.?)
	for maybe_loc in w.boards[b].locations {
		loc := maybe_loc.? or_continue
		c := w.locations[loc].character.? or_continue
		if w.characters[c].type == .Block {
			w.locations[loc].character = nil
			recycle_character(w, c)
		}
	}
}

room_consume_sticky_buns :: proc(w: ^World, av, other: Character_Id) {
	SUPPLIES_INCREASE :: 5
	msg(w, .White, .Black, "+%d Buns", SUPPLIES_INCREASE)
	set_stat(w, av, .Supplies, stat(w, av, .Supplies) + SUPPLIES_INCREASE)
	spawn_character(w, character_board(w, av), .Sticky_Buns)
	recycle_character(w, other)
}

Butthole_Result :: enum { Nothing, Jools, Trap, Teleport, Armour, Flogger, Potion }

room_check_butthole :: proc(w: ^World, av, other: Character_Id) {
	msg(w, .Brown, .Black, "Checking...")
	spawn_character(w, character_board(w, av), .Butthole)
	table := []Weighted(Butthole_Result){
		{.Nothing, 10}, {.Jools, 40}, {.Trap, 20}, {.Teleport, 10}, {.Armour, 40}, {.Flogger, 5}, {.Potion, 10},
	}
	switch rng_weighted(table) {
	case .Nothing:  msg(w, .Dark_Gray, .Black, "Nothing!")
	case .Jools:    room_trigger_jools(w)
	case .Trap:     room_trigger_trap(w)
	case .Teleport: room_trigger_teleport(w)
	case .Armour:
		msg(w, .Yellow, .Black, "+%d Armour", 1)
		change_stat(w, av, .Armour, 1)
	case .Flogger:
		msg(w, .Dark_Gray, .Black, "+%d Floggers", 1)
		change_stat(w, av, .Floggers, 1)
	case .Potion:
		msg(w, .Red, .Black, "+%d Potion", 1)
		change_stat(w, av, .Potions, 1)
	}
	recycle_character(w, other)
}

room_trigger_trap :: proc(w: ^World) {
	TRAP_DAMAGE :: 1
	av := w.avatar.?
	msg(w, .Black, .Red, "Trap!")
	if n, _ := get_stat(w, av, .Antitrap_Spray); n > 0 {
		msg(w, .Black, .Light_Blue, "-1 Spray")
		change_stat(w, av, .Antitrap_Spray, -1)
	} else {
		msg(w, .Black, .Red, "-%d Health", TRAP_DAMAGE)
		change_stat(w, av, .Health, -TRAP_DAMAGE)
	}
}

room_trigger_jools :: proc(w: ^World) {
	table := []Weighted(int){{5, 1}, {6, 2}, {7, 3}, {8, 2}, {9, 1}}
	jools := rng_weighted(table)
	msg(w, .Light_Green, .Black, "+%d Jools", jools)
	change_stat(w, w.avatar.?, .Jools, jools)
}

room_trigger_teleport :: proc(w: ^World) {
	msg(w, .Light_Blue, .Black, "Teleport!")
	av := w.avatar.?
	b := character_board(w, av)
	bd := w.boards[b]
	loc: Location_Id
	for {
		loc = board_location(w, b, rng_range(0, bd.columns - 1), rng_range(0, bd.rows - 1)).?
		if location_character(w, loc) == nil do break
	}
	w.locations[w.characters[av].location].character = nil
	w.locations[loc].character = av
	w.characters[av].location = loc
}

room_damage_avatar :: proc(w: ^World, damage: int) {
	damage := damage
	av := w.avatar.?
	armour, _ := get_stat(w, av, .Armour)
	if damage > 0 && armour > 0 {
		absorption := min(damage, armour)
		change_stat(w, av, .Armour, -absorption)
		msg(w, .Black, .Yellow, "-%d Armour", absorption)
		damage -= absorption
	}
	if damage > 0 {
		change_stat(w, av, .Health, -damage)
		msg(w, .Black, .Red, "-%d Health", damage)
	}
}

room_attack_pawn :: proc(w: ^World) {
	room_damage_avatar(w, 1)
	room_add_xp(w, 1)
	b := character_board(w, w.avatar.?)
	if !board_has_character_type(w, b, .Pawn) && !board_has_character_type(w, b, .Bishop) {
		spawn_character(w, b, .Bishop)
		if !board_has_character_type(w, b, .Shoppe) do spawn_character(w, b, .Shoppe)
	}
}

room_attack_bishop :: proc(w: ^World) {
	av := w.avatar.?
	if n, _ := get_stat(w, av, .Floggers); n > 0 {
		change_stat(w, av, .Floggers, -1)
		msg(w, .Black, .Dark_Gray, "BISHOP FLOGGED!")
		msg(w, .Dark_Gray, .Black, "-1 Flogger")
		room_add_xp(w, 5)
	} else {
		room_damage_avatar(w, 3)
		room_add_xp(w, 3)
	}
	b := character_board(w, av)
	if !board_has_character_type(w, b, .Pawn) && !board_has_character_type(w, b, .Bishop) {
		for _ in 0 ..< 8 do spawn_character(w, b, .Pawn)
	}
}

room_add_xp :: proc(w: ^World, xp: int) {
	av := w.avatar.?
	change_stat(w, av, .XP, xp)
	msg(w, .Cyan, .Black, "+%d XP", xp)
	for stat(w, av, .XP_Goal) <= stat(w, av, .XP) {
		msg(w, .Cyan, .Black, "+1 XP Level")
		msg(w, .Light_Red, .Black, "+1 Max HP")
		msg(w, .Red, .Black, "+1 HP")
		goal := stat(w, av, .XP_Goal)
		change_stat(w, av, .XP_Level, 1)
		change_stat(w, av, .XP, -goal)
		change_stat(w, av, .XP_Goal, goal) // sic: C++ adds the goal to itself, doubling it each level
		set_stat_max(w, av, .Health, stat_max(w, av, .Health) + 1)
		change_stat(w, av, .Health, 1)
	}
}
