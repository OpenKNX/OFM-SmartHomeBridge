#pragma once
#include "KnxChannelBase.h"

class KnxChannelDisplay;

class DisplayBridge : public ChannelBridgeBase<KnxChannelDisplay>
{
public:
    virtual void setValue(double value) = 0;
};

enum DisplayType
{
    DisplayTypeTemperature,
    DisplayTypeHumidity,
    DisplayTypeLux,
};

class KnxChannelDisplay : public KnxChannelBase
{
        DynamicPointerArray<DisplayBridge> displayBridges;
    public:
        KnxChannelDisplay(uint16_t channelIndex);
        DisplayType getDisplayType();
        void add(DisplayBridge* displayBridge);
        void remove(DisplayBridge* displayBridge);
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual const std::string name() override;

};
