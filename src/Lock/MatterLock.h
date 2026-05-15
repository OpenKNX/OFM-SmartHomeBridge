#pragma once

#include "Matter/MatterBridgeDeviceBase.h"
#include "Lock/KnxChannelLock.h"

class MatterLockBridge final : public MatterBridgeDeviceBase, public LockBridge
{
public:
    explicit MatterLockBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setLocked(bool lock) override;
    void setBlocked(bool lock) override;
    void setUnlocking(bool unlocking) override;
    void setLocking(bool locking) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};