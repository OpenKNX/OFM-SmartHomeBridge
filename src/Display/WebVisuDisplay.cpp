#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuDisplay.h"

#include "../WebVisuBridge.h"

WebVisuDisplay::WebVisuDisplay(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuDisplay::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuDisplay::webVisuKind() const
{
    return "display";
}

void WebVisuDisplay::setValue(double value)
{
    _value = value;
    _hasText = false;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

void WebVisuDisplay::setValue(const char* value)
{
    _textValue = value != nullptr ? value : "";
    _hasText = true;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

std::string WebVisuDisplay::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuDisplay::webVisuDetailHtml(uint8_t channelIndex) const
{
    const std::string value = _channel != nullptr
                                  ? _channel->currentValueAsString()
                                  : (_hasText ? _textValue : std::to_string(_value));
    return renderCard(channelIndex, _name, "Display", value, "");
}

std::string WebVisuDisplay::webVisuJson(uint8_t channelIndex) const
{
    const std::string value = _channel != nullptr
                                  ? _channel->currentValueAsString()
                                  : (_hasText ? _textValue : std::to_string(_value));
    std::string json = "{";
    json += "\"kind\":\"display\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"value\":\"" + jsonEscape(value) + "\",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

bool WebVisuDisplay::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    (void)action;
    (void)message;
    return false;
}

#endif