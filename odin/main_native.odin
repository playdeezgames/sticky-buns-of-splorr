#+build !js
package sbos

import "core:os"

// `--check [out.ppm]` runs the headless sanity checks; `--shots <dir>` writes store-page screenshots (check.odin);
// otherwise the SDL2 loop starts.
main :: proc() {
	if len(os.args) > 1 && os.args[1] == "--check" {
		check_main()
		return
	}
	if len(os.args) > 2 && os.args[1] == "--shots" {
		shots_main(os.args[2])
		return
	}
	platform_run()
}
