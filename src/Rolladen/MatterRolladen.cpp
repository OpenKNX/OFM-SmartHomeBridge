#include "Rolladen/MatterRolladen.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include <esp_matter_bridge.h>

namespace
{
constexpr uint8_t kDeviceTypeRolladen = 31;
constexpr uint8_t kDeviceTypeMarkise = 32;

// WindowCovering EndProductType enum values (Matter spec).
constexpr uint8_t kEndProductTypeRollerShade = 0x00;
constexpr uint8_t kEndProductTypeRollerShutter = 0x11;
constexpr uint8_t kEndProductTypeInteriorVenetianBlind = 0x0C;
constexpr uint8_t kEndProductTypeAwningTerracePatio = 0x13;

void applyWindowCoveringEndProductType(uint16_t endpointId, uint8_t channelDeviceType)
{
    auto wcCluster = esp_matter::cluster::get(endpointId, matterbridge::windowCoveringClusterId);
    if (wcCluster == nullptr)
        return;

    uint8_t endProductType = kEndProductTypeRollerShade;
    if (channelDeviceType == 30)
        endProductType = kEndProductTypeInteriorVenetianBlind;
    else if (channelDeviceType == 32)
        endProductType = kEndProductTypeAwningTerracePatio;
    else if (channelDeviceType == 31)
        endProductType = kEndProductTypeRollerShutter;

    constexpr uint32_t endProductTypeAttrId = chip::app::Clusters::WindowCovering::Attributes::EndProductType::Id;
    if (!matterbridge::hasAttribute(endpointId, matterbridge::windowCoveringClusterId, endProductTypeAttrId))
    {
        esp_matter::cluster::window_covering::attribute::create_end_product_type(wcCluster, endProductType);
        return;
    }

    auto updated = matterbridge::u8Value(endProductType);
    esp_matter::attribute::update(endpointId, matterbridge::windowCoveringClusterId, endProductTypeAttrId, &updated);
}
} // namespace

MatterRolladenBridge::MatterRolladenBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterRolladenBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

     _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                               esp_matter::endpoint::window_covering_device::get_device_type_id(),
                                               static_cast<MatterBridgeDeviceBase *>(this));
    if (_device != nullptr)
    {
        matterbridge::setDeviceName(_device, _channel->getNameInUTF8());

        // Create optional Lift Percent100ths attributes (mandatory only under PA feature flag,
        // so window_covering_device does not create them automatically).
        auto ep = _device->persistent_info.device_endpoint_id;
        auto wcCluster = esp_matter::cluster::get(ep, matterbridge::windowCoveringClusterId);
        if (wcCluster != nullptr)
        {
            esp_matter::cluster::window_covering::attribute::create_current_position_lift_percent_100ths(
                wcCluster, nullable<uint16_t>());
            esp_matter::cluster::window_covering::attribute::create_target_position_lift_percent_100ths(
                wcCluster, nullable<uint16_t>());
        }    
        applyWindowCoveringEndProductType(ep, ParamBRI_CHDeviceType);
        setPosition(_channel->mainFunctionValue() ? 100 : 0);
    }
}

void MatterRolladenBridge::setPosition(uint8_t position)
{
    if (_device == nullptr)
        return;

    auto ep = _device->persistent_info.device_endpoint_id;
    auto matterPosition = static_cast<uint16_t>(std::clamp<int>(position, 0, 100) * 100);
    matterbridge::reportU16(ep, matterbridge::windowCoveringClusterId,
                            matterbridge::currentPositionLiftPercent100thsAttrId, matterPosition);
    matterbridge::reportU16(ep, matterbridge::windowCoveringClusterId,
                            matterbridge::targetPositionLiftPercent100thsAttrId, matterPosition);
}

void MatterRolladenBridge::setMovement(MoveState)
{
}

void MatterRolladenBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t clusterId,
                                                 uint32_t attributeId, esp_matter_attr_val_t *val)
{
    if (clusterId == matterbridge::windowCoveringClusterId &&
        attributeId == matterbridge::targetPositionLiftPercent100thsAttrId)
    {
        _channel->commandPosition(this, static_cast<uint8_t>(val->val.u16 / 100));
    }
}