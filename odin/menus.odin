package sbos

import "core:fmt"
import "core:strings"

Menu_Action :: enum {
	Continue, Restart, Use_Potion, // game menu
	Leave, Buy_Potion, Increase_Max_Supplies, Buy_Flogger, Buy_Armour, Buy_Antitrap_Spray, // shoppe
}

Menu_Item :: struct {
	action: Menu_Action,
	label:  string, // owned
}

// Shared by the game menu and the shoppe (both are the same list-and-cursor widget in C++).
// Items are built lazily on the first update after entering, like InitializeMenuItems.
Menu_State :: struct {
	items: [dynamic]Menu_Item,
	index: int,
}

menu_add :: proc(m: ^Menu_State, action: Menu_Action, label: string) {
	append(&m.items, Menu_Item{action, strings.clone(label)})
}

menu_clear :: proc(m: ^Menu_State) {
	for item in m.items do delete(item.label)
	clear(&m.items)
}

menu_destroy :: proc(m: ^Menu_State) {
	menu_clear(m)
	delete(m.items)
}

menu_draw :: proc(g: ^Game, m: ^Menu_State, first_row: int) {
	fb_fill(&g.fb, 0, 0, VIEW_COLUMNS, VIEW_ROWS, 0, .Black, .Black)
	for item, i in m.items {
		selected := i == m.index
		fb_write_text(&g.fb, 0, first_row + i, item.label, .Black if selected else .White, .White if selected else .Black)
	}
}

// Shared command loop. Returns the next state; `choose` handles GREEN.
// YELLOW leaves and always clears the items (the C++ kept stale items on YELLOW/Leave,
// which let the shoppe offer things the avatar could no longer afford).
menu_update :: proc(g: ^Game, m: ^Menu_State, self: Game_State, build: proc(^Game, ^Menu_State), choose: proc(^Game, ^Menu_State, Menu_Action) -> Game_State) -> Game_State {
	next := self
	for next == self {
		if len(m.items) == 0 do build(g, m)
		cmd := command_read(g) or_break
		#partial switch cmd {
		case .Yellow: next = .Room
		case .Up:     m.index = (m.index + len(m.items) - 1) % len(m.items)
		case .Down:   m.index = (m.index + 1) % len(m.items)
		case .Green:  next = choose(g, m, m.items[m.index].action)
		}
	}
	if next != self {
		menu_clear(m)
	} else if len(m.items) == 0 {
		build(g, m)
	}
	return next
}

// ---- game menu -------------------------------------------------------

menu_build :: proc(g: ^Game, m: ^Menu_State) {
	w := &g.world
	av := w.avatar.?
	menu_add(m, .Continue, "Continue")
	menu_add(m, .Restart, "Restart")
	if stat(w, av, .Health) > 0 && stat(w, av, .Potions) > 0 {
		menu_add(m, .Use_Potion, "Use Potion")
	}
	m.index = 0
}

menu_choose :: proc(g: ^Game, m: ^Menu_State, a: Menu_Action) -> Game_State {
	w := &g.world
	#partial switch a {
	case .Restart:
		world_initialize(w)
	case .Use_Potion:
		av := w.avatar.?
		benefit := stat_max(w, av, .Health) - stat(w, av, .Health)
		change_stat(w, av, .Health, benefit)
		change_stat(w, av, .Potions, -1)
		msg(w, .Light_Red, .Black, "+%d Health", benefit)
		msg(w, .Red, .Black, "-1 Potion")
	}
	return .Room // Continue, Restart and Use_Potion all return to the room
}

menu_update_game :: proc(g: ^Game) -> Game_State {
	return menu_update(g, &g.menu, .Game_Menu, menu_build, menu_choose)
}

menu_draw_game :: proc(g: ^Game) {
	menu_draw(g, &g.menu, 2)
	fb_write_text(&g.fb, 0, 0, "Game Menu:", .White)
}

// ---- shoppe ----------------------------------------------------------

Shoppe_Prices :: struct {
	supplies, potion, flogger, armour, spray: int,
}

shoppe_prices :: proc(w: ^World) -> Shoppe_Prices {
	av := w.avatar.?
	level := stat(w, av, .XP_Level)
	armour, _ := get_stat(w, av, .Armour)
	return {
		supplies = stat_max(w, av, .Supplies) * level,
		potion   = 10 * level,
		flogger  = 25 * level,
		armour   = (1 + armour) * level,
		spray    = 1 * level,
	}
}

shoppe_item_price :: proc(p: Shoppe_Prices, a: Menu_Action) -> int {
	#partial switch a {
	case .Increase_Max_Supplies: return p.supplies
	case .Buy_Potion:            return p.potion
	case .Buy_Flogger:           return p.flogger
	case .Buy_Armour:            return p.armour
	case .Buy_Antitrap_Spray:    return p.spray
	}
	return 0
}

shoppe_build :: proc(g: ^Game, m: ^Menu_State) {
	w := &g.world
	av := w.avatar.?
	jools, _ := get_stat(w, av, .Jools)
	p := shoppe_prices(w)
	menu_add(m, .Leave, "Leave")
	if jools >= p.supplies do menu_add(m, .Increase_Max_Supplies, fmt.tprintf("+1 Maximum Buns(%d)", p.supplies))
	if jools >= p.potion   do menu_add(m, .Buy_Potion, fmt.tprintf("+1 Potion(%d)", p.potion))
	if jools >= p.armour   do menu_add(m, .Buy_Armour, fmt.tprintf("+1 Armour(%d)", p.armour))
	if jools >= p.flogger  do menu_add(m, .Buy_Flogger, fmt.tprintf("+1 Flogger(%d)", p.flogger))
	if jools >= p.spray    do menu_add(m, .Buy_Antitrap_Spray, fmt.tprintf("+1 Anti-trap Spray(%d)", p.spray))
	m.index = 0
}

shoppe_choose :: proc(g: ^Game, m: ^Menu_State, a: Menu_Action) -> Game_State {
	w := &g.world
	av := w.avatar.?
	p := shoppe_prices(w)
	#partial switch a {
	case .Leave:
		return .Room
	case .Increase_Max_Supplies:
		change_stat(w, av, .Jools, -p.supplies)
		set_stat_max(w, av, .Supplies, stat_max(w, av, .Supplies) + 1)
	case .Buy_Potion:
		change_stat(w, av, .Jools, -p.potion)
		change_stat(w, av, .Potions, 1)
	case .Buy_Flogger:
		change_stat(w, av, .Jools, -p.flogger)
		change_stat(w, av, .Floggers, 1)
	case .Buy_Armour:
		change_stat(w, av, .Jools, -p.armour)
		change_stat(w, av, .Armour, 1)
	case .Buy_Antitrap_Spray:
		change_stat(w, av, .Jools, -p.spray)
		change_stat(w, av, .Antitrap_Spray, 1)
	}
	menu_clear(m) // rebuild offers (affordability changed) on the next update
	return .Shoppe
}

shoppe_update :: proc(g: ^Game) -> Game_State {
	return menu_update(g, &g.shoppe, .Shoppe, shoppe_build, shoppe_choose)
}

shoppe_draw :: proc(g: ^Game) {
	menu_draw(g, &g.shoppe, 3)
	fb_write_text(&g.fb, 0, 0, "Shoppe:", .White)
	jools, _ := get_stat(&g.world, g.world.avatar.?, .Jools)
	fb_write_text(&g.fb, 0, 1, fmt.tprintf("Jools: %d", jools), .Light_Green)
}
