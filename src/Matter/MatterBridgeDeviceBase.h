#pragma once

#include <Arduino.h>
#include <esp_matter.h>
#include <esp_matter_bridge.h>

class MatterBridge;

class MatterBridgeDeviceBase
{
protected:
    MatterBridge *_bridge = nullptr;
    esp_matter_bridge::device_t *_device = nullptr;

public:
    explicit MatterBridgeDeviceBase(MatterBridge *bridge) : _bridge(bridge) {}

    virtual ~MatterBridgeDeviceBase()
    {
        if (_device != nullptr)
            esp_matter_bridge::remove_device(_device);
    }

    uint16_t endpointId() const
    {
        return _device != nullptr ? _device->persistent_info.device_endpoint_id : 0;
    }

    virtual void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                                       uint32_t attributeId, esp_matter_attr_val_t *val) = 0;
};