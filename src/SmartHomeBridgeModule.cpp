#include <Arduino.h>
#include <knx.h>
#include "hardware.h"
#ifndef SMARTHOMEBRIDGE_DEVICESONLY
#include <WiFi.h>
#include <NetworkModule.h>
#include "HomeKitBridge.h"
#include "HueBridge.h"
#ifdef OPENKNX_WEBSERVER
#include "WebVisuBridge.h"
#endif
#endif
#include "SmartHomeBridgeModule.h"
#include "./Switch/KnxChannelSwitch.h"
#include "./Dimmer/KnxChannelDimmer.h"
#include "./RGB/KnxChannelRGB.h"
#include "./Scene/KnxChannelScene.h"
#include "./Rolladen/KnxChannelRolladen.h"
#include "./Jalousie/KnxChannelJalousie.h"
#include "./Thermostat/KnxChannelThermostat.h"
#include "./Display/KnxChannelDisplay.h"
#include "./Alarm/KnxChannelAlarm.h"
#include "./Fan/KnxChannelFan.h"
#include "./DoorWindow/KnxChannelDoorWindow.h"
#include "./Media/KnxChannelMedia.h"
#include "./Lock/KnxChannelLock.h"

#include "knxprod.h"
#include "ISO8859_15ToUTF8.h"

SmartHomeBridgeModule::SmartHomeBridgeModule()
{
}

const std::string SmartHomeBridgeModule::name()
{
  return "SmartHomeBridgeModule";
}

void SmartHomeBridgeModule::showInformations()
{
}

const std::string SmartHomeBridgeModule::version()
{
  return MODULE_SmartHomeBridge_Version;
}

SmartHomeBridgeModule::~SmartHomeBridgeModule()
{
  if (_utf8Name != nullptr)
  {
    delete _utf8Name;
    _utf8Name = nullptr;
  }
}

const char *SmartHomeBridgeModule::getNameInUTF8()
{
  return _utf8Name;
}

void SmartHomeBridgeModule::setup(bool configured)
{
  if (bridgeInterfaces == nullptr)
    bridgeInterfaces = new DynamicPointerArray<BridgeBase>();

  ChannelOwnerModule::setup(configured);
}

void SmartHomeBridgeModule::setup()
{
  setNumberOfChannels(ParamBRI_VisibleChannels);
#ifdef SMARTHOMEBRIDGE_DEVICESONLY
  logDebugP("Setup Devices");
#else
  logDebugP("Setup Bridge");
#endif
  _utf8Name = convertISO8859_15ToUTF8((const char *)ParamBRI_BridgeName);

#ifndef SMARTHOMEBRIDGE_DEVICESONLY
  bool homeKitEnabled = ParamBRI_HomeKitEnabled;
  if (homeKitEnabled)
  {
    logDebugP("Homekit enabled");
    addBridge(new HomeKitBridge());
  }

  bool hueEnabled = ParamBRI_HueEnabled;
  if (hueEnabled)
  {
    logDebugP("Hue enabled");
    addBridge(new HueBridge());
  }

#ifdef OPENKNX_WEBSERVER
  logDebugP("WebVisu enabled");
  addBridge(new WebVisuBridge());
#endif
#else
  startBridge();
#endif

  // Do not call base class here, because this creates the channels
}

void SmartHomeBridgeModule::addBridge(BridgeBase *bridge)
{
  if (bridgeInterfaces == nullptr)
    bridgeInterfaces = new DynamicPointerArray<BridgeBase>();
  bridgeInterfaces->push_back(bridge);
}

