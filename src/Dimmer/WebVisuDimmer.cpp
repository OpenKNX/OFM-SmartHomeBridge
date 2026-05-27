#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuDimmer.h"

#include "../WebVisuBridge.h"

WebVisuDimmer::WebVisuDimmer(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuDimmer::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuDimmer::webVisuKind() const
{
    return "dimmer";
}

std::string WebVisuDimmer::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return renderWidgetHtml(channelIndex, _name, _lastBrightness);
}

std::string WebVisuDimmer::webVisuDetailHtml(uint8_t channelIndex) const
{
    return renderWidgetHtml(channelIndex, _name, _lastBrightness);
}

std::string WebVisuDimmer::webVisuJson(uint8_t channelIndex) const
{
    return buildDeviceJson(channelIndex, _name, _lastBrightness);
}

bool WebVisuDimmer::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    if (_channel == nullptr)
    {
        return false;
    }

    if (action == "toggle")
    {
        _channel->commandMainFunctionClick();
        return true;
    }

    if (action == "setDimmerPower")
    {
        bool power = false;
        if (!parseBoolField(message, "power", power))
        {
            return false;
        }
        _channel->commandPower(this, power);
        return true;
    }

    if (action == "setDimmer")
    {
        int brightness = 0;
        if (!parseIntField(message, "brightness", brightness))
        {
            return false;
        }

        if (brightness < 0)
            brightness = 0;
        if (brightness > 100)
            brightness = 100;

        _channel->commandBrightness(this, (uint8_t)brightness);
        return true;
    }

    return false;
}

void WebVisuDimmer::setBrightness(uint8_t brightness)
{
    if (_channel == nullptr || _webVisuBridge == nullptr)
        return;

    publishBrightness(brightness);
}

void WebVisuDimmer::publishBrightness(uint8_t brightness)
{
    if (_channel == nullptr || _webVisuBridge == nullptr)
        return;

    _lastBrightness = brightness;
    _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
}

std::string WebVisuDimmer::buildDeviceJson(uint8_t channelIndex,
                                           const std::string& name,
                                           uint8_t brightness)
{
    const bool power = brightness > 0;
    const std::string widgetHtml = renderWidgetHtml(channelIndex, name, brightness);

    std::string json = "{";
    json += "\"kind\":\"dimmer\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(name) + "\",";
    json += "\"power\":";
    json += power ? "true" : "false";
    json += ",";
    json += "\"brightness\":" + std::to_string((int)brightness) + ",";
    json += "\"html\":\"" + jsonEscape(widgetHtml) + "\"";
    json += "}";
    return json;
}

std::string WebVisuDimmer::renderWidgetHtml(uint8_t channelIndex,
                                            const std::string& name,
                                            uint8_t brightness)
{
    const bool isOn = brightness > 0;
    const int channelOneBased = (int)channelIndex + 1;

    std::string controls;
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{\"action\":\"setDimmerPower\",\"channel\":";
    controls += std::to_string(channelOneBased);
    controls += ",\"power\":";
    controls += isOn ? "false" : "true";
    controls += "}'>";
    controls += isOn ? "Aus" : "Ein";
    controls += "</button>";

    std::string detail;
    detail += "<input class=\"webvisu-slider\" type=\"range\" min=\"0\" max=\"100\" step=\"1\" data-wv-payload-template='{\"action\":\"setDimmer\",\"channel\":__CHANNEL__,\"brightness\":__VALUE__}' data-channel=\"";
    detail += std::to_string(channelOneBased);
    detail += "\" value=\"";
    detail += std::to_string((int)brightness);
    detail += "\">";

    return renderCard(channelIndex, name, "Dimmer", std::to_string((int)brightness) + "%", controls, detail);
}

#endif
