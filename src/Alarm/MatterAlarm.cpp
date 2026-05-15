#include "Alarm/MatterAlarm.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include <esp_matter_bridge.h>

MatterAlarmBridge::MatterAlarmBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterAlarmBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

    uint32_t matterType = esp_matter::endpoint::occupancy_sensor::get_device_type_id();
    switch (_channel->getAlarmType())
    {
    case AlarmTypeContact:
        matterType = esp_matter::endpoint::contact_sensor::get_device_type_id();
        break;
    case AlarmTypeCarbonMonoxid:
    case AlarmTypeSmoke:
        matterType = esp_matter::endpoint::smoke_co_alarm::get_device_type_id();
        break;
    case AlarmTypeCarbonDioxid:
        matterType = esp_matter::endpoint::air_quality_sensor::get_device_type_id();
        break;
    case AlarmTypeLeak:
        matterType = esp_matter::endpoint::water_leak_detector::get_device_type_id();
        break;
    default:
        matterType = esp_matter::endpoint::occupancy_sensor::get_device_type_id();
        break;
    }

    _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(), matterType,
                                               static_cast<MatterBridgeDeviceBase *>(this));
    if (_device != nullptr)
        setDetected(_channel->mainFunctionValue());
}

void MatterAlarmBridge::setDetected(bool detected)
{
    if (_device == nullptr)
        return;

    auto alarmType = _channel->getAlarmType();
    if (alarmType == AlarmTypeContact || alarmType == AlarmTypeLeak)
    {
        matterbridge::reportBool(_device->persistent_info.device_endpoint_id, matterbridge::boolStateClusterId,
                                 matterbridge::boolStateAttrId, detected);
    }
    else if (alarmType == AlarmTypeSmoke)
    {
        matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::smokeCoAlarmClusterId,
                               matterbridge::smokeStateAttrId,
                               detected ? static_cast<uint8_t>(chip::app::Clusters::SmokeCoAlarm::AlarmStateEnum::kCritical)
                                        : static_cast<uint8_t>(chip::app::Clusters::SmokeCoAlarm::AlarmStateEnum::kNormal));
    }
    else if (alarmType == AlarmTypeCarbonMonoxid)
    {
        matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::smokeCoAlarmClusterId,
                               matterbridge::coStateAttrId,
                               detected ? static_cast<uint8_t>(chip::app::Clusters::SmokeCoAlarm::AlarmStateEnum::kCritical)
                                        : static_cast<uint8_t>(chip::app::Clusters::SmokeCoAlarm::AlarmStateEnum::kNormal));
    }
    else if (alarmType == AlarmTypeCarbonDioxid)
    {
        matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::airQualityClusterId,
                               matterbridge::measuredAirQualityAttrId,
                               detected ? static_cast<uint8_t>(chip::app::Clusters::AirQuality::AirQualityEnum::kPoor)
                                        : static_cast<uint8_t>(chip::app::Clusters::AirQuality::AirQualityEnum::kGood));
    }
    else
    {
        matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::occupancyClusterId,
                               matterbridge::occupancyAttrId, detected ? 1 : 0);
    }
}

void MatterAlarmBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t,
                                              uint32_t, esp_matter_attr_val_t *)
{
}