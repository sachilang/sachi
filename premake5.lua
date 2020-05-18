#!lua 
solution "Sachi"
	configurations { "Debug", "Release" }
 	platforms { "Win32", "Win64" }
	location "vs2019"
	language "C++"
	
	libdirs {
		"lib"
	}

  	filter "platforms:Win32"
		architecture "x32"

  	filter "platforms:Win64"
		architecture "x64"

project "cJSON"
	kind "StaticLib"

	files {
		"external/cJSON/*.h",
		"external/cJSON/*.c"
	}

	includedirs {
		"external/cJSON"
	}
	
	excludes {
		"external/cJSON/test.c"
	}
	
	targetdir "lib"

	defines {"CJSON_EXPORT_SYMBOLS"}
	
	filter "configurations:Debug"
		targetname "libcjson_d"
	
	filter "configurations:Release"
		targetname "libcjson"
 
project "sachiexe"
	kind "ConsoleApp"

	files {
		"sachi/*.h",
		"sachi/*.c",
		"sachi/*.cpp",
		"sachi/object/*.h",
		"sachi/object/*.c",
		"sachi/node/*.h",
		"sachi/node/*.c",
	}

	includedirs {
		"./",
		"external"
	}

	links {
		"cJSON"
	}
	
	libdirs { "lib" }
	
	targetdir "./"
	
	debugdir "%{prj.location}/.."
	
	defines {
		"CJSON_HIDE_SYMBOLS",
		"SACHI_HIDE_SYMBOLS"
	}

	warnings 'Extra'
 
project "test"
	kind "ConsoleApp"

	files {
		"sachi/*.h",
		"sachi/*.c",
		"sachi/object/*.h",
		"sachi/object/*.c",
		"sachi/node/*.h",
		"sachi/node/*.c",
		"test/*.h",
		"test/*.cpp",
	}

	includedirs {
		"./",
		"external"
	}
	
	excludes {
		"sachi/node.c",
		"sachi/parser.c",
	}

	links {
		"cJSON"
	}
	
	libdirs { "lib" }
	
	targetdir "./"
	
	debugdir "%{prj.location}/.."
	
	defines {
		"CJSON_HIDE_SYMBOLS",
		"SACHI_HIDE_SYMBOLS",
		"WITH_MEMORY_TRACKER"
	}

	warnings 'Extra'