#include "installer.h"
#include <windows.h>
#include <vector>

int Installer::launchProcess(const std::string& commandLine) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Create a mutable command line buffer
    std::vector<char> cmdBuf(commandLine.begin(), commandLine.end());
    cmdBuf.push_back('\0');

    if (!CreateProcessA(
        nullptr,
        cmdBuf.data(),   // command line
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &si,
        &pi
    )) {
        return -1;
    }

    // Wait for exit
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Get exit code
    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    // Cleanup
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return static_cast<int>(exitCode);
}

bool Installer::install(const std::string& installerPath, const std::string& args) {
    // Quote the installer path and append args
    std::string cmd = '"' + installerPath + '"';
    if (!args.empty()) {
        cmd += ' ' + args;
    }
    int code = launchProcess(cmd);
    return (code == 0);
}

