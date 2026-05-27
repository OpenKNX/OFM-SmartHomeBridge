#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelFan.h"
#include "../WebVisuWidgetBase.h"

class WebVisuBridge;

class WebVisuFan : public FanBridge, public WebVisuWidgetBase
{
public:
    explicit WebVisuFan(WebVisuBridge* webVisuBridge);

    virtual void setWebVisuName(const std::string& name) override;

    virtual void setAutomatic(bool automatic) override;
    virtual void setPower(bool on) override;

    virtual std::string webVisuKind() const override;
    virtual std::string webVisuOverviewHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuDetailHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuJson(uint8_t channelIndex) const override;
    virtual bool webVisuHandleCommand(const std::string& action, const std::string& message) override;

private:
    std::string _name = "Unbenannt";
    bool _power = false;
    bool _automatic = false;
    WebVisuBridge* _webVisuBridge;
};

#endif