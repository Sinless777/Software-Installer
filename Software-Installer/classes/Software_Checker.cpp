#include "Software_Checker.h"
#include <windows.h>
#include <string>
#include <vector>

// Convert narrow std::string to wide std::wstring (UTF-8)
std::wstring Software_Checker::toWide(const std::string& narrow) {
    if (narrow.empty()) return {};
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, narrow.c_str(), -1, nullptr, 0);
    if (size_needed == 0) return {};
    std::wstring wide;
    wide.resize(size_needed);
    MultiByteToWideChar(CP_UTF8, 0, narrow.c_str(), -1, &wide[0], size_needed);
    if (!wide.empty() && wide.back() == L'\0') wide.pop_back();
    return wide;
}

bool Software_Checker::checkUninstallKey(HKEY rootKey, const std::wstring& subKeyPath, const std::string& displayName) {
    std::wstring wDisplay = toWide(displayName);
    for (DWORD viewFlag : {KEY_WOW64_64KEY, KEY_WOW64_32KEY}) {
        HKEY hKey = nullptr;
        if (RegOpenKeyExW(rootKey, subKeyPath.c_str(), 0, KEY_READ | viewFlag, &hKey) != ERROR_SUCCESS) {
            continue;
        }
        DWORD index = 0;
        wchar_t subkeyName[256];
        DWORD nameLen = _countof(subkeyName);
        while (RegEnumKeyExW(hKey, index, subkeyName, &nameLen, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
            HKEY hSub = nullptr;
            if (RegOpenKeyExW(hKey, subkeyName, 0, KEY_READ | viewFlag, &hSub) == ERROR_SUCCESS) {
                wchar_t dispName[512];
                DWORD dispSize = sizeof(dispName);
                DWORD type = 0;
                if (RegQueryValueExW(hSub, L"DisplayName", nullptr, &type, reinterpret_cast<LPBYTE>(dispName), &dispSize) == ERROR_SUCCESS
                    && type == REG_SZ) {
                    std::wstring wDisp(dispName);
                    if (wDisp.find(wDisplay) != std::wstring::npos) {
                        RegCloseKey(hSub);
                        RegCloseKey(hKey);
                        return true;
                    }
                }
                RegCloseKey(hSub);
            }
            ++index;
            nameLen = _countof(subkeyName);
        }
        RegCloseKey(hKey);
    }
    return false;
}

bool Software_Checker::isInstalled(const std::string& displayName) {
    static const std::wstring uninstallPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
    return checkUninstallKey(HKEY_LOCAL_MACHINE, uninstallPath, displayName)
        || checkUninstallKey(HKEY_CURRENT_USER, uninstallPath, displayName);
}
