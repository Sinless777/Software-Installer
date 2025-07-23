#pragma once

#include <string>
#include <vector>

/// Helper class: non-instantiable collection of utility methods for process execution,
/// file I/O, user interaction, timing, and environment handling.
class Helper {
    public:
        // Prevent instantiation
        Helper() = delete;
        ~Helper() = delete;

        static bool elevateSelf();

        /**
         * Process execution:
         * - runCommand: executes a system command, returns exit code
         * - execCommand: executes a command, optionally captures stdout
         */
        static int runCommand(const std::string& cmd);
        static std::string execCommand(const std::string& cmd, bool captureOutput = true);

        /**
         * File operations:
         * - fileExists: check if a path exists
         * - readFile / writeFile: text file I/O
         * - createDirectory: recursively create directories
         * - removeFile: delete a file
         */
        static bool fileExists(const std::string& path);
        static std::string readFile(const std::string& path);
        static bool writeFile(const std::string& path, const std::string& content);
        static bool createDirectory(const std::string& path);
        static bool removeFile(const std::string& path);

        /**
         * Path utilities:
         * - getExecutablePath: full path of current module
         * - getExecutableDirectory: directory containing current module
         * - combinePath: safely join two path segments
         */
        static std::string getExecutablePath();
        static std::string getExecutableDirectory();
        static std::string combinePath(const std::string& a, const std::string& b);

        /**
         * User interaction:
         * - askYesNo: prompt with a yes/no question
         */
        static bool askYesNo(const std::string& question);

        /**
         * Timing:
         * - sleepMs: sleep current thread for given milliseconds
         */
        static void sleepMs(int ms);

        /**
         * Environment variables:
         * - getEnv: retrieve value for a key (empty if unset)
         * - setEnv: set an environment variable
         */
        static std::string getEnv(const std::string& var);
        static bool setEnv(const std::string& var, const std::string& value, bool overwrite = true);

        // Console clear
        static void clearConsole();

        // User privileges:
        static bool isAdmin();
        static bool isElevated();

        // ANSCII art
        static const char* Anscii_Art_Computer_Setup_Ansc;

        /// Extracts the filename portion from a URL (the substring after the last '/' or '\\').
        static std::string extractFileName(const std::string& url);

    private:
        static std::wstring getExecutablePathW();
};
