workspace "ecs_samples"
	location "../build/samples"
	architecture "x86_64"
	configurations { "debug", "release" }

project "ecs"
	location  "../build/samples/ecs"

	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	targetdir "../bin/%{cfg.system}/%{cfg.buildcfg}/output"
	objdir    "../bin/%{cfg.system}/%{cfg.buildcfg}/intermediate"

	enablepch "Off"

	includedirs { "../src/include" }

	files {
		"../src/sources/**.cpp",
		"../src/sources/**.h",
		"../src/sources/**.hpp",
		"../src/include/ecs/**.h",
		"../src/include/ecs/**.hpp"
	}

	filter "configurations:debug"
		defines { "_DEBUG" }
		symbols "On"

	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "Speed"
		symbols "On"

project "sample_basic_usage"
	location  "../build/samples/basic_usage"

	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	targetdir "../bin/%{cfg.system}/%{cfg.buildcfg}/output"
	objdir    "../bin/%{cfg.system}/%{cfg.buildcfg}/intermediate"

	enablepch "Off"

	includedirs { "../src/include" }
	links { "ecs" }

	files {
		"basic_usage/**.cpp",
		"basic_usage/**.h",
		"basic_usage/**.hpp"
	}

	filter "configurations:debug"
		defines { "_DEBUG" }
		symbols "On"

	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "Speed"
		symbols "On"
