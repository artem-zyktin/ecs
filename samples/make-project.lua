group "samples"

project "sample_basic_usage"
	location  "build/samples/basic_usage"

	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	targetdir "bin/%{cfg.system}/%{cfg.buildcfg}/output"
	objdir    "bin/%{cfg.system}/%{cfg.buildcfg}/intermediate"

	enablepch "Off"

	includedirs { "src/include" }
	links { "ecs" }

	files {
		"samples/basic_usage/**.cpp",
		"samples/basic_usage/**.h",
		"samples/basic_usage/**.hpp"
	}

	filter "configurations:debug"
		defines { "_DEBUG" }
		symbols "On"

	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "Speed"
		symbols "On"

	filter {}

group ""
