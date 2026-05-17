#include "Fan/MatterFan.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include "knxprod.h"
#include <esp_matter_bridge.h>

MatterFanBridge::MatterFanBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterFanBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

    _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                               esp_matter::endpoint::fan::get_device_type_id(),
                                               static_cast<MatterBridgeDeviceBase *>(this));
    if (_device != nullptr)
    {
        matterbridge::setDeviceName(_device, _channel->getNameInUTF8());
        setPower(_channel->mainFunctionValue());
        setAutomatic(_automatic);
    }
}

void MatterFanBridge::setAutomatic(bool automatic)
{
    uint8_t _channelIndex = _channel != nullptr ? _channel->channelIndex() : 0;
    if (ParamBRI_CHFanAutomatic == 0)
        return;

    _automatic = automatic;
    if (_device == nullptr)
        return;

    matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::fanControlClusterId,
                           matterbridge::fanModeAttrId, automatic ? 0 : 1);
}

void MatterFanBridge::setPower(bool on)
{
    _power = on;
    if (_device == nullptr)
        return;

    matterbridge::reportBool(_device->persistent_info.device_endpoint_id, matterbridge::onOffClusterId,
                             matterbridge::onOffAttrId, on);
    matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::fanControlClusterId,
                           matterbridge::percentCurrentAttrId, on ? 100 : 0);
    matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::fanControlClusterId,
                           matterbridge::percentSettingAttrId, on ? 100 : 0);
}

void MatterFanBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t clusterId,
                                            uint32_t attributeId, esp_matter_attr_val_t *val)
{
    if (clusterId == matterbridge::onOffClusterId && attributeId == matterbridge::onOffAttrId)
        _channel->commandPower(this, val->val.b);
    else if (clusterId == matterbridge::fanControlClusterId && attributeId == matterbridge::fanModeAttrId)
        _channel->commandAutomatic(this, val->val.u8 == 0);
}