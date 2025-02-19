#pragma once
#include "HomeSpan.h"
#include <Espalexa.h>
#include "SmartHomeBridgeModule.h"

class HueBridge : public BridgeBase
{
public:
    Espalexa espalexa;
public:
    virtual SwitchBridge* createSwitch(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual DimmerBridge* createDimmer(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual RGBBridge* createRGB(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual RolladenBridge* createJalousien(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual RolladenBridge* createRolladen(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual FanBridge* createFan(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual DoorWindowBridge* createDoorWindow(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);     

    virtual const std::string name() override;
    virtual void initialize(SmartHomeBridgeModule *bridge) override;
    virtual void start(SmartHomeBridgeModule *bridge) override;
    virtual void loop() override;
    virtual void getInformation(String& result) override;
#ifdef OPENKNX_DUALCORE
    virtual void loop1() override;
#endif
    virtual void processInputKo(GroupObject& ko) override;
    virtual bool processCommand(const std::string cmd, bool diagnoseKo) override;
    virtual void showHelp() override;

};