#include "HomeKitBridge.h"
#include "SmartHomeBridgeModule.h"
#include "NetworkModule.h"
#include "Switch/KnxChannelSwitch.h"
#include "Switch/HomeKitSwitch.h"
#include "Dimmer/KnxChannelDimmer.h"
#include "Dimmer/HomeKitDimmer.h"
#include "RGB/KnxChannelRGB.h"
#include "RGB/HomeKitRGB.h"
#include "Rolladen/KnxChannelRolladen.h"
#include "Rolladen/HomeKitRolladen.h"
#include "Thermostat/KnxChannelThermostat.h"
#include "Thermostat/HomeKitThermostat.h"
#include "Display/KnxChannelDisplay.h"
#include "Display/HomeKitDisplay.h"
#include "Sensor/KnxChannelSensor.h"
#include "Sensor/HomeKitSensor.h"
#include "Fan/KnxChannelFan.h"
#include "Fan/HomeKitFan.h"
#include "DoorWindow/KnxChannelDoorWindow.h"
#include "DoorWindow/HomeKitDoorWindow.h"

#ifndef HOMESPAN_STACK_SIZE
#define HOMESPAN_STACK_SIZE 8192
#endif

void HomeKitBridge::initialize(SmartHomeBridgeModule *bridge)
{
    _bridge = bridge;
    homeSpan.useEthernet();
    homeSpan.setSerialInputDisable(true);
    homeSpan.setPairingCode((const char *)ParamBRI_PairingCode);
    homeSpan.setPortNum(8080);
    homeSpan.begin(Category::Bridges, bridge->getNameInUTF8());
    new SpanAccessory();
    new Service::AccessoryInformation();
    new Characteristic::Identify();
}

bool HomeKitBridge::processCommand(const std::string cmd, bool diagnoseKo)
{
    if (cmd == "hk")
    {
        openknx.console.disableConsole(true);
        homeSpan.setSerialInputDisable(false);
        return true;
    }
    return false;
}

void HomeKitBridge::showHelp()
{
    openknx.console.printHelpLine("HK", "Switch to HomeKit console");
}

SwitchBridge* HomeKitBridge::createSwitch(uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
    return new HomeKitSwitch(homekitAID);
}

DimmerBridge* HomeKitBridge::createDimmer(uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
    return new HomeKitDimmer(homekitAID);
}

RGBBridge* HomeKitBridge::createRGB(uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
    return new HomeKitRGB(homekitAID);
}

RolladenBridge* HomeKitBridge::createJalousien(uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
    return new HomeKitRolladen(homekitAID);
}

RolladenBridge* HomeKitBridge::createRolladen(uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
    return new HomeKitRolladen(homekitAID);
}

ThermostatBridge* HomeKitBridge::createThermostat(uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
    return new HomeKitThermostat(homekitAID);
}

DisplayBridge* HomeKitBridge::createDisplay(uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
    return new HomeKitDisplay(homekitAID);
}

SensorBridge* HomeKitBridge::createSensor(uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
    return new HomeKitSensor(homekitAID);
}

FanBridge* HomeKitBridge::createFan(uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
    return new HomeKitFan(homekitAID);
}

DoorWindowBridge* HomeKitBridge::createDoorWindow(uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
    return new HomeKitDoorWindow(homekitAID);
}


void HomeKitBridge::initWebServer(WebServer &webServer)
{
    webServer.on("/resetPairing", HTTP_POST, [this]()
                 { this->serveResetPairingPage(); });
}

void HomeKitBridge::serveResetPairingPage()
{
    auto webServer = _bridge->getWebServer();

    String res = "<!DOCTYPE html><html lang=\"en\"><meta charset=\"UTF-8\"><meta http-equiv=\"refresh\" content=\"3;url=/\"><title>";
    res + "HomeKit Pairing Reset";
    res += "</title><body>";
    res += "<br>HomeKit Pairing reseted</br>";
    res += "</body>";
    webServer->send(200, "text/html;charset=UTF-8", res);
    homeSpan.processSerialCommand("F");
}

const std::string HomeKitBridge::name()
{
    return "HomeKitBridge";
}

void HomeKitBridge::start(SmartHomeBridgeModule *bridge)
{
    BridgeBase::start(bridge);
  

    homeSpan.autoPoll(HOMESPAN_STACK_SIZE);
}
void HomeKitBridge::loop()
{
}

void HomeKitBridge::processInputKo(GroupObject &ko)
{
}

void HomeKitBridge::getInformation(String &result)
{
    result += "<h3>HomeKit</h3>";
    auto handle = homeSpan.getAutoPollTask();
    if (handle != nullptr)
    {
        auto minFreeStack = uxTaskGetStackHighWaterMark(handle);
        if (minFreeStack != 0)
        {
            result += "Maximale Stack Verwendung: ";
            result += HOMESPAN_STACK_SIZE - minFreeStack;
            result += " von " + (String)HOMESPAN_STACK_SIZE;
            // HomeKit Factory Reset
            result += "<form method='post' action='/resetPairing'><input name='resetPairing' type='hidden' value='1'><input type='submit' value='Alle HomeKit Kopplungen Löschen'></form>";
        }
    }
}
