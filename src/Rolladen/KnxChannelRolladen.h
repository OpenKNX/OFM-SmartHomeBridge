#pragma once
#include "KnxChannelBase.h"

class KnxChannelRolladen;

enum MoveState
{
    MoveStateHold,
    MoveStateUp,
    MoveStateDown
};

enum BlindsHandling
{
    BlindsHandlingNothing,
    BlindsHandlingSendDown,
    BlindsHandlingSendUp,
    BlindsHandlingSendUpAndDown
};

class RolladenBridge : public ChannelBridgeBase<KnxChannelRolladen>
{
public:
    virtual void setPosition(uint8_t position) = 0;
    virtual void setMovement(MoveState movement) = 0;
    virtual void setSlatPosition(uint8_t slatPosition){}
};

class KnxChannelRolladen : public KnxChannelBase
{
    protected:
       DynamicPointerArray<RolladenBridge> interfaces; 
    public:
        KnxChannelRolladen(uint16_t channelIndex);
        void add(RolladenBridge* interface);
        void remove(RolladenBridge* interface);
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;
    
    protected:
        volatile bool updatePosition = false;

        virtual void setup() override;
        virtual void loop() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual BlindsHandling getBlindsHandling();
        virtual bool useStop();
        uint8_t currentPosition();

    public:
        virtual bool commandPosition(RolladenBridge* interface, uint8_t position);
        virtual void commandSlatPosition(RolladenBridge* interface, uint8_t slatPosition) {}
        virtual void commandMainFunctionClick() override;
        virtual const std::string name() override;
        virtual std::string currentValueAsString() override;
        virtual bool mainFunctionValue() override;
        virtual MainFunctionStateImage mainFunctionImage() override;
        virtual MainFunctionStateImage mainFunctionTypeImage(uint8_t value) override;
};
