workspace "OurPaint3D"
	architecture "x64"
	startproject "OurPaint3D"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "OurPaint3D/vendor/GLFW/include"
IncludeDir["glad"] = "OurPaint3D/vendor/glad/include"
IncludeDir["imgui"] = "OurPaint3D/vendor/imgui"
IncludeDir["glm"] = "OurPaint3D/vendor/glm"
IncludeDir["stb_image"] = "OurPaint3D/vendor/stb_image"

group "Dependencies"
	include "OurPaint3D/vendor/GLFW"
	include "OurPaint3D/vendor/glad"
	include "OurPaint3D/vendor/imgui"

group ""

project "OurPaint3D"
	location "OurPaint3D"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	toolset "clang"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "OurPaint3D/src/pch.cpp"

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	files
	{
		"%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
	}

	links
	{
		"GLFW",
		"glad",
		"imgui",
	}

	filter "system:windows"
		systemversion "latest"
		links
		{
			"opengl32.lib",
		}

	filter "system:not windows"
		links
		{
			"X11",
			"Xrandr",
			"Xinerama",
			"Xi",
			"Xxf86vm",
			"Xcursor",
			"dl",
			"GL",
			"m",
			"pthread",
		}

	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
