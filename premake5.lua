workspace "PCSX2F-TTT-WidescreenFix"
	configurations { "Release", "Debug" }
	location "project_files"
	startproject "PCSX2F-TTT-WidescreenFix"

project "PCSX2F-TTT-WidescreenFix"
	kind "Makefile"
	language "C++"
	targetdir "output/plugins"
	targetname "PCSX2F-TTT-WidescreenFix"
	targetextension ".elf"
	vpaths { ["*"] = "source/plugin" }

	files {
		"source/plugin/**.cpp",
		"source/plugin/**.h",
		"source/plugin/module.json",
	}
	includedirs { "source/plugin", "external/injector/include" }

	debugdir "$(PCSX2_FORK_DIR)"
	debugcommand "$(PCSX2_FORK_DIR)/pcsx2-qtx64-avx2.exe"

	local build = 'powershell -NoProfile -ExecutionPolicy Bypass -File "%{wks.location}/../tools/build.ps1"'
	buildcommands { build }
	rebuildcommands { build .. " -Clean", build }
	cleancommands { build .. " -Clean" }
