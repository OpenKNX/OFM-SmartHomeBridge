#pragma once

#include "BridgeBase.h"
#include <Arduino.h>
#include <esp_matter.h>

class MatterBridge : public BridgeBase
{
    esp_matter::node_t *_node = nullptr;
    uint16_t _aggregatorEndpointId = 0;
    uint32_t _matterSetupPasscode = 20202021;
    std::string _bridgeNodeLabel;

public:
    MatterBridge() = default;

    esp_matter::node_t *node() const;
    uint16_t parentEndpointId() const;

    virtual SwitchBridge *createSwitch(KnxChannelSwitch &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual DimmerBridge *createDimmer(KnxChannelDimmer &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual RGBBridge *createRGB(KnxChannelRGB &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual RolladenBridge *createJalousien(KnxChannelJalousie &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual RolladenBridge *createRolladen(KnxChannelRolladen &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual ThermostatBridge *createThermostat(KnxChannelThermostat &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual DeviceBridge *createDisplay(KnxChannelDisplay &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual AlarmBridge *createSensor(KnxChannelAlarm &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual FanBridge *createFan(KnxChannelFan &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual DoorWindowBridge *createDoorWindow(KnxChannelDoorWindow &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual SceneBridge *createScene(KnxChannelScene &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual LockBridge *createLock(KnxChannelLock &channel, uint8_t _channelIndex, uint8_t deviceType) override;
    virtual MediaBridge *createMedia(KnxChannelMedia &channel, uint8_t _channelIndex, uint8_t deviceType) override;

    virtual const std::string name() override;
    virtual void initialize(SmartHomeBridgeModule *bridge) override;
    virtual void start(SmartHomeBridgeModule *bridge) override;
    virtual void loop() override;
    virtual void processInputKo(GroupObject &ko) override;
    virtual void getInformation(String &result) override;
    virtual bool processCommand(const std::string cmd, bool diagnoseKo) override;
    virtual void showHelp() override;

private:
    static esp_err_t deviceTypeCallback(esp_matter::endpoint_t *endpoint, uint32_t deviceTypeId, void *privData);
    static esp_err_t attributeCallback(esp_matter::attribute::callback_type_t type, uint16_t endpointId,
                                       uint32_t clusterId, uint32_t attributeId,
                                       esp_matter_attr_val_t *val, void *privData);

    esp_err_t configureNode(esp_matter::node_t *node);
    esp_err_t startMatter();
    esp_err_t factoryReset();
};
