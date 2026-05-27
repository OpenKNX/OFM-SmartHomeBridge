#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuDoorWindow.h"

#include "../WebVisuBridge.h"

WebVisuDoorWindow::WebVisuDoorWindow(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuDoorWindow::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuDoorWindow::webVisuKind() const
{
    return "doorwindow";
}

void WebVisuDoorWindow::setPosition(uint8_t position)
{
    _position = position;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

void WebVisuDoorWindow::setMovement(DoorWindowMoveState movement)
{
    _movement = movement;
}

void WebVisuDoorWindow::setObstructionDetected(bool obstructionDetected)
{
    _obstructionDetected = obstructionDetected;
}

std::string WebVisuDoorWindow::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuDoorWindow::webVisuDetailHtml(uint8_t channelIndex) const
{
    const int channelOneBased = (int)channelIndex + 1;
    std::string controls;
    controls += "<input class=\"webvisu-slider\" type=\"range\" min=\"0\" max=\"100\" step=\"1\" data-wv-payload-template='{";
    controls += "\"action\":\"setDoorWindow\",\"channel\":__CHANNEL__,\"position\":__VALUE__}' data-channel=\"" + std::to_string(channelOneBased) + "\" value=\"" + std::to_string((int)_position) + "\">";
    controls += "<span class=\"webvisu-type\">" + std::string(_obstructionDetected ? "Blockiert" : (_movement == DoorWindowMoveStateOpening ? "Fährt auf" : (_movement == DoorWindowMoveStateClosing ? "Fährt zu" : "Ruhe"))) + "</span>";

    return renderCard(channelIndex, _name, "DoorWindow", std::to_string((int)_position) + "%", controls);
}

std::string WebVisuDoorWindow::webVisuJson(uint8_t channelIndex) const
{
    std::string json = "{";
    json += "\"kind\":\"doorwindow\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"position\":" + std::to_string((int)_position) + ",";
    json += "\"obstruction\":" + std::string(_obstructionDetected ? "true" : "false") + ",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

bool WebVisuDoorWindow::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    if (action != "setDoorWindow" || _channel == nullptr)
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