#+build js
package sbos

import "core:sys/wasm/js"

foreign import sbos_js "sbos"

@(default_calling_convention = "contextless")
foreign sbos_js {
	// Implemented in web/index.html: puts a VIEW_WIDTH x VIEW_HEIGHT RGBA buffer on the canvas.
	present :: proc(pixels: [^]u8, width, height: i32) ---
}

// The browser drives the frame loop by calling the exported `step`, so state is global.
@(private = "file")
web: struct {
	game:   Game,
	pixels: [VIEW_WIDTH * VIEW_HEIGHT * 4]u8,
}

web_key_command :: proc(key: string) -> (cmd: Command, ok: bool) {
	switch key {
	case "w", "W", "z", "Z", "ArrowUp":    return .Up, true
	case "a", "A", "q", "Q", "ArrowLeft":  return .Left, true
	case "s", "S", "ArrowDown":            return .Down, true
	case "d", "D", "ArrowRight":           return .Right, true
	case " ":                              return .Green, true
	case "Backspace", "Escape":            return .Red, true
	case "Tab":                            return .Yellow, true
	case "Enter":                          return .Blue, true
	}
	return
}

on_key_down :: proc(e: js.Event) {
	if cmd, ok := web_key_command(e.key.key); ok {
		command_write(&web.game, cmd)
		js.event_prevent_default() // keep Tab/Space/arrows from moving focus or scrolling
	}
}

// Called once from main (before the first frame).
platform_run :: proc() {
	game_init(&web.game)
	js.add_window_event_listener(.Key_Down, nil, on_key_down)
}

@(export)
step :: proc(dt: f64) -> (keep_going: bool) {
	g := &web.game
	game_update(g)
	game_draw(g)
	render_to_rgba(&g.fb, web.pixels[:])
	present(raw_data(web.pixels[:]), VIEW_WIDTH, VIEW_HEIGHT)
	free_all(context.temp_allocator)
	return true
}
