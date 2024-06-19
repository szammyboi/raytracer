---@diagnostic disable: lowercase-global, undefined-global

require "ninja"

workspace "Raytracing Assigment"
    configurations {"release", "debug"}
    architecture "x86_64"
    flags "MultiProcessorCompile"
    toolset "gcc"

outputdir = "%{cfg.buildcfg}"

project "GLFW"
	kind "StaticLib"
    systemversion "latest"
	language "C"
	staticruntime "off"
	warnings "off"

	targetdir ("build/bin/" .. outputdir)
    objdir ("build/bin-int/")

	files
	{
		"./vendor/glfw/include/GLFW/glfw3.h",
		"./vendor/glfw/include/GLFW/glfw3native.h",
		"./vendor/glfw/src/**.h",
        "./vendor/glfw/src/**.c"
	}

	defines 
	{ 
		"_CRT_SECURE_NO_WARNINGS"
	}

    links 
    {
        "gdi32"
    }
    
	filter "configurations:debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:release"
		runtime "Release"
		optimize "speed"

    filter "system:linux"
        pic "On"
        defines "_GLFW_X11"

    filter "system:windows"
        defines "_GLFW_WIN32"

project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
    targetdir ("build/bin/" .. outputdir)
    objdir ("build/bin-int/")

    files
    {
        "./vendor/glad/include/glad/gl.h",
        "./vendor/glad/include/KHR/khrplatform.h",
        "./vendor/glad/src/gl.c"
    }

    includedirs
    {
        "./vendor/glad/include"
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:release"
        runtime "Release"
        optimize "on"


project "imgui"
    kind "StaticLib"
    language "C++"

    targetdir ("build/bin/" .. outputdir)
    objdir ("build/bin-int/")

    includedirs 
    {
        "vendor/imgui",
        "vendor/glad/include",
        "vendor/glfw/include"
    }

    files
    {
        "vendor/imgui/*.cpp",
        "vendor/imgui/backends/imgui_impl_glfw.cpp",
        "vendor/imgui/backends/imgui_impl_opengl3.cpp",
        "vendor/imgui/misc/cpp/**.cpp"
    }

    defines 
    {
        "_IMGUI_WIN32",
        "_CRT_SECURE_NO_WARNINGS",
        "IMGUI_IMPL_OPENGL_LOADER_GLAD"
    }

    systemversion "latest"
    cppdialect "C++17"
    staticruntime "On"

project "Raytracer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("build/bin/" .. outputdir)
    objdir ("build/bin-int/")

    symbols "on"
    buildoptions {"-Werror", "-Wuninitialized", "-Wno-narrowing"}
    
    defines {"YAML_CPP_STATIC_DEFINE"}

    files 
    { 
        "src/**.h",
        "src/**.cpp",
        "./vendor/glad/include/gl.h"
    }

    includedirs
    {
        "./vendor/glm",
        "./vendor/glfw/include",
        "./vendor/glad/include",
        "./vendor/stb/include",
        "./vendor/imgui",
        "./vendor/imgui/backends",
        "./vendor/imgui/misc/cpp",
    }

    links
    {
        "glfw",
        "Glad",
        "imgui",
        "opengl32",
        "gdi32",
    }
