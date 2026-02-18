include "scripts/postbuild.lua"

workspace "ecs"
	location "build/"
	architecture "x86_64"
	configurations { "debug", "release" }

project "ecs"
	location  "build/"

	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	targetdir "bin/%{cfg.system}/%{cfg.buildcfg}/output"
	objdir    "bin/%{cfg.system}/%{cfg.buildcfg}/intermediate"

	enablepch "Off"

	includedirs { "src/include" }

	-- source files
	files {
		"src/sources/**.cpp",
		"src/sources/**.h",
		"src/sources/**.hpp",
		"src/include/ecs/**.h",
		"src/include/ecs/**.hpp"
	}

	-- project files
	files {
		"make-project.lua",
		"scripts/postbuild.lua"
	}
	
	postbuild()

	filter "system:windows"
		files {
			"make-project-windows.bat"
		}

	filter "system:linux"
		files {
			"make-project-linux.sh"
		}

	filter "system:macosx"
		files {
			"make-project-macos.sh"
		}

	filter "configurations:debug"
		defines { "_DEBUG" }
		symbols "On"

	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "Speed"
		symbols "On"
