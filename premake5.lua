workspace "Engine"
	architecture "x64"
	startproject "Sandbox"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.sysstem}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "Engine/vendor/Glad/include"
IncludeDir["imgui"] = "Engine/vendor/imgui"
IncludeDir["glm"] = "Engine/vendor/glm"
IncludeDir["stb"] = "Engine/vendor/stb_image"
IncludeDir["assimp"] = "Engine/vendor/assimp/include"

include "Engine/vendor/GLFW"
include "Engine/vendor/Glad"
include "Engine/vendor/imgui"


project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "EGpch.h"
	pchsource "Engine/src/EGpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"EG_ENABLE_ASSERTS"
	}

	links
	{
		"GLFW",
		"Glad",
		"imgui",
		"opengl32.lib", 
		"assimp-vc142-mtd.lib",
		"Winmm.lib"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{prj.name}/src/Engine/Core",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.assimp}"
	}

	libdirs
	{
		"%{prj.name}/vendor/assimp/lib",
		"%{prj.name}/vendor/SDL/lib/x64"
	}


	filter "system:windows"
		systemversion "latest"

		defines
		{
			"EG_PLATFORM_WINDOWS",
			"EG_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "EG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "EG_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "EG_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	staticruntime "on"
	language "C++"
	cppdialect "C++17"

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
		"%{IncludeDir.imgui}/../",
		"Engine/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.assimp}"
	}

	links 
	{
		"Engine"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"EG_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "EG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "EG_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "EG_DIST"
		runtime "Release"
		optimize "On"