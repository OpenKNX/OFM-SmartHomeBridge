#include <knx.h>
#include "knxprod.h"
#include "KnxChannelThermostat.h"
#include "BridgeBase.h"

#define KO_TARGET_TEMPERATURE KoBRI_KO1_, DPT_Value_Temp
#define KO_TARGET_TEMPERATURE_FEEDBACK KoBRI_KO2_, DPT_Value_Temp
#define KO_CURRENT_TEMPERATUR_FEEDBACK KoBRI_KO3_, DPT_Value_Temp
#define KO_HVAC KoBRI_KO4_, DPT_Value_1_Ucount /*DPT_HVACContrMode currently not supported*/
#define KO_HEADING KoBRI_KO4_, DPT_Switch
#define KO_HVAC_FEEDBACK KoBRI_KO5_, DPT_Value_1_Ucount /*DPT_HVACContrMode currently not supported*/
#define KO_HEADING_FEEDBACK KoBRI_KO5_, DPT_Switch
#define KO_HEADING_ACTIVE_FEEDBACK KoBRI_KO6_, DPT_Switch
#define KO_HEADING_ACTIVE_PERCENT_FEEDBACK KoBRI_KO6_, DPT_Scaling
#define KO_COOLING KoBRI_KO7_, DPT_Switch
#define KO_COOLING_FEEDBACK KoBRI_KO8_, DPT_Switch
#define KO_COOLING_ACTIVE_FEEDBACK KoBRI_KO9_, DPT_Switch
#define KO_COOLING_ACTIVE_PERCENT_FEEDBACK KoBRI_KO9_, DPT_Scaling

float KnxChannelThermostat::DEFAULT_TEMPERATURE = 22;

KnxChannelThermostat::KnxChannelThermostat(uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      thermostatBridges()
{
}

ChannelBridge *KnxChannelThermostat::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createThermostat(*this, _channelIndex, ParamBRI_CHDeviceType);
}

void KnxChannelThermostat::deleteBridgeDevice(ChannelBridge *device)
{
    remove((ThermostatBridge *)device);
}

void KnxChannelThermostat::add(ThermostatBridge *thermostatBridge)
{
    thermostatBridges.push_back(thermostatBridge);
    thermostatBridge->initialize(this);
    thermostatBridge->setCurrentTemperature(koGet(KO_CURRENT_TEMPERATUR_FEEDBACK));
    thermostatBridge->setTargetTemperature(koGet(KO_TARGET_TEMPERATURE_FEEDBACK));
    if (ParamBRI_CHThermostatFeedbackKoType == 0)
    {
        // Bit KO
        updateBridgeFromKo(KO_HEADING_FEEDBACK, thermostatBridge);
        updateBridgeFromKo(KO_COOLING_FEEDBACK, thermostatBridge);
    }
    else
    {
        // HVAC KO
        updateBridgeFromKo(KO_HVAC_FEEDBACK, thermostatBridge);
    }
    if (ParamBRI_CHThemostateHeatingFeedbackKoType == 0)
        updateBridgeFromKo(KO_HEADING_ACTIVE_FEEDBACK, thermostatBridge);
    else if (ParamBRI_CHThemostateHeatingFeedbackKoType == 1)
        updateBridgeFromKo(KO_HEADING_ACTIVE_PERCENT_FEEDBACK, thermostatBridge);
    if (ParamBRI_CHThemostateCoolingFeedbackKoType == 0)
        updateBridgeFromKo(KO_COOLING_ACTIVE_FEEDBACK, thermostatBridge);
    else if (ParamBRI_CHThemostateCoolingFeedbackKoType == 1)
        updateBridgeFromKo(KO_COOLING_ACTIVE_PERCENT_FEEDBACK, thermostatBridge);
}

void KnxChannelThermostat::remove(ThermostatBridge *thermostatBridge)
{
    thermostatBridges.remove(thermostatBridge);
    delete thermostatBridge;
}

const std::string KnxChannelThermostat::name()
{
    return std::string("Thermostat");
}

ThermostatDisplayUnit KnxChannelThermostat::GetDisplayTemperaturUnit()
{
    return (ThermostatDisplayUnit)ParamBRI_CHThermostatTemperaturUnitType;
}

