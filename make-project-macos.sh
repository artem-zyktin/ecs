#!/usr/bin/env bash
set -euo pipefail

PREMAKE="tools/premake/bin/macos/premake5"
SCRIPT="make-project.lua"

"$PREMAKE" --file="$SCRIPT" xcode4
