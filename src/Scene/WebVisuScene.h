#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelScene.h"
#include "../WebVisuWidgetBase.h"

class WebVisuBridge;

class WebVisuScene : public SceneBridge, public WebVisuWidgetBase
{
public:
    explicit WebVisuScene(WebVisuBridge* webVisuBridge);

    virtual void setWebVisuName(const std::string& name) override;

    virtual void setActivating(bool activating) override;

    virtual std::string webVisuKind() const override;
    virtual std::string webVisuOverviewHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuDetailHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuJson(uint8_t channelIndex) const override;
    virtual bool webVisuHandleCommand(const std::string& action, const std::string& message) override;

private:
    std::string _name = "Unbenannt";
    bool _activating = false;
    WebVisuBridge* _webVisuBridge;
};

#endif