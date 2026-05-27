#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelAlarm.h"
#include "../WebVisuWidgetBase.h"

class WebVisuBridge;

class WebVisuAlarm : public AlarmBridge, public WebVisuWidgetBase
{
public:
    explicit WebVisuAlarm(WebVisuBridge* webVisuBridge);

    virtual void setWebVisuName(const std::string& name) override;

    virtual void setDetected(bool detected) override;

    virtual std::string webVisuKind() const override;
    virtual std::string webVisuOverviewHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuDetailHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuJson(uint8_t channelIndex) const override;
    virtual bool webVisuHandleCommand(const std::string& action, const std::string& message) override;

private:
    std::string _name = "Unbenannt";
    bool _detected = false;
    WebVisuBridge* _webVisuBridge;
};

#endif