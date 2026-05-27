#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

#include "KnxChannelThermostat.h"
#include "../WebVisuWidgetBase.h"

class WebVisuBridge;

class WebVisuThermostat : public ThermostatBridge, public WebVisuWidgetBase
{
public:
    explicit WebVisuThermostat(WebVisuBridge* webVisuBridge);

    virtual void setWebVisuName(const std::string& name) override;

    virtual void setTargetTemperature(double temperature) override;
    virtual void setCurrentTemperature(double temperature) override;
    virtual void setMode(ThermostatMode mode) override;
    virtual void setCurrentState(ThermostatCurrentState currentState) override;

    virtual std::string webVisuKind() const override;
    virtual std::string webVisuOverviewHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuDetailHtml(uint8_t channelIndex) const override;
    virtual std::string webVisuJson(uint8_t channelIndex) const override;
    virtual bool webVisuHandleCommand(const std::string& action, const std::string& message) override;

private:
    std::string _name = "Unbenannt";
    double _targetTemperature = 0;
    double _currentTemperature = 0;
    ThermostatMode _mode = ThermostatModeOff;
    ThermostatCurrentState _currentState = ThermostatCurrentStateOff;
    WebVisuBridge* _webVisuBridge;
};

#endif