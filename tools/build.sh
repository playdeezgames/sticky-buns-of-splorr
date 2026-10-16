#!/bin/bash
# Builds the browser release bundle in dist/web (page, odin.js, wasm, NOTICES.txt). Needs ~/ODIN/odin.
set -euo pipefail
cd "$(dirname "$0")/.."
make odin-dist
