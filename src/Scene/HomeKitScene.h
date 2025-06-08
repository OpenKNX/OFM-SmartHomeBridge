#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HomeSpan.h"
#include "KnxChannelScene.h"

class HomeKitScene : public SceneBridge
{
    int device;
    Characteristic::On *power;
    class ServiceImplementationLightBulb : public Service::LightBulb
    {
        HomeKitScene* parent;
        bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementationLightBulb(HomeKitScene* parent) : parent(parent)
        {
        }
    };
public:
    HomeKitScene(int device);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setActivating(bool activating) override;
   
};

#endif