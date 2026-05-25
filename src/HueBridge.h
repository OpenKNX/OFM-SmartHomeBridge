#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include <Espalexa.h>
#include "SmartHomeBridgeModule.h"

class HueBridge : public BridgeBase
{
public:
    Espalexa espalexa;
public:
    virtual SwitchBridge* createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual DimmerBridge* createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual RGBBridge* createRGB(KnxChannelRGB& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual RolladenBridge* createJalousien(KnxChannelJalousie& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual RolladenBridge* createRolladen(KnxChannelRolladen& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual FanBridge* createFan(KnxChannelFan& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual DoorWindowBridge* createDoorWindow(KnxChannelDoorWindow& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;     
    virtual SceneBridge* createScene(KnxChannelScene& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual LockBridge* createLock(KnxChannelLock& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;

    virtual const std::string name() override;
    virtual void initialize(SmartHomeBridgeModule *bridge) override;
    virtual void start(SmartHomeBridgeModule *bridge) override;
    virtual void loop() override;
    virtual void registerWebPages() override;
#ifdef OPENKNX_DUALCORE
    virtual void loop1() override;
#endif
    virtual void processInputKo(GroupObject& ko) override;
    virtual bool processCommand(const std::string cmd, bool diagnoseKo) override;
    virtual void showHelp() override;

};

#endif