#+build !js
package sbos

import "core:fmt"
import "core:os"
import "core:strings"

SYMBOLS := [Character_Type]u8 {
	.Knight = 'N', .Block = 'X', .Sticky_Buns = '@', .Butthole = '*',
	.Pawn = 'p', .Bishop = 'B', .Flogger = '!', .Potion = '&', .Shoppe = '$',
}

// Headless check: build a world and print the board and avatar stats.
check_main :: proc() {
	w: World
	defer world_destroy(&w)
	world_initialize(&w)

	b := character_board(&w, w.avatar.?)
	bd := w.boards[b]
	for row in 0 ..< bd.rows {
		for column in 0 ..< bd.columns {
			ch := u8('.')
			loc := board_location(&w, b, column, row).?
			if c, ok := location_character(&w, loc).?; ok do ch = SYMBOLS[w.characters[c].type]
			fmt.printf("%c ", ch)
		}
		fmt.println()
	}

	av := w.avatar.?
	for t in Statistic_Type {
		if v, ok := get_stat(&w, av, t); ok {
			fmt.printfln("%v = %v (max %v)", t, v, stat_max(&w, av, t))
		}
	}

	// Sanity: knight-move neighbor counts (corner=2, centre=8) and symmetry.
	corner := board_location(&w, b, 0, 0).?
	centre := board_location(&w, b, 3, 3).?
	count :: proc(w: ^World, l: Location_Id) -> (n: int) {
		for m in Knight_Move do if location_neighbor(w, l, m) != nil do n += 1
		return
	}
	fmt.printfln("neighbors corner=%v centre=%v", count(&w, corner), count(&w, centre))
	assert(count(&w, corner) == 2 && count(&w, centre) == 8)
	assert(board_has_character_type(&w, b, .Pawn))
	assert(!board_has_character_type(&w, b, .Bishop))

	// Restart must not leak state.
	world_initialize(&w)
	assert(len(w.characters) == 11)
	fmt.println("ok")
	bot_play(5000)

	// Render a test frame to a PPM (pass a path as the first argument).
	if len(os.args) > 2 {
		fb: Frame_Buffer
		fb_fill(&fb, 0, 0, VIEW_COLUMNS, VIEW_ROWS, 0, .Black, .Black)
		fb_write_text(&fb, 25, 0, "Buns: 15/15", .Magenta)
		fb_write_text(&fb, 25, 1, "Health: 3/3", .Light_Red)
		for row in 0 ..< bd.rows {
			for column in 0 ..< bd.columns {
				loc := board_location(&w, b, column, row).?
				light := w.locations[loc].light
				fb_fill(&fb, column * 3, row * 3, 3, 3, 0, .Black, .Light_Cyan if light else .Cyan)
				if c, ok := location_character(&w, loc).?; ok {
					fb_set_cell(&fb, column * 3 + 1, row * 3 + 1, SYMBOLS[w.characters[c].type], .Black)
				}
			}
		}
		fb_set_cell(&fb, 0, 24, 0xc9, .White, .Blue)
		pixels := make([]u8, VIEW_WIDTH * VIEW_HEIGHT * 4)
		defer delete(pixels)
		render_to_rgba(&fb, pixels)
		sb := strings.builder_make()
		defer strings.builder_destroy(&sb)
		fmt.sbprintf(&sb, "P6\n%d %d\n255\n", VIEW_WIDTH, VIEW_HEIGHT)
		for i := 0; i < len(pixels); i += 4 {
			strings.write_bytes(&sb, pixels[i:i + 3])
		}
		_ = os.write_entire_file(os.args[2], sb.buf[:])
	}
}

// Random bot: feeds random commands through the real game loop and checks invariants.
bot_play :: proc(ticks: int) {
	g: Game
	defer game_destroy(&g)
	game_init(&g)
	pixels := make([]u8, VIEW_WIDTH * VIEW_HEIGHT * 4)
	defer delete(pixels)
	restarts := 0
	menu_cmds := [4]Command{.Up, .Down, .Green, .Green}
	visits: [Game_State]int
	for _ in 0 ..< ticks {
		// Aim the cursor at a random knight-move neighbour half the time so moves actually happen.
		g.room.x, g.room.y = rng_range(0, BOARD_COLUMNS - 1), rng_range(0, BOARD_ROWS - 1)
		queue_push(&g, .Green)
		if g.state == .Room && rng_range(0, 29) == 0 do g.state = .Shoppe // shoppe is hard to reach randomly
		if g.state != .Room {
			for _ in 0 ..< rng_range(1, 4) do queue_push(&g, menu_cmds[rng_range(0, 3)])
			if rng_range(0, 4) == 0 do queue_push(&g, .Yellow)
			if rng_range(0, 3) == 0 do set_stat(&g.world, g.world.avatar.?, .Jools, rng_range(0, 60))
		}
		if rng_range(0, 9) == 0 do queue_push(&g, .Yellow)
		if rng_range(0, 9) == 0 do queue_push(&g, .Red)
		game_update(&g)
		game_draw(&g)
		render_to_rgba(&g.fb, pixels)

		av := g.world.avatar.?
		knights := 0
		for c in g.world.characters do if c.type == .Knight do knights += 1
		assert(knights >= 1)
		if g.state == .Shoppe {
			// every offered item must be affordable right now
			p := shoppe_prices(&g.world)
			jools := stat(&g.world, av, .Jools)
			for item in g.shoppe.items {
				price := shoppe_item_price(p, item.action)
				assert(price <= jools)
			}
		}
		hp, _ := get_stat(&g.world, av, .Health)
		assert(hp <= stat_max(&g.world, av, .Health))
		visits[g.state] += 1
		if hp <= 0 && g.state == .Room {
			world_initialize(&g.world)
			restarts += 1
		}
		free_all(context.temp_allocator)
	}
	fmt.printfln("bot: %d ticks ok, %d deaths/restarts, visits %v", ticks, restarts, visits)
}

queue_push :: proc(g: ^Game, c: Command) {
	command_write(g, c)
}

// ---- art generation ----------------------------------------------------

write_ppm :: proc(path: string, fb: ^Frame_Buffer) {
	pixels := make([]u8, VIEW_WIDTH * VIEW_HEIGHT * 4)
	defer delete(pixels)
	render_to_rgba(fb, pixels)
	sb := strings.builder_make()
	defer strings.builder_destroy(&sb)
	fmt.sbprintf(&sb, "P6\n%d %d\n255\n", VIEW_WIDTH, VIEW_HEIGHT)
	for i := 0; i < len(pixels); i += 4 {
		strings.write_bytes(&sb, pixels[i:i + 3])
	}
	err := os.write_entire_file(path, sb.buf[:])
	assert(err == nil, "could not write ppm")
}

// One real knight move: aims the cursor at a legal destination (preferring things worth hitting) and presses Green.
shots_move :: proc(g: ^Game) {
	w := &g.world
	av := w.avatar.?
	from := w.characters[av].location
	b := w.locations[from].board
	best := -1
	bx, by: int
	for m in Knight_Move {
		dest := location_neighbor(w, from, m).? or_continue
		score := 1
		if c, ok := location_character(w, dest).?; ok {
			#partial switch w.characters[c].type {
			case .Block:    continue
			case .Butthole: score = 4
			case .Sticky_Buns: score = 3
			case .Pawn:     score = 2
			}
		}
		if score > best {
			best = score
			bx, by = int(w.boards[b].columns), 0 // placeholder, replaced below
			for column in 0 ..< w.boards[b].columns {
				for row in 0 ..< w.boards[b].rows {
					if board_location(w, b, column, row).? == dest do bx, by = column, row
				}
			}
		}
	}
	if best < 0 do return
	g.room.x, g.room.y = bx, by
	command_write(g, .Green)
	game_update(g)
}

// `--shots <dir>`: plays a short real game and writes 01-play.ppm, 02-menu.ppm, 03-shoppe.ppm for tools/make_cover.py.
shots_main :: proc(dir: string) {
	g: Game
	defer game_destroy(&g)
	game_init(&g)
	w := &g.world
	av := w.avatar.?

	for moves := 0; moves < 14; {
		if stat(w, av, .Health) <= 0 {
			world_initialize(w)
			av = w.avatar.?
			moves = 0
		}
		shots_move(&g)
		moves += 1
	}
	set_stat(w, av, .Jools, 34)
	g.state = .Room
	game_draw(&g)
	write_ppm(fmt.tprintf("%s/01-play.ppm", dir), &g.fb)

	set_stat(w, av, .Potions, 1)
	set_stat(w, av, .Health, 1)
	command_write(&g, .Yellow)
	game_update(&g)
	game_update(&g)
	game_draw(&g)
	write_ppm(fmt.tprintf("%s/02-menu.ppm", dir), &g.fb)

	command_write(&g, .Yellow)
	game_update(&g)
	set_stat(w, av, .Jools, 60)
	set_stat(w, av, .XP_Level, 2)
	g.state = .Shoppe
	game_update(&g)
	game_draw(&g)
	write_ppm(fmt.tprintf("%s/03-shoppe.ppm", dir), &g.fb)
	fmt.println("wrote shots to", dir)
}
