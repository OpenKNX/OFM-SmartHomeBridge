#include <knx.h>
#include "KnxProd.h"
#include "KnxChannelAlarm.h"
#include "BridgeBase.h"

#define KO_SENSOR_FEEDBACK   KoBRI_KO1_, DPT_Switch

KnxChannelAlarm::KnxChannelAlarm(uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      sensorBridges()
{
}

ChannelBridge* KnxChannelAlarm::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createSensor(*this, _channelIndex, ParamBRI_CHDeviceType);
}

void KnxChannelAlarm::deleteBridgeDevice(ChannelBridge *device)
{
    remove((AlarmBridge *)device);
}

void KnxChannelAlarm::add(AlarmBridge* sensorBridge)
{
    sensorBridges.push_back(sensorBridge);
    sensorBridge->initialize(this);
    auto value = (bool) koGet(KO_SENSOR_FEEDBACK);
    if (ParamBRI_CHAlarmInvert)
        value = !value;   
    sensorBridge->setDetected(value);       
}

void KnxChannelAlarm::remove(AlarmBridge* sensorBridge)
{
    sensorBridges.remove(sensorBridge);
    delete sensorBridge;
}

const std::string KnxChannelAlarm::name()
{
    return std::string("Sensor");
}

AlarmType KnxChannelAlarm::getAlarmType()
{
    return (AlarmType) ParamBRI_CHAlarmType;
}

void KnxChannelAlarm::setup()
{
    koSetWithoutSend(KO_SENSOR_FEEDBACK, ParamBRI_CHAlarmInvert == 1);
    koSendReadRequest(KO_SENSOR_FEEDBACK);
}

void KnxChannelAlarm::processInputKo(GroupObject &ko)
{
    if (isKo(ko, KoBRI_KO1_))
    {
        bool value = koGet(KO_SENSOR_FEEDBACK);
        if (!ParamBRI_CHAlarmInvert)
            value = !value;
        for (auto it = sensorBridges.begin(); it != sensorBridges.end(); ++it)
        {
            (*it)->setDetected(value);
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();

    }
}

MainFunctionStateImage KnxChannelAlarm::mainFunctionTypeImage(uint8_t value)
{
    std::string image = "Type";
    image += std::to_string(ParamBRI_CHDeviceType);
    image += "_";
    image += std::to_string(ParamBRI_CHAlarmType);
    image += ".png";
    return {true, image};
}



void KnxChannelAlarm::commandMainFunctionClick()
{
    
}

std::string KnxChannelAlarm::currentValueAsString()
{
    return koGet(KO_SENSOR_FEEDBACK) ? "Aktiv" : "Inaktiv";
}

bool KnxChannelAlarm::mainFunctionValue()
{
    return koGet(KO_SENSOR_FEEDBACK);
}

MainFunctionStateImage KnxChannelAlarm::mainFunctionImage()
{
   return calculateMainFunctionImage(KO_SENSOR_FEEDBACK, 0, LIMIT_NOT_USED, 100);
}
