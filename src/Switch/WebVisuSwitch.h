#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelSwitch.h"
#include "../WebVisuWidgetBase.h"

class WebVisuBridge;

class WebVisuSwitch : public SwitchBridge, public WebVisuWidgetBase
{
  public:
    explicit WebVisuSwitch(WebVisuBridge* webVisuBridge);
    virtual void setPower(bool on) override;
    static std::string buildDeviceJson(uint8_t channelIndex, const std::string& name, bool power);
    static std::string renderWidgetHtml(uint8_t channelIndex, const std::string& name, bool power);

  private:
    std::string _name = "Unbenannt";
    bool _power = false;
    WebVisuBridge* _webVisuBridge;
};

#endif
