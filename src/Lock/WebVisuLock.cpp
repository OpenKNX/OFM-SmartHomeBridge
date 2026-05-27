#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuLock.h"

#include "../WebVisuBridge.h"

WebVisuLock::WebVisuLock(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuLock::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuLock::webVisuKind() const
{
    return "lock";
}

void WebVisuLock::setLocked(bool lock)
{
    _locked = lock;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

void WebVisuLock::setBlocked(bool lock)
{
    _blocked = lock;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

void WebVisuLock::setUnlocking(bool unlocking)
{
    _unlocking = unlocking;
}

void WebVisuLock::setLocking(bool locking)
{
    _locking = locking;
}

std::string WebVisuLock::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuLock::webVisuDetailHtml(uint8_t channelIndex) const
{
    const int channelOneBased = (int)channelIndex + 1;
    const std::string actionLabel = _locked ? "Entriegeln" : "Verriegeln";
    const std::string stateText = _blocked ? "Blockiert" : (_locking ? "Verriegeln" : (_unlocking ? "Entriegeln" : (_locked ? "Verriegelt" : "Entriegelt")));

    std::string controls;
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"setLock\",\"channel\":" + std::to_string(channelOneBased) + ",\"lock\":";
    controls += _locked ? "false" : "true";
    controls += "}'>" + actionLabel + "</button>";

    if (_blocked)
    {
        controls += "<span class=\"webvisu-type\">Gesperrt</span>";
    }

    return renderCard(channelIndex, _name, "Lock", stateText, controls);
}

std::string WebVisuLock::webVisuJson(uint8_t channelIndex) const
{
    std::string json = "{";
    json += "\"kind\":\"lock\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"locked\":" + std::string(_locked ? "true" : "false") + ",";
    json += "\"blocked\":" + std::string(_blocked ? "true" : "false") + ",";
    json += "\"unlocking\":" + std::string(_unlocking ? "true" : "false") + ",";
    json += "\"locking\":" + std::string(_locking ? "true" : "false") + ",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

bool WebVisuLock::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    if (action != "setLock" || _channel == nullptr)
    {
        return false;
    }

    bool lock = false;
    if (!parseBoolField(message, "lock", lock))
    {
        return false;
    }

    _channel->commandLock(this, lock);
    return true;
}

#endif