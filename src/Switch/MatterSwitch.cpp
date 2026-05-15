#include "Switch/MatterSwitch.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include "knxprod.h"
#include <esp_matter_bridge.h>

MatterSwitchBridge::MatterSwitchBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterSwitchBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

    uint32_t matterType = esp_matter::endpoint::on_off_switch::get_device_type_id();
    if (ParamBRI_CHDeviceType == 11)
        matterType = esp_matter::endpoint::on_off_plugin_unit::get_device_type_id();
    else if (ParamBRI_CHDeviceType == 20)
        matterType = esp_matter::endpoint::on_off_light::get_device_type_id();

    _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(), matterType,
                                               static_cast<MatterBridgeDeviceBase *>(this));
    if (_device != nullptr)
        setPower(_channel->mainFunctionValue());
}

void MatterSwitchBridge::setPower(bool value)
{
    if (_device == nullptr)
        return;

    matterbridge::reportBool(_device->persistent_info.device_endpoint_id, matterbridge::onOffClusterId,
                             matterbridge::onOffAttrId, value);
}

void MatterSwitchBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t clusterId,
                                               uint32_t attributeId, esp_matter_attr_val_t *val)
{
    if (clusterId == matterbridge::onOffClusterId && attributeId == matterbridge::onOffAttrId)
        _channel->commandPower(this, val->val.b);
}