#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelDimmer.h"
#include "../WebVisuWidgetBase.h"

class WebVisuBridge;

class WebVisuDimmer : public DimmerBridge, public WebVisuWidgetBase
{
  public:
    explicit WebVisuDimmer(WebVisuBridge* webVisuBridge);
    virtual void setBrightness(uint8_t brightness) override;
    static std::string renderWidgetHtml(uint8_t channelIndex, const std::string& name, uint8_t brightness);

  private:
    WebVisuBridge* _webVisuBridge;
};

#endif
