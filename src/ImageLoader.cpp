#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "ImageLoader.h"
#include "ImageLoaderEmbeddedIcons.h"

#include "OpenKNX/Crypto/Base64.h"

#include <algorithm>
#include <cctype>
#include <vector>

#if defined(OPENKNX_LITTLE_FS) && OPENKNX_LITTLE_FS
    #include <LittleFS.h>
#endif

namespace
{
const char* TYPE10_BASE64 =
    "iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAAAXNSR0IArs4c6QAAAERlWElmTU0AKgAAAAgAAYdpAAQAAAABAAAAGgAAAAAAA6ABAAMAAAABAAEAAKACAAQAAAABAAAAQKADAAQAAAABAAAAQAAAAABGUUKwAAAD+ElEQVR4Ae2YP6wNQRTG3xMPifhXIIJI5BUqBRKiI1FTSTQqUUokon0VoVGJV6kUEj15JZHQCI2EoHjIbRASRBAP3y/XynX2zH07187sLeYkJ/futzPn++bb3ZnZnZgoURwoDhQHigPFgeJAcSCvAzdE98skWPZYkp1xzAiLAWN2QbLLKXdAdsvHjLDtO2Cfxrc64RipDUdr0aYBp6XqrnJOmcIEalIbDrjGJial5LJycF2/p+NhJsTuA6hFzUEOOOHuNBBwRTkorPo/zIQYA7zBVxxwd2rCmcDgEfhdeVTpRYwBx1Xgh7IatP1FQydxUKwhYS91bvcQVTEGUGa/sqe0g+cYDWjJGivFNq/0BD0WvkU5LGINoNY25VOlxzkvHE3Z4qKYPCGvhW9uoGIUAyi7VRm6E9CUJTaJ5avSGvBN2K6GCkY1gPJ7lMwvlh9NaIuKUfYBp8Sw3GE5J+yhg7cNPVDB805RNKEtaSxT9fdK6z6THueaxv/cAXDABafVgbYYHROxdwCz7TqljQsCuC1zBVxw2kBb1IoQa8ARy6jjL8prDp4aghNuG55G2+bvcawBrMc2bgr4bMEMx3Decng8jU6zPhRjwAp12eFUuuNguaDbDhEa0dooYgyYVsUppyqzclfhcaMRrY0ixgBv8oPkVSOmNI1C3CGtNRUxBqyt9e4DHwJ4DjjEHdJa0xRjQKjtz1rVfECIO6S1pqxxQ/X8VOvdB9YE8BxwiDuktaZpaQ0JA28Cp7YLfxs4F4LvOyc8zGn2DwS3FyGtXtvGGEvLgtJuP082rtB+Q7itHjQmWQZ523rmjOGAg+WCPG40ojVJzKqqdfyjsKwfI/6MDE64rR40No6YSZCic07lVcKOOXhqCE64bXgabZuRj3nVfKe0rj8X5u0SRyZapCNccFodaIt6HV6Exz19ySFGyFm3dRoQLjt4jtGWPPjmxyRjBfBqujM5e58DLsuPpibfI1uRGLoLXqj6xlYY/CLUhsMOPtvVr2Qx+fQCQh4JT2ECNantDR4t3oQoOF0cUmlvY4RArlKbjwO1QlceDWjpJGbE6l0RMJ5TJqsp5ahBX2p4z3zFi4bOYlLMV5WVGO+X5eqEMmazRFv6eEvdIAfcaOg0eKFazAREs2u7rmT/vle5XsmaTfIfjHO08XZ4gwPnP5wxL3Nqni64CjPKBaUV2vYxHHB1fuWloRZMRj1l24Ou6lEbjrEOliP2Cd5mqRpI7C+1qJl9qRPnyMGuDNHeu0NTA+hLjWw7PHG1Hkxyh5WzyifKYfME52hDW/rQN2l0MZHwtWZauUFZ3dJ8w+MzFpsdbvcSxYHiQHGgOFAcKA4UB4oDxYHEDvwGoRrD8DUouW8AAAAASUVORK5CYII=";

const char* TYPE20_BASE64 =
    "iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAAAXNSR0IArs4c6QAAAERlWElmTU0AKgAAAAgAAYdpAAQAAAABAAAAGgAAAAAAA6ABAAMAAAABAAEAAKACAAQAAAABAAAAQKADAAQAAAABAAAAQAAAAABGUUKwAAACUUlEQVR4Ae1aO07EMBBdEDS0tNScgT8HoKGBhpJrUNLyp+UCUCNxA+5Ax/cIFDQwI/SkXcsjxZsZe1aZkaKJQ/I8fn4vziaMRhHBQDAQDAyYgTmjsZ8S7qoy9gvhnShjmsAtE+oPbb/KG2MytmrMq6L9g+1TWjTAZUzGdh9PVKH27AOPsV2HlfxBgLoNFpTplOT/SP18F/a1ROfvJdfABnfJcTfNnPzfqbppVhu+hq/F7CO7tYEk/8se03ORIUDdBj3qm7j0OFMsz9rmxFlljQ0Bk/tyFzn5f1CVfZZatsEbbZA/sjsbWMgfMzwTNpDkv4VR9MgzYQML+YMz9zaQ5H+FEShk1zawlD+4W6cd3ADHs4vVICf/Tyq4z90fA0d2awNJ/teoXDGfE9b47PN+84ciSf7bigMHlEsb1JA/CHBng5ryBwmubCDJfwfVGmRXNsjJ/4sGrXn3TzlkG7zSlt4MuZaqIcn/pkIVLmzQQv7gdo12UgVwu+pDUQv5g4DmNpDkf4sKK+Qz6iNVQbWHIkn+uxUGji6a2iAn/3Q2WrW5NtOQ5N9qwGm/xTYoXbOl9/6mrBeA47tB50tKCTjojNzuRLMavcsfdiiyQcmnMZbXUbuJLeqZa40IBhwzcE+1wbPIfKx6lK4C1Qu07jAIsGbYO34owPsMWdcXCrBm2Dt+KMD7DFnXFwqwZtg7/uAVwO/Y+8Y0P2L4G99K0jH/S91zcqxL87DLSdI5GgTwr7mW0WsMg7dAENBSux76LnkpKtX7IP0hjgcDwUAwEAw4Z+APGpe7kl57MrEAAAAASUVORK5CYII=";

const char* MISSING_FILE_BASE64 =
    "iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAAAXNSR0IArs4c6QAAAERlWElmTU0AKgAAAAgAAYdpAAQAAAABAAAAGgAAAAAAA6ABAAMAAAABAAEAAKACAAQAAAABAAAAQKADAAQAAAABAAAAQAAAAABGUUKwAAADIElEQVR4Ae1bPcwOQRB+iSgEiRCVSD4dEpFo6IWESoEQOkoiWoUrVVRfIlEqxE8tUYriKwnx0yiIQqEgVCJ4nuQ2mWzu3v1592bu7r1JJjuzOzs78+zPvbf3fbPZRBMCEwITAkuMwLrM3Heg3znwQfCWTB/zuq2h8c48A8u20xj8O/hfx1zBf+/oKCL6A+46eee/6hMC3C7vFZPvHQgHDJLvHIQNCUtspcX2cUt9avUudDjS0ulmXV+1tKtU8/BzMyLLUoO3+ZdjVaUGc37WO2EgJVdCVTLWoQHA3IuCMEQAioIwVACKgTBkAIqAMHQAFgZhDAAsBMJYAMgGYUwAZIEwNgAcCMcpxNAYAWDeW2OSp81YAYjNf5byNhjtNNOQb5U5V3R8WcqmaQVkQ7dYR870XvDu2s1nlO62qa7SKbS3wGakdR18GcwLEElfoNwD3wb/kg19kdsuLGLj2w/Dj2B5wdEk04a2sdTkg7FGkdYZsAfRPAezDFGKbchXsF0DAO73B+DtXjQ/oD+rmbIk2rJPzlNB+ikq526BU4jCX6arqON54Igy63w79g2R34d69BYIOZftuQA8hBMZ5BPp1JPZJm3ZN0TS3sm9AuATMnCBsTw0JyO2SVv2DZG0d3I0ABqPwRfIYKPI4o2QffGdV7HT04urGgBcSIh6xbP96unFVY2nQErQ1zxjfibvlPoEwA1kyl+IjvgV+pZTuio1tkAo9k0w4E/g857hFeivvbriqjUA25DRU/BhkdlfyFfBd0VdZ6IlANx+98Ey+d/QL4IfgVXI8gw4gQxPiiw586rJc2xLAC6J5ClyNajNvBvbEoB9Loi6XPV0FdXyDHiJDHkL5OitEzRLSwDOaibaNpYlAP4LC2+F1ckSAP/AM7n8sDwE1We7acClB8ByC5jseX8VWAJwxg/GQp+2QALqfFFpIv80b7LRrmuLdaE4LP9Y2l12xpaMtTjxOf0BHBuElR1j7Ow3xTE451WVVXKhcRkbY+yUeHrzU1YoGO12xpT8ZMldKl3/01TKDP6E8SswvyV+S+k42U4ITAhMCCw9Av8BpFqvobqt5SwAAAAASUVORK5CYII=";
}

std::string ImageLoader::loadImage(const std::string& fileName, bool useStateColor, bool state)
{
    (void)useStateColor;
    (void)state;
    const std::string normalized = normalize(fileName);

    const std::string fsImage = fileToDataUri(findFilePathCaseInsensitive(fileName), normalized);
    if (!fsImage.empty())
    {
        return fsImage;
    }

    const char* payload = imageBase64(normalized);
    std::string dataUri = "data:image/png;base64,";
    dataUri += payload;
    return dataUri;
}

std::string ImageLoader::imageTag(const std::string& fileName,
                                  const std::string& alt,
                                  const std::string& cssClass,
                                  bool useStateColor,
                                  bool state)
{
    std::string effectiveClass = cssClass;
    if (useStateColor)
    {
        effectiveClass += " webvisu-icon-recolor ";
        effectiveClass += state ? "webvisu-icon-state-on" : "webvisu-icon-state-off";
    }

    std::string html;
    html += "<img class=\"" + escapeHtml(effectiveClass) + "\" src=\"";
    html += loadImage(fileName, useStateColor, state);
    html += "\" alt=\"" + escapeHtml(alt) + "\">";
    return html;
}

std::string ImageLoader::normalize(const std::string& fileName)
{
    std::string lower = fileName;
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
        return (char)std::tolower(c);
    });
    return lower;
}

std::string ImageLoader::findFilePathCaseInsensitive(const std::string& fileName)
{
#if defined(OPENKNX_LITTLE_FS) && OPENKNX_LITTLE_FS
    if (fileName.empty())
    {
        return std::string();
    }

    std::string relative = fileName;
    while (!relative.empty() && relative[0] == '/')
    {
        relative.erase(relative.begin());
    }
    const std::string lowerRelative = normalize(relative);

    const std::string rootPath = "/" + relative;
    if (LittleFS.exists(rootPath.c_str()))
    {
        return rootPath;
    }

    const std::string iconPath = "/icons/" + relative;
    if (LittleFS.exists(iconPath.c_str()))
    {
        return iconPath;
    }

    auto searchDirectory = [&](const char* basePath) -> std::string {
        File root = LittleFS.open(basePath, "r");
        if (!root || !root.isDirectory())
        {
            return std::string();
        }

        File file = root.openNextFile();
        while (file)
        {
            if (!file.isDirectory())
            {
                std::string currentName = file.name();
                size_t lastSlash = currentName.find_last_of('/');
                if (lastSlash != std::string::npos)
                {
                    currentName = currentName.substr(lastSlash + 1);
                }

                if (normalize(currentName) == lowerRelative)
                {
                    std::string foundPath = file.name();
                    file.close();
                    root.close();
                    return foundPath;
                }
            }

            file.close();
            file = root.openNextFile();
        }

        root.close();
        return std::string();
    };

    std::string found = searchDirectory("/");
    if (!found.empty())
    {
        return found;
    }

    return searchDirectory("/icons");
#else
    (void)fileName;
    return std::string();
#endif
}

std::string ImageLoader::fileToDataUri(const std::string& filePath, const std::string& normalizedFileName)
{
#if defined(OPENKNX_LITTLE_FS) && OPENKNX_LITTLE_FS
    if (filePath.empty())
    {
        return std::string();
    }

    File image = LittleFS.open(filePath.c_str(), "r");
    if (!image)
    {
        return std::string();
    }

    const size_t size = image.size();
    if (size == 0)
    {
        image.close();
        return std::string();
    }

    std::vector<uint8_t> raw(size);
    const size_t read = image.read(raw.data(), raw.size());
    image.close();
    if (read != size)
    {
        return std::string();
    }

    const size_t outputSize = (size + 2) / 3 * 4 + 1;
    std::vector<char> encoded(outputSize, '\0');
    const size_t written = OpenKNX::Crypto::Base64::encode(raw.data(), raw.size(), encoded.data(), encoded.size());
    if (written == 0)
    {
        return std::string();
    }

    std::string dataUri = "data:" + mimeTypeForFile(normalizedFileName) + ";base64,";
    dataUri.append(encoded.data(), written);
    return dataUri;
#else
    (void)filePath;
    (void)normalizedFileName;
    return std::string();
#endif
}

std::string ImageLoader::mimeTypeForFile(const std::string& normalizedFileName)
{
    if (normalizedFileName.size() >= 4 && normalizedFileName.rfind(".svg") == normalizedFileName.size() - 4)
    {
        return "image/svg+xml";
    }

    if (normalizedFileName.size() >= 4 && normalizedFileName.rfind(".jpg") == normalizedFileName.size() - 4)
    {
        return "image/jpeg";
    }

    if (normalizedFileName.size() >= 5 && normalizedFileName.rfind(".jpeg") == normalizedFileName.size() - 5)
    {
        return "image/jpeg";
    }

    if (normalizedFileName.size() >= 4 && normalizedFileName.rfind(".gif") == normalizedFileName.size() - 4)
    {
        return "image/gif";
    }

    if (normalizedFileName.size() >= 5 && normalizedFileName.rfind(".webp") == normalizedFileName.size() - 5)
    {
        return "image/webp";
    }

    return "image/png";
}

const char* ImageLoader::imageBase64(const std::string& normalizedFileName)
{
    const char* embedded = imageLoaderEmbeddedIconBase64(normalizedFileName);
    if (embedded != nullptr)
        return embedded;

    if (normalizedFileName == "type10.png")
        return TYPE10_BASE64;
    if (normalizedFileName == "type20.png")
        return TYPE20_BASE64;
    return MISSING_FILE_BASE64;
}

std::string ImageLoader::escapeHtml(const std::string& value)
{
    std::string escaped;
    escaped.reserve(value.size());
    for (char c : value)
    {
        switch (c)
        {
        case '&':
            escaped += "&amp;";
            break;
        case '<':
            escaped += "&lt;";
            break;
        case '>':
            escaped += "&gt;";
            break;
        case '"':
            escaped += "&quot;";
            break;
        default:
            escaped += c;
            break;
        }
    }
    return escaped;
}

#endif
