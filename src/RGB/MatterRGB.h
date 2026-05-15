#pragma once

#include "Matter/MatterBridgeDeviceBase.h"
#include "RGB/KnxChannelRGB.h"

class MatterRGBBridge final : public MatterBridgeDeviceBase, public RGBBridge
{
    uint8_t _hue = 0;
    uint8_t _saturation = 0;
    uint8_t _brightness = 254;

public:
    explicit MatterRGBBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setRGB(uint32_t rgb) override;
    void setPower(bool on) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};