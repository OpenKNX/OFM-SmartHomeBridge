#pragma once
#include "OpenKNX.h"
#include "ChannelOwnerModule.h"
#include "Component.h"
#ifndef SMARTHOMEBRIDGE_DEVICESONLY
    #include <WebServer.h>
    #if !defined(ARDUINO_ARCH_ESP32)
    #error Your archetecture is not supported by the smart home brigde. Currently only ESP32 is supported.
    #endif
#endif

class SmartHomeBridgeModule;
class HueBridge;
class SwitchBridge;
class DimmerBridge;
class RGBBridge;
class JalousienBridge;
class RolladenBridge;
class ThermostatBridge;
class DisplayBridge;
class SensorBridge;
class FanBridge;
class DoorWindowBridge;

class BridgeBase : public OpenKNX::Base
{
public:
    virtual SwitchBridge* createSwitch(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual DimmerBridge* createDimmer(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual RGBBridge* createRGB(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual RolladenBridge* createJalousien(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual RolladenBridge* createRolladen(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual ThermostatBridge* createThermostat(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual DisplayBridge* createDisplay(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual SensorBridge* createSensor(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual FanBridge* createFan(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual DoorWindowBridge* createDoorWindow(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);     

    virtual void initialize(SmartHomeBridgeModule* bridge) = 0;
#ifndef SMARTHOMEBRIDGE_DEVICESONLY  
    virtual void initWebServer(WebServer& webServer) {};
#endif
    virtual void start(SmartHomeBridgeModule* bridge) {};
    virtual void loop() = 0;
    virtual void processInputKo(GroupObject& ko) = 0;
    virtual void getInformation(String& result) = 0;
    virtual bool processCommand(const std::string cmd, bool diagnoseKo) = 0;
    virtual void showHelp() = 0;
};

class SmartHomeBridgeModule : public ChannelOwnerModule
{
    private:
#ifndef SMARTHOMEBRIDGE_DEVICESONLY
        const uint16_t webServerPort = 80;
        WebServer* webServer = nullptr;
#endif
        const char* _utf8Name = nullptr;
        DynamicPointerArray<BridgeBase>* bridgeInterfaces = nullptr;
        volatile bool started = false;
        void startBridge();
    protected:
        virtual const std::string name() override;
        virtual const std::string version() override;
        virtual void showInformations() override;
 
        virtual void setup() override;
        virtual void loop() override;
#ifdef OPENKNX_DUALCORE
        virtual void loop1() override;
#endif
        virtual void processInputKo(GroupObject &ko) override;
        virtual OpenKNX::Channel* createChannel(uint8_t _channelIndex /* this parameter is used in macros, do not rename */); 
    public:
        SmartHomeBridgeModule();
        ~SmartHomeBridgeModule();
        void addBridge(BridgeBase* bridge);
        const char* getNameInUTF8();
        bool processCommand(const std::string cmd, bool diagnoseKo) override;
        void showHelp() override;
#ifndef SMARTHOMEBRIDGE_DEVICESONLY
        WebServer* getWebServer();
        uint16_t getWebServerPort();
        void serveHomePage();
        void serveProgModePage();
        void serveFirmwareUpdatePage();
        void serveRebootPage();
#endif
};

extern SmartHomeBridgeModule openknxSmartHomeBridgeModule;