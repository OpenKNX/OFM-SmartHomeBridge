#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelRGB.h"
#include "../WebVisuWidgetBase.h"

class WebVisuBridge;

class WebVisuRGB : public RGBBridge, public WebVisuWidgetBase
{
public:
    explicit WebVisuRGB(WebVisuBridge* webVisuBridge);

    virtual void setWebVisuName(const std::string& name) override;

    virtual void setRGB(uint32_t rgb) override;
    virtual void setPower(bool on) override;

    virtual std::string webVisuKind() const override;
    virtual std::string webVisuOverviewHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuDetailHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuJson(uint8_t channelIndex) const override;
    virtual bool webVisuHandleCommand(const std::string& action, const std::string& message) override;

private:
    std::string _name = "Unbenannt";
    uint32_t _rgb = 0xFFFFFF;
    bool _power = false;
    WebVisuBridge* _webVisuBridge;
};

#endif