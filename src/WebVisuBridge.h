#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>
#include <vector>

#include "SmartHomeBridgeModule.h"

class WebVisuBridge : public BridgeBase
{
  public:
    virtual SwitchBridge* createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual DimmerBridge* createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;

    virtual const std::string name() override;
    virtual void initialize(SmartHomeBridgeModule* bridge) override;
    virtual void loop() override;
    virtual void registerWebPages() override;
    virtual bool processCommand(const std::string cmd, bool diagnoseKo) override;
    virtual void showHelp() override;

    void broadcastUpdate(const std::string& deviceJson);

  private:
    static constexpr const char* MENU_URI = "/devices";
    static constexpr const char* SOCKET_URI = "/devices/ws";

    SmartHomeBridgeModule* _bridge = nullptr;
    std::vector<std::string> _pendingCommands;

    void queueCommand(const uint8_t* data, int length);
    void processPendingCommands();
    void processCommandMessage(const std::string& message);

    std::string buildPageHtml() const;
    std::string buildSnapshotMessage() const;

    void sendSnapshotToClient(int clientId);

    static bool parseStringField(const std::string& message, const char* key, std::string& value);
    static bool parseIntField(const std::string& message, const char* key, int& value);
    static bool parseBoolField(const std::string& message, const char* key, bool& value);
};

#endif
