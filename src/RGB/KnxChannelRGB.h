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
    public:
        KnxChannelRGB(uint16_t channelIndex);
        void add(RGBBridge* RGBBridge);
        void remove(RGBBridge* RGBBridge);
    protected:
        uint32_t lastColor = 0xFFFFFF;
        uint32_t lastColorLessOtherThanWhite = 0x808080;
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandPower(RGBBridge* RGBBridge, bool on);
        void commandRGB(RGBBridge* RGBBridge, uint32_t rgb);
        virtual const std::string name() override;
};
