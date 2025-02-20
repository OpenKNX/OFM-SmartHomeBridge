#include "BridgeBase.h"

#include "./Switch/KnxChannelSwitch.h"
#include "./Dimmer/KnxChannelDimmer.h"
#include "./RGB/KnxChannelRGB.h"
#include "./Rolladen/KnxChannelRolladen.h"
#include "./Jalousie/KnxChannelJalousie.h"
#include "./Thermostat/KnxChannelThermostat.h"
#include "./Display/KnxChannelDisplay.h"
#include "./Sensor/KnxChannelSensor.h"
#include "./Fan/KnxChannelFan.h"
#include "./DoorWindow/KnxChannelDoorWindow.h"

SwitchBridge* BridgeBase::createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
    return nullptr;
}

DimmerBridge* BridgeBase::createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
    return nullptr;
}

RGBBridge* BridgeBase::createRGB(KnxChannelRGB& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
    return nullptr;
}

RolladenBridge* BridgeBase::createJalousien(KnxChannelJalousie& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
    return nullptr;
}

RolladenBridge* BridgeBase::createRolladen(KnxChannelRolladen& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
    return nullptr;
}

ThermostatBridge* BridgeBase::createThermostat(KnxChannelThermostat& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
    return nullptr;
}

DisplayBridge* BridgeBase::createDisplay(KnxChannelDisplay& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
    return nullptr;
}

SensorBridge* BridgeBase::createSensor(KnxChannelSensor& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
    return nullptr;
}

FanBridge* BridgeBase::createFan(KnxChannelFan& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
    return nullptr;
} 

DoorWindowBridge* BridgeBase::createDoorWindow(KnxChannelDoorWindow& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
    return nullptr;
}