#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HomeSpan.h"
#include "KnxChannelAlarm.h"

class HomeKitAlarm : public AlarmBridge
{
    int device;
    SpanCharacteristic *currentValue = nullptr;
public:
    HomeKitAlarm(int device);
    virtual void setup(uint8_t _channelIndex) override;
    void createAccessory();

    boolean update();
    virtual void setDetected(bool value) override;
};

#endif