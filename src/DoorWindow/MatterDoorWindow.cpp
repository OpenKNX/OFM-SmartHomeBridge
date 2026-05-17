#include "DoorWindow/MatterDoorWindow.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include <esp_matter_bridge.h>

MatterDoorWindowBridge::MatterDoorWindowBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterDoorWindowBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

    _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                               esp_matter::endpoint::window_covering_device::get_device_type_id(),
                                               static_cast<MatterBridgeDeviceBase *>(this));
    if (_device != nullptr)
    {
        matterbridge::setDeviceName(_device, _channel->getNameInUTF8());
        setPosition(_channel->mainFunctionValue() ? 100 : 0);
    }
}

void MatterDoorWindowBridge::setPosition(uint8_t position)
{
    if (_device == nullptr)
        return;

    auto matterPosition = static_cast<uint16_t>(std::clamp<int>(position, 0, 100) * 100);
    auto currentVal = matterbridge::u16Value(matterPosition);
    auto targetVal = matterbridge::u16Value(matterPosition);
    esp_matter::attribute::report(_device->persistent_info.device_endpoint_id, matterbridge::windowCoveringClusterId,
                                  matterbridge::currentPositionLiftPercent100thsAttrId, &currentVal);
    esp_matter::attribute::report(_device->persistent_info.device_endpoint_id, matterbridge::windowCoveringClusterId,
                                  matterbridge::targetPositionLiftPercent100thsAttrId, &targetVal);
}

void MatterDoorWindowBridge::setMovement(DoorWindowMoveState)
{
}

void MatterDoorWindowBridge::setObstructionDetected(bool)
{
}

void MatterDoorWindowBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t clusterId,
                                                   uint32_t attributeId, esp_matter_attr_val_t *val)
{
    if (clusterId == matterbridge::windowCoveringClusterId &&
        attributeId == matterbridge::targetPositionLiftPercent100thsAttrId)
    {
        _channel->commandPosition(this, static_cast<uint8_t>(val->val.u16 / 100));
    }
}