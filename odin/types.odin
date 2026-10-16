package sbos

// Mirrors the C++ enums. Order matches src/*type.h (palette order matters for Color).

Color :: enum u8 {
	Black, Blue, Green, Cyan, Red, Magenta, Brown, Light_Gray,
	Dark_Gray, Light_Blue, Light_Green, Light_Cyan, Light_Red, Light_Magenta, Yellow, White,
}

Character_Type :: enum {
	Knight, Block, Sticky_Buns, Butthole, Pawn, Bishop, Flogger, Potion, Shoppe,
}

Statistic_Type :: enum {
	Supplies, Health, Attack, Defend, Potions, Jools, XP, XP_Level, XP_Goal,
	Armour, Streak, Floggers, Antitrap_Spray,
}

Knight_Move :: enum { UUR, RRU, RRD, DDR, DDL, LLD, LLU, UUL }

KNIGHT_DELTAS := [Knight_Move][2]int {
	.UUR = {1, -2}, .RRU = {2, -1}, .RRD = {2, 1}, .DDR = {1, 2},
	.DDL = {-1, 2}, .LLD = {-2, 1}, .LLU = {-2, -1}, .UUL = {-1, -2},
}

// Everything cross-references by index, never by pointer (as in the C++ WorldData).
Board_Id     :: distinct int
Location_Id  :: distinct int
Character_Id :: distinct int

Stat :: struct {
	set:      bool,
	value:    int,
	min, max: int,
}

Character :: struct {
	type:     Character_Type,
	location: Location_Id,
	stats:    [Statistic_Type]Stat,
}

Location :: struct {
	board:     Board_Id,
	character: Maybe(Character_Id),
	light:     bool,
	neighbors: [Knight_Move]Maybe(Location_Id),
}

Board :: struct {
	columns, rows: int,
	locations:     [dynamic]Maybe(Location_Id), // column + row*columns
}

Message :: struct {
	text:   string, // owned; freed by clear_messages
	fg, bg: Color,
}

World :: struct {
	boards:     [dynamic]Board,
	locations:  [dynamic]Location,
	characters: [dynamic]Character,
	recycled:   [dynamic]Character_Id, // kept sorted-by-lowest-first on take, like std::set
	avatar:     Maybe(Character_Id),
	messages:   [dynamic]Message,
}
