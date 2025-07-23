#pragma once

#include <string>

/// Logger class providing thread-safe, timestamped logging at various severity levels.
class Logger {
public:
    /// Convenience wrappers for each level
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);
    static void fatal(const std::string& message);

private:
    /// Log severity levels (for internal use)
    enum class Level {
        DEBUG,
        INFO,
        WARN,
        ERR,
        FATAL  // Logs the message and then exits the program
    };

    /// Core logging function (thread-safe, internal)
    /// @param level Severity level
    /// @param message The message to log
    static void log(Level level, const std::string& message);
};
