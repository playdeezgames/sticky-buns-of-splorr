#+build js
package sbos

// `main` runs once at module start; the page then calls the exported `step` each frame.
main :: proc() {
	platform_run()
}
