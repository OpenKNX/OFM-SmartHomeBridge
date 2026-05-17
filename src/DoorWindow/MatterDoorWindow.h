#pragma once

#include "Matter/MatterBridgeDeviceBase.h"
#include "DoorWindow/KnxChannelDoorWindow.h"

class MatterDoorWindowBridge final : public MatterBridgeDeviceBase, public DoorWindowBridge
{
public:
    explicit MatterDoorWindowBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setPosition(uint8_t position) override;
    void setMovement(DoorWindowMoveState movement) override;
    void setObstructionDetected(bool obstructionDetected) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};