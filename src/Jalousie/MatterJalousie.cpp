#include "Jalousie/MatterJalousie.h"

#include "Matter/MatterBridgeCommon.h"

MatterJalousieBridge::MatterJalousieBridge(MatterBridge *bridge) : MatterRolladenBridge(bridge)
{
}

void MatterJalousieBridge::setup(uint8_t channelIndex)
{
    MatterRolladenBridge::setup(channelIndex);

    if (_device == nullptr)
        return;

    // Create optional Tilt Percent100ths attributes (not created by window_covering_device).
    auto ep = _device->persistent_info.device_endpoint_id;
    auto wcCluster = esp_matter::cluster::get(ep, matterbridge::windowCoveringClusterId);
    if (wcCluster != nullptr)
    {
        esp_matter::cluster::window_covering::attribute::create_current_position_tilt_percent_100ths(
            wcCluster, nullable<uint16_t>());
        esp_matter::cluster::window_covering::attribute::create_target_position_tilt_percent_100ths(
            wcCluster, nullable<uint16_t>());
    }
}

void MatterJalousieBridge::setSlatPosition(uint8_t slatPosition)
{
    if (_device == nullptr)
        return;

    auto ep = _device->persistent_info.device_endpoint_id;
    auto tilt = static_cast<uint16_t>(std::clamp<int>(slatPosition, 0, 100) * 100);
    matterbridge::reportU16(ep, matterbridge::windowCoveringClusterId,
                            matterbridge::currentPositionTiltPercent100thsAttrId, tilt);
    matterbridge::reportU16(ep, matterbridge::windowCoveringClusterId,
                            matterbridge::targetPositionTiltPercent100thsAttrId, tilt);
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