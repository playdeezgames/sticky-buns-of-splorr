#!/bin/bash
# Prepares the itch.io build: tests, the web bundle, a zip for inspection. Pushes ONLY with --push. Never commits, never deletes
# anything outside build/ and dist/. Usage: tools/ship.sh [--push]
set -euo pipefail
cd "$(dirname "$0")/.."
# The page: https://thegrumpygamedev.itch.io/sticky-buns-of-splorr
TARGET="thegrumpygamedev/sticky-buns-of-splorr:html"
ZIP="build/sticky-buns-html5.zip"

tools/test.sh
tools/build.sh
mkdir -p build
rm -f "$ZIP"
(cd dist/web && zip -q -9 -r "../../$ZIP" .)
echo "zip: $ZIP ($(du -h "$ZIP" | cut -f1)); wasm $(du -h dist/web/sbos.wasm | cut -f1)"
unzip -l "$ZIP" | tail -8

if [ "${1:-}" = "--push" ]; then
	if [ -n "$(git status --porcelain)" ]; then echo "note: the working tree has uncommitted changes; what is pushed is the working tree, not a commit"; fi
	butler push dist/web "$TARGET"
	echo "pushed to $TARGET. It goes live after itch.io processes it; check: butler status $TARGET"
else
	echo "not pushed (run tools/ship.sh --push when you want it live; the target is $TARGET)"
fi
