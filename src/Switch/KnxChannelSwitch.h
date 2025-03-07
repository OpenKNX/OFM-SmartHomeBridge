#pragma once
#include "KnxChannelBase.h"

class KnxChannelSwitch;

class SwitchBridge : public ChannelBridgeBase<KnxChannelSwitch>
{
public:
    virtual void setPower(bool on) = 0;
};

class KnxChannelSwitch : public KnxChannelBase
{
        DynamicPointerArray<SwitchBridge> switchBridges; 
    public:
        KnxChannelSwitch(uint16_t channelIndex);
        void add(SwitchBridge* switchBridge);
        void remove(SwitchBridge* switchBridge);
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandPower(SwitchBridge* switchBridge, bool on);
        virtual void commandMainFunctionClick() override;
        virtual const std::string name() override;
};
