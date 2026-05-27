#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuRGB.h"

#include <cstdio>
#include <cstdlib>

#include "../WebVisuBridge.h"

namespace
{
uint32_t parseHexColor(const std::string& value)
{
    std::string hex = value;
    if (!hex.empty() && hex[0] == '#')
    {
        hex.erase(0, 1);
    }
    if (hex.size() != 6)
    {
        return 0;
    }
    return (uint32_t)std::strtoul(hex.c_str(), nullptr, 16);
}
}

WebVisuRGB::WebVisuRGB(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuRGB::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuRGB::webVisuKind() const
{
    return "rgb";
}

void WebVisuRGB::setRGB(uint32_t rgb)
{
    _rgb = rgb;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

void WebVisuRGB::setPower(bool on)
{
    _power = on;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

std::string WebVisuRGB::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuRGB::webVisuDetailHtml(uint8_t channelIndex) const
{
    const int channelOneBased = (int)channelIndex + 1;
    char color[16];
    std::snprintf(color, sizeof(color), "#%06X", (unsigned int)(_rgb & 0xFFFFFF));

    std::string controls;
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"setRGBPower\",\"channel\":" + std::to_string(channelOneBased) + ",\"power\":";
    controls += _power ? "false" : "true";
    controls += "}'>" + std::string(_power ? "Aus" : "Ein") + "</button>";
    controls += "<input type=\"color\" class=\"webvisu-slider\" data-wv-payload-template='{";
    controls += "\"action\":\"setRGB\",\"channel\":__CHANNEL__,\"color\":\"__VALUE__\"}' data-channel=\"" + std::to_string(channelOneBased) + "\" value=\"" + std::string(color) + "\">";

    return renderCard(channelIndex, _name, "RGB", std::string(color), controls);
}

std::string WebVisuRGB::webVisuJson(uint8_t channelIndex) const
{
    char color[16];
    std::snprintf(color, sizeof(color), "#%06X", (unsigned int)(_rgb & 0xFFFFFF));
    std::string json = "{";
    json += "\"kind\":\"rgb\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"power\":" + std::string(_power ? "true" : "false") + ",";
    json += "\"rgb\":\"" + std::string(color) + "\",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

bool WebVisuRGB::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    if (_channel == nullptr)
    {
        return false;
    }

    if (action == "setRGBPower")
    {
        bool power = false;
        if (!parseBoolField(message, "power", power))
        {
            return false;
        }
        _channel->commandPower(this, power);
        return true;
    }

    if (action == "setRGB")
    {
        std::string color;
        if (!parseStringField(message, "color", color))
        {
            return false;
        }
        _channel->commandRGB(this, parseHexColor(color));
        return true;
    }

    return false;
}

#endif