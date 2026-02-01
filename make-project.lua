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

	includedirs { "src" }

	files {
		"src/ecs/**.h",
		"src/ecs/**.hpp",
		"src/ecs/**.cpp",
		"make-project.lua"
	}

	filter "configurations:debug"
		defines { "_DEBUG" }
		symbols "On"

	filter "configurations:relase"
		defines { "NDEBUG" }
		optimize "Speed"
		symbols "On"
