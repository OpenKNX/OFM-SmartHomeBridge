#pragma once
#include "KnxChannelBase.h"

class KnxChannelLock;

class LockBridge : public ChannelBridgeBase<KnxChannelLock>
{
public:
    virtual void setLocked(bool lock) = 0;
    virtual void setBlocked(bool lock) = 0;
};

class KnxChannelLock : public KnxChannelBase
{
        DynamicPointerArray<LockBridge> lockBridges; 
    private:
        bool isLocked();
    public:
        KnxChannelLock(uint16_t channelIndex);
        void add(LockBridge* lockBridge);
        void remove(LockBridge* lockBridge);
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandLock(LockBridge* switchBridge, bool lock);
        virtual void commandMainFunctionClick() override;
        virtual const std::string name() override;
        virtual std::string currentValueAsString() override;
        virtual bool mainFunctionValue() override;
        virtual MainFunctionStateImage mainFunctionImage() override;
};
