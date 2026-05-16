#include "Scene/MatterScene.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include <esp_matter_bridge.h>

MatterSceneBridge::MatterSceneBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterSceneBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

    _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                               esp_matter::endpoint::on_off_light::get_device_type_id(),
                                               static_cast<MatterBridgeDeviceBase *>(this));
    if (_device != nullptr)
    {
        matterbridge::setDeviceName(_device, _channel->getNameInUTF8());
        setActivating(false);
    }
}

void MatterSceneBridge::setActivating(bool activating)
{
    if (_device == nullptr)
        return;

    matterbridge::reportBool(_device->persistent_info.device_endpoint_id, matterbridge::onOffClusterId,
                             matterbridge::onOffAttrId, activating);
}

void MatterSceneBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t clusterId,
                                              uint32_t attributeId, esp_matter_attr_val_t *val)
{
    if (clusterId == matterbridge::onOffClusterId && attributeId == matterbridge::onOffAttrId && val->val.b)
        _channel->commandActivate(this);
}