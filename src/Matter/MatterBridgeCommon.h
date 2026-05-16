#pragma once

#include <Arduino.h>
#include <esp_matter.h>
#include <esp_matter_attribute_utils.h>
#include <esp_matter_bridge.h>

#ifdef INADDR_NONE
#undef INADDR_NONE
#endif

#include <algorithm>
#include <cmath>
#include <string>

namespace matterbridge
{
constexpr uint32_t onOffClusterId = chip::app::Clusters::OnOff::Id;
constexpr uint32_t levelControlClusterId = chip::app::Clusters::LevelControl::Id;
constexpr uint32_t colorControlClusterId = chip::app::Clusters::ColorControl::Id;
constexpr uint32_t fanControlClusterId = chip::app::Clusters::FanControl::Id;
constexpr uint32_t thermostatClusterId = chip::app::Clusters::Thermostat::Id;
constexpr uint32_t doorLockClusterId = chip::app::Clusters::DoorLock::Id;
constexpr uint32_t windowCoveringClusterId = chip::app::Clusters::WindowCovering::Id;
constexpr uint32_t basicInformationClusterId = chip::app::Clusters::BasicInformation::Id;
constexpr uint32_t bridgedDeviceBasicInformationClusterId = chip::app::Clusters::BridgedDeviceBasicInformation::Id;
constexpr uint32_t occupancyClusterId = chip::app::Clusters::OccupancySensing::Id;
constexpr uint32_t smokeCoAlarmClusterId = chip::app::Clusters::SmokeCoAlarm::Id;
constexpr uint32_t humidityClusterId = chip::app::Clusters::RelativeHumidityMeasurement::Id;
constexpr uint32_t temperatureClusterId = chip::app::Clusters::TemperatureMeasurement::Id;
constexpr uint32_t illuminanceClusterId = chip::app::Clusters::IlluminanceMeasurement::Id;
constexpr uint32_t airQualityClusterId = chip::app::Clusters::AirQuality::Id;
constexpr uint32_t boolStateClusterId = chip::app::Clusters::BooleanState::Id;

constexpr uint32_t onOffAttrId = chip::app::Clusters::OnOff::Attributes::OnOff::Id;
constexpr uint32_t currentLevelAttrId = chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Id;
constexpr uint32_t currentHueAttrId = chip::app::Clusters::ColorControl::Attributes::CurrentHue::Id;
constexpr uint32_t currentSaturationAttrId = chip::app::Clusters::ColorControl::Attributes::CurrentSaturation::Id;
constexpr uint32_t fanModeAttrId = chip::app::Clusters::FanControl::Attributes::FanMode::Id;
constexpr uint32_t percentSettingAttrId = chip::app::Clusters::FanControl::Attributes::PercentSetting::Id;
constexpr uint32_t percentCurrentAttrId = chip::app::Clusters::FanControl::Attributes::PercentCurrent::Id;
constexpr uint32_t systemModeAttrId = chip::app::Clusters::Thermostat::Attributes::SystemMode::Id;
// RunningMode may be absent in some generated CHIP namespace variants; use the spec attribute id directly.
constexpr uint32_t runningModeAttrId = 0x001E;
constexpr uint32_t localTemperatureAttrId = chip::app::Clusters::Thermostat::Attributes::LocalTemperature::Id;
constexpr uint32_t heatingSetpointAttrId = chip::app::Clusters::Thermostat::Attributes::OccupiedHeatingSetpoint::Id;
constexpr uint32_t coolingSetpointAttrId = chip::app::Clusters::Thermostat::Attributes::OccupiedCoolingSetpoint::Id;
constexpr uint32_t lockStateAttrId = chip::app::Clusters::DoorLock::Attributes::LockState::Id;
constexpr uint32_t targetPositionTiltPercent100thsAttrId = chip::app::Clusters::WindowCovering::Attributes::TargetPositionTiltPercent100ths::Id;
constexpr uint32_t currentPositionTiltPercent100thsAttrId = chip::app::Clusters::WindowCovering::Attributes::CurrentPositionTiltPercent100ths::Id;
constexpr uint32_t currentPositionLiftPercent100thsAttrId = chip::app::Clusters::WindowCovering::Attributes::CurrentPositionLiftPercent100ths::Id;
constexpr uint32_t targetPositionLiftPercent100thsAttrId = chip::app::Clusters::WindowCovering::Attributes::TargetPositionLiftPercent100ths::Id;
constexpr uint32_t occupancyAttrId = chip::app::Clusters::OccupancySensing::Attributes::Occupancy::Id;
constexpr uint32_t boolStateAttrId = chip::app::Clusters::BooleanState::Attributes::StateValue::Id;
constexpr uint32_t smokeStateAttrId = chip::app::Clusters::SmokeCoAlarm::Attributes::SmokeState::Id;
constexpr uint32_t coStateAttrId = chip::app::Clusters::SmokeCoAlarm::Attributes::COState::Id;
constexpr uint32_t measuredTemperatureAttrId = chip::app::Clusters::TemperatureMeasurement::Attributes::MeasuredValue::Id;
constexpr uint32_t measuredHumidityAttrId = chip::app::Clusters::RelativeHumidityMeasurement::Attributes::MeasuredValue::Id;
constexpr uint32_t measuredIlluminanceAttrId = chip::app::Clusters::IlluminanceMeasurement::Attributes::MeasuredValue::Id;
constexpr uint32_t measuredAirQualityAttrId = chip::app::Clusters::AirQuality::Attributes::AirQuality::Id;
constexpr uint32_t nodeLabelAttrId = chip::app::Clusters::BasicInformation::Attributes::NodeLabel::Id;
constexpr uint32_t productNameAttrId = chip::app::Clusters::BasicInformation::Attributes::ProductName::Id;
constexpr uint32_t bridgedNodeLabelAttrId = chip::app::Clusters::BridgedDeviceBasicInformation::Attributes::NodeLabel::Id;

inline esp_matter_attr_val_t boolValue(bool value)
{
    esp_matter_attr_val_t result{};
    result.type = ESP_MATTER_VAL_TYPE_BOOLEAN;
    result.val.b = value;
    return result;
}

inline esp_matter_attr_val_t u8Value(uint8_t value)
{
    esp_matter_attr_val_t result{};
    result.type = ESP_MATTER_VAL_TYPE_UINT8;
    result.val.u8 = value;
    return result;
}

inline esp_matter_attr_val_t i16Value(int16_t value)
{
    esp_matter_attr_val_t result{};
    result.type = ESP_MATTER_VAL_TYPE_INT16;
    result.val.i16 = value;
    return result;
}

inline esp_matter_attr_val_t u16Value(uint16_t value)
{
    esp_matter_attr_val_t result{};
    result.type = ESP_MATTER_VAL_TYPE_UINT16;
    result.val.u16 = value;
    return result;
}

inline esp_matter_attr_val_t textValue(const char *value)
{
    std::string copy = value != nullptr ? value : "";
    return esp_matter_char_str(copy.data(), static_cast<uint16_t>(copy.size()));
}

inline bool hasAttribute(uint16_t endpointId, uint32_t clusterId, uint32_t attributeId)
{
    if (esp_matter::cluster::get(endpointId, clusterId) == nullptr)
        return false;

    return esp_matter::attribute::get(endpointId, clusterId, attributeId) != nullptr;
}

inline esp_err_t reportBool(uint16_t endpointId, uint32_t clusterId, uint32_t attributeId, bool value)
{
    if (!hasAttribute(endpointId, clusterId, attributeId))
        return ESP_ERR_NOT_FOUND;

    auto reported = boolValue(value);
    return esp_matter::attribute::report(endpointId, clusterId, attributeId, &reported);
}

inline esp_err_t reportU8(uint16_t endpointId, uint32_t clusterId, uint32_t attributeId, uint8_t value)
{
    if (!hasAttribute(endpointId, clusterId, attributeId))
        return ESP_ERR_NOT_FOUND;

    auto attribute = esp_matter::attribute::get(endpointId, clusterId, attributeId);

    esp_matter_attr_val_t current{};
    esp_matter_attr_val_t reported = u8Value(value);
    if (esp_matter::attribute::get_val(attribute, &current) == ESP_OK &&
        current.type == ESP_MATTER_VAL_TYPE_NULLABLE_UINT8)
    {
        reported = esp_matter_nullable_uint8(value);
    }

    return esp_matter::attribute::report(endpointId, clusterId, attributeId, &reported);
}

inline esp_err_t reportI16(uint16_t endpointId, uint32_t clusterId, uint32_t attributeId, int16_t value)
{
    if (!hasAttribute(endpointId, clusterId, attributeId))
        return ESP_ERR_NOT_FOUND;

    auto reported = i16Value(value);
    return esp_matter::attribute::report(endpointId, clusterId, attributeId, &reported);
}

inline esp_err_t reportU16(uint16_t endpointId, uint32_t clusterId, uint32_t attributeId, uint16_t value)
{
    if (!hasAttribute(endpointId, clusterId, attributeId))
        return ESP_ERR_NOT_FOUND;

    auto reported = u16Value(value);
    return esp_matter::attribute::report(endpointId, clusterId, attributeId, &reported);
}

inline esp_err_t reportText(uint16_t endpointId, uint32_t clusterId, uint32_t attributeId, const char *value)
{
    if (!hasAttribute(endpointId, clusterId, attributeId))
        return ESP_ERR_NOT_FOUND;

    auto reported = textValue(value);
    return esp_matter::attribute::report(endpointId, clusterId, attributeId, &reported);
}

inline esp_err_t setDeviceName(esp_matter_bridge::device_t *device, const char *name)
{
    if (device == nullptr || name == nullptr)
        return ESP_ERR_INVALID_ARG;

    uint16_t endpointId = device->persistent_info.device_endpoint_id;
    if (hasAttribute(endpointId, bridgedDeviceBasicInformationClusterId, bridgedNodeLabelAttrId))
    {
        return reportText(endpointId, bridgedDeviceBasicInformationClusterId, bridgedNodeLabelAttrId, name);
    }
    if (hasAttribute(endpointId, basicInformationClusterId, nodeLabelAttrId))
    {
        return reportText(endpointId, basicInformationClusterId, nodeLabelAttrId, name);
    }

    return ESP_ERR_NOT_FOUND;
}

inline esp_err_t setEndpointName(uint16_t endpointId, const char *name)
{
    if (name == nullptr)
        return ESP_ERR_INVALID_ARG;

    return reportText(endpointId, basicInformationClusterId, nodeLabelAttrId, name);
}

inline esp_err_t setEndpointProductName(uint16_t endpointId, const char *name)
{
    if (name == nullptr)
        return ESP_ERR_INVALID_ARG;

    return reportText(endpointId, basicInformationClusterId, productNameAttrId, name);
}
}