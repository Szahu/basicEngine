workspace "Engine"
	architecture "x64"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.sysstem}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"

include "Engine/vendor/glfw"

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "EGpch.h"
	pchsource "Engine/src/EGpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}"
	}

	libdirs
	{
		
	}


	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"EG_PLATFORM_WINDOWS",
			"EG_BUILD_DLL"
		}
	
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "EG_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "EG_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "EG_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"

	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/vendor/spdlog/include",
		"Engine/src"
	}

	links 
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"EG_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "EG_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "EG_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "EG_DIST"
		optimize "On"