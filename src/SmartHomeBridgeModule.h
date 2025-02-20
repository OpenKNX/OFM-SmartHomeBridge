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

class KnxChannelSwitch;
class KnxChannelDimmer;
class KnxChannelRGB;
class KnxChannelRolladen;
class KnxChannelJalousie;
class KnxChannelThermostat;
class KnxChannelDisplay;
class KnxChannelSensor;
class KnxChannelFan;
class KnxChannelDoorWindow;

class BridgeBase : public OpenKNX::Base
{
public:
    virtual void createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual void createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual void createRGB(KnxChannelRGB& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual void createJalousien(KnxChannelJalousie& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual void createRolladen(KnxChannelRolladen& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual void createThermostat(KnxChannelThermostat& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual void createDisplay(KnxChannelDisplay& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual void createSensor(KnxChannelSensor& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual void createFan(KnxChannelFan& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);
    virtual void createDoorWindow(KnxChannelDoorWindow& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType);     

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