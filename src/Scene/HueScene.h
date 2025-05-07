#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HueBridge.h"
#include "KnxChannelScene.h"

class HueScene : public SceneBridge
{
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;
public:
    HueScene(HueBridge* bridge);
    virtual void setup(uint8_t _channelIndex) override;
    boolean update();
    virtual void setActivating(bool activating) override;
};

#endif