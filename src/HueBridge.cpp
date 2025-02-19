#include "HueBridge.h"
#include "Switch/KnxChannelSwitch.h"
#include "Switch/HueSwitch.h"
#include "Dimmer/KnxChannelDimmer.h"
#include "Dimmer/HueDimmer.h"
#include "RGB/KnxChannelRGB.h"
#include "RGB/HueRGB.h"
#include "Rolladen/KnxChannelRolladen.h"
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

SwitchBridge* HueBridge::createSwitch(uint8_t _channelIndex, uint8_t deviceType)
{
    return ParamBRI_CHSwitchHueEmulation ? new HueSwitch(this) : nullptr;
}

DimmerBridge* HueBridge::createDimmer(uint8_t _channelIndex, uint8_t deviceType)
{
    return ParamBRI_CHSwitchHueEmulation ? new HueDimmer(this) : nullptr;
}

RGBBridge* HueBridge::createRGB(uint8_t _channelIndex, uint8_t deviceType)
{
    return ParamBRI_CHSwitchHueEmulation ? new HueRGB(this) : nullptr;
}

RolladenBridge* HueBridge::createJalousien(uint8_t _channelIndex, uint8_t deviceType)
{
    return ParamBRI_CHJalousieHueEmulation ? new HueRolladen(this) : nullptr;
}

RolladenBridge* HueBridge::createRolladen(uint8_t _channelIndex, uint8_t deviceType)
{
    return ParamBRI_CHJalousieHueEmulation ? new HueRolladen(this) : nullptr;
}

FanBridge* HueBridge::createFan(uint8_t _channelIndex, uint8_t deviceType)
{
    return ParamBRI_CHFanHueEmulation ? new HueFan(this) : nullptr;
}

DoorWindowBridge* HueBridge::createDoorWindow(uint8_t _channelIndex, uint8_t deviceType)
{
    return ParamBRI_CHDoorHueEmulation ? new HueDoorWindow(this) : nullptr;
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