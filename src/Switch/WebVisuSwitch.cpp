#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuSwitch.h"

#include "../WebVisuBridge.h"

WebVisuSwitch::WebVisuSwitch(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuSwitch::setPower(bool on)
{
    if (_channel == nullptr || _webVisuBridge == nullptr)
        return;

    const char* channelName = _channel->getNameInUTF8();
    _name = channelName == nullptr ? "Unbenannt" : std::string(channelName);
    _power = on;
    const uint8_t channelIndex = _channel->channelIndex();
    const std::string deviceJson = buildDeviceJson(channelIndex, _name, _power);

    _webVisuBridge->broadcastUpdate(deviceJson);
}

std::string WebVisuSwitch::buildDeviceJson(uint8_t channelIndex, const std::string& name, bool power)
{
    const uint8_t brightness = power ? 100 : 0;
    const std::string widgetHtml = renderWidgetHtml(channelIndex, name, power);

    std::string json = "{";
    json += "\"kind\":\"switch\",";
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

std::string WebVisuSwitch::renderWidgetHtml(uint8_t channelIndex, const std::string& name, bool power)
{
    const std::string value = power ? "Ein" : "Aus";
    const std::string next = power ? "Aus" : "Ein";

    std::string controls;
    controls += "<button class=\"webvisu-btn\" data-action=\"toggle\" data-channel=\"";
    controls += std::to_string((int)channelIndex + 1);
    controls += "\">" + next + "</button>";

    return renderCard(channelIndex, name, "Switch", value, controls);
}

#endif
