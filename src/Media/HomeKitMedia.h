#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HomeSpan.h"
#include "KnxChannelMedia.h"
#if 0
class HomeKitMedia : public MediaBridge
{
    int device;
    Characteristic::On *power;
    Characteristic::Brightness *level;
    class ServiceImplementation : public Service::LightBulb
    {
        HomeKitMedia* parent;
        virtual bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementation(HomeKitMedia* parent) : parent(parent)
        {
        }
    };
public:
    HomeKitMedia(int device);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setBrightness(uint8_t brightness) override;
};

#endif
#endif