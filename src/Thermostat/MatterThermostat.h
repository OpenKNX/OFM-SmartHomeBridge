#pragma once

#include "Matter/MatterBridgeDeviceBase.h"
#include "Thermostat/KnxChannelThermostat.h"

class MatterThermostatBridge final : public MatterBridgeDeviceBase, public ThermostatBridge
{
    double _targetTemperature = 21.0;
    double _currentTemperature = 21.0;
    ThermostatMode _mode = ThermostatModeOff;
    ThermostatCurrentState _currentState = ThermostatCurrentStateOff;

public:
    explicit MatterThermostatBridge(MatterBridge *bridge);

    void setup(uint8_t channelIndex) override;
    void setTargetTemperature(double temperature) override;
    void setCurrentTemperature(double temperature) override;
    void setMode(ThermostatMode mode) override;
    void setCurrentState(ThermostatCurrentState currentState) override;
    void handleMatterAttribute(esp_matter::attribute::callback_type_t type, uint32_t clusterId,
                               uint32_t attributeId, esp_matter_attr_val_t *val) override;
};