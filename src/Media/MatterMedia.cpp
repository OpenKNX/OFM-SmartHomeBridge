#include "Media/MatterMedia.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include <esp_matter_bridge.h>

MatterMediaBridge::MatterMediaBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterMediaBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

    _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                               esp_matter::endpoint::dimmable_light::get_device_type_id(),
                                               static_cast<MatterBridgeDeviceBase *>(this));
    if (_device != nullptr)
    {
        matterbridge::setDeviceName(_device, _channel->getNameInUTF8());
    }
}

void MatterMediaBridge::setPlay(bool play)
{
    if (_device == nullptr)
        return;

    matterbridge::reportBool(_device->persistent_info.device_endpoint_id, matterbridge::onOffClusterId,
                             matterbridge::onOffAttrId, play);
}

void MatterMediaBridge::setVolume(uint8_t volume)
{
    if (_device == nullptr)
        return;

    matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::levelControlClusterId,
                           matterbridge::currentLevelAttrId, volume);
}

void MatterMediaBridge::setTitle(const char *text)
{
    if (_device == nullptr)
        return;

    matterbridge::reportText(_device->persistent_info.device_endpoint_id, matterbridge::basicInformationClusterId,
                             matterbridge::nodeLabelAttrId, text);
}

void MatterMediaBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t clusterId,
                                              uint32_t attributeId, esp_matter_attr_val_t *val)
{
    if (clusterId == matterbridge::onOffClusterId && attributeId == matterbridge::onOffAttrId)
        _channel->commandPlay(this, val->val.b);
    else if (clusterId == matterbridge::levelControlClusterId && attributeId == matterbridge::currentLevelAttrId)
        _channel->commandVolume(this, val->val.u8);
}