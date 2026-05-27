#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>
#include <vector>
#include <functional>

#include "SmartHomeBridgeModule.h"

namespace OpenKNX
{
  namespace Network
  {
    class WebRequest;
    class WebResponse;
  }
}

class WebVisuBridge : public BridgeBase
{
  public:
    virtual SwitchBridge* createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual DimmerBridge* createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual RGBBridge* createRGB(KnxChannelRGB& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual SceneBridge* createScene(KnxChannelScene& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual RolladenBridge* createJalousien(KnxChannelJalousie& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual RolladenBridge* createRolladen(KnxChannelRolladen& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual ThermostatBridge* createThermostat(KnxChannelThermostat& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual DeviceBridge* createDisplay(KnxChannelDisplay& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual AlarmBridge* createSensor(KnxChannelAlarm& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual FanBridge* createFan(KnxChannelFan& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual DoorWindowBridge* createDoorWindow(KnxChannelDoorWindow& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual MediaBridge* createMedia(KnxChannelMedia& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual LockBridge* createLock(KnxChannelLock& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;

    virtual const std::string name() override;
    virtual void initialize(SmartHomeBridgeModule* bridge) override;
    virtual void loop() override;
    virtual void registerWebPages() override;
    virtual bool processCommand(const std::string cmd, bool diagnoseKo) override;
    virtual void showHelp() override;

    void broadcastUpdate(const std::string& deviceJson);
    void broadcastChannelUpdate(uint8_t channelIndex);

  private:
    static constexpr const char* MENU_URI = "/devices";
    static constexpr const char* SOCKET_URI = "/devices/ws";

    SmartHomeBridgeModule* _bridge = nullptr;
    std::vector<std::string> _pendingCommands;
    std::vector<MainFunctionChangedHandler> _channelChangedHandlers;
    std::vector<bool> _channelChangedHandlerRegistered;
    std::vector<class WebVisuWidgetBase*> _webVisuWidgets;

    void queueCommand(const uint8_t* data, int length);
    void processPendingCommands();
    void processCommandMessage(const std::string& message);
    void ensureChangeHandlersRegistered();
    void registerWebVisuWidget(uint8_t channelIndex, class WebVisuWidgetBase* widget);
    class WebVisuWidgetBase* webVisuWidget(uint8_t channelIndex) const;

    std::string buildPageHtml() const;
    std::string buildDetailPageHtml(uint8_t channelIndex) const;
    std::string buildSnapshotMessage() const;
    std::string buildDeviceJson(KnxChannelBase& channel, uint8_t channelIndex) const;
    std::string buildDetailWidgetHtml(KnxChannelBase& channel,
                      uint8_t channelIndex,
                      const std::string& name,
                      const std::string& type,
                      const std::string& value,
                      bool state,
                      const std::string& imageUrl,
                      bool supportMainAction,
                      bool allowRecolor) const;
    std::string buildImageUrl(const std::string& imageFile) const;

    void handleDetailRequest(const OpenKNX::Network::WebRequest& req, OpenKNX::Network::WebResponse& res) const;
    void handleImageRequest(const OpenKNX::Network::WebRequest& req, OpenKNX::Network::WebResponse& res) const;

    void sendSnapshotToClient(int clientId);

    static bool startsWith(const std::string& value, const std::string& prefix);
    static std::string jsonEscape(const std::string& input);
    static std::string urlEncode(const std::string& input);
    static bool tryParseChannelOneBasedFromUri(const std::string& uri, const char* prefix, uint8_t& channelIndex);

    static bool parseStringField(const std::string& message, const char* key, std::string& value);
    static bool parseIntField(const std::string& message, const char* key, int& value);
    static bool parseBoolField(const std::string& message, const char* key, bool& value);
};

#endif
