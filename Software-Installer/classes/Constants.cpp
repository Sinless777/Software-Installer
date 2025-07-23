// Constants.cpp

#include "Constants.h"

// -----------------------------------------------------------------------------
// Windows Software Installer URLs
// -----------------------------------------------------------------------------
// These constants hold the direct download URLs for the latest installer
// binaries.  When you need to update to a newer version of an app, simply
// replace the URL below with the new release's download link.
// Note: Ensure the URL points to a valid installer for your target platform.

const string Windows_Obsidian_URL =
"https://github.com/obsidianmd/obsidian-releases/releases/download/v1.8.10/Obsidian-1.8.10.exe";
const string Windows_Discord_URL =
"https://stable.dl2.discordapp.net/distro/app/stable/win/x64/1.0.9200/DiscordSetup.exe";
const string Windows_NotepadPlusPlus_URL =
"https://github.com/notepad-plus-plus/notepad-plus-plus/releases/download/v8.8.2/npp.8.8.2.Installer.x64.exe";
const string Windows_VSCode_URL =
"https://vscode.download.prss.microsoft.com/dbazure/download/stable/7adae6a56e34cb64d08899664b814cf620465925/VSCodeUserSetup-x64-1.102.1.exe";
const string Windows_7Zip_URL =
"https://www.7-zip.org/a/7z2500-x64.exe";
const string Windows_VisualStudio_URL =
"https://c2rsetup.officeapps.live.com/c2r/downloadVS.aspx?sku=community&channel=Release&version=VS2022"
"&source=VSLandingPage&cid=2030:0631e9e8c608479c89d9b500c3ff010c";


// -----------------------------------------------------------------------------
// Software_List Vector
// -----------------------------------------------------------------------------
// This vector drives the "Download & install" menu option.  Each entry
// consists of:
//   - name:        Display name shown to the user
//   - url:         Download link (one of the constants above)
//   - silentFlags: Command-line flags for silent or unattended install
//                  (per the installer’s documentation)

const vector<Software> Software_List = {
    { "Obsidian",      Windows_Obsidian_URL,      "/S" },
    { "Discord",       Windows_Discord_URL,       "/S" },
    { "Notepad++",     Windows_NotepadPlusPlus_URL, "/S" },
    { "VSCode",        Windows_VSCode_URL,        "/VERYSILENT /NORESTART" },
    { "7-Zip",         Windows_7Zip_URL,          "/S" },
    { "Visual Studio", Windows_VisualStudio_URL,  "--quiet --wait" }
};


// -----------------------------------------------------------------------------
// Software_Installers Map
// -----------------------------------------------------------------------------
// This map provides an alternative lookup by name.  Key is the same display
// name as in Software_List, value is a {url, silentFlags} pair.  You can
// either iterate the vector (preserves order) or look up individual entries
// in this map.

const unordered_map<string, pair<string, string>> Software_Installers = {
    { "Obsidian",      { Windows_Obsidian_URL,      "/S" } },
    { "Discord",       { Windows_Discord_URL,       "/S" } },
    { "Notepad++",     { Windows_NotepadPlusPlus_URL, "/S" } },
    { "VSCode",        { Windows_VSCode_URL,        "/VERYSILENT /NORESTART" } },
    { "7-Zip",         { Windows_7Zip_URL,          "/S" } },
    { "Visual Studio", { Windows_VisualStudio_URL,  "--quiet --wait" } }
};
