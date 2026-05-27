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
    virtual void setWebVisuName(const std::string& name) override;
    virtual void setPower(bool on) override;
    virtual std::string webVisuKind() const override;
    virtual std::string webVisuOverviewHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuDetailHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuJson(uint8_t channelIndex) const override;
    virtual bool webVisuHandleCommand(const std::string& action, const std::string& message) override;
    static std::string buildDeviceJson(uint8_t channelIndex, const std::string& name, bool power);
    static std::string renderWidgetHtml(uint8_t channelIndex, const std::string& name, bool power);

  private:
    std::string _name = "Unbenannt";
    bool _power = false;
    WebVisuBridge* _webVisuBridge;
};

#endif
