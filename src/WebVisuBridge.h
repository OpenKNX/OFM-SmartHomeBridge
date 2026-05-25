#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <map>
#include <string>
#include <vector>

#include "SmartHomeBridgeModule.h"

class WebVisuBridge : public BridgeBase
{
  public:
    enum class DeviceKind : uint8_t
    {
        Switch,
        Dimmer
    };

    struct DeviceState
    {
        DeviceKind kind = DeviceKind::Switch;
        uint8_t channelIndex = 0;
        std::string name;
        bool power = false;
        uint8_t brightness = 0;
    };

    virtual SwitchBridge* createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;
    virtual DimmerBridge* createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex /* this parameter is used in macros, do not rename */, uint8_t deviceType) override;

    virtual const std::string name() override;
    virtual void initialize(SmartHomeBridgeModule* bridge) override;
    virtual void loop() override;
    virtual void registerWebPages() override;
    virtual bool processCommand(const std::string cmd, bool diagnoseKo) override;
    virtual void showHelp() override;

    void reportSwitchState(uint8_t channelIndex, const char* name, bool power);
    void reportDimmerState(uint8_t channelIndex, const char* name, uint8_t brightness);

  private:
    static constexpr const char* MENU_URI = "/geraete";
    static constexpr const char* SOCKET_URI = "/geraete/ws";

    SmartHomeBridgeModule* _bridge = nullptr;
    std::map<uint8_t, DeviceState> _devices;
    std::vector<std::string> _pendingCommands;

    void queueCommand(const uint8_t* data, int length);
    void processPendingCommands();
    void processCommandMessage(const std::string& message);

    DeviceState* findDeviceByChannelOneBased(int channel);

    std::string buildPageHtml() const;
    std::string buildSnapshotMessage() const;
    std::string buildUpdateMessage(const DeviceState& device) const;
    std::string buildDeviceJson(const DeviceState& device) const;

    void sendSnapshotToClient(int clientId);
    void broadcastUpdate(const DeviceState& device);

    static bool parseStringField(const std::string& message, const char* key, std::string& value);
    static bool parseIntField(const std::string& message, const char* key, int& value);
    static bool parseBoolField(const std::string& message, const char* key, bool& value);
    static std::string jsonEscape(const std::string& input);
};

#endif
