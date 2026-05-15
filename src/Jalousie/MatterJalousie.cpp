#include "Jalousie/MatterJalousie.h"

#include "Matter/MatterBridgeCommon.h"

MatterJalousieBridge::MatterJalousieBridge(MatterBridge *bridge) : MatterRolladenBridge(bridge)
{
}

void MatterJalousieBridge::setSlatPosition(uint8_t slatPosition)
{
    if (_device == nullptr)
        return;

    auto tilt = static_cast<uint16_t>(std::clamp<int>(slatPosition, 0, 100) * 100);
    auto currentVal = matterbridge::u16Value(tilt);
    auto targetVal = matterbridge::u16Value(tilt);
    esp_matter::attribute::report(_device->persistent_info.device_endpoint_id, matterbridge::windowCoveringClusterId,
                                  matterbridge::currentPositionTiltPercent100thsAttrId, &currentVal);
    esp_matter::attribute::report(_device->persistent_info.device_endpoint_id, matterbridge::windowCoveringClusterId,
                                  matterbridge::targetPositionTiltPercent100thsAttrId, &targetVal);
}

void MatterJalousieBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                                                 uint32_t attributeId, esp_matter_attr_val_t *val)
{
    MatterRolladenBridge::handleMatterAttribute(type, clusterId, attributeId, val);

    if (clusterId == matterbridge::windowCoveringClusterId &&
        attributeId == matterbridge::targetPositionTiltPercent100thsAttrId)
    {
        static_cast<KnxChannelJalousie *>(_channel)->commandSlatPosition(this, static_cast<uint8_t>(val->val.u16 / 100));
    }
}