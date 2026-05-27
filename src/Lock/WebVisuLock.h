#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelLock.h"
#include "../WebVisuWidgetBase.h"

class WebVisuBridge;

class WebVisuLock : public LockBridge, public WebVisuWidgetBase
{
public:
    explicit WebVisuLock(WebVisuBridge* webVisuBridge);

    virtual void setWebVisuName(const std::string& name) override;

    virtual void setLocked(bool lock) override;
    virtual void setBlocked(bool lock) override;
    virtual void setUnlocking(bool unlocking) override;
    virtual void setLocking(bool locking) override;

    virtual std::string webVisuKind() const override;
    virtual std::string webVisuOverviewHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuDetailHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuJson(uint8_t channelIndex) const override;
    virtual bool webVisuHandleCommand(const std::string& action, const std::string& message) override;

private:
    std::string _name = "Unbenannt";
    bool _locked = false;
    bool _blocked = false;
    bool _unlocking = false;
    bool _locking = false;
    WebVisuBridge* _webVisuBridge;
};

#endif