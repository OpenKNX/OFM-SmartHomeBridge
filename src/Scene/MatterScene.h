#pragma once

#include "Matter/MatterBridgeDeviceBase.h"
#include "Scene/KnxChannelScene.h"

class MatterSceneBridge final : public MatterBridgeDeviceBase, public SceneBridge
{
public:
    explicit MatterSceneBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setActivating(bool activating) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};