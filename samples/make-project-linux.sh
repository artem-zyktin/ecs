#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
PREMAKE="$REPO_DIR/tools/premake/bin/linux/premake5"
SCRIPT="$SCRIPT_DIR/make-project-standalone.lua"

"$PREMAKE" --file="$SCRIPT" gmake2
