
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

const std::string HueBridge::name()
{
    return "HueBridge";
}

void HueBridge::start(SmartHomeBridgeModule *bridge)
{
    uint8_t mac[6];
    openknxNetwork.macAddress(mac);
    espalexa.begin(bridge->getWebServer(), bridge->getWebServerPort(), mac);
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


void HueBridge::getInformation(String& result) 
{
    result += "<h3>Hue</h3>";
    result += "Anzahl der Geräte: " + (String) espalexa.getNumberOfDevices();  
    result += "<br><a href=\"espalexa\">Information (in Englisch)</a>";
}
#endif