#pragma once
#include "KnxChannelBase.h"

class KnxChannelAlarm;

class AlarmBridge : public ChannelBridgeBase<KnxChannelAlarm>
{
public:
    virtual void setDetected(bool detected) = 0;
};

enum AlarmType
{
    AlarmTypeContact = 0,
    AlarmTypeMotion = 1,
    AlarmTypeOccupancy = 2,
    AlarmTypeLeak = 3,
    AlarmTypeSmoke = 4,
    AlarmTypeCarbonDioxid = 5,
    AlarmTypeCarbonMonoxid = 6
};
             
class KnxChannelAlarm : public KnxChannelBase
{
        DynamicPointerArray<AlarmBridge > sensorBridges;
    public:
        KnxChannelAlarm(uint16_t channelIndex);
        void add(AlarmBridge* sensorBridge);
        void remove(AlarmBridge* sensorBridge);
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;
        AlarmType getAlarmType();
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;
        virtual const std::string name() override;
        virtual MainFunctionStateImage mainFunctionTypeImage(uint8_t value) override;
    public:
        virtual void commandMainFunctionClick() override;
        virtual bool supportMainFunctionClick() override { return false; }
        virtual std::string currentValueAsString() override;
        virtual bool mainFunctionValue() override;
        virtual MainFunctionStateImage mainFunctionImage() override;
};
