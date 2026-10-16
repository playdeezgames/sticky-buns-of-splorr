#+build !js
package sbos

import "core:fmt"
import sdl "vendor:sdl2"

SCALE :: 4
GAME_TITLE :: "Sticky Buns of SPLORR!!"

keycode_command :: proc(k: sdl.Keycode) -> (cmd: Command, ok: bool) {
	#partial switch k {
	case .w, .z, .UP:      return .Up, true
	case .a, .q, .LEFT:    return .Left, true
	case .s, .DOWN:        return .Down, true
	case .d, .RIGHT:       return .Right, true
	case .SPACE:           return .Green, true
	case .BACKSPACE, .ESCAPE: return .Red, true
	case .TAB:             return .Yellow, true
	case .RETURN:          return .Blue, true
	}
	return
}

platform_run :: proc() {
	if sdl.Init({.VIDEO}) != 0 {
		fmt.eprintln("SDL_Init failed:", sdl.GetError())
		return
	}
	defer sdl.Quit()

	window := sdl.CreateWindow(GAME_TITLE, sdl.WINDOWPOS_CENTERED, sdl.WINDOWPOS_CENTERED,
		VIEW_WIDTH * SCALE, VIEW_HEIGHT * SCALE, {.SHOWN})
	defer sdl.DestroyWindow(window)
	renderer := sdl.CreateRenderer(window, -1, sdl.RENDERER_PRESENTVSYNC)
	defer sdl.DestroyRenderer(renderer)
	sdl.RenderSetLogicalSize(renderer, VIEW_WIDTH, VIEW_HEIGHT)
	texture := sdl.CreateTexture(renderer, .RGBA32, .STREAMING, VIEW_WIDTH, VIEW_HEIGHT)
	defer sdl.DestroyTexture(texture)

	g: Game
	game_init(&g)
	defer game_destroy(&g)
	pixels := make([]u8, VIEW_WIDTH * VIEW_HEIGHT * 4)
	defer delete(pixels)

	for !g.quit {
		event: sdl.Event
		for sdl.PollEvent(&event) {
			#partial switch event.type {
			case .QUIT:
				g.quit = true
			case .KEYDOWN:
				if cmd, ok := keycode_command(event.key.keysym.sym); ok {
					command_write(&g, cmd)
				}
			}
		}
		game_update(&g)
		game_draw(&g)
		render_to_rgba(&g.fb, pixels)
		sdl.UpdateTexture(texture, nil, raw_data(pixels), VIEW_WIDTH * 4)
		sdl.RenderCopy(renderer, texture, nil, nil)
		sdl.RenderPresent(renderer)
		free_all(context.temp_allocator)
	}
}
