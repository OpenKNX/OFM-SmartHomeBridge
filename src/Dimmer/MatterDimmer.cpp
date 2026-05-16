#include "Dimmer/MatterDimmer.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include "knxprod.h"
#include <esp_matter_bridge.h>

MatterDimmerBridge::MatterDimmerBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterDimmerBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

    uint32_t matterType = ParamBRI_CHDeviceType == 20
                              ? esp_matter::endpoint::dimmable_light::get_device_type_id()
                              : esp_matter::endpoint::dimmable_plugin_unit::get_device_type_id();

    _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(), matterType,
                                               static_cast<MatterBridgeDeviceBase *>(this));
    if (_device != nullptr)
    {
        matterbridge::setDeviceName(_device, _channel->getNameInUTF8());
        setBrightness(_channel->mainFunctionValue() ? 254 : 0);
    }
}

void MatterDimmerBridge::setBrightness(uint8_t brightness)
{
    if (_device == nullptr)
        return;

    matterbridge::reportBool(_device->persistent_info.device_endpoint_id, matterbridge::onOffClusterId,
                             matterbridge::onOffAttrId, brightness > 0);
    matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::levelControlClusterId,
                           matterbridge::currentLevelAttrId,
                           static_cast<uint8_t>(std::clamp<int>(brightness, 0, 254)));
}

void MatterDimmerBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t clusterId,
                                               uint32_t attributeId, esp_matter_attr_val_t *val)
{
    if (clusterId == matterbridge::onOffClusterId && attributeId == matterbridge::onOffAttrId)
    {
        _channel->commandPower(this, val->val.b);
    }
    else if (clusterId == matterbridge::levelControlClusterId && attributeId == matterbridge::currentLevelAttrId)
    {
        _channel->commandBrightness(this, val->val.u8);
    }
}