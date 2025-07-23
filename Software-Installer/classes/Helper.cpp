#include "Helper.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <shlobj.h>  // for SHCreateDirectoryExA
#include <shlwapi.h>   // for PathRemoveFileSpecW
#include <shellapi.h>  // for ShellExecuteW

using namespace std;

int Helper::runCommand(const std::string& cmd) {
    return std::system(cmd.c_str());
}

string Helper::execCommand(const std::string& cmd, bool captureOutput) {
    if (!captureOutput) {
        std::system(cmd.c_str());
        return {};
    }
    std::string output;
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe) return {};
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        output += buffer;
    }
    _pclose(pipe);
    return output;
}

bool Helper::fileExists(const std::string& path) {
    DWORD attrs = GetFileAttributesA(path.c_str());
    return (attrs != INVALID_FILE_ATTRIBUTES);
}

string Helper::readFile(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs) return {};
    std::ostringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

bool Helper::writeFile(const std::string& path, const std::string& content) {
    std::ofstream ofs(path);
    if (!ofs) return false;
    ofs << content;
    return true;
}

bool Helper::createDirectory(const std::string& path) {
    int result = SHCreateDirectoryExA(nullptr, path.c_str(), nullptr);
    return (result == ERROR_SUCCESS || result == ERROR_ALREADY_EXISTS);
}

bool Helper::removeFile(const std::string& path) {
    return DeleteFileA(path.c_str()) != 0;
}

string Helper::getExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    return std::string(buffer);
}

string Helper::getExecutableDirectory() {
    std::string path = getExecutablePath();
    size_t pos = path.find_last_of("\\/");
    return (pos == std::string::npos) ? std::string() : path.substr(0, pos);
}

string Helper::combinePath(const std::string& a, const std::string& b) {
    if (a.empty()) return b;
    std::string result = a;
    char sep = '\\';
    if (result.back() != '\\' && result.back() != '/') {
        result.push_back(sep);
    }
    result += b;
    return result;
}

bool Helper::askYesNo(const std::string& question) {
    // Get console handle and save original attributes
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    WORD originalAttrs = 0;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        originalAttrs = csbi.wAttributes;
    }

    // Print the question in bright cyan
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << question;

    // Print the prompt "(Y/N): " in bright yellow
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << " (Y/N): ";

    // Restore original attributes for user input
    SetConsoleTextAttribute(hConsole, originalAttrs);

    std::string answer;
    std::getline(std::cin, answer);

    // Interpret first character only, ignore case
    if (!answer.empty()) {
        char c = answer[0];
        return (c == 'y' || c == 'Y');
    }
    return false;
}

void Helper::sleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

string Helper::getEnv(const std::string& var) {
    // Use secure _dupenv_s to avoid unsafe getenv
    char* buffer = nullptr;
    size_t len = 0;
    if (_dupenv_s(&buffer, &len, var.c_str()) == 0 && buffer) {
        std::string value(buffer);
        free(buffer);
        return value;
    }
    return {};
}

bool Helper::setEnv(const std::string& var, const std::string& value, bool overwrite) {
    return SetEnvironmentVariableA(
        var.c_str(),
        overwrite ? value.c_str() : (getEnv(var).empty() ? value.c_str() : nullptr)
    ) != 0;
}

void Helper::clearConsole() {
    // Clear the console screen
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
    
    FillConsoleOutputCharacterA(hConsole, ' ', size, coord, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, size, coord, &written);
    
    SetConsoleCursorPosition(hConsole, coord);
}

bool Helper::isAdmin() {
    BOOL isAdmin = FALSE;
    PSID administratorsGroup = nullptr;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2,
        SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &administratorsGroup)) {
        CheckTokenMembership(nullptr, administratorsGroup, &isAdmin);
        FreeSid(administratorsGroup);
    }
    return isAdmin != FALSE;
}

bool Helper::isElevated() {
    HANDLE token = nullptr;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        TOKEN_ELEVATION elevation;
        DWORD size;
        if (GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &size)) {
            CloseHandle(token);
            return elevation.TokenIsElevated != 0;
        }
        CloseHandle(token);
    }
    return false;
}

string Helper::extractFileName(const std::string& url) {
    // find last slash or backslash
    std::size_t pos = url.find_last_of("/\\");
    if (pos == std::string::npos) {
        // no path separator, return the entire URL
        return url;
    }
    // return substring after the last separator
    return url.substr(pos + 1);
}

const char* Helper::Anscii_Art_Computer_Setup_Ansc = R"(
  ___      _________                               __                   _________       __                   ___     
 / _ \_/\  \_   ___ \  ____   _____ ______  __ ___/  |_  ___________   /   _____/ _____/  |_ __ ________    / _ \_/\ 
 \/ \___/  /    \  \/ /  _ \ /     \\____ \|  |  \   __\/ __ \_  __ \  \_____  \_/ __ \   __\  |  \____ \   \/ \___/ 
           \     \___(  <_> )  Y Y  \  |_> >  |  /|  | \  ___/|  | \/  /        \  ___/|  | |  |  /  |_> >           
            \______  /\____/|__|_|  /   __/|____/ |__|  \___  >__|    /_______  /\___  >__| |____/|   __/            
                   \/             \/|__|                    \/                \/     \/           |__|               
)";

bool Helper::elevateSelf() {
    // Check if already running elevated
    BOOL isElev = FALSE;
    HANDLE token = nullptr;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        TOKEN_ELEVATION elevInfo;
        DWORD retLen = 0;
        if (GetTokenInformation(token, TokenElevation, &elevInfo, sizeof(elevInfo), &retLen)) {
            isElev = elevInfo.TokenIsElevated;
        }
        CloseHandle(token);
    }
    if (isElev) {
        return true;  // already elevated
    }

    // Build command-line: quote the EXE path
    std::wstring exe = getExecutablePathW();
    // You can append any args you need here; using nullptr means no args
    HINSTANCE result = ShellExecuteW(
        nullptr,
        L"runas",               // verb = "run as administrator"
        exe.c_str(),            // application to run
        nullptr,                // parameters
        nullptr,                // working directory
        SW_SHOWNORMAL
    );

    // If the user approved the UAC prompt, result > 32
    if ((INT_PTR)result > 32) {
        // Relaunch succeeded – shut down this process
        ExitProcess(0);
    }
    // Else: user refused elevation or error
    return false;
}

std::wstring Helper::getExecutablePathW() {
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    return std::wstring(buffer);
}