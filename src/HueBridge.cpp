
#ifndef SMARTHOMEBRIDGE_DEVICESONLY  
#include "HueBridge.h"
#include "Switch/KnxChannelSwitch.h"
#include "Switch/HueSwitch.h"
#include "Dimmer/KnxChannelDimmer.h"
#include "Dimmer/HueDimmer.h"
#include "RGB/KnxChannelRGB.h"
#include "RGB/HueRGB.h"
#include "Rolladen/KnxChannelRolladen.h"
#include "Jalousie/KnxChannelJalousie.h"
#include "Jalousie/HueJalousie.h"
#include "Rolladen/HueRolladen.h"
#include "Fan/KnxChannelFan.h"
#include "Fan/HueFan.h"
#include "DoorWindow/KnxChannelDoorWindow.h"
#include "DoorWindow/HueDoorWindow.h"
#include "Scene/KnxChannelScene.h"
#include "Scene/HueScene.h"
#include "Lock/KnxChannelLock.h"
#include "Lock/HueLock.h"

void HueBridge::initialize(SmartHomeBridgeModule *bridge)
{

}

bool HueBridge::processCommand(const std::string cmd, bool diagnoseKo)
{
    return false;
}

void HueBridge::showHelp()
{
    
}

SwitchBridge* HueBridge::createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (!ParamBRI_CHSwitchHueEmulation)
        return nullptr;
    auto bridge = new HueSwitch(this);
    channel.add(bridge);
    return bridge;
}

DimmerBridge* HueBridge::createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (!ParamBRI_CHSwitchHueEmulation)
        return nullptr;
    auto bridge = new HueDimmer(this);
    channel.add(bridge);
    return bridge;
}

RGBBridge* HueBridge::createRGB(KnxChannelRGB& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (!ParamBRI_CHSwitchHueEmulation)
        return nullptr;
    auto bridge = new HueRGB(this);
    channel.add(bridge);
    return bridge;
}

RolladenBridge* HueBridge::createJalousien(KnxChannelJalousie& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (!ParamBRI_CHJalousieHueEmulation)
        return nullptr;
    auto bridge = new HueJalousie(this);
    channel.add(bridge);
    return bridge;
}

RolladenBridge* HueBridge::createRolladen(KnxChannelRolladen& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (!ParamBRI_CHJalousieHueEmulation)
        return nullptr;
    auto bridge = new HueRolladen(this);
    channel.add(bridge);
    return bridge;
}

FanBridge* HueBridge::createFan(KnxChannelFan& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (!ParamBRI_CHFanHueEmulation)
        return nullptr;
    auto bridge = new HueFan(this);
    channel.add(bridge);
    return bridge;
}

DoorWindowBridge* HueBridge::createDoorWindow(KnxChannelDoorWindow& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (!ParamBRI_CHDoorHueEmulation)
        return nullptr;
    auto bridge = new HueDoorWindow(this);
    channel.add(bridge);
    return bridge;
}

SceneBridge* HueBridge::createScene(KnxChannelScene& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (!ParamBRI_CHSceneHueEmulation)
        return nullptr;
    auto bridge = new HueScene(this);
    channel.add(bridge);
    return bridge;
}

LockBridge* HueBridge::createLock(KnxChannelLock& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (!ParamBRI_CHLockHueEmulation)
        return nullptr;
    auto bridge = new HueLock(this);
    channel.add(bridge);
    return bridge;
}

const std::string HueBridge::name()
{
    return "HueBridge";
}

void HueBridge::start(SmartHomeBridgeModule *bridge)
{
    uint8_t mac[6];
    openknxNetwork.macAddress(mac);
#ifdef OPENKNX_WEBSERVER
    auto webServer = bridge->getWebServer();
    if (webServer != nullptr)
    {
        espalexa.begin(webServer, bridge->getWebServerPort(), mac);
    }
    else
    {
        espalexa.beginWithNetworkWebserver(80, mac);
    }   

#else
    espalexa.begin(nullptr, 80, mac);
#endif
}

void HueBridge::loop()
{
    espalexa.loop();
}

#ifdef OPENKNX_DUALCORE
void HueBridge::loop1()
{
}
#endif

void HueBridge::processInputKo(GroupObject& groupObject)
{
    
}



void HueBridge::registerWebPages()
{
#ifdef OPENKNX_WEBSERVER
    openknxNetwork.webserver.addMenuItem("Hue", "/hue", 51);
    openknxNetwork.webserver.addRoute(OpenKNX::Network::WEB_GET, "/hue", [this](OpenKNX::Network::WebRequest&, OpenKNX::Network::WebResponse& res) {
        std::string html;
        getInformation(html);
        res.setLayout(true);
        res.setActiveMenu("/hue");
        res.send(html.c_str());
    });
#endif
}

void HueBridge::getInformation(std::string& result)
{
    result += "<h3 style='margin-bottom:0.75em;'>Hue Emulation</h3>";
    result += "<p style='margin-bottom:0.75em;'>Anzahl der Ger&auml;te: ";
    result += std::to_string(espalexa.getNumberOfDevices());
    result += "</p><p><a href=\"/espalexa\">Technische Information für Entwickler (in Englisch)</a></p>";
}
#endif
