#pragma once
#include "../Rolladen/KnxChannelRolladen.h"
#include "../Jalousie/KnxChannelJalousie.h"

class KnxChannelJalousie;

class KnxChannelJalousie : public KnxChannelRolladen
{
    public:
        KnxChannelJalousie(uint16_t channelIndex);
        void add(RolladenBridge* interface);
        void remove(RolladenBridge* interface);
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;

    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual BlindsHandling getBlindsHandling() override;
        virtual bool useStop() override;

    public:
        virtual bool commandPosition(RolladenBridge* interface, uint8_t position) override;
        void commandSlatPosition(RolladenBridge* interface, uint8_t slatPosition);
        virtual void commandMainFunctionClick() override;
        virtual const std::string name() override;
        virtual MainFunctionStateImage mainFunctionImage() override;
};
