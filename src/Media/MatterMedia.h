#pragma once

#include "Matter/MatterBridgeDeviceBase.h"
#include "Media/KnxChannelMedia.h"

class MatterMediaBridge final : public MatterBridgeDeviceBase, public MediaBridge
{
public:
    explicit MatterMediaBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setPlay(bool play) override;
    void setVolume(uint8_t volume) override;
    void setTitle(const char *text) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};