#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuDimmer.h"

#include "../WebVisuBridge.h"

WebVisuDimmer::WebVisuDimmer(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuDimmer::setBrightness(uint8_t brightness)
{
    if (_channel == nullptr || _webVisuBridge == nullptr)
        return;

    _webVisuBridge->reportDimmerState(_channel->channelIndex(), _channel->getNameInUTF8(), brightness);
}

std::string WebVisuDimmer::renderWidgetHtml(uint8_t channelIndex, const std::string& name, uint8_t brightness)
{
    const bool isOn = brightness > 0;
    const int channelOneBased = (int)channelIndex + 1;

    std::string controls;
    controls += "<button class=\"webvisu-btn\" data-action=\"toggle\" data-channel=\"";
    controls += std::to_string(channelOneBased);
    controls += "\">";
    controls += isOn ? "Aus" : "Ein";
    controls += "</button>";

    std::string detail;
    detail += "<input class=\"webvisu-slider\" type=\"range\" min=\"0\" max=\"100\" step=\"1\" data-action=\"setDimmer\" data-channel=\"";
    detail += std::to_string(channelOneBased);
    detail += "\" value=\"";
    detail += std::to_string((int)brightness);
    detail += "\">";

    return renderCard(channelIndex, name, "Dimmer", std::to_string((int)brightness) + "%", controls, detail);
}

#endif
