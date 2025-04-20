#include <knx.h>
#include "KnxProd.h"
#include "KnxChannelSensor.h"
#include "BridgeBase.h"

#define KO_SENSOR_FEEDBACK   KoBRI_KO1_, DPT_Switch

KnxChannelSensor::KnxChannelSensor(uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      sensorBridges()
{
}

ChannelBridge* KnxChannelSensor::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createSensor(*this, _channelIndex, ParamBRI_CHDeviceType);
}

void KnxChannelSensor::deleteBridgeDevice(ChannelBridge *device)
{
    remove((SensorBridge *)device);
}

void KnxChannelSensor::add(SensorBridge* sensorBridge)
{
    sensorBridges.push_back(sensorBridge);
    sensorBridge->initialize(this);
    auto value = (bool) koGet(KO_SENSOR_FEEDBACK);
    if (ParamBRI_CHContactAlarmSensorInvert)
        value = !value;   
    sensorBridge->setDetected(value);       
}

void KnxChannelSensor::remove(SensorBridge* sensorBridge)
{
    sensorBridges.remove(sensorBridge);
    delete sensorBridge;
}

const std::string KnxChannelSensor::name()
{
    return std::string("Sensor");
}

SensorType KnxChannelSensor::getSensorType()
{
    return (SensorType) ParamBRI_CHDeviceType;
}

void KnxChannelSensor::setup()
{
    koSetWithoutSend(KO_SENSOR_FEEDBACK, ParamBRI_CHContactAlarmSensorInvert == 1);
    koSendReadRequest(KO_SENSOR_FEEDBACK);
}

void KnxChannelSensor::processInputKo(GroupObject &ko)
{
    if (isKo(ko, KoBRI_KO1_))
    {
        bool value = koGet(KO_SENSOR_FEEDBACK);
        if (!ParamBRI_CHContactAlarmSensorInvert)
            value = !value;
        for (auto it = sensorBridges.begin(); it != sensorBridges.end(); ++it)
        {
            (*it)->setDetected(value);
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();

    }
}

void KnxChannelSensor::commandMainFunctionClick()
{
    
}

std::string KnxChannelSensor::currentValueAsString()
{
    return koGet(KO_SENSOR_FEEDBACK) ? "Aktiv" : "Inaktiv";
}

bool KnxChannelSensor::mainFunctionValue()
{
    return koGet(KO_SENSOR_FEEDBACK);
}

MainFunctionStateImage KnxChannelSensor::mainFunctionImage()
{
   return calculateMainFunctionImage(KO_SENSOR_FEEDBACK, 0, LIMIT_NOT_USED, 100);
}
