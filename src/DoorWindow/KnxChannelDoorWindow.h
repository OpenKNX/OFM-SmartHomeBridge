#pragma once
#include "KnxChannelBase.h"

class KnxChannelDoorWindow;

enum DoorWindowMoveState : byte
{
    DoorWindowMoveStateHold,
    DoorWindowMoveStateClosing,
    DoorWindowMoveStateOpening
};

enum DoorWindowHandling : byte
{
    DoorWindowHandlingNothing,
    DoorWindowHandlingSendClose,
    DoorWindowHandlingSendOpen,
    DoorWindowHandlingSendOpenAndClose
};

class DoorWindowBridge : public ChannelBridgeBase<KnxChannelDoorWindow>
{
public:
    virtual void setPosition(uint8_t position) = 0;
    virtual void setMovement(DoorWindowMoveState movement) = 0;
    virtual void setObstructionDetected(bool obstructionDetected) = 0;
};

class KnxChannelDoorWindow : public KnxChannelBase
{
        DoorWindowMoveState _currentMovement = DoorWindowMoveStateHold;
        DynamicPointerArray<DoorWindowBridge> interfaces; 
    public:
        KnxChannelDoorWindow(uint16_t channelIndex);
        void add(DoorWindowBridge* interface);
        void remove(DoorWindowBridge* interface);
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;

    protected:
        volatile bool updatePosition = false;

        virtual void setup() override;
        virtual void loop() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual bool useStop();
        virtual DoorWindowHandling getDoorWindowHandling();
        uint8_t currentPosition();

    public:
        virtual bool commandPosition(DoorWindowBridge* interface, uint8_t position);
        virtual void commandMainFunctionClick() override;
        virtual const std::string name() override;
        virtual std::string currentValueAsString() override;
        virtual bool mainFunctionValue() override;
        virtual MainFunctionStateImage mainFunctionImage() override;
};
