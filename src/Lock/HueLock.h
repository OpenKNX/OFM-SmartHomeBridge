#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HueBridge.h"
#include "KnxChannelLock.h"

class HueLock : public LockBridge
{
    HueBridge* hueBridge;
    EspalexaDevice* espalexaDevice;
public:
    HueLock(HueBridge* bridge);
    virtual void setup(uint8_t _channelIndex) override;
    boolean update();
    virtual void setLocked(bool locked) override;
    virtual void setBlocked(bool blocked) override;
};

#endif