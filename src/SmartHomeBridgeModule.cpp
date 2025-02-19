#include <Arduino.h>
#include <knx.h>
#include "hardware.h"
#ifndef SMARTHOMEBRIDGE_DEVICESONLY
#include <WiFi.h>
#include <NetworkModule.h>
#include "HomeSpan.h"
#include "HomeKitBridge.h"
#include "HueBridge.h"
#endif
#include "SmartHomeBridgeModule.h"
#include "./Switch/KnxChannelSwitch.h"
#include "./Dimmer/KnxChannelDimmer.h"
#include "./RGB/KnxChannelRGB.h"
#include "./Rolladen/KnxChannelRolladen.h"
#include "./Jalousie/KnxChannelJalousie.h"
#include "./Thermostat/KnxChannelThermostat.h"
#include "./Display/KnxChannelDisplay.h"
#include "./Sensor/KnxChannelSensor.h"
#include "./Fan/KnxChannelFan.h"
#include "./DoorWindow/KnxChannelDoorWindow.h"



#include "knxprod.h"
#include "CP1252ToUTF8.h"


SwitchBridge* BridgeBase::createSwitch(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
  return nullptr;
}

DimmerBridge* BridgeBase::createDimmer(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
  return nullptr;
}

RGBBridge* BridgeBase::createRGB(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
  return nullptr;
}

RolladenBridge* BridgeBase::createJalousien(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
  return nullptr;
}

RolladenBridge* BridgeBase::createRolladen(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
  return nullptr;
}

ThermostatBridge* BridgeBase::createThermostat(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
  return nullptr;
}

DisplayBridge* BridgeBase::createDisplay(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
  return nullptr;
}

SensorBridge* BridgeBase::createSensor(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
  return nullptr;
}

FanBridge* BridgeBase::createFan(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
  return nullptr;
}

DoorWindowBridge* BridgeBase::createDoorWindow(uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
  return nullptr;
}


SmartHomeBridgeModule::SmartHomeBridgeModule()
{
}

const std::string SmartHomeBridgeModule::name()
{
  return "SmartHomeBridgeModule";
}

