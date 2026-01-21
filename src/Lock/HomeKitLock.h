#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HomeSpan.h"
#include "KnxChannelLock.h"

class HomeKitLock : public LockBridge
{
    int device;
    bool _locked = false;
    bool _blocked = false;
    bool _targetStateInitialized = false;
    volatile bool _ignoreUpdate = false;
    Characteristic::LockCurrentState *lockCurrentState;
    Characteristic::LockTargetState *lockTargetState;
   
    class ServiceImplementationLock : public Service::LockMechanism
    {
        HomeKitLock* parent;
        bool update() override
        {
            return parent->update();
        }
    public:
        ServiceImplementationLock(HomeKitLock* parent) : parent(parent)
        {
        }
    };
    void updateState();
public:
    HomeKitLock(int device);
    virtual void setup(uint8_t _channelIndex) override;

    boolean update();
    virtual void setLocked(bool locked) override;
    virtual void setBlocked(bool blocked) override;
};

#endif