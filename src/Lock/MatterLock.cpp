#include "Lock/MatterLock.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include <esp_matter_bridge.h>

MatterLockBridge::MatterLockBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterLockBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

    _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                               esp_matter::endpoint::door_lock::get_device_type_id(),
                                               static_cast<MatterBridgeDeviceBase *>(this));
    if (_device != nullptr)
        setLocked(_channel->mainFunctionValue());
}

void MatterLockBridge::setLocked(bool lock)
{
    if (_device == nullptr)
        return;

    matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::doorLockClusterId,
                           matterbridge::lockStateAttrId,
                           lock ? static_cast<uint8_t>(chip::app::Clusters::DoorLock::DlLockState::kLocked)
                                : static_cast<uint8_t>(chip::app::Clusters::DoorLock::DlLockState::kUnlocked));
}

void MatterLockBridge::setBlocked(bool)
{
}

void MatterLockBridge::setUnlocking(bool)
{
}

void MatterLockBridge::setLocking(bool)
{
}

void MatterLockBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t,
                                             uint32_t attributeId, esp_matter_attr_val_t *val)
{
    if (attributeId == matterbridge::lockStateAttrId)
    {
        const auto lockState = static_cast<uint8_t>(val->val.u8);
        if (lockState == static_cast<uint8_t>(chip::app::Clusters::DoorLock::DlLockState::kLocked))
            _channel->commandLock(this, true);
        else if (lockState == static_cast<uint8_t>(chip::app::Clusters::DoorLock::DlLockState::kUnlocked))
            _channel->commandLock(this, false);
    }
}