void SmartHomeBridgeModule::showInformations()
{
#ifdef MODULE_SmartHomeBridge_Version
    openknx.logger.logWithPrefixAndValues(logPrefix(), "Smart Home Bridge %s", MODULE_SmartHomeBridge_Version);
#else
    openknx.logger.logWithPrefix(logPrefix(), "Smart Home Bridge");
#endif  
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

void SmartHomeBridgeModule::setup()
{
  setNumberOfChannels(ParamBRI_VisibleChannels);
  logDebugP("Setup Bridge");
  _utf8Name = convert1252ToUTF8((const char *)ParamBRI_BridgeName);

#ifndef SMARTHOMEBRIDGE_DEVICESONLY
  webServer = new WebServer(webServerPort);
 
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
  switch (deviceType)
  {
  case 0:
  {
    logInfoP("Device: %d - Inactive", _channelIndex + 1);
    return nullptr;
  }
  case 10:
  case 11:
  {
    logInfoP("Device: %d - On/Off", _channelIndex + 1);
    auto switchBridges = new DynamicPointerArray<SwitchBridge>();
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      auto switchBridge = (*it)->createSwitch(_channelIndex, deviceType);
      if (switchBridge != nullptr)
          switchBridges->push_back(switchBridge);
    }
    return new KnxChannelSwitch(switchBridges, _channelIndex);
  }
  case 20:
  {
    switch (ParamBRI_CHLightType)
    {
    case 0:
    {
      logInfoP("Device: %d - On/Off Light", _channelIndex + 1);
      auto onOffBridges = new DynamicPointerArray<SwitchBridge>();
      for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      {
        auto onOffBridge = (*it)->createSwitch(_channelIndex, deviceType);
        if (onOffBridge != nullptr)
          onOffBridges->push_back(onOffBridge);
      }
      return new KnxChannelSwitch(onOffBridges, _channelIndex);
    }
    case 1:
    {
      logInfoP("Device: %d - Dimmer", _channelIndex + 1);
      auto dimmerBridges = new DynamicPointerArray<DimmerBridge>();
      for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      {
        auto dimmerBridge = (*it)->createDimmer(_channelIndex, deviceType);
        if (dimmerBridge != nullptr)
          dimmerBridges->push_back(dimmerBridge);
      }
      return new KnxChannelDimmer(dimmerBridges, _channelIndex);
    }
    case 2:
    {
      logInfoP("Device: %d - RGB", _channelIndex + 1);
      auto rdbBridges = new DynamicPointerArray<RGBBridge>();
      for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      {
        auto rdbBridge = (*it)->createRGB(_channelIndex, deviceType);
        if (rdbBridge != nullptr)
          rdbBridges->push_back(rdbBridge);
      }
      return new KnxChannelRGB(rdbBridges, _channelIndex);
    }
    }
    logInfoP("Device: %d - Unkown type subdevice %d for %d", _channelIndex + 1, ParamBRI_CHLightType, deviceType);
    return nullptr;
  }
  case 30:
  {
    logInfoP("Device: %d - Jalousien", _channelIndex + 1);
    auto jalousieBridges = new DynamicPointerArray<RolladenBridge>();
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      auto jalousieBridge = (*it)->createJalousien(_channelIndex, deviceType);
      if (jalousieBridge != nullptr)
        jalousieBridges->push_back(jalousieBridge);
    }
    return new KnxChannelJalousie(jalousieBridges, _channelIndex);
  }
  case 31:
  case 32:
  {
    logInfoP("Device: %d - Rolladen", _channelIndex + 1);
    auto rolladenBridges = new DynamicPointerArray<RolladenBridge>();
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      auto rolladenBridge = (*it)->createRolladen(_channelIndex, deviceType);
      if (rolladenBridge != nullptr)
        rolladenBridges->push_back(rolladenBridge);
    }
    return new KnxChannelRolladen(rolladenBridges, _channelIndex);
  }
  case 50:
  {
    logInfoP("Device: %d - Thermostat", _channelIndex + 1);
    auto thermostatBridges = new DynamicPointerArray<ThermostatBridge>();
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      auto thermostatBridge = (*it)->createThermostat(_channelIndex, deviceType);
      if (thermostatBridge != nullptr)
        thermostatBridges->push_back(thermostatBridge);
    }
    return new KnxChannelThermostat(thermostatBridges, _channelIndex);
  }
  case 60:
  case 61:
  case 62:
  {
    logInfoP("Device: %d - Display", _channelIndex + 1);
    auto displayBridges = new DynamicPointerArray<DisplayBridge>();
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      auto displayBridge = (*it)->createDisplay(_channelIndex, deviceType);
      if (displayBridge != nullptr)
        displayBridges->push_back(displayBridge);
    }
    return new KnxChannelDisplay(displayBridges, _channelIndex);
  }
  case 70:
  case 71:
  case 72:
  case 73:
  case 74:
  case 75:
  case 76:
  {
    logInfoP("Device: %d - Sensor", _channelIndex + 1);
    auto sensorBridges = new DynamicPointerArray<SensorBridge>();
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      auto sensorBridge = (*it)->createSensor(_channelIndex, deviceType);
      if (sensorBridge != nullptr)
        sensorBridges->push_back(sensorBridge);
    }
    return new KnxChannelSensor(sensorBridges, _channelIndex);
  }
  case 80:
  {
    logInfoP("Device: %d - Fan", _channelIndex + 1);
    auto fanBridges = new DynamicPointerArray<FanBridge>();
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      auto fanBridge = (*it)->createFan(_channelIndex, deviceType);
      if (fanBridge != nullptr)
        fanBridges->push_back(fanBridge);
    }
    return new KnxChannelFan(fanBridges, _channelIndex);
  }
  case 90:
  case 91:
  case 92:
  {
    logInfoP("Device: %d - DoorWindow", _channelIndex + 1);
    auto doorWindowBridges = new DynamicPointerArray<DoorWindowBridge>();
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      auto doorWindowBridge = (*it)->createDoorWindow(_channelIndex, deviceType);
      if (doorWindowBridge != nullptr)
        doorWindowBridges->push_back(doorWindowBridge);
    }
    return new KnxChannelDoorWindow(doorWindowBridges, _channelIndex);
  }
  default:
  {
    logInfoP("Device: %d - Unkown device type %d", _channelIndex + 1, deviceType);
    return nullptr;
  }
  }
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
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

  logDebugP("Start webserver");
  webServer = new WebServer(80);
  // serve pages
  webServer->on("/", HTTP_GET, [this]()
                { this->serveHomePage(); });
  webServer->on("/updateFW", HTTP_GET, [this]()
                { this->serveFirmwareUpdatePage(); });
  webServer->on("/progMode", HTTP_POST, [this]()
                { this->serveProgModePage(); });
  webServer->on("/reboot", HTTP_POST, [this]()
                { this->serveRebootPage(); });
  // handling uploading firmware file
  webServer->on(
      "/update", HTTP_POST, [this]()
      {
    webServer->sendHeader("Connection", "close");
    webServer->send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart(); },
      [this]()
      {
        HTTPUpload &upload = webServer->upload();
        if (upload.status == UPLOAD_FILE_START)
        {
          Serial.printf("Update: %s\n", upload.filename.c_str());
          if (!Update.begin(UPDATE_SIZE_UNKNOWN))
          { // start with max available size
            Update.printError(Serial);
          }
        }
        else if (upload.status == UPLOAD_FILE_WRITE)
        {
          /* flashing firmware to ESP*/
          if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
          {
            Update.printError(Serial);
          }
        }
        else if (upload.status == UPLOAD_FILE_END)
        {
          if (Update.end(true))
          { // true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
          }
          else
          {
            Update.printError(Serial);
          }
        }
      });
#endif
  logDebugP("Initialize briges");
  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    (*it)->initialize(this);

  createChannels();

#ifndef SMARTHOMEBRIDGE_DEVICESONLY  
  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    (*it)->initWebServer(*webServer);

  webServer->enableDelay(false);
#endif
  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    (*it)->start(this);

#ifndef SMARTHOMEBRIDGE_DEVICESONLY  
  webServer->begin();
#endif
}



void SmartHomeBridgeModule::loop()
{
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

  bool connected = openknxNetwork.connected();
  if (connected && !started)
  {
    startBridge();
  }
  if (webServer != nullptr)
    webServer->handleClient();
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

WebServer *SmartHomeBridgeModule::getWebServer()
{
  return webServer;
}

uint16_t SmartHomeBridgeModule::getWebServerPort()
{
  return webServerPort;
}

const char *firmwareUpdatePage =
    "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
    "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
    "<input type='file' name='update'>"
    "<input type='submit' value='Update'>"
    "</form>"
    "<div id='prg'>progress: 0%</div>"
    "<script>"
    "$('form').submit(function(e){"
    "e.preventDefault();"
    "var form = $('#upload_form')[0];"
    "var data = new FormData(form);"
    " $.ajax({"
    "url: '/update',"
    "type: 'POST',"
    "data: data,"
    "contentType: false,"
    "processData:false,"
    "xhr: function() {"
    "var xhr = new window.XMLHttpRequest();"
    "xhr.upload.addEventListener('progress', function(evt) {"
    "if (evt.lengthComputable) {"
    "var per = evt.loaded / evt.total;"
    "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
    "}"
    "}, false);"
    "return xhr;"
    "},"
    "success:function(d, s) {"
    "console.log('success!')"
    "},"
    "error: function (a, b, c) {"
    "}"
    "});"
    "});"
    "</script>";

void SmartHomeBridgeModule::serveFirmwareUpdatePage()
{
  webServer->send(200, "text/html;charset=UTF-8", firmwareUpdatePage);
}

void SmartHomeBridgeModule::serveRebootPage()
{
  String res = "<!DOCTYPE html><html lang=\"en\"><meta charset=\"UTF-8\"><meta http-equiv=\"refresh\" content=\"20;url=/\"><title>";
  res + "Smart Home Bridge Reboot";
  res += "</title><body>";
  res += "<br>Smart Home Bridge is rebooting...</br>";
  res += "</body>";
  webServer->send(200, "text/html;charset=UTF-8", res);
  vTaskDelay(1000);
  openknx.restart();
}

void SmartHomeBridgeModule::serveProgModePage()
{
  auto progMode = webServer->arg("progMode") == "1";
  if (progMode)
    knx.progMode(true);
  else
    knx.progMode(false);
  String res = "<!DOCTYPE html><html lang=\"en\"><meta charset=\"UTF-8\"><meta http-equiv=\"refresh\" content=\"3;url=/\"><title>";
  res + "Smart Home Bridge Prog Mode";
  res += "</title><body>";
  res += "<br>Prog mode ";
  res += progMode ? "activated" : "deactivated";
  res += "</br>";
  res += "</body>";
  webServer->send(200, "text/html;charset=UTF-8", res);
}

void SmartHomeBridgeModule::serveHomePage()
{
  auto name = String(getNameInUTF8());
  name.replace("<", "&lt;");
  name.replace(">", "&gt;");
  name.replace("&", "&amp;");
  name.replace("\"", "&quot;");

  String res = "<!DOCTYPE html><html lang=\"en\"><meta charset=\"UTF-8\"><meta http-equiv=\"refresh\" content=\"10;url=/\"><title>";
  res += name;
  res += "</title><body>";
  res += "<h1>OpenKNX SmartHome Bridge</h1>";
  res += "© Copyright OpenKNX, Michael Geramb, 2023-";
  res += (__DATE__ + sizeof(__DATE__) - 5);
  res += "<br><br>Name: ";
  res += name;
  res += "<br>IP Address: ";
  res += openknxNetwork.localIP().toString();
  res += "<br>ETS Gerätetype: 0x";
  char etsType[5];
  sprintf(etsType, "%02X%02X", MAIN_OpenKnxId, MAIN_ApplicationNumber);
  res += etsType;
  res += "<br>ETS App Version: ";
  res += MAIN_ApplicationVersion;
  res += "<br>KNX Address: ";
  res += openknx.info.humanIndividualAddress().c_str();
  res += "<br>KNX Version: ";
  res += KNX_Version;
  res += "<br>Version: ";
  res += MAIN_Version;
  res += "<br>Common Version: ";
  res += MODULE_Common_Version;
  res += "<br>Logikmodul Version: ";
  res += MODULE_LogicModule_Version;
  res += "<br>Arduino Version: ";
  res += ESP_ARDUINO_VERSION_MAJOR;
  res += ".";
  res += ESP_ARDUINO_VERSION_MINOR;
  res += ".";
  res += ESP_ARDUINO_VERSION_PATCH;
  res += "<br>Verwendete Kanäle: " + (String)getNumberOfUsedChannels();
  res += " von " + (String)BRI_ChannelCount;
  res += "<br>Freier Heap: " + (String)ESP.getFreeHeap() + " of " + (String)ESP.getHeapSize();
  res += "<br>Minimaler freier Heap: " + (String)ESP.getMinFreeHeap();
  res += "<br>Größter freie Heapblock: " + (String)ESP.getMaxAllocHeap();
  if (ESP.getFreePsram() > 0)
  {
    res += "<br>Freier PSRAM: " + (String)ESP.getFreePsram() + " of " + (String)ESP.getPsramSize();
    res += "<br>Minimaler freier PSRAM: " + (String)ESP.getMinFreePsram();
  }
  res += "<br>Maximale Stack Verwendung: " + (String)(8192 - uxTaskGetStackHighWaterMark(nullptr));
  res += " von 8192";
  res += "<br>Laufzeit: " + (String)millis();
  res += "<h2>Bridges:</h2>";
  for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
  {
    (*it)->getInformation(res);
    res += "<br>";
  }
  // prog button
  res += "<h2>Control</h2><form method='post' action='/progMode'><input name='progMode' type='hidden' value='";
  res += knx.progMode() ? "0" : "1";
  res += "'><input type='submit' value='";
  res += knx.progMode() ? "Stopp KNX Adressen Programmierungsmodus" : "Start KNX Adressen Programmierungsmodus";
  res += "'></form>";
  // reset button
  res += "<form method='post' action='/reboot'><input type='submit' value='Gerät neustarten'></form>";
  // firmware update button
  // res += "<form action='/updateFW'><button type='submit'>Update Firmware</button></form>";
  res += "</body>";
  webServer->send(200, "text/html;charset=UTF-8", res);
}

SmartHomeBridgeModule openknxSmartHomeBridgeModule;