local install_dir = "../bin/%{cfg.system}/%{cfg.buildcfg}/install"
local install_include_dir = install_dir .. "/include"
local install_lib_dir = install_dir .. "/lib"

local premake_cmd
local host_os = os.host()

if host_os == "windows" then
	premake_cmd = "../tools/premake/bin/windows/premake5.exe"
elseif host_os == "macos" then
	premake_cmd = "../tools/premake/bin/macos/premake5"
elseif host_os == "linux" then
	premake_cmd = "../tools/premake/bin/linux/premake5"
end

function postbuild()
	postbuildcommands {
		"{RMDIR} " .. install_dir,
		"{MKDIR} " .. install_dir,
        "{MKDIR} " .. install_include_dir,
        "{MKDIR} " .. install_lib_dir,
		'{COPY} "../src/ecs/include/" "'   .. install_include_dir .. '"',
		'{COPYFILE} "%{cfg.buildtarget.abspath}" "' .. install_lib_dir .. '"'
	}
end

