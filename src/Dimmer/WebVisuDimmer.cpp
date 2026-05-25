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

#endif
