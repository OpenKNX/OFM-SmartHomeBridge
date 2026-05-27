#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelMedia.h"
#include "../WebVisuWidgetBase.h"

class WebVisuBridge;

class WebVisuMedia : public MediaBridge, public WebVisuWidgetBase
{
public:
    explicit WebVisuMedia(WebVisuBridge* webVisuBridge);

    virtual void setWebVisuName(const std::string& name) override;

    virtual void setPlay(bool play) override;
    virtual void setVolume(uint8_t volume) override;
    virtual void setTitle(const char* text) override;

    virtual std::string webVisuKind() const override;
    virtual std::string webVisuOverviewHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuDetailHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuJson(uint8_t channelIndex) const override;
    virtual bool webVisuHandleCommand(const std::string& action, const std::string& message) override;

private:
    std::string _name = "Unbenannt";
    bool _play = false;
    uint8_t _volume = 0;
    std::string _title;
    WebVisuBridge* _webVisuBridge;
};

#endif