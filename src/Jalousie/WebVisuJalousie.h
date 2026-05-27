#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelJalousie.h"
#include "../Rolladen/WebVisuRolladen.h"

class WebVisuBridge;

class WebVisuJalousie : public WebVisuRolladen
{
public:
    explicit WebVisuJalousie(WebVisuBridge* webVisuBridge);

    virtual void setSlatPosition(uint8_t slatPosition) override;

    virtual std::string webVisuKind() const override;
    virtual bool webVisuHandleCommand(const std::string& action, const std::string& message) override;

protected:
    virtual std::string webVisuCardTypeLabel() const override;
    virtual std::string webVisuExtraControlsHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuExtraJsonFields(uint8_t channelIndex) const override;

private:
    uint8_t _slatPosition = 0;
};

#endif