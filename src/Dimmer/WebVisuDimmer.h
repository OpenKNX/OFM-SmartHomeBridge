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
    static std::string buildDeviceJson(uint8_t channelIndex, const std::string& name, uint8_t brightness);
    static std::string renderWidgetHtml(uint8_t channelIndex, const std::string& name, uint8_t brightness);

  private:
    std::string _name = "Unbenannt";
    uint8_t _lastBrightness = 0;
    WebVisuBridge* _webVisuBridge;
    void publishBrightness(uint8_t brightness);
};

#endif
