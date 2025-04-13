#pragma once
#include "KnxChannelBase.h"

class KnxChannelDisplay;

class DisplayBridge : public ChannelBridgeBase<KnxChannelDisplay>
{
public:
    virtual void setValue(double value) = 0;
    virtual void setValue(const char* value) = 0;
};

enum DisplayType
{
    DisplayTypeTemperature,
    DisplayTypeHumidity,
    DisplayTypeLux,
    DisplayTyppeRain,
    DisplayTypeSnow,
    DisplayTypeWind,
    DisplayTypeText = 9,
};

class KnxChannelDisplay : public KnxChannelBase
{
        DynamicPointerArray<DisplayBridge> displayBridges;

        double lastValue = 0;
        std::string lastStringValue = std::string();
        bool hasValue = false;
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
        virtual bool mainFunctionPreferValueDisplay() override;
        int getIconState();

};