void KnxChannelThermostat::commandTargetTemperature(ThermostatBridge *thermostatBridge, double temperature)
{
    logDebugP("Received changed. Temperature %f", temperature);

    for (auto it = thermostatBridges.begin(); it != thermostatBridges.end(); ++it)
    {
        if ((*it) != thermostatBridge)
        {
            (*it)->setCurrentTemperature(temperature);
        }
    }
    koSet(KO_TARGET_TEMPERATURE, temperature, true);
}

bool KnxChannelThermostat::commandMode(ThermostatBridge *thermostatBridge, ThermostatMode mode)
{
    logDebugP("Received changed. Mode %d", mode);
    _currentMode = mode;
    bool heading = false;
    bool cooling = false;
    ;
    switch (mode)
    {
    case ThermostatMode::ThermostatModeOff:
        heading = false;
        cooling = false;
        break;
    case ThermostatMode::ThermostatModeHeating:
        if ((ParamBRI_CHThermostatMode & 1) == 0)
            return false;
        heading = true;
        cooling = false;
        break;
    case ThermostatMode::ThermostatModeCooling:
        if ((ParamBRI_CHThermostatMode & 2) == 0)
            return false;
        heading = false;
        cooling = true;
        break;
    case ThermostatMode::ThermostatModeAutoHeatingCooling:
        if ((ParamBRI_CHThermostatMode & 3) == 0)
            return false;
        heading = true;
        cooling = true;
        break;
    }

    if (ParamBRI_CHThermostatKoType == 0)
    {
        // Bit KO control
        // Handle Switch or lock mode to set KO
        if (ParamBRI_CHThermostatKoModeHeating == 0)
            koSet(KO_HEADING, heading, true);
        else
            koSet(KO_HEADING, !heading, true);

        if (ParamBRI_CHThermostatKoModeCooling == 0)
            koSet(KO_COOLING, cooling, true);
        else
            koSet(KO_COOLING, !cooling, true);
    }
    else
    {
        // HVAC KO control
        if (heading && cooling)
            koSet(KO_HVAC, (uint8_t)0, true); // Auto
        else if (heading)
            koSet(KO_HVAC, (uint8_t)1, true); // Heating
        else if (cooling)
            koSet(KO_HVAC, (uint8_t)3, true); // Cooling
        else
            koSet(KO_HVAC, (uint8_t)6, true); // Off
    }

    // Inform other bridges
    for (auto it = thermostatBridges.begin(); it != thermostatBridges.end(); ++it)
    {
        if ((*it) != thermostatBridge)
        {
            (*it)->setMode(mode);
            (*it)->mainFunctionValueChanged();
        }
    }
    mainFunctionValueChanged();
    updateCurrentState();
    return true;
}

void KnxChannelThermostat::commandMainFunctionClick()
{
    // <Enumeration Value="7" Id="%ENID%" Text="Heizen, Kühlen, Automatik"             />
    // <Enumeration Value="3" Id="%ENID%" Text="Heizen oder Kühlen (Manuelle Auswahl)"   />
    // <Enumeration Value="1" Id="%ENID%" Text="Heizen"                                  />
    // <Enumeration Value="2" Id="%ENID%" Text="Kühlen"
    switch (ParamBRI_CHThermostatMode)
    {
    case 7:

        if (_currentMode == ThermostatMode::ThermostatModeOff)
            commandMode(nullptr, ThermostatMode::ThermostatModeHeating);
        else if (_currentMode == ThermostatMode::ThermostatModeHeating)
            commandMode(nullptr, ThermostatMode::ThermostatModeCooling);
        else if (_currentMode == ThermostatMode::ThermostatModeCooling)
            commandMode(nullptr, ThermostatMode::ThermostatModeAutoHeatingCooling);
        else if (_currentMode == ThermostatMode::ThermostatModeAutoHeatingCooling)
            commandMode(nullptr, ThermostatMode::ThermostatModeOff);
        break;

    case 3:
        if (_currentMode == ThermostatMode::ThermostatModeOff)
            commandMode(nullptr, ThermostatMode::ThermostatModeHeating);
        else if (_currentMode == ThermostatMode::ThermostatModeHeating)
            commandMode(nullptr, ThermostatMode::ThermostatModeCooling);
        else if (_currentMode == ThermostatMode::ThermostatModeCooling)
            commandMode(nullptr, ThermostatMode::ThermostatModeOff);
        break;

    case 1:
        if (_currentMode == ThermostatMode::ThermostatModeHeating)
            commandMode(nullptr, ThermostatMode::ThermostatModeOff);
        else
            commandMode(nullptr, ThermostatMode::ThermostatModeHeating);
        break;

    case 2:
        if (_currentMode == ThermostatMode::ThermostatModeCooling)
            commandMode(nullptr, ThermostatMode::ThermostatModeOff);
        else
            commandMode(nullptr, ThermostatMode::ThermostatModeCooling);
        break;
    }
}

void KnxChannelThermostat::setup()
{
    koSetWithoutSend(KO_CURRENT_TEMPERATUR_FEEDBACK, DEFAULT_TEMPERATURE);
    koSendReadRequest(KO_CURRENT_TEMPERATUR_FEEDBACK);
    koSetWithoutSend(KO_TARGET_TEMPERATURE_FEEDBACK, DEFAULT_TEMPERATURE);
    koSendReadRequest(KO_TARGET_TEMPERATURE_FEEDBACK);
    if (ParamBRI_CHThermostatFeedbackKoType == 1) // HVAC KO
    {
        koSetWithoutSend(KO_HVAC_FEEDBACK, (uint8_t)6); // Off
        koSendReadRequest(KO_HVAC_FEEDBACK);
    }
    if ((ParamBRI_CHThermostatMode & 1) == 0) // Heading Bit
    {
        // Heating enabled
        if (ParamBRI_CHThermostatFeedbackKoType == 0) // Bit KO
        {
            koSetWithoutSend(KO_HEADING_FEEDBACK, ParamBRI_CHThermostatKoModeHeatingFeedback == 1);
            koSendReadRequest(KO_HEADING_FEEDBACK);
        }
        switch (ParamBRI_CHThemostateHeatingFeedbackKoType)
        {
        case 0:
            koSetWithoutSend(KO_HEADING_ACTIVE_FEEDBACK, false);
            koSendReadRequest(KO_HEADING_ACTIVE_FEEDBACK);
            break;
        case 1:
            koSetWithoutSend(KO_HEADING_ACTIVE_PERCENT_FEEDBACK, false);
            koSendReadRequest(KO_HEADING_ACTIVE_PERCENT_FEEDBACK);
            break;
        }
    }
    if ((ParamBRI_CHThermostatMode & 2) == 0) // Cooling Bit
    {
        // Cooling enabled
        if (ParamBRI_CHThermostatFeedbackKoType == 0) // Bit KO
        {
            koSetWithoutSend(KO_COOLING_FEEDBACK, ParamBRI_CHThermostatKoModeCoolingFeedback == 1);
            koSendReadRequest(KO_COOLING_FEEDBACK);
        }
        switch (ParamBRI_CHThemostateCoolingFeedbackKoType)
        {
        case 0:
            koSetWithoutSend(KO_COOLING_ACTIVE_FEEDBACK, false);
            koSendReadRequest(KO_COOLING_ACTIVE_FEEDBACK);
            break;
        case 1:
            koSetWithoutSend(KO_COOLING_ACTIVE_PERCENT_FEEDBACK, false);
            koSendReadRequest(KO_COOLING_ACTIVE_PERCENT_FEEDBACK);
            break;
        }
    }
}

void KnxChannelThermostat::processInputKo(GroupObject &ko)
{
    updateBridgeFromKo(ko, nullptr);
}

void KnxChannelThermostat::updateBridgeFromKo(GroupObject &ko, Dpt dummy, ThermostatBridge *thermostatBridge)
{
    updateBridgeFromKo(ko, thermostatBridge);
}

void KnxChannelThermostat::updateBridgeFromKo(GroupObject &ko, ThermostatBridge *thermostatBridge)
{
    if (isKo(ko, KO_TARGET_TEMPERATURE_FEEDBACK))
    {
        // update target temperature
        double temperature = koGet(KO_TARGET_TEMPERATURE_FEEDBACK);
        logDebugP("Received ko target temperature: %f", temperature);

        if (thermostatBridge != nullptr)
            thermostatBridge->setTargetTemperature(temperature);
        else
        {
            for (auto it = thermostatBridges.begin(); it != thermostatBridges.end(); ++it)
            {
                (*it)->setTargetTemperature(temperature);
                (*it)->mainFunctionValueChanged();
            }
            mainFunctionValueChanged();
        }
    }
    else if (isKo(ko, KO_CURRENT_TEMPERATUR_FEEDBACK))
    {
        // update current temperature
        double temperature = koGet(KO_CURRENT_TEMPERATUR_FEEDBACK);
        logDebugP("Received ko current temperature: %f", temperature);

        if (thermostatBridge != nullptr)
            thermostatBridge->setCurrentTemperature(temperature);
        else
        {
            for (auto it = thermostatBridges.begin(); it != thermostatBridges.end(); ++it)
            {
                (*it)->setCurrentTemperature(temperature);
            }
        }
    }
    else if (ParamBRI_CHThermostatFeedbackKoType == 0 && (isKo(ko, KO_COOLING_FEEDBACK) || isKo(ko, KO_HEADING_FEEDBACK)))
    {
        // Update current mode
        bool heading = koGet(KO_HEADING_FEEDBACK);
        if ((bool)ParamBRI_CHThermostatKoModeHeatingFeedback)
            heading = !heading;
        bool cooling = koGet(KO_COOLING_FEEDBACK);
        if ((bool)ParamBRI_CHThermostatKoModeCoolingFeedback)
            cooling = !cooling;

        ThermostatMode mode = ThermostatMode::ThermostatModeOff;
        if (cooling && heading)
            mode = ThermostatMode::ThermostatModeAutoHeatingCooling;
        else if (cooling)
            mode = ThermostatMode::ThermostatModeCooling;
        else if (heading)
            mode = ThermostatMode::ThermostatModeHeating;

        _currentMode = mode;
        if (thermostatBridge != nullptr)
            thermostatBridge->setMode(mode);
        else
        {
            for (auto it = thermostatBridges.begin(); it != thermostatBridges.end(); ++it)
            {
                (*it)->setMode(mode);
                (*it)->mainFunctionValueChanged();
            }
            mainFunctionValueChanged();
        }
    }
    else if (ParamBRI_CHThermostatFeedbackKoType == 1 && (isKo(ko, KO_HVAC_FEEDBACK)))
    {

        // Update current mode
        uint8_t hvac = koGet(KO_HVAC_FEEDBACK);
        ThermostatMode mode = ThermostatMode::ThermostatModeOff;
        switch (hvac)
        {
        case 0: // Auto
            mode = ThermostatMode::ThermostatModeAutoHeatingCooling;
            break;
        case 1: // Heating
            mode = ThermostatMode::ThermostatModeHeating;
            break;
        case 3: // Cooling
            mode = ThermostatMode::ThermostatModeCooling;
            break;
        }

        _currentMode = mode;
        updateCurrentState();
        logDebugP("Receive current mode: %d", (int)_currentMode);
        if (thermostatBridge != nullptr)
            thermostatBridge->setMode(mode);
        else
        {
            for (auto it = thermostatBridges.begin(); it != thermostatBridges.end(); ++it)
            {
                (*it)->setMode(mode);
                (*it)->mainFunctionValueChanged();
            }
            mainFunctionValueChanged();
        }
    }
    if ((ParamBRI_CHThemostateHeatingFeedbackKoType != 2 && isKo(ko, KO_HEADING_ACTIVE_FEEDBACK)) ||
        (ParamBRI_CHThemostateCoolingFeedbackKoType != 2 && isKo(ko, KO_COOLING_ACTIVE_FEEDBACK)) ||
        ((ParamBRI_CHThemostateHeatingFeedbackKoType == 2 || ParamBRI_CHThemostateCoolingFeedbackKoType == 2) && (isKo(ko, KO_TARGET_TEMPERATURE_FEEDBACK) || isKo(ko, KO_CURRENT_TEMPERATUR_FEEDBACK))))
    {
        updateCurrentState();
    }
}

void KnxChannelThermostat::updateCurrentState()
{
    // Update current state
    bool heading = isHeatingActive();
    bool cooling = isCoolingActive();

    ThermostatCurrentState state = ThermostatCurrentState::ThermostatCurrentStateOff;
    if (heading)
        state = ThermostatCurrentState::ThermostatCurrentStateHeating;
    if (cooling)
        state = ThermostatCurrentState::ThermostatCurrentStateCooling;
    if (_currentState == state)
        return;
    _currentState = state;
  
    for (auto it = thermostatBridges.begin(); it != thermostatBridges.end(); ++it)
    {
        (*it)->setCurrentState(state);
        (*it)->mainFunctionValueChanged();
    }
    mainFunctionValueChanged();
    
}

std::string KnxChannelThermostat::currentValueAsString()
{
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%0.1f°", (double)koGet(KO_TARGET_TEMPERATURE_FEEDBACK));
    return std::string(buffer);
}

bool KnxChannelThermostat::mainFunctionValue()
{
    return _currentState != ThermostatCurrentState::ThermostatCurrentStateOff;
}

bool KnxChannelThermostat::isHeatingActive()
{
    if ((ParamBRI_CHThermostatMode & 1) == 0) // Check heating bit
        return false;
    if (_currentMode != ThermostatMode::ThermostatModeHeating && _currentMode != ThermostatMode::ThermostatModeAutoHeatingCooling)
        return false;
    switch (ParamBRI_CHThemostateHeatingFeedbackKoType)
    {
    case 0:
        return (boolean)koGet(KO_HEADING_ACTIVE_FEEDBACK);
    case 1:
        return 0 < (uint8_t)koGet(KO_HEADING_ACTIVE_PERCENT_FEEDBACK);
    default:
        float currentTemperature = koGet(KO_CURRENT_TEMPERATUR_FEEDBACK);
        float targetTemperature = koGet(KO_TARGET_TEMPERATURE_FEEDBACK);
        return (targetTemperature - currentTemperature >= 0.5);
    }
}

bool KnxChannelThermostat::isCoolingActive()
{
    if ((ParamBRI_CHThermostatMode & 2) == 0) // Check cooling bit
        return false;
    if (_currentMode != ThermostatMode::ThermostatModeCooling && _currentMode != ThermostatMode::ThermostatModeAutoHeatingCooling)
        return false;
    switch (ParamBRI_CHThemostateCoolingFeedbackKoType)
    {
    case 0:
        return (boolean)koGet(KO_COOLING_ACTIVE_FEEDBACK);
    case 1:
        return 0 < (uint8_t)koGet(KO_COOLING_ACTIVE_PERCENT_FEEDBACK);
    default:
        float currentTemperature = koGet(KO_CURRENT_TEMPERATUR_FEEDBACK);
        float targetTemperature = koGet(KO_TARGET_TEMPERATURE_FEEDBACK);
        bool result = (currentTemperature - targetTemperature >= 0.5);
        return result;
    }
}

MainFunctionStateImage KnxChannelThermostat::mainFunctionImage()
{
    switch (_currentMode)
    {
    case ThermostatMode::ThermostatModeHeating:
        return calculateMainFunctionImage(100, LIMIT_NOT_USED, 100, LIMIT_NOT_USED);
    case ThermostatMode::ThermostatModeCooling:
        return calculateMainFunctionImage(100, LIMIT_NOT_USED, LIMIT_NOT_USED, 100);
    case ThermostatMode::ThermostatModeAutoHeatingCooling:
        if (_currentState == ThermostatCurrentState::ThermostatCurrentStateHeating)
            return calculateMainFunctionImage(100, LIMIT_NOT_USED, 100, LIMIT_NOT_USED);
        else if (_currentState == ThermostatCurrentState::ThermostatCurrentStateCooling)
            return calculateMainFunctionImage(100, LIMIT_NOT_USED, LIMIT_NOT_USED, 100);
    }
    return calculateMainFunctionImage(0, 0, LIMIT_NOT_USED, LIMIT_NOT_USED);
}
