#pragma once

#include "Rolladen/MatterRolladen.h"
#include "Jalousie/KnxChannelJalousie.h"

class MatterJalousieBridge final : public MatterRolladenBridge
{
public:
    explicit MatterJalousieBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setSlatPosition(uint8_t slatPosition) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};