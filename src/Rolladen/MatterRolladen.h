#pragma once

#include "Matter/MatterBridgeDeviceBase.h"
#include "Rolladen/KnxChannelRolladen.h"

class MatterRolladenBridge : public MatterBridgeDeviceBase, public RolladenBridge
{
public:
    explicit MatterRolladenBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setPosition(uint8_t position) override;
    void setMovement(MoveState movement) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};