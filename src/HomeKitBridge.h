#pragma once
#include "HomeSpan.h"
#include "SmartHomeBridgeModule.h"

class HomeKitBridge : public BridgeBase
{
    SmartHomeBridgeModule* _bridge;
    void serveResetPairingPage();
public:
    virtual SwitchBridge* createSwitch(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual DimmerBridge* createDimmer(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual RGBBridge* createRGB(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual RolladenBridge* createJalousien(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual RolladenBridge* createRolladen(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual ThermostatBridge* createThermostat(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual DisplayBridge* createDisplay(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual SensorBridge* createSensor(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual FanBridge* createFan(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual DoorWindowBridge* createDoorWindow(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;     

    virtual const std::string name() override;
    virtual void initialize(SmartHomeBridgeModule *bridge) override;
    virtual void initWebServer(WebServer& webServer) override;
    virtual void start(SmartHomeBridgeModule *bridge) override;
    virtual void loop() override;
    virtual void processInputKo(GroupObject& ko) override;
    virtual void getInformation(String& result) override;
    virtual bool processCommand(const std::string cmd, bool diagnoseKo) override;
    virtual void showHelp() override;
};