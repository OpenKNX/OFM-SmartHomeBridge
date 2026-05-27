#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuThermostat.h"

#include <cstdlib>

#include "../WebVisuBridge.h"

WebVisuThermostat::WebVisuThermostat(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuThermostat::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuThermostat::webVisuKind() const
{
    return "thermostat";
}

void WebVisuThermostat::setTargetTemperature(double temperature)
{
    _targetTemperature = temperature;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

void WebVisuThermostat::setCurrentTemperature(double temperature)
{
    _currentTemperature = temperature;
}

void WebVisuThermostat::setMode(ThermostatMode mode)
{
    _mode = mode;
}

void WebVisuThermostat::setCurrentState(ThermostatCurrentState currentState)
{
    _currentState = currentState;
}

std::string WebVisuThermostat::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuThermostat::webVisuDetailHtml(uint8_t channelIndex) const
{
    const int channelOneBased = (int)channelIndex + 1;
    std::string controls;
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"stepThermostatTarget\",\"channel\":" + std::to_string(channelOneBased) + ",\"delta\":-0.5}'>-</button>";
    controls += "<input class=\"webvisu-slider\" type=\"number\" step=\"0.5\" data-wv-payload-template='{";
    controls += "\"action\":\"setThermostatTarget\",\"channel\":__CHANNEL__,\"target\":__VALUE__}' data-channel=\"" + std::to_string(channelOneBased) + "\" value=\"" + std::to_string(_targetTemperature) + "\">";
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"stepThermostatTarget\",\"channel\":" + std::to_string(channelOneBased) + ",\"delta\":0.5}'>+</button>";
    controls += "<select data-wv-payload-template='{";
    controls += "\"action\":\"setThermostatMode\",\"channel\":__CHANNEL__,\"mode\":\"__VALUE__\"}' data-channel=\"" + std::to_string(channelOneBased) + "\">";
    controls += "<option value=\"0\"" + std::string(_mode == ThermostatModeOff ? " selected" : "") + ">Aus</option>";
    controls += "<option value=\"1\"" + std::string(_mode == ThermostatModeHeating ? " selected" : "") + ">Heizen</option>";
    controls += "<option value=\"2\"" + std::string(_mode == ThermostatModeCooling ? " selected" : "") + ">Kühlen</option>";
    controls += "<option value=\"3\"" + std::string(_mode == ThermostatModeAutoHeatingCooling ? " selected" : "") + ">Auto</option>";
    controls += "</select>";
    return renderCard(channelIndex, _name, "Thermostat", std::to_string(_currentTemperature) + " / " + std::to_string(_targetTemperature), controls);
}

std::string WebVisuThermostat::webVisuJson(uint8_t channelIndex) const
{
    std::string json = "{";
    json += "\"kind\":\"thermostat\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"target\":" + std::to_string(_targetTemperature) + ",";
    json += "\"current\":" + std::to_string(_currentTemperature) + ",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

bool WebVisuThermostat::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    if (_channel == nullptr)
    {
        return false;
    }

    if (action == "stepThermostatTarget")
    {
        double delta = 0;
        if (!parseDoubleField(message, "delta", delta))
        {
            return false;
        }
        _channel->commandTargetTemperature(this, _targetTemperature + delta);
        return true;
    }

    if (action == "setThermostatTarget")
    {
        double target = 0;
        if (!parseDoubleField(message, "target", target))
        {
            return false;
        }
        _channel->commandTargetTemperature(this, target);
        return true;
    }

    if (action == "setThermostatMode")
    {
        std::string mode;
        if (!parseStringField(message, "mode", mode))
        {
            return false;
        }
        _channel->commandMode(this, (ThermostatMode)std::atoi(mode.c_str()));
        return true;
    }

    return false;
}

#endif