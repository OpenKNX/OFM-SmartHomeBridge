#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HomeKitBridge.h"
#include "SmartHomeBridgeModule.h"
#include "NetworkModule.h"
#include "Switch/KnxChannelSwitch.h"
#include "Switch/HomeKitSwitch.h"
#include "Dimmer/KnxChannelDimmer.h"
#include "Dimmer/HomeKitDimmer.h"
#include "RGB/KnxChannelRGB.h"
#include "RGB/HomeKitRGB.h"
#include "Jalousie/KnxChannelJalousie.h"
#include "Jalousie/HomeKitJalousie.h"
#include "Rolladen/KnxChannelRolladen.h"
#include "Rolladen/HomeKitRolladen.h"
#include "Thermostat/KnxChannelThermostat.h"
#include "Thermostat/HomeKitThermostat.h"
#include "Display/KnxChannelDisplay.h"
#include "Display/HomeKitDisplay.h"
#include "Alarm/KnxChannelAlarm.h"
#include "Alarm/HomeKitAlarm.h"
#include "Fan/KnxChannelFan.h"
#include "Fan/HomeKitFan.h"
#include "DoorWindow/KnxChannelDoorWindow.h"
#include "DoorWindow/HomeKitDoorWindow.h"
#include "Scene/KnxChannelScene.h"
#include "Scene/HomeKitScene.h"
#include "Lock/KnxChannelLock.h"
#include "Lock/HomeKitLock.h"

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
    openknx.console.printHelpLine("hk", "Switch to HomeKit console");
}

SwitchBridge* HomeKitBridge::createSwitch(KnxChannelSwitch& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitSwitch(homekitAID);
    channel.add(bridge);
    return bridge;
}

DimmerBridge* HomeKitBridge::createDimmer(KnxChannelDimmer& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitDimmer(homekitAID);
    channel.add(bridge);
    return bridge;
}

RGBBridge* HomeKitBridge::createRGB(KnxChannelRGB& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitRGB(homekitAID);
    channel.add(bridge);
    return bridge;
}

RolladenBridge* HomeKitBridge::createJalousien(KnxChannelJalousie& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitJalousie(homekitAID);
    channel.add(bridge);
    return bridge;
}

RolladenBridge* HomeKitBridge::createRolladen(KnxChannelRolladen& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitRolladen(homekitAID);
    channel.add(bridge);
    return bridge;
}

ThermostatBridge* HomeKitBridge::createThermostat(KnxChannelThermostat& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitThermostat(homekitAID);
    channel.add(bridge);
    return bridge;
}

DeviceBridge* HomeKitBridge::createDisplay(KnxChannelDisplay& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitDisplay(homekitAID);
    channel.add(bridge);
    return bridge;
}

AlarmBridge* HomeKitBridge::createSensor(KnxChannelAlarm& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitAlarm(homekitAID);
    channel.add(bridge);
    return bridge;
}

FanBridge* HomeKitBridge::createFan(KnxChannelFan& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitFan(homekitAID);
    channel.add(bridge);
    return bridge;
}

SceneBridge* HomeKitBridge::createScene(KnxChannelScene& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitScene(homekitAID);
    channel.add(bridge);
    return bridge;
}

LockBridge* HomeKitBridge::createLock(KnxChannelLock& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitLock(homekitAID);
    channel.add(bridge);
    return bridge;
}

DoorWindowBridge* HomeKitBridge::createDoorWindow(KnxChannelDoorWindow& channel,  uint8_t _channelIndex, uint8_t deviceType)
{
    int homekitAID = _channelIndex + 2; // Homekit bridge has AID1
    auto bridge = new HomeKitDoorWindow(homekitAID);
    channel.add(bridge);
    return bridge;
}
void HomeKitBridge::registerWebPages()
{
#ifdef OPENKNX_WEBSERVER
    openknxNetwork.webserver.addMenuItem("HomeKit", "/homekit", 50);
    openknxNetwork.webserver.addRoute(OpenKNX::Network::WEB_GET, "/homekit", [this](OpenKNX::Network::WebRequest&, OpenKNX::Network::WebResponse& res) {
        std::string html = "<h3>HomeKit</h3>";
        auto handle = homeSpan.getAutoPollTask();
        if (handle != nullptr)
        {
            auto minFreeStack = uxTaskGetStackHighWaterMark(handle);
            if (minFreeStack != 0)
            {
                html += "<p>Maximale Stack Verwendung: ";
                html += std::to_string(HOMESPAN_STACK_SIZE - minFreeStack);
                html += " von ";
                html += std::to_string(HOMESPAN_STACK_SIZE);
                html += "</p>";
                html += "<form method='post' action='/resetPairing'>";
                html += "<input name='resetPairing' type='hidden' value='1'>";
                html += "<input type='submit' value='Alle HomeKit Kopplungen L\xC3\xB6schen'>";
                html += "</form>";
            }
        }
        res.setLayout(true);
        res.setActiveMenu("/homekit");
        res.send(html.c_str());
    });
    openknxNetwork.webserver.addRoute(OpenKNX::Network::WEB_POST, "/resetPairing", [this](OpenKNX::Network::WebRequest&, OpenKNX::Network::WebResponse& res) {
        homeSpan.processSerialCommand("F");
        res.setStatus(303);
        res.setHeader("Location", "/homekit");
        res.send("");
    });
#endif
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


#endif
