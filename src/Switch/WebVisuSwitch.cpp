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

#endif
