#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelRolladen.h"
#include "../WebVisuWidgetBase.h"

class WebVisuBridge;

class WebVisuRolladen : public RolladenBridge, public WebVisuWidgetBase
{
public:
    explicit WebVisuRolladen(WebVisuBridge* webVisuBridge);

    virtual void setWebVisuName(const std::string& name) override;

    virtual void setPosition(uint8_t position) override;
    virtual void setMovement(MoveState movement) override;

    virtual std::string webVisuKind() const override;
    virtual std::string webVisuOverviewHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuDetailHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuJson(uint8_t channelIndex) const override;
    virtual bool webVisuHandleCommand(const std::string& action, const std::string& message) override;

protected:
    static const char* movementText(MoveState movement);
    virtual std::string webVisuCardTypeLabel() const;
    virtual std::string webVisuExtraControlsHtml(uint8_t channelIndex) const;
    virtual std::string webVisuExtraJsonFields(uint8_t channelIndex) const;

    std::string _name = "Unbenannt";
    uint8_t _position = 0;
    MoveState _movement = MoveStateHold;
    WebVisuBridge* _webVisuBridge;
};

#endif