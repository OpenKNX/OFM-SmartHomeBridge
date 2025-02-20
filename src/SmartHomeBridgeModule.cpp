#include <Arduino.h>
#include <knx.h>
#include "hardware.h"
#ifndef SMARTHOMEBRIDGE_DEVICESONLY
#include <WiFi.h>
#include <NetworkModule.h>
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


void BridgeBase::createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
}

void BridgeBase::createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
}

void BridgeBase::createRGB(KnxChannelRGB& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
}

void BridgeBase::createJalousien(KnxChannelJalousie& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
}

void BridgeBase::createRolladen(KnxChannelRolladen& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
}

void BridgeBase::createThermostat(KnxChannelThermostat& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
}

void BridgeBase::createDisplay(KnxChannelDisplay& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
}

void BridgeBase::createSensor(KnxChannelSensor& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
}

void BridgeBase::createFan(KnxChannelFan& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
} 

void BridgeBase::createDoorWindow(KnxChannelDoorWindow& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType)
{
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
    auto channel = new KnxChannelSwitch(_channelIndex);
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      (*it)->createSwitch(*channel, _channelIndex, deviceType);
    }
    return channel;
  }
  case 20:
  {
    switch (ParamBRI_CHLightType)
    {
    case 0:
    {
      logInfoP("Device: %d - On/Off Light", _channelIndex + 1);
      auto channel = new KnxChannelSwitch(_channelIndex);
      for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      {
        (*it)->createSwitch(*channel, _channelIndex, deviceType);
      }
      return channel;
    }
    case 1:
    {
      logInfoP("Device: %d - Dimmer", _channelIndex + 1);
      auto channel = new KnxChannelDimmer(_channelIndex);
      for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      {
        (*it)->createDimmer(*channel, _channelIndex, deviceType);
      }
      return channel;
    }
    case 2:
    {
      logInfoP("Device: %d - RGB", _channelIndex + 1);
      auto channel = new KnxChannelRGB(_channelIndex);
      for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
      {
        (*it)->createRGB(*channel, _channelIndex, deviceType);
      }
      return channel;
    }
    }
    logInfoP("Device: %d - Unkown type subdevice %d for %d", _channelIndex + 1, ParamBRI_CHLightType, deviceType);
    return nullptr;
  }
  case 30:
  {
    logInfoP("Device: %d - Jalousien", _channelIndex + 1);
    auto channel = new KnxChannelJalousie(_channelIndex);
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      (*it)->createJalousien(*channel, _channelIndex, deviceType);
    }
    return channel;
  }
  case 31:
  case 32:
  {
    logInfoP("Device: %d - Rolladen", _channelIndex + 1);
    auto channel = new KnxChannelRolladen(_channelIndex);
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      (*it)->createRolladen(*channel, _channelIndex, deviceType);
    }
    return channel;
  }
  case 50:
  {
    logInfoP("Device: %d - Thermostat", _channelIndex + 1);
    auto channel = new KnxChannelThermostat(_channelIndex);
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      (*it)->createThermostat(*channel, _channelIndex, deviceType);
    }
    return channel;
  }
  case 60:
  case 61:
  case 62:
  {
    logInfoP("Device: %d - Display", _channelIndex + 1);
    auto channel = new KnxChannelDisplay(_channelIndex);
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      (*it)->createDisplay(*channel, _channelIndex, deviceType);
    }
    return channel;
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
    auto channel = new KnxChannelSensor(_channelIndex);
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      (*it)->createSensor(*channel, _channelIndex, deviceType);
    }
    return channel;
  }
  case 80:
  {
    logInfoP("Device: %d - Fan", _channelIndex + 1);
    auto channel = new KnxChannelFan(_channelIndex);
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      (*it)->createFan(*channel, _channelIndex, deviceType);
    }
    return channel;
  }
  case 90:
  case 91:
  case 92:
  {
    logInfoP("Device: %d - DoorWindow", _channelIndex + 1);
    auto channel = new KnxChannelDoorWindow(_channelIndex);
    for (auto it = bridgeInterfaces->begin(); it != bridgeInterfaces->end(); ++it)
    {
      (*it)->createDoorWindow(*channel, _channelIndex, deviceType);
    }
    return channel;
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

#ifndef SMARTHOMEBRIDGE_DEVICESONLY  
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
#endif

SmartHomeBridgeModule openknxSmartHomeBridgeModule;