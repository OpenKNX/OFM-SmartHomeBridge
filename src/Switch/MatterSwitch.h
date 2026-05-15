#pragma once

#include "Matter/MatterBridgeDeviceBase.h"
#include "Switch/KnxChannelSwitch.h"

class MatterSwitchBridge final : public MatterBridgeDeviceBase, public SwitchBridge
{
public:
    explicit MatterSwitchBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setPower(bool value) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};