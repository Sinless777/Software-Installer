#pragma once

#include <string>

/// Installer: runs downloaded installer executables with appropriate silent flags.
class Installer {
public:
    /// Runs an installer with given arguments.
    /// @param installerPath Full path to the installer executable.
    /// @param args Command-line arguments for silent or custom installation.
    /// @return true if the installer process exited with code 0.
    static bool install(const std::string& installerPath, const std::string& args = "");

private:
    /// Helper to launch a process and wait for its exit code.
    /// @param commandLine Full command line to execute (quoted paths as needed).
    /// @return the process exit code, or -1 on failure to launch.
    static int launchProcess(const std::string& commandLine);
};
