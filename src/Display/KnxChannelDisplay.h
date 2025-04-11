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
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;

    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual const std::string name() override;
    public:
        virtual void commandMainFunctionClick() override;
        virtual bool supportMainFunctionClick() override { return false; }
        virtual std::string currentValueAsString() override;
        virtual bool mainFunctionValue() override;
        virtual MainFunctionStateImage mainFunctionImage() override;

};
