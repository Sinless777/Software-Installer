#include "Software_Downloader.h"
#include "Helper.h"    // for fileExists()
#include "Logger.h"    // for info(), error(), debug()
#include <urlmon.h>
#include <windows.h>

#pragma comment(lib, "urlmon.lib")

bool Software_Downloader::downloadFile(const std::string& url, const std::string& destinationPath) {
    Logger::info("Downloading " + url + " to " + destinationPath);
    HRESULT hr = URLDownloadToFileA(
        nullptr,
        url.c_str(),
        destinationPath.c_str(),
        0,
        nullptr
    );
    if (FAILED(hr)) {
        Logger::error("Failed to download file from " + url + " to " + destinationPath
            + ". HRESULT: " + std::to_string(hr));
        return false;
    }
    return true;
}

bool Software_Downloader::downloadUrl(const std::string& url, const std::string& destinationDir) {
    Logger::debug("Downloading URL: " + url + " to directory: " + destinationDir);
    std::string filename = extractFileName(url);
    std::string fullPath = buildDestinationPath(destinationDir, filename);

    // Skip download if already present
    if (Helper::fileExists(fullPath)) {
        Logger::info("File already exists: " + fullPath);
        return true;
    }
    return downloadFile(url, fullPath);
}

std::string Software_Downloader::extractFileName(const std::string& url) {
    Logger::debug("Extracting filename from URL: " + url);
    // Remove query string portion
    size_t qpos = url.find('?');
    std::string path = (qpos == std::string::npos ? url : url.substr(0, qpos));
    // Find last slash/backslash
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) {
        return path;
    }
    return path.substr(pos + 1);
}

std::string Software_Downloader::buildDestinationPath(const std::string& dir, const std::string& filename) {
    Logger::debug("Building destination path for directory: " + dir + " and filename: " + filename);
    if (dir.empty()) {
        return filename;
    }
    std::string path = dir;
    char sep = '\\';
    if (path.back() != '\\' && path.back() != '/') {
        path.push_back(sep);
    }
    path += filename;
    return path;
}
