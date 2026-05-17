#pragma once

#include "Matter/MatterBridgeDeviceBase.h"
#include "Dimmer/KnxChannelDimmer.h"

class MatterDimmerBridge final : public MatterBridgeDeviceBase, public DimmerBridge
{
public:
    explicit MatterDimmerBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setBrightness(uint8_t brightness) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};