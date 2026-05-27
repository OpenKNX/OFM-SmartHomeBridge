#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuAlarm.h"

#include "../WebVisuBridge.h"

WebVisuAlarm::WebVisuAlarm(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuAlarm::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuAlarm::webVisuKind() const
{
    return "alarm";
}

void WebVisuAlarm::setDetected(bool detected)
{
    _detected = detected;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

std::string WebVisuAlarm::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuAlarm::webVisuDetailHtml(uint8_t channelIndex) const
{
    const std::string stateText = _detected ? "Alarm" : "Normal";
    const std::string typeLabel = _detected ? "Alarm aktiv" : "Sensor";
    const std::string controls = _detected ? "<span class=\"webvisu-type\">Achtung</span>" : "";
    return renderCard(channelIndex, _name, typeLabel, stateText, controls);
}

std::string WebVisuAlarm::webVisuJson(uint8_t channelIndex) const
{
    std::string json = "{";
    json += "\"kind\":\"alarm\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"detected\":" + std::string(_detected ? "true" : "false") + ",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

bool WebVisuAlarm::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    (void)action;
    (void)message;
    return false;
}

#endif