#pragma once
#include "KnxChannelBase.h"

class KnxChannelSensor;

class SensorBridge : public ChannelBridgeBase<KnxChannelSensor>
{
public:
    virtual void setDetected(bool detected) = 0;
};

enum SensorType
{
    SensorTypeContact = 70,
    SensorTypeMotion = 71,
    SensorTypeOccupancy = 72,
    SensorTypeLeak = 73,
    SensorTypeSmoke = 74,
    SensorTypeCarbonDioxid = 75,
    SensorTypeCarbonMonoxid = 76
};
             
class KnxChannelSensor : public KnxChannelBase
{
        DynamicPointerArray<SensorBridge > sensorBridges;
    public:
        KnxChannelSensor(uint16_t channelIndex);
        void add(SensorBridge* sensorBridge);
        void remove(SensorBridge* sensorBridge);
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;
        SensorType getSensorType();
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
