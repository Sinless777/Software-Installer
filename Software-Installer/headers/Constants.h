#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Structs.h"

using namespace std;

// -----------------------------------------------------------------------------
// Windows Software Installer URLs
// -----------------------------------------------------------------------------
// These extern constants hold the download links for the latest Windows installers.
// When a new version is released, update the URL below to point directly to the
// desired installer binary (usually an .exe or .msi).
// Example usage: download URL via Software_Downloader::downloadUrl(Windows_Obsidian_URL, destDir);
extern const string Windows_Obsidian_URL;
extern const string Windows_Discord_URL;
extern const string Windows_NotepadPlusPlus_URL;
extern const string Windows_VSCode_URL;
extern const string Windows_7Zip_URL;
extern const string Windows_VisualStudio_URL;

// -----------------------------------------------------------------------------
// Linux Software Installer URLs
// -----------------------------------------------------------------------------
// (Enable support for Linux targets by adding extern declarations below.)
// You can add entries for .deb, .rpm or shell scripts, then use downloadUrl
// and Installer::install in a Linux branch of your setup code.
// extern const string Linux_<AppName>_URL;

// -----------------------------------------------------------------------------
// Software_List Vector
// -----------------------------------------------------------------------------
// Defines the ordered list of applications presented to the user.
// Each Software struct includes:
//   • name:        Display name in the menu
//   • url:         Installer download URL (use one of the constants above)
//   • silentFlags: Command-line flags for unattended installation
//                  (most installers document /S, /quiet, etc.)
// Modify this vector in Constants.cpp to add, remove, or reorder apps.
extern const vector<Software> Software_List;

// -----------------------------------------------------------------------------
// Software_Installers Map
// -----------------------------------------------------------------------------
// Provides a quick lookup from display name to its installer details.
// Useful when you need to fetch URL and silent flags by name without iterating.
// Key = Software.name; Value = pair<url, silentFlags>
// Keeps code DRY if you reference a specific entry elsewhere (e.g., custom workflows).
extern const unordered_map<string, pair<string, string>> Software_Installers;