OpenKNX::Channel *SmartHomeBridgeModule::createChannel(uint8_t _channelIndex /* this parameter is used in macros, do not rename */)
{

  uint8_t deviceType = ParamBRI_CHDeviceType;
  if (ParamBRI_CHDisableChannel && deviceType != 0)
  {
    logInfoP("Device: %d - Disabled", _channelIndex + 1);
    return nullptr;
  }
  KnxChannelBase *channel = nullptr;
  switch (deviceType)
  {
  case 0:
  {
    logInfoP("Device: %d - Inactive", _channelIndex + 1);
    return nullptr;
  }
  case 10:
  case 11:
    channel = new KnxChannelSwitch(_channelIndex);
    break;
  case 12:
    channel = new KnxChannelLock(_channelIndex);
    break;
  case 20:
    switch (ParamBRI_CHLightType)
    {
    case 0:
      channel = new KnxChannelSwitch(_channelIndex);
      break;
    case 1:
      channel = new KnxChannelDimmer(_channelIndex);
      break;
    case 2:
      channel = new KnxChannelRGB(_channelIndex);
      break;
    default:
      logInfoP("Device: %d - Unkown type subdevice %d for %d", _channelIndex + 1, ParamBRI_CHLightType, deviceType);
      return nullptr;
    }
    break;
  case 30:
    channel = new KnxChannelJalousie(_channelIndex);
    break;
  case 31:
  case 32:
    channel = new KnxChannelRolladen(_channelIndex);
    break;
  case 40:
    channel = new KnxChannelScene(_channelIndex);
    break;
  case 41:
    channel = new KnxChannelMedia(_channelIndex);
    break;
  case 50:
    channel = new KnxChannelThermostat(_channelIndex);
    break;
  case 60:
  case 61:
  case 62:
    channel = new KnxChannelDisplay(_channelIndex);
    break;
  case 70:
    channel = new KnxChannelAlarm(_channelIndex);
    break;
  case 80:
    channel = new KnxChannelFan(_channelIndex);
    break;
  case 90:
  case 91:
  case 92:
    channel = new KnxChannelDoorWindow(_channelIndex);
    break;
  default:
    logInfoP("Device: %d - Unkown device type %d", _channelIndex + 1, deviceType);
    return nullptr;
  }
  logInfoP("Device: %d - %s", _channelIndex + 1, channel->name().c_str());
  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
  {
    channel->createBridgeDevice(**it);
  }
  return channel;  
  
}

KnxChannelBase* SmartHomeBridgeModule::getChannel(uint8_t channelIndex)
{
  if (_pChannels == nullptr)
    return nullptr;
  if (channelIndex >= getNumberOfChannels())
    return nullptr;

  return (KnxChannelBase*)_pChannels[channelIndex];
}

bool SmartHomeBridgeModule::processCommand(const std::string cmd, bool diagnoseKo)
{
  if (bridgeInterfaces != nullptr)
  {
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      if ((*it)->processCommand(cmd, diagnoseKo))
        return true;
    }
  }
  return false;
}

void SmartHomeBridgeModule::showHelp()
{
  if (bridgeInterfaces != nullptr)
  {
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      (*it)->showHelp();
  }
}

void SmartHomeBridgeModule::startBridge()
{
  started = true;
  logDebugP("Initialize briges");
  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    (*it)->initialize(this);

  createChannels();

#ifndef SMARTHOMEBRIDGE_DEVICESONLY
  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    (*it)->registerWebPages();
#endif
  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    (*it)->start(this);
}

void SmartHomeBridgeModule::loop()
{
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

  bool connected = openknxNetwork.connected();
  if (connected && !started)
  {
    startBridge();
  }
#endif
  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    (*it)->loop();

  ChannelOwnerModule::loop();
}

#ifdef OPENKNX_DUALCORE
void SmartHomeBridgeModule::loop1()
{
  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    (*it)->loop1();

  ChannelOwnerModule::loop1();
}
#endif

void SmartHomeBridgeModule::processInputKo(GroupObject &ko)
{
  if (bridgeInterfaces != nullptr)
  {
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      (*it)->processInputKo(ko);
  }
  ChannelOwnerModule::processInputKo(ko);
}

SmartHomeBridgeModule openknxSmartHomeBridgeModule;