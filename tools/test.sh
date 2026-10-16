#!/bin/bash
# Native build, then the headless world checks and the 5000-tick random bot (menus and shoppe included).
set -euo pipefail
cd "$(dirname "$0")/.."
make odin-check >/dev/null
echo "tests ok"
