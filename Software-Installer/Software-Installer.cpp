// Computer_Setup.cpp

#define NOMINMAX

#include "Helper.h"
#include "Constants.h"
#include "Installer.h"
#include "Logger.h"
#include "Software_Downloader.h"

#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <windows.h>

using namespace std;

static void showHeader() {
    // Print ASCII art in bold green
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    WORD originalAttrs = 0;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        originalAttrs = csbi.wAttributes;
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << Helper::Anscii_Art_Computer_Setup_Ansc << "\n\n";
    SetConsoleTextAttribute(hConsole, originalAttrs);
}

static void showMenu() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    WORD original = 0;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        original = csbi.wAttributes;
    }

    // Print numbered options with bold cyan numbers and white text
    // Option 1
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Bold cyan
    cout << " 1) ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // White
    cout << "Add software to Constants.h\n";

    // Option 2
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << " 2) ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "Download & install software\n";

    // Option 3
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << " 3) ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "Exit\n\n";

    // Prompt
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Bold green prompt
    cout << "Select an option: ";

    // Restore
    SetConsoleTextAttribute(hConsole, original);
}

int main() {
    // Relaunch elevated if necessary
    if (!Helper::elevateSelf()) {
        Logger::error("Administrator privileges are required to run this setup.");
        return 1;
    }
    Logger::info("Running with administrator privileges.");
    Helper::sleepMs(1000);

    int choice = 0;
    do {
        // Clear, header, menu
        //Helper::clearConsole();
        showHeader();
        showMenu();

        // Read and consume input
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Handle choice without clearing logs
        switch (choice) {
        case 1:
            Logger::info("Please edit Constants.h and update Software_List.");
            break;

        case 2: {
            Logger::info("Proceeding with download & install...");
            Helper::sleepMs(1000);
            Helper::clearConsole();
            vector<Software> toInstall;
            for (const auto& sw : Software_List) {
                if (Helper::askYesNo("Include " + sw.name + "?")) {
                    toInstall.push_back(sw);
                }
            }
            if (toInstall.empty()) {
                Logger::info("No software selected; returning to menu.");
                break;
            }
            string tmpDir = Helper::combinePath(Helper::getExecutableDirectory(), "tmp");
            if (!Helper::createDirectory(tmpDir)) {
                Logger::error("Failed to create tmp directory: " + tmpDir);
                break;
            }
            for (const auto& sw : toInstall) {
                Logger::info("Downloading " + sw.name + "...");
                string filename = Helper::extractFileName(sw.url);
                string path = Helper::combinePath(tmpDir, filename);
                if (!Helper::fileExists(path)) {
                    if (!Software_Downloader::downloadFile(sw.url, path)) {
                        Logger::error("Download failed for " + sw.name);
                        continue;
                    }
                }
                else {
                    Logger::info("Using cached installer for " + sw.name);
                }
                Logger::info(sw.name + " downloaded.");
                Logger::info("Installing " + sw.name + "...");
                if (Installer::install(path, sw.silentFlags)) {
                    Logger::info(sw.name + " installed successfully");
                }
                else {
                    Logger::error("Installation failed for " + sw.name);
                }
            }
            break;
        }

        case 3:
            Logger::info("Exiting setup. Goodbye!");
            break;

        default:
            Logger::warn("Invalid option; please try again.");
            break;
        }
        Helper::sleepMs(1000);
    } while (choice != 3);

    return 0;
}
