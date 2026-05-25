
#pragma once
#include "OpenKNX.h"

#ifndef SMARTHOMEBRIDGE_DEVICESONLY
class WebServer;
#endif
class SmartHomeBridgeModule;
class HueBridge;
class SwitchBridge;
class DimmerBridge;
class SceneBridge;
class RGBBridge;
class RolladenBridge;
class ThermostatBridge;
class DeviceBridge;
class AlarmBridge;
class FanBridge;
class DoorWindowBridge;
class MediaBridge;
class LockBridge;

class KnxChannelSwitch;
class KnxChannelDimmer;
class KnxChannelRGB;
class KnxChannelScene;
class KnxChannelRolladen;
class KnxChannelJalousie;
class KnxChannelThermostat;
class KnxChannelDisplay;
class KnxChannelAlarm;
class KnxChannelFan;
class KnxChannelDoorWindow;
class KnxChannelMedia;
class KnxChannelLock;

class BridgeBase : public OpenKNX::Base
{
public:
    virtual SwitchBridge* createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual DimmerBridge* createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual RGBBridge* createRGB(KnxChannelRGB& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual SceneBridge* createScene(KnxChannelScene& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual RolladenBridge* createJalousien(KnxChannelJalousie& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual RolladenBridge* createRolladen(KnxChannelRolladen& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual ThermostatBridge* createThermostat(KnxChannelThermostat& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual DeviceBridge* createDisplay(KnxChannelDisplay& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual AlarmBridge*createSensor(KnxChannelAlarm& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual FanBridge* createFan(KnxChannelFan& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual DoorWindowBridge* createDoorWindow(KnxChannelDoorWindow& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);     
    virtual MediaBridge* createMedia(KnxChannelMedia& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual LockBridge* createLock(KnxChannelLock& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);

    virtual void initialize(SmartHomeBridgeModule* bridge) {};
#ifndef SMARTHOMEBRIDGE_DEVICESONLY  
    virtual void registerWebPages() {};
#endif
    virtual void start(SmartHomeBridgeModule* bridge) {};
    virtual void loop() {};
    virtual void processInputKo(GroupObject& ko) {};
    virtual bool processCommand(const std::string cmd, bool diagnoseKo) { return false;};
    virtual void showHelp() {};
};
