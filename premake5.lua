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
		targetname "libcJSON_d"
	
	filter "configurations:Release"
		targetname "libcJSON"
 
project "sachiexe"
	kind "ConsoleApp"

	files {
		"sachi/*.h",
		"sachi/*.c",
		"sachi/*.cpp",
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
		"test/*.h",
		"test/*.c",
		"test/*.cpp"
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