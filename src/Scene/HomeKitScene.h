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
    class ServiceImplementationScene : public Service::Scene
    {
        HomeKitScene* parent;
        bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementationScene(HomeKitScene* parent) : parent(parent)
        {
        }
    };
    class ServiceImplementationOutlet : public Service::Outlet
    {
        HomeKitScene* parent;
        bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementationOutlet(HomeKitScene* parent) : parent(parent)
        {
        }
    };
public:
    HomeKitScene(int device);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setActivated(bool activating) override;
};

#endif