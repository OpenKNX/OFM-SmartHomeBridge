#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HueBridge.h"
#include "KnxChannelMedia.h"

class HueMedia : public MediaBridge
{
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;

public:
    HueMedia(HueBridge* bridge);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setBrightness(uint8_t brightness) override;
};

#endif