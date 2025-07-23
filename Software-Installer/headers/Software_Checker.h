#pragma once

#include <string>
#include <wtypes.h>

/// Software_Checker: determines if specified software is already installed on Windows.
class Software_Checker {
public:
    /// Checks if a program with the given display name exists under the
    /// "Uninstall" registry keys (both HKLM and HKCU).
    /// @param displayName The exact or partial name shown in "Programs and Features".
    /// @return true if found, false otherwise.
    static bool isInstalled(const std::string& displayName);

private:
    /// Helper: queries a registry uninstall key.
    /// @param rootKey HKEY_LOCAL_MACHINE or HKEY_CURRENT_USER
    /// @param subKeyPath Path under root to scan.
    /// @param displayName The name to match.
    /// @return true if matching entry found.
    static bool checkUninstallKey(HKEY rootKey, const std::wstring& subKeyPath, const std::string& displayName);

    /// Converts a narrow string (UTF-8 / ANSI) to a wide string (UTF-16) for Win32 APIs.
    static std::wstring toWide(const std::string& narrow);
};
