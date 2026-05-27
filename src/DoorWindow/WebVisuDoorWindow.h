#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelDoorWindow.h"
#include "../WebVisuWidgetBase.h"

class WebVisuBridge;

class WebVisuDoorWindow : public DoorWindowBridge, public WebVisuWidgetBase
{
public:
    explicit WebVisuDoorWindow(WebVisuBridge* webVisuBridge);

    virtual void setWebVisuName(const std::string& name) override;

    virtual void setPosition(uint8_t position) override;
    virtual void setMovement(DoorWindowMoveState movement) override;
    virtual void setObstructionDetected(bool obstructionDetected) override;

    virtual std::string webVisuKind() const override;
    virtual std::string webVisuOverviewHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuDetailHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuJson(uint8_t channelIndex) const override;
    virtual bool webVisuHandleCommand(const std::string& action, const std::string& message) override;

private:
    std::string _name = "Unbenannt";
    uint8_t _position = 0;
    DoorWindowMoveState _movement = DoorWindowMoveStateHold;
    bool _obstructionDetected = false;
    WebVisuBridge* _webVisuBridge;
};

#endif