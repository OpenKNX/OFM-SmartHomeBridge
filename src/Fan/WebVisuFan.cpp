#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuFan.h"

#include "../WebVisuBridge.h"

WebVisuFan::WebVisuFan(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuFan::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuFan::webVisuKind() const
{
    return "fan";
}

void WebVisuFan::setAutomatic(bool automatic)
{
    _automatic = automatic;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

void WebVisuFan::setPower(bool on)
{
    _power = on;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

std::string WebVisuFan::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuFan::webVisuDetailHtml(uint8_t channelIndex) const
{
    const int channelOneBased = (int)channelIndex + 1;
    const std::string powerLabel = _power ? "Ausschalten" : "Einschalten";
    const std::string automaticLabel = _automatic ? "Manuell" : "Automatik";

    std::string controls;
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"toggle\",\"channel\":" + std::to_string(channelOneBased);
    controls += "}'>" + powerLabel + "</button>";

    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"setFanAutomatic\",\"channel\":" + std::to_string(channelOneBased) + ",\"automatic\":";
    controls += _automatic ? "false" : "true";
    controls += "}'>" + automaticLabel + "</button>";

    const std::string value = std::string("Leistung: ") + (_power ? "Ein" : "Aus") + ", " + (_automatic ? "Automatik" : "Manuell");
    return renderCard(channelIndex, _name, "Fan", value, controls);
}

std::string WebVisuFan::webVisuJson(uint8_t channelIndex) const
{
    std::string json = "{";
    json += "\"kind\":\"fan\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"power\":" + std::string(_power ? "true" : "false") + ",";
    json += "\"automatic\":" + std::string(_automatic ? "true" : "false") + ",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

bool WebVisuFan::webVisuHandleCommand(const std::string& action, const std::string& message)
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

    if (action == "setFanAutomatic")
    {
        bool automatic = false;
        if (!parseBoolField(message, "automatic", automatic))
        {
            return false;
        }

        _channel->commandAutomatic(this, automatic);
        return true;
    }

    if (action == "setFanPower")
    {
        bool power = false;
        if (!parseBoolField(message, "power", power))
        {
            return false;
        }

        _channel->commandPower(this, power);
        return true;
    }

    return false;
}

#endif