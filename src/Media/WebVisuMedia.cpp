#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuMedia.h"

#include "../WebVisuBridge.h"

WebVisuMedia::WebVisuMedia(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuMedia::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuMedia::webVisuKind() const
{
    return "media";
}

void WebVisuMedia::setPlay(bool play)
{
    _play = play;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

void WebVisuMedia::setVolume(uint8_t volume)
{
    _volume = volume;
}

void WebVisuMedia::setTitle(const char* text)
{
    _title = text != nullptr ? text : "";
}

std::string WebVisuMedia::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuMedia::webVisuDetailHtml(uint8_t channelIndex) const
{
    const int channelOneBased = (int)channelIndex + 1;
    std::string controls;
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"setMediaPlay\",\"channel\":" + std::to_string(channelOneBased) + ",\"play\":";
    controls += _play ? "false" : "true";
    controls += "}'>" + std::string(_play ? "Pause" : "Play") + "</button>";
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"mediaPrevious\",\"channel\":" + std::to_string(channelOneBased) + "}'>Zurück</button>";
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"mediaNext\",\"channel\":" + std::to_string(channelOneBased) + "}'>Weiter</button>";
    controls += "<input class=\"webvisu-slider\" type=\"range\" min=\"0\" max=\"100\" step=\"1\" data-wv-payload-template='{";
    controls += "\"action\":\"setMediaVolume\",\"channel\":__CHANNEL__,\"volume\":__VALUE__}' data-channel=\"" + std::to_string(channelOneBased) + "\" value=\"" + std::to_string((int)_volume) + "\">";
    return renderCard(channelIndex, _name, "Media", _title.empty() ? std::to_string((int)_volume) + "%" : _title, controls);
}

std::string WebVisuMedia::webVisuJson(uint8_t channelIndex) const
{
    std::string json = "{";
    json += "\"kind\":\"media\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"play\":" + std::string(_play ? "true" : "false") + ",";
    json += "\"volume\":" + std::to_string((int)_volume) + ",";
    json += "\"title\":\"" + jsonEscape(_title) + "\",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

bool WebVisuMedia::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    if (_channel == nullptr)
    {
        return false;
    }

    if (action == "setMediaPlay")
    {
        bool play = false;
        if (!parseBoolField(message, "play", play))
        {
            return false;
        }
        _channel->commandPlay(this, play);
        return true;
    }

    if (action == "setMediaVolume")
    {
        int volume = 0;
        if (!parseIntField(message, "volume", volume))
        {
            return false;
        }
        if (volume < 0) volume = 0;
        if (volume > 100) volume = 100;
        _channel->commandVolume(this, (uint8_t)volume);
        return true;
    }

    if (action == "mediaNext")
    {
        _channel->commandNext(this);
        return true;
    }

    if (action == "mediaPrevious")
    {
        _channel->commandPrevious(this);
        return true;
    }

    return false;
}

#endif