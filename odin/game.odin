package sbos

import "core:container/queue"

Command :: enum { Up, Right, Down, Left, Green, Red, Blue, Yellow }

Game_State :: enum { Room, Game_Menu, Shoppe }

Game :: struct {
	world:    World,
	fb:       Frame_Buffer,
	commands: queue.Queue(Command),
	state:    Game_State,
	room:     Room_State,
	menu:     Menu_State,
	shoppe:   Menu_State,
	quit:     bool,
}

game_init :: proc(g: ^Game) {
	queue.init(&g.commands)
	g.state = .Room
	world_initialize(&g.world)
}

game_destroy :: proc(g: ^Game) {
	queue.destroy(&g.commands)
	menu_destroy(&g.menu)
	menu_destroy(&g.shoppe)
	world_destroy(&g.world)
}

command_write :: proc(g: ^Game, c: Command) {
	queue.push_back(&g.commands, c)
}

command_read :: proc(g: ^Game) -> (cmd: Command, ok: bool) {
	if queue.len(g.commands) == 0 do return
	return queue.pop_front(&g.commands), true
}

// One tick: drain commands in the active state, possibly switching states.
game_update :: proc(g: ^Game) {
	switch g.state {
	case .Room:      g.state = room_update(g)
	case .Game_Menu: g.state = menu_update_game(g)
	case .Shoppe:    g.state = shoppe_update(g)
	}
}

game_draw :: proc(g: ^Game) {
	switch g.state {
	case .Room:      room_draw(g)
	case .Game_Menu: menu_draw_game(g)
	case .Shoppe:    shoppe_draw(g)
	}
}
