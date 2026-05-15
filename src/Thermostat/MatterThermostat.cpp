#include "Thermostat/MatterThermostat.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include <esp_matter_bridge.h>

MatterThermostatBridge::MatterThermostatBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterThermostatBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

    _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                               esp_matter::endpoint::thermostat::get_device_type_id(),
                                               static_cast<MatterBridgeDeviceBase *>(this));
    if (_device != nullptr)
    {
        setCurrentTemperature(_currentTemperature);
        setTargetTemperature(_targetTemperature);
        setMode(_mode);
        setCurrentState(_currentState);
    }
}

void MatterThermostatBridge::setTargetTemperature(double temperature)
{
    _targetTemperature = temperature;
    if (_device == nullptr)
        return;

    matterbridge::reportI16(_device->persistent_info.device_endpoint_id, matterbridge::thermostatClusterId,
                            matterbridge::heatingSetpointAttrId,
                            static_cast<int16_t>(std::lround(temperature * 100.0)));
    matterbridge::reportI16(_device->persistent_info.device_endpoint_id, matterbridge::thermostatClusterId,
                            matterbridge::coolingSetpointAttrId,
                            static_cast<int16_t>(std::lround(temperature * 100.0)));
}

void MatterThermostatBridge::setCurrentTemperature(double temperature)
{
    _currentTemperature = temperature;
    if (_device == nullptr)
        return;

    matterbridge::reportI16(_device->persistent_info.device_endpoint_id, matterbridge::thermostatClusterId,
                            matterbridge::localTemperatureAttrId,
                            static_cast<int16_t>(std::lround(temperature * 100.0)));
}

void MatterThermostatBridge::setMode(ThermostatMode mode)
{
    _mode = mode;
    if (_device == nullptr)
        return;

    auto matterMode = chip::app::Clusters::Thermostat::SystemModeEnum::kOff;
    switch (mode)
    {
    case ThermostatModeHeating:
        matterMode = chip::app::Clusters::Thermostat::SystemModeEnum::kHeat;
        break;
    case ThermostatModeCooling:
        matterMode = chip::app::Clusters::Thermostat::SystemModeEnum::kCool;
        break;
    case ThermostatModeAutoHeatingCooling:
        matterMode = chip::app::Clusters::Thermostat::SystemModeEnum::kAuto;
        break;
    case ThermostatModeOff:
    default:
        matterMode = chip::app::Clusters::Thermostat::SystemModeEnum::kOff;
        break;
    }

    matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::thermostatClusterId,
                           matterbridge::systemModeAttrId, static_cast<uint8_t>(matterMode));
}

void MatterThermostatBridge::setCurrentState(ThermostatCurrentState currentState)
{
    _currentState = currentState;
    if (_device == nullptr)
        return;

    auto matterState = chip::app::Clusters::Thermostat::ThermostatRunningModeEnum::kOff;
    switch (currentState)
    {
    case ThermostatCurrentStateHeating:
        matterState = chip::app::Clusters::Thermostat::ThermostatRunningModeEnum::kHeat;
        break;
    case ThermostatCurrentStateCooling:
        matterState = chip::app::Clusters::Thermostat::ThermostatRunningModeEnum::kCool;
        break;
    case ThermostatCurrentStateOff:
    default:
        matterState = chip::app::Clusters::Thermostat::ThermostatRunningModeEnum::kOff;
        break;
    }

    matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::thermostatClusterId,
                           matterbridge::runningModeAttrId, static_cast<uint8_t>(matterState));
}

void MatterThermostatBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t clusterId,
                                                   uint32_t attributeId, esp_matter_attr_val_t *val)
{
    if (clusterId != matterbridge::thermostatClusterId)
        return;

    if (attributeId == matterbridge::heatingSetpointAttrId || attributeId == matterbridge::coolingSetpointAttrId)
    {
        _channel->commandTargetTemperature(this, static_cast<double>(val->val.i16) / 100.0);
    }
    else if (attributeId == matterbridge::systemModeAttrId)
    {
        switch (val->val.u8)
        {
        case static_cast<uint8_t>(chip::app::Clusters::Thermostat::SystemModeEnum::kHeat):
            _channel->commandMode(this, ThermostatModeHeating);
            break;
        case static_cast<uint8_t>(chip::app::Clusters::Thermostat::SystemModeEnum::kCool):
            _channel->commandMode(this, ThermostatModeCooling);
            break;
        case static_cast<uint8_t>(chip::app::Clusters::Thermostat::SystemModeEnum::kAuto):
            _channel->commandMode(this, ThermostatModeAutoHeatingCooling);
            break;
        default:
            _channel->commandMode(this, ThermostatModeOff);
            break;
        }
    }
}