#pragma once
#include "KnxChannelBase.h"

class KnxChannelRGB;


class RGBBridge : public ChannelBridgeBase<KnxChannelRGB>
{
public:
    virtual void setRGB(uint32_t rgb) = 0;
    virtual void setPower(bool on) = 0;
};

class KnxChannelRGB : public KnxChannelBase
{
        DynamicPointerArray<RGBBridge> RGBBridges;
        uint32_t getColorForBehavior(uint8_t behavior);
    public:
        KnxChannelRGB(uint16_t channelIndex);
        void add(RGBBridge* RGBBridge);
        void remove(RGBBridge* RGBBridge);
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;

    protected:
        uint32_t lastColor = 0xFFFFFF;
        uint32_t lastColorLessOtherThanWhite = 0x808080;
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandPower(RGBBridge* RGBBridge, bool on);
        virtual void commandMainFunctionClick() override;
        void commandRGB(RGBBridge* RGBBridge, uint32_t rgb);
        virtual const std::string name() override;
        virtual std::string currentValueAsString() override;
        virtual bool mainFunctionValue() override;
        virtual MainFunctionStateImage mainFunctionImage() override;
};
