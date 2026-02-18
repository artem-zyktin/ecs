#!/usr/bin/env bash
set -euo pipefail

PREMAKE="tools/premake/bin/linux/premake5"
SCRIPT="make-project.lua"

"$PREMAKE" --file="$SCRIPT" gmake2
