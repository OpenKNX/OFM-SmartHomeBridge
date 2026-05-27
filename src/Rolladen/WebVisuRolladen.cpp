#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuRolladen.h"

#include "../WebVisuBridge.h"

WebVisuRolladen::WebVisuRolladen(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

const char* WebVisuRolladen::movementText(MoveState movement)
{
    if (movement == MoveStateUp)
        return "Fährt auf";
    if (movement == MoveStateDown)
        return "Fährt ab";
    return "Ruhe";
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
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

std::string WebVisuRolladen::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuRolladen::webVisuDetailHtml(uint8_t channelIndex) const
{
    const int channelOneBased = (int)channelIndex + 1;
    std::string controls;
    std::string positionLine = "Position: " + std::to_string((int)_position) + "%";
    if (_movement != MoveStateHold)
    {
        positionLine += " <span class=\"webvisu-type\">" + std::string(movementText(_movement)) + "</span>";
    }
    controls += "<div class=\"webvisu-value\" style=\"width:100%;\">" + positionLine + "</div>";
    controls += "<div style=\"display:flex;align-items:center;gap:8px;\">";
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"setRolladen\",\"channel\":" + std::to_string(channelOneBased) + ",\"position\":100}'>↓</button>";
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"setRolladen\",\"channel\":" + std::to_string(channelOneBased) + ",\"position\":0}'>↑</button>";
    controls += "<input class=\"webvisu-slider\" style=\"flex:1;\" type=\"range\" min=\"0\" max=\"100\" step=\"1\" data-wv-payload-template='{";
    controls += "\"action\":\"setRolladen\",\"channel\":__CHANNEL__,\"position\":__VALUE__}' data-channel=\"" + std::to_string(channelOneBased) + "\" value=\"" + std::to_string((int)_position) + "\">";
    controls += "</div>";
    controls += webVisuExtraControlsHtml(channelIndex);

    return renderCard(channelIndex, _name, webVisuCardTypeLabel(), "", controls);
}

std::string WebVisuRolladen::webVisuJson(uint8_t channelIndex) const
{
    std::string json = "{";
    json += "\"kind\":\"" + jsonEscape(webVisuKind()) + "\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"position\":" + std::to_string((int)_position) + ",";
    json += "\"movement\":\"" + jsonEscape(movementText(_movement)) + "\"";
    json += webVisuExtraJsonFields(channelIndex);
    json += ",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

std::string WebVisuRolladen::webVisuCardTypeLabel() const
{
    return "Rolladen";
}

std::string WebVisuRolladen::webVisuExtraControlsHtml(uint8_t channelIndex) const
{
    (void)channelIndex;
    return "";
}

std::string WebVisuRolladen::webVisuExtraJsonFields(uint8_t channelIndex) const
{
    (void)channelIndex;
    return "";
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