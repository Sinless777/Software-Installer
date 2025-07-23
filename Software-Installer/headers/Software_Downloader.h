#pragma once

#include <string>
#include "Constants.h"

/// Downloads installers for common software using URLDownloadToFile.
class Software_Downloader {
public:
    /// Downloads any installer URL to the specified destination directory.
    /// @param url The download URL constant (e.g., from Constants.h).
    /// @param destinationDir Directory in which to save the downloaded file.
    /// @return true on success, false otherwise.
    static bool downloadUrl(const std::string& url, const std::string& destinationDir);

    /// Downloads a file from a URL to the specified path.
    /// @param url The download URL.
    /// @param destinationPath Full path (including filename) to save the file.
    /// @return true on success, false otherwise.
    static bool downloadFile(const std::string& url, const std::string& destinationPath);

    /// Convenience methods for downloading specific software installers.
    static bool downloadObsidian(const std::string& destinationDir);
    static bool downloadDiscord(const std::string& destinationDir);
    static bool downloadNotepadPlusPlus(const std::string& destinationDir);
    static bool downloadVSCode(const std::string& destinationDir);
    static bool download7Zip(const std::string& destinationDir);
    static bool downloadVisualStudio(const std::string& destinationDir);

private:
    /// Extracts the filename portion from a URL.
    static std::string extractFileName(const std::string& url);

    /// Builds a full destination path by combining directory and filename.
    static std::string buildDestinationPath(const std::string& dir, const std::string& filename);
};
