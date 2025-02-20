#pragma once
#include "KnxChannelBase.h"

class KnxChannelFan;

class FanBridge : public ChannelBridgeBase<KnxChannelFan>
{
public:
    virtual void setAutomatic(bool automatic) = 0;
    virtual void setPower(bool on) = 0;
};

class KnxChannelFan : public KnxChannelBase
{
        DynamicPointerArray<FanBridge> fanBridges; 
    public:
        KnxChannelFan(uint16_t channelIndex);
        void add(FanBridge* fanBridge);
        void remove(FanBridge* fanBridge);
        virtual void* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(void* device) override;

    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandPower(FanBridge* fanBridge, bool on);
        void commandAutomatic(FanBridge* fanBridge, bool on);
        virtual const std::string name() override;
};
