#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuJalousie.h"

#include "../WebVisuBridge.h"

WebVisuJalousie::WebVisuJalousie(WebVisuBridge* webVisuBridge)
    : WebVisuRolladen(webVisuBridge)
{
}

std::string WebVisuJalousie::webVisuKind() const
{
    return "jalousie";
}

void WebVisuJalousie::setSlatPosition(uint8_t slatPosition)
{
    _slatPosition = slatPosition;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

std::string WebVisuJalousie::webVisuCardTypeLabel() const
{
    return "Jalousie";
}

std::string WebVisuJalousie::webVisuExtraControlsHtml(uint8_t channelIndex) const
{
    const int channelOneBased = (int)channelIndex + 1;
    std::string controls;
    controls += "<div class=\"webvisu-value\" style=\"width:100%;\">Lamellen: " + std::to_string((int)_slatPosition) + "%</div>";
    controls += "<div style=\"display:flex;align-items:center;gap:8px;\">";
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"setJalousieSlat\",\"channel\":" + std::to_string(channelOneBased) + ",\"slat\":100}'>↓</button>";
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"setJalousieSlat\",\"channel\":" + std::to_string(channelOneBased) + ",\"slat\":0}'>↑</button>";
    controls += "<input class=\"webvisu-slider\" style=\"flex:1;\" type=\"range\" min=\"0\" max=\"100\" step=\"1\" data-wv-payload-template='{";
    controls += "\"action\":\"setJalousieSlat\",\"channel\":__CHANNEL__,\"slat\":__VALUE__}' data-channel=\"" + std::to_string(channelOneBased) + "\" value=\"" + std::to_string((int)_slatPosition) + "\">";
    controls += "</div>";
    return controls;
}

std::string WebVisuJalousie::webVisuExtraJsonFields(uint8_t channelIndex) const
{
    (void)channelIndex;
    return ",\"slat\":" + std::to_string((int)_slatPosition);
}

bool WebVisuJalousie::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    if (_channel == nullptr)
    {
        return false;
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

    return WebVisuRolladen::webVisuHandleCommand(action, message);
}

#endif