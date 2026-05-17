#pragma once

#include "Matter/MatterBridgeDeviceBase.h"
#include "Fan/KnxChannelFan.h"

class MatterFanBridge final : public MatterBridgeDeviceBase, public FanBridge
{
    bool _automatic = false;
    bool _power = false;

public:
    explicit MatterFanBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setAutomatic(bool automatic) override;
    void setPower(bool on) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};