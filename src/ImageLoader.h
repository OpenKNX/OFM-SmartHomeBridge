#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <cstdint>
#include <string>

class ImageLoader
{
  public:
    static std::string loadImage(const std::string& fileName, bool useStateColor = false, bool state = false);
    static std::string imageTag(const std::string& fileName,
                                const std::string& alt,
                                const std::string& cssClass = "webvisu-icon",
                                bool useStateColor = false,
                                bool state = false);

  private:
    static std::string findFilePathCaseInsensitive(const std::string& fileName);
    static std::string fileToDataUri(const std::string& filePath, const std::string& normalizedFileName);
    static std::string mimeTypeForFile(const std::string& normalizedFileName);

    static std::string normalize(const std::string& fileName);
    static const char* imageBase64(const std::string& normalizedFileName);
    static std::string escapeHtml(const std::string& value);
};

#endif
