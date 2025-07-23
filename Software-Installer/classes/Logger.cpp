#include "Logger.h"
#include <iostream>
#include <string>  // for std::string
#include <mutex>
#include <windows.h>
#include <cstdlib>

// Mutex for thread-safe logging
namespace { static std::mutex logMutex; }

void Logger::log(Logger::Level level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);

    // Get console handle and save original attributes
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    WORD originalAttrs = 0;
    if (GetConsoleScreenBufferInfo(hConsole, &bufferInfo)) {
        originalAttrs = bufferInfo.wAttributes;
    }

    // Choose color based on level
    WORD color = originalAttrs;
    switch (level) {
    case Logger::Level::DEBUG:
        color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // Grey
        break;
    case Logger::Level::INFO:
        color = FOREGROUND_BLUE | FOREGROUND_INTENSITY;             // Bright Blue
        break;
    case Logger::Level::WARN:
        color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Yellow
        break;
    case Logger::Level::ERR:
        color = FOREGROUND_RED;                                    // Red
        break;
    case Logger::Level::FATAL:
        color = FOREGROUND_RED | FOREGROUND_INTENSITY;             // Bright Red
        break;
    }

    // Set color and print message
    SetConsoleTextAttribute(hConsole, color);
    const char* label = nullptr;
    switch (level) {
    case Logger::Level::DEBUG: label = "DEBUG"; break;
    case Logger::Level::INFO:  label = "INFO";  break;
    case Logger::Level::WARN:  label = "WARN";  break;
    case Logger::Level::ERR: label = "ERROR"; break;
    case Logger::Level::FATAL: label = "FATAL"; break;
    }
    std::cout << "[" << label << "] " << message << std::endl;

    // Restore original console attributes
    SetConsoleTextAttribute(hConsole, originalAttrs);

    // Exit on fatal messages
    if (level == Logger::Level::FATAL) {
        std::exit(EXIT_FAILURE);
    }
}

// Convenience wrappers
void Logger::debug(const std::string& message) { Logger::log(Logger::Level::DEBUG, message); }
void Logger::info(const std::string& message) { Logger::log(Logger::Level::INFO, message); }
void Logger::warn(const std::string& message) { Logger::log(Logger::Level::WARN, message); }
void Logger::error(const std::string& message) { Logger::log(Logger::Level::ERR, message); }
void Logger::fatal(const std::string& message) { Logger::log(Logger::Level::FATAL, message); }
