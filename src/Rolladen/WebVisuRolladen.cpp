#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuRolladen.h"

#include "../WebVisuBridge.h"

WebVisuRolladen::WebVisuRolladen(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuRolladen::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuRolladen::webVisuKind() const
{
    return "rolladen";
}

void WebVisuRolladen::setPosition(uint8_t position)
{
    _position = position;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

void WebVisuRolladen::setMovement(MoveState movement)
{
    _movement = movement;
}

std::string WebVisuRolladen::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuRolladen::webVisuDetailHtml(uint8_t channelIndex) const
{
    const int channelOneBased = (int)channelIndex + 1;
    std::string controls;
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"setRolladen\",\"channel\":" + std::to_string(channelOneBased) + ",\"position\":0}'>Zu</button>";
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"setRolladen\",\"channel\":" + std::to_string(channelOneBased) + ",\"position\":50}'>Stopp</button>";
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"setRolladen\",\"channel\":" + std::to_string(channelOneBased) + ",\"position\":100}'>Auf</button>";
    controls += "<input class=\"webvisu-slider\" type=\"range\" min=\"0\" max=\"100\" step=\"1\" data-wv-payload-template='{";
    controls += "\"action\":\"setRolladen\",\"channel\":__CHANNEL__,\"position\":__VALUE__}' data-channel=\"" + std::to_string(channelOneBased) + "\" value=\"" + std::to_string((int)_position) + "\">";

    const std::string state = _movement == MoveStateUp ? "Fährt auf" : (_movement == MoveStateDown ? "Fährt zu" : "Ruhe");
    return renderCard(channelIndex, _name, "Rolladen", std::to_string((int)_position) + "% / " + state, controls);
}

std::string WebVisuRolladen::webVisuJson(uint8_t channelIndex) const
{
    std::string json = "{";
    json += "\"kind\":\"rolladen\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"position\":" + std::to_string((int)_position) + ",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

bool WebVisuRolladen::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    if (action != "setRolladen" || _channel == nullptr)
    {
        return false;
    }

    int position = 0;
    if (!parseIntField(message, "position", position))
    {
        return false;
    }

    if (position < 0) position = 0;
    if (position > 100) position = 100;
    _channel->commandPosition(this, (uint8_t)position);
    return true;
}

#endif