package sbos

import "core:slice"
import "core:strings"

BOARD_COLUMNS :: 8
BOARD_ROWS    :: 8

// ---- stats -----------------------------------------------------------

stat_clamped :: proc(s: Stat) -> int {
	return clamp(s.value, s.min, s.max)
}

character_init_stats :: proc(c: ^Character) {
	for &s in c.stats {
		s = Stat{min = min(int), max = max(int)}
	}
}

get_stat :: proc(w: ^World, id: Character_Id, t: Statistic_Type) -> (value: int, ok: bool) {
	s := w.characters[id].stats[t]
	if !s.set do return 0, false
	return stat_clamped(s), true
}

// Panics if unset, like `*GetStatistic(...)` on an empty optional would misbehave.
stat :: proc(w: ^World, id: Character_Id, t: Statistic_Type) -> int {
	v, ok := get_stat(w, id, t)
	assert(ok, "statistic not set")
	return v
}

set_stat :: proc(w: ^World, id: Character_Id, t: Statistic_Type, v: Maybe(int)) {
	s := &w.characters[id].stats[t]
	if val, ok := v.?; ok {
		s.set = true
		s.value = clamp(val, s.min, s.max)
	} else {
		s.set = false
	}
}

change_stat :: proc(w: ^World, id: Character_Id, t: Statistic_Type, delta: int) {
	set_stat(w, id, t, stat(w, id, t) + delta)
}

set_stat_min :: proc(w: ^World, id: Character_Id, t: Statistic_Type, v: Maybe(int)) {
	w.characters[id].stats[t].min = v.? or_else min(int)
}

set_stat_max :: proc(w: ^World, id: Character_Id, t: Statistic_Type, v: Maybe(int)) {
	w.characters[id].stats[t].max = v.? or_else max(int)
}

stat_max :: proc(w: ^World, id: Character_Id, t: Statistic_Type) -> int {
	return w.characters[id].stats[t].max
}

// ---- boards / locations ---------------------------------------------

create_board :: proc(w: ^World, columns, rows: int) -> Board_Id {
	b := Board{columns = columns, rows = rows}
	b.locations = make([dynamic]Maybe(Location_Id), columns * rows)
	append(&w.boards, b)
	return Board_Id(len(w.boards) - 1)
}

create_location :: proc(w: ^World, board: Board_Id, light: bool) -> Location_Id {
	append(&w.locations, Location{board = board, light = light})
	return Location_Id(len(w.locations) - 1)
}

board_set_location :: proc(w: ^World, b: Board_Id, column, row: int, l: Maybe(Location_Id)) {
	bd := &w.boards[b]
	bd.locations[column + row * bd.columns] = l
}

board_location :: proc(w: ^World, b: Board_Id, column, row: int) -> Maybe(Location_Id) {
	bd := &w.boards[b]
	return bd.locations[column + row * bd.columns]
}

location_character :: proc(w: ^World, l: Location_Id) -> Maybe(Character_Id) {
	return w.locations[l].character
}

location_neighbor :: proc(w: ^World, l: Location_Id, m: Knight_Move) -> Maybe(Location_Id) {
	return w.locations[l].neighbors[m]
}

board_has_character_type :: proc(w: ^World, b: Board_Id, t: Character_Type) -> bool {
	for maybe_loc in w.boards[b].locations {
		loc := maybe_loc.? or_continue
		c := w.locations[loc].character.? or_continue
		if w.characters[c].type == t do return true
	}
	return false
}

// ---- characters ------------------------------------------------------

// Reuses the lowest recycled index first (std::set::begin semantics).
create_character :: proc(w: ^World, t: Character_Type, l: Location_Id) -> Character_Id {
	c := Character{type = t, location = l}
	character_init_stats(&c)
	id: Character_Id
	if len(w.recycled) == 0 {
		append(&w.characters, c)
		id = Character_Id(len(w.characters) - 1)
	} else {
		lowest := 0
		for r, i in w.recycled do if r < w.recycled[lowest] do lowest = i
		id = w.recycled[lowest]
		unordered_remove(&w.recycled, lowest)
		w.characters[id] = c
	}
	w.locations[l].character = id
	return id
}

recycle_character :: proc(w: ^World, id: Character_Id) {
	if !slice.contains(w.recycled[:], id) do append(&w.recycled, id)
}

character_board :: proc(w: ^World, id: Character_Id) -> Board_Id {
	return w.locations[w.characters[id].location].board
}

// ---- messages --------------------------------------------------------

clone_string :: proc(s: string) -> string {
	return strings.clone(s)
}

add_message :: proc(w: ^World, text: string, fg, bg: Color) {
	append(&w.messages, Message{text = clone_string(text), fg = fg, bg = bg})
}

clear_messages :: proc(w: ^World) {
	for m in w.messages do delete(m.text)
	clear(&w.messages)
}

// ---- init ------------------------------------------------------------

// NOTE: the C++ WorldData::Clear() only cleared boards (locations/characters leaked
// and messages persisted across restarts). This clears everything; observable
// behaviour is the same except that stale messages no longer survive a restart.
world_clear :: proc(w: ^World) {
	for &b in w.boards do delete(b.locations)
	clear(&w.boards)
	clear(&w.locations)
	clear(&w.characters)
	clear(&w.recycled)
	w.avatar = nil
	clear_messages(w)
}

world_destroy :: proc(w: ^World) {
	world_clear(w)
	delete(w.boards); delete(w.locations); delete(w.characters)
	delete(w.recycled); delete(w.messages)
}

initialize_board :: proc(w: ^World) -> Board_Id {
	b := create_board(w, BOARD_COLUMNS, BOARD_ROWS)
	cols, rows := w.boards[b].columns, w.boards[b].rows
	for column in 0 ..< cols {
		for row in 0 ..< rows {
			board_set_location(w, b, column, row, create_location(w, b, (column + row) % 2 == 1))
		}
	}
	for column in 0 ..< cols {
		for row in 0 ..< rows {
			loc := board_location(w, b, column, row).?
			for m in Knight_Move {
				d := KNIGHT_DELTAS[m]
				dx, dy := column + d.x, row + d.y
				if dx >= 0 && dy >= 0 && dx < cols && dy < rows {
					w.locations[loc].neighbors[m] = board_location(w, b, dx, dy)
				}
			}
		}
	}
	return b
}

spawn_character :: proc(w: ^World, b: Board_Id, t: Character_Type) -> Character_Id {
	column, row: int
	for {
		column = rng_range(0, BOARD_COLUMNS - 1)
		row = rng_range(0, BOARD_ROWS - 1)
		if location_character(w, board_location(w, b, column, row).?) == nil do break
	}
	return create_character(w, t, board_location(w, b, column, row).?)
}

populate_board :: proc(w: ^World, b: Board_Id) {
	KNIGHT_INITIAL_SUPPLIES :: 15
	KNIGHT_INITIAL_HEALTH   :: 3
	KNIGHT_INITIAL_ARMOUR   :: 5

	avatar := spawn_character(w, b, .Knight)
	w.avatar = avatar

	set_stat_min(w, avatar, .Supplies, 0)
	set_stat_max(w, avatar, .Supplies, KNIGHT_INITIAL_SUPPLIES)
	set_stat(w, avatar, .Supplies, KNIGHT_INITIAL_SUPPLIES)
	set_stat_min(w, avatar, .Health, 0)
	set_stat_max(w, avatar, .Health, KNIGHT_INITIAL_HEALTH)
	set_stat(w, avatar, .Health, KNIGHT_INITIAL_HEALTH)

	for t in ([]Statistic_Type{.Attack, .Defend, .Potions, .Floggers, .Jools, .XP, .Streak, .Antitrap_Spray}) {
		set_stat_min(w, avatar, t, 0)
		set_stat(w, avatar, t, 0)
	}
	set_stat_min(w, avatar, .XP_Level, 1)
	set_stat(w, avatar, .XP_Level, 1)
	set_stat(w, avatar, .XP_Goal, 5)
	set_stat_min(w, avatar, .Armour, 0)
	set_stat(w, avatar, .Armour, KNIGHT_INITIAL_ARMOUR)

	spawn_character(w, b, .Sticky_Buns)
	spawn_character(w, b, .Butthole)
	for _ in 0 ..< 8 do spawn_character(w, b, .Pawn)
}

world_initialize :: proc(w: ^World) {
	world_clear(w)
	populate_board(w, initialize_board(w))
}
