#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuSwitch.h"

#include "../WebVisuBridge.h"

WebVisuSwitch::WebVisuSwitch(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
    logDebug("Visu", "WebVisuSwitch created");
}

void WebVisuSwitch::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuSwitch::webVisuKind() const
{
    return "switch";
}

std::string WebVisuSwitch::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return renderWidgetHtml(channelIndex, _name, _power);
}

std::string WebVisuSwitch::webVisuDetailHtml(uint8_t channelIndex) const
{
    return renderWidgetHtml(channelIndex, _name, _power);
}

std::string WebVisuSwitch::webVisuJson(uint8_t channelIndex) const
{
    return buildDeviceJson(channelIndex, _name, _power);
}

bool WebVisuSwitch::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    if (action == "toggle")
    {
        if (_channel != nullptr && _channel->supportMainFunctionClick())
        {
            _channel->commandMainFunctionClick();
            return true;
        }
        return false;
    }

    if (action == "setSwitch")
    {
        bool power = false;
        if (!parseBoolField(message, "power", power))
        {
            return false;
        }
        setPower(power);
        return true;
    }

    return false;
}

void WebVisuSwitch::setPower(bool on)
{
    if (_channel == nullptr || _webVisuBridge == nullptr)
        return;

    _power = on;
    _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
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
