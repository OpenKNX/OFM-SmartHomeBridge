#pragma once

#include "Matter/MatterBridgeDeviceBase.h"
#include "Display/KnxChannelDisplay.h"

class MatterDisplayBridge final : public MatterBridgeDeviceBase, public DeviceBridge
{
    DisplayType _displayType = DisplayTypeText;

public:
    explicit MatterDisplayBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setValue(double value) override;
    void setValue(const char *value) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};