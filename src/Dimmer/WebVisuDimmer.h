#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "KnxChannelDimmer.h"

class WebVisuBridge;

class WebVisuDimmer : public DimmerBridge
{
  public:
    explicit WebVisuDimmer(WebVisuBridge* webVisuBridge);
    virtual void setBrightness(uint8_t brightness) override;

  private:
    WebVisuBridge* _webVisuBridge;
};

#endif
