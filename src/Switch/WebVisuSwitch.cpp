#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuSwitch.h"

#include "../WebVisuBridge.h"

WebVisuSwitch::WebVisuSwitch(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuSwitch::setPower(bool on)
{
    if (_channel == nullptr || _webVisuBridge == nullptr)
        return;

    _webVisuBridge->reportSwitchState(_channel->channelIndex(), _channel->getNameInUTF8(), on);
}

std::string WebVisuSwitch::renderWidgetHtml(uint8_t channelIndex, const std::string& name, bool power)
{
    const std::string value = power ? "Ein" : "Aus";
    const std::string next = power ? "Aus" : "Ein";

    std::string controls;
    controls += "<button class=\"webvisu-btn\" data-action=\"toggle\" data-channel=\"";
    controls += std::to_string((int)channelIndex + 1);
    controls += "\">" + next + "</button>";

    return renderCard(channelIndex, name, "Switch", value, controls);
}

#endif
