#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuJalousie.h"

#include "../WebVisuBridge.h"

WebVisuJalousie::WebVisuJalousie(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuJalousie::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuJalousie::webVisuKind() const
{
    return "jalousie";
}

void WebVisuJalousie::setPosition(uint8_t position)
{
    _position = position;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

void WebVisuJalousie::setMovement(MoveState movement)
{
    _movement = movement;
}

void WebVisuJalousie::setSlatPosition(uint8_t slatPosition)
{
    _slatPosition = slatPosition;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

std::string WebVisuJalousie::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuJalousie::webVisuDetailHtml(uint8_t channelIndex) const
{
    const int channelOneBased = (int)channelIndex + 1;
    std::string controls;
    controls += "<input class=\"webvisu-slider\" type=\"range\" min=\"0\" max=\"100\" step=\"1\" data-wv-payload-template='{";
    controls += "\"action\":\"setRolladen\",\"channel\":__CHANNEL__,\"position\":__VALUE__}' data-channel=\"" + std::to_string(channelOneBased) + "\" value=\"" + std::to_string((int)_position) + "\">";
    controls += "<input class=\"webvisu-slider\" type=\"range\" min=\"0\" max=\"100\" step=\"1\" data-wv-payload-template='{";
    controls += "\"action\":\"setJalousieSlat\",\"channel\":__CHANNEL__,\"slat\":__VALUE__}' data-channel=\"" + std::to_string(channelOneBased) + "\" value=\"" + std::to_string((int)_slatPosition) + "\">";
    return renderCard(channelIndex, _name, "Jalousie", std::to_string((int)_position) + "% / Lamellen " + std::to_string((int)_slatPosition) + "%", controls);
}

std::string WebVisuJalousie::webVisuJson(uint8_t channelIndex) const
{
    std::string json = "{";
    json += "\"kind\":\"jalousie\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"position\":" + std::to_string((int)_position) + ",";
    json += "\"slat\":" + std::to_string((int)_slatPosition) + ",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

bool WebVisuJalousie::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    if (_channel == nullptr)
    {
        return false;
    }

    if (action == "setRolladen")
    {
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

    if (action == "setJalousieSlat")
    {
        int slat = 0;
        if (!parseIntField(message, "slat", slat))
        {
            return false;
        }
        if (slat < 0) slat = 0;
        if (slat > 100) slat = 100;
        ((KnxChannelJalousie*)_channel)->commandSlatPosition(this, (uint8_t)slat);
        return true;
    }

    return false;
}

#endif