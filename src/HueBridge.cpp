
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

void HueBridge::createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (ParamBRI_CHSwitchHueEmulation)
         channel.add(new HueSwitch(this));
}

void HueBridge::createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (ParamBRI_CHSwitchHueEmulation)
        channel.add(new HueDimmer(this));
}

void HueBridge::createRGB(KnxChannelRGB& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (ParamBRI_CHSwitchHueEmulation)
        channel.add(new HueRGB(this));
}

void HueBridge::createJalousien(KnxChannelJalousie& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (ParamBRI_CHJalousieHueEmulation)
        channel.add(new HueRolladen(this));
}

void HueBridge::createRolladen(KnxChannelRolladen& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (ParamBRI_CHJalousieHueEmulation)
        channel.add(new HueRolladen(this));
}

void HueBridge::createFan(KnxChannelFan& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (ParamBRI_CHFanHueEmulation)
        channel.add(new HueFan(this));
}

void HueBridge::createDoorWindow(KnxChannelDoorWindow& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    if (ParamBRI_CHDoorHueEmulation)
        channel.add(new HueDoorWindow(this));
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