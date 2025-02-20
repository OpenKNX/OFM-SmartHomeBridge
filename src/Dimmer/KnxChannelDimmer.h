#pragma once
#include "KnxChannelBase.h"

class KnxChannelDimmer;

class DimmerBridge : public ChannelBridgeBase<KnxChannelDimmer>
{
public:
    virtual void setBrightness(uint8_t brightness) = 0;
};

class KnxChannelDimmer : public KnxChannelBase
{
      DynamicPointerArray<DimmerBridge> dimmerBridges;
    public:
        KnxChannelDimmer(uint16_t channelIndex);
        void add(DimmerBridge* dimmerBridge);
        void remove(DimmerBridge* dimmerBridge);
        virtual void* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(void* device) override;

    protected:
        uint8_t lastBrighness = 100;
        uint8_t lastBrighnessLessThan100 = 50;
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandPower(DimmerBridge* dimmerBridge, bool on);
        void commandBrightness(DimmerBridge* dimmerBridge, uint8_t brightness);
        virtual const std::string name() override;
};
