#include "Display/MatterDisplay.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include <esp_matter_bridge.h>

MatterDisplayBridge::MatterDisplayBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterDisplayBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

    _displayType = _channel->getDisplayType();
    switch (_displayType)
    {
    case DisplayTypeTemperature:
        _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                                   esp_matter::endpoint::temperature_sensor::get_device_type_id(),
                                                   static_cast<MatterBridgeDeviceBase *>(this));
        break;
    case DisplayTypeHumidity:
        _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                                   esp_matter::endpoint::humidity_sensor::get_device_type_id(),
                                                   static_cast<MatterBridgeDeviceBase *>(this));
        break;
    case DisplayTypeLux:
        _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                                   esp_matter::endpoint::light_sensor::get_device_type_id(),
                                                   static_cast<MatterBridgeDeviceBase *>(this));
        break;
    case DisplayTypeRain:
    case DisplayTypeSnow:
        _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                                   esp_matter::endpoint::rain_sensor::get_device_type_id(),
                                                   static_cast<MatterBridgeDeviceBase *>(this));
        break;
    case DisplayTypeWind:
        _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                                   esp_matter::endpoint::air_quality_sensor::get_device_type_id(),
                                                   static_cast<MatterBridgeDeviceBase *>(this));
        break;
    default:
        _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                                   esp_matter::endpoint::occupancy_sensor::get_device_type_id(),
                                                   static_cast<MatterBridgeDeviceBase *>(this));
        break;
    }
}

void MatterDisplayBridge::setValue(double value)
{
    if (_device == nullptr)
        return;

    switch (_displayType)
    {
    case DisplayTypeTemperature:
        matterbridge::reportI16(_device->persistent_info.device_endpoint_id, matterbridge::temperatureClusterId,
                                matterbridge::measuredTemperatureAttrId,
                                static_cast<int16_t>(std::lround(value * 100.0)));
        break;
    case DisplayTypeHumidity:
        matterbridge::reportU16(_device->persistent_info.device_endpoint_id, matterbridge::humidityClusterId,
                                matterbridge::measuredHumidityAttrId,
                                static_cast<uint16_t>(std::clamp<int>(std::lround(value * 100.0), 0, 10000)));
        break;
    case DisplayTypeLux:
        matterbridge::reportU16(_device->persistent_info.device_endpoint_id, matterbridge::illuminanceClusterId,
                                matterbridge::measuredIlluminanceAttrId,
                                static_cast<uint16_t>(std::clamp<int>(std::lround(value), 0, 65534)));
        break;
    case DisplayTypeRain:
    case DisplayTypeSnow:
        matterbridge::reportBool(_device->persistent_info.device_endpoint_id, matterbridge::boolStateClusterId,
                                 matterbridge::boolStateAttrId, value > 0.0);
        break;
    case DisplayTypeWind:
        matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::airQualityClusterId,
                               matterbridge::measuredAirQualityAttrId,
                               value > 0.0 ? static_cast<uint8_t>(chip::app::Clusters::AirQuality::AirQualityEnum::kFair)
                                           : static_cast<uint8_t>(chip::app::Clusters::AirQuality::AirQualityEnum::kGood));
        break;
    default:
        matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::occupancyClusterId,
                               matterbridge::occupancyAttrId,
                               static_cast<uint8_t>(std::clamp<int>(std::lround(value), 0, 1)));
        break;
    }
}

void MatterDisplayBridge::setValue(const char *)
{
}

void MatterDisplayBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t,
                                                uint32_t, esp_matter_attr_val_t *)
{
}