#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuDimmer.h"

#include "../WebVisuBridge.h"

WebVisuDimmer::WebVisuDimmer(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
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
    const char* channelName = _channel->getNameInUTF8();
    _name = channelName == nullptr ? "Unbenannt" : std::string(channelName);
    const uint8_t channelIndex = _channel->channelIndex();
    const std::string deviceJson = buildDeviceJson(channelIndex, _name, _lastBrightness);

    _webVisuBridge->broadcastUpdate(deviceJson);
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
    controls += "<button class=\"webvisu-btn\" data-action=\"setDimmer\" data-channel=\"";
    controls += std::to_string(channelOneBased);
    controls += "\" data-brightness=\"";
    controls += std::to_string(isOn ? 0 : 100);
    controls += "\">";
    controls += isOn ? "Aus" : "Ein";
    controls += "</button>";

    std::string detail;
    detail += "<input class=\"webvisu-slider\" type=\"range\" min=\"0\" max=\"100\" step=\"1\" data-action=\"setDimmer\" data-channel=\"";
    detail += std::to_string(channelOneBased);
    detail += "\" value=\"";
    detail += std::to_string((int)brightness);
    detail += "\">";

    return renderCard(channelIndex, name, "Dimmer", std::to_string((int)brightness) + "%", controls, detail);
}

#endif
