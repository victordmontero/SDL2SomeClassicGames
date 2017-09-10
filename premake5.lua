require ("cmake")
require ("netbeans")
require ("vscode")

local assetsFolder = "assets"
local dlls = "libs/dlls"

workspace("SDL2ClassicGames")
	configurations({"Debug","Release"})
    platforms({"Linux","Windows"})

    filter("platforms:Windows")
        postbuildcommands({
            "echo %{prj.name}",
            "{COPY} ../"..dlls.." ../bin/%{cfg.platform}/%{prj.name}/%{cfg.buildcfg}"
        })

    project ("VictAsteroids")
		location("VictAsteroids")
		kind("ConsoleApp")
		language("C++")
        targetdir("bin/%{cfg.platform}/%{prj.name}/%{cfg.buildcfg}")

		files({"%{prj.name}/src/**.h","%{prj.name}/src/**.c","%{prj.name}/src/**.cpp"})

        postbuildcommands({
            "echo %{prj.name}",
            "{COPY} "..assetsFolder.." ../bin/%{cfg.platform}/%{prj.name}/%{cfg.buildcfg}/assets"
        })

		filter("configurations:Debug")
			defines ("DEBUG")
			symbols("On")

		filter("configurations:Release")
			defines("NDEBUG")
			optimize("On")

        filter({"platforms:Linux"})
            defines("LINUX")
            system("linux")
            links{"SDL2main","SDL2","SDL2_image","SDL2_ttf"}

        filter({"platforms:Windows"})
            system("windows")
            includedirs({
                "libs/SDL2_image-2.0.1/i686-w64-mingw32/include/SDL2",
                "libs/SDL2-2.0.5/i686-w64-mingw32/include/SDL2",
                "libs/SDL2_ttf-2.0.14/i686-w64-mingw32/include/SDL2"
                })
            libdirs({
                "libs/SDL2_image-2.0.1/i686-w64-mingw32/lib",
                "libs/SDL2-2.0.5/i686-w64-mingw32/lib",
                "libs/SDL2_ttf-2.0.14/i686-w64-mingw32/lib"})
            links{"Mingw32","SDL2main","SDL2","SDL2_image","SDL2_ttf"}
