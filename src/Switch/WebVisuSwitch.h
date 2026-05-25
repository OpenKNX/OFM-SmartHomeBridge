#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "KnxChannelSwitch.h"

class WebVisuBridge;

class WebVisuSwitch : public SwitchBridge
{
  public:
    explicit WebVisuSwitch(WebVisuBridge* webVisuBridge);
    virtual void setPower(bool on) override;

  private:
    WebVisuBridge* _webVisuBridge;
};

#endif
