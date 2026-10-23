[CmdletBinding()]
param([switch]$Clean)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$builder = Join-Path $root 'external/ps2sdk/plugins/build-module.ps1'
$project = Join-Path $root 'source/plugin/module.json'
if (!(Test-Path $builder)) { throw 'ps2sdk is missing, run tools/setup.ps1' }

& $builder -Project $project -Clean:$Clean
if ($Clean) { return }

$emulator = $env:PCSX2_FORK_DIR
if (!$emulator -or !(Test-Path $emulator)) { return }

$plugins = Join-Path $emulator 'plugins'
New-Item -ItemType Directory -Force -Path $plugins | Out-Null
Copy-Item (Join-Path $root 'output/plugins/PCSX2F-TTT-WidescreenFix.elf') $plugins -Force
$ini = Join-Path $plugins 'PCSX2F-TTT-WidescreenFix.ini'
if (!(Test-Path $ini)) { Copy-Item (Join-Path $root 'resources/PCSX2F-TTT-WidescreenFix.ini') $ini }
