#include <knx.h>
#include "knxprod.h"
#include "KnxChannelThermostat.h"
#include "BridgeBase.h"

#define KO_TARGET_TEMPERATURE              KoBRI_KO1_, DPT_Value_Temp
#define KO_TARGET_TEMPERATURE_FEEDBACK     KoBRI_KO2_, DPT_Value_Temp
#define KO_CURRENT_TEMPERATUR_FEEDBACK     KoBRI_KO3_, DPT_Value_Temp
#define KO_HEADING                         KoBRI_KO4_, DPT_Switch
#define KO_HEADING_FEEDBACK                KoBRI_KO5_, DPT_Switch
#define KO_HEADING_ACTIVE_FEEDBACK         KoBRI_KO6_, DPT_Switch
#define KO_HEADING_ACTIVE_PERCENT_FEEDBACK KoBRI_KO6_, DPT_Scaling
#define KO_COOLING                         KoBRI_KO7_, DPT_Switch
#define KO_COOLING_FEEDBACK                KoBRI_KO8_, DPT_Switch
#define KO_COOLING_ACTIVE_FEEDBACK         KoBRI_KO9_, DPT_Switch
#define KO_COOLING_ACTIVE_PERCENT_FEEDBACK KoBRI_KO9_, DPT_Scaling


float KnxChannelThermostat::DEFAULT_TEMPERATURE = 22;

KnxChannelThermostat::KnxChannelThermostat(uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      thermostatBridges()
{
}

ChannelBridge* KnxChannelThermostat::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createThermostat(*this, _channelIndex, ParamBRI_CHDeviceType);
}

void KnxChannelThermostat::deleteBridgeDevice(ChannelBridge *device)
{
    remove((ThermostatBridge *)device);
}

void KnxChannelThermostat::add(ThermostatBridge* thermostatBridge)
{
    thermostatBridges.push_back(thermostatBridge);
    thermostatBridge->initialize(this);
    thermostatBridge->setCurrentTemperature(koGet(KO_CURRENT_TEMPERATUR_FEEDBACK));
    thermostatBridge->setTargetTemperature(koGet(KO_TARGET_TEMPERATURE_FEEDBACK));
    updateBridgeFromKo(KO_HEADING_FEEDBACK, thermostatBridge);
    updateBridgeFromKo(KO_COOLING_FEEDBACK, thermostatBridge);
    updateBridgeFromKo(KO_HEADING_ACTIVE_FEEDBACK, thermostatBridge);
    updateBridgeFromKo(KO_COOLING_ACTIVE_FEEDBACK, thermostatBridge);
    updateBridgeFromKo(KO_HEADING_ACTIVE_PERCENT_FEEDBACK, thermostatBridge);
    updateBridgeFromKo(KO_COOLING_ACTIVE_PERCENT_FEEDBACK, thermostatBridge);
}    

void KnxChannelThermostat::remove(ThermostatBridge* thermostatBridge)
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
    return (ThermostatDisplayUnit) ParamBRI_CHThermostatTemperaturUnitType;
}

void KnxChannelThermostat::commandTargetTemperature(ThermostatBridge* thermostatBridge, double temperature)
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


bool KnxChannelThermostat::commandMode(ThermostatBridge* thermostatBridge, ThermostatMode mode)
{
    logDebugP("Received changed. Mode %d", mode);

    bool heading = false;
    bool cooling = false;
    switch (mode)
    {
        case ThermostatMode::ThermostatModeOff:
            heading = false;
            cooling = false;
            break;
        case ThermostatMode::ThermostatModeHeating:
            if (ParamBRI_CHThermostatMode & 1 == 0)
                return false; 
            heading = true;
            cooling = false;
            break;
        case ThermostatMode::ThermostatModeCooling:
            if (ParamBRI_CHThermostatMode & 2 == 0)
                return false; 
            heading = false;
            cooling = true;
            break;
        case ThermostatMode::ThermostatModeAutoHeatingCooling:
            if (ParamBRI_CHThermostatMode & 3 == 0)
                return false; 
            heading = true;
            cooling = true;
            break;
    }

    // Handle Switch or lock mode to set KO
    if (ParamBRI_CHThermostatKoModeHeating == 0)
        koSet(KO_HEADING, heading, true);
    else
        koSet(KO_HEADING, !heading, true);

    if (ParamBRI_CHThermostatKoModeHeating == 0)
        koSet(KO_COOLING, heading, true);
    else
        koSet(KO_COOLING, !heading, true);
    
    // Inform other bridges
    for (auto it = thermostatBridges.begin(); it != thermostatBridges.end(); ++it)
    {
        if ((*it) != thermostatBridge)
        {
            (*it)->setMode(mode);
        }
    }
    mainFunctionValueChanged();
    return true;
}

void KnxChannelThermostat::commandMainFunctionClick()
{
    // <Enumeration Value="7" Id="%ENID%" Text="Heizen und Kühlen Automatik"             />
    // <Enumeration Value="3" Id="%ENID%" Text="Heizen oder Kühlen (Manuelle Auswahl)"   />
    // <Enumeration Value="1" Id="%ENID%" Text="Heizen"                                  />
    // <Enumeration Value="2" Id="%ENID%" Text="Kühlen"
    switch (ParamBRI_CHThermostatMode)
    {
        case 7:
            if (koGet(KO_HEADING_FEEDBACK) || koGet(KO_COOLING_FEEDBACK))
                commandMode(nullptr, ThermostatMode::ThermostatModeOff);
            else
                commandMode(nullptr, ThermostatMode::ThermostatModeAutoHeatingCooling);
        break;

        case 3:
            if (koGet(KO_HEADING_FEEDBACK))
                commandMode(nullptr, ThermostatMode::ThermostatModeCooling);
            else if (koGet(KO_COOLING_FEEDBACK))
                commandMode(nullptr, ThermostatMode::ThermostatModeOff);
            else
                commandMode(nullptr, ThermostatMode::ThermostatModeHeating);
        break;

        case 1:
            if (koGet(KO_HEADING_FEEDBACK))
                commandMode(nullptr, ThermostatMode::ThermostatModeOff);
            else
                commandMode(nullptr, ThermostatMode::ThermostatModeHeating);
        break;

        case 2:
            if (koGet(KO_COOLING_FEEDBACK))
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
    koSetWithoutSend(KO_HEADING_FEEDBACK, ParamBRI_CHThermostatKoModeHeatingFeedback == 1);
    koSendReadRequest(KO_HEADING_FEEDBACK);
    if (ParamBRI_CHThemostateHeatingFeedbackKoType == 0)
    {
        koSetWithoutSend(KO_HEADING_ACTIVE_FEEDBACK, false);
        koSendReadRequest(KO_HEADING_ACTIVE_FEEDBACK);
    }
    else
    {
        koSetWithoutSend(KO_HEADING_ACTIVE_PERCENT_FEEDBACK, false);
        koSendReadRequest(KO_HEADING_ACTIVE_PERCENT_FEEDBACK);
    }
    koSetWithoutSend(KO_COOLING_FEEDBACK, ParamBRI_CHThermostatKoModeCoolingFeedback == 1);
    koSendReadRequest(KO_COOLING_FEEDBACK);
    if (ParamBRI_CHThemostateCoolingFeedbackKoType == 0)
    {
        koSetWithoutSend(KO_COOLING_ACTIVE_FEEDBACK, false);
        koSendReadRequest(KO_COOLING_ACTIVE_FEEDBACK);
    }
    else
    {
        koSetWithoutSend(KO_COOLING_ACTIVE_PERCENT_FEEDBACK, false);
        koSendReadRequest(KO_COOLING_ACTIVE_PERCENT_FEEDBACK);
    }
}

void KnxChannelThermostat::processInputKo(GroupObject &ko)
{
    updateBridgeFromKo(ko, nullptr);
}

void KnxChannelThermostat::updateBridgeFromKo(GroupObject &ko, Dpt dummy, ThermostatBridge* thermostatBridge)
{
    updateBridgeFromKo(ko, thermostatBridge);
}

void KnxChannelThermostat::updateBridgeFromKo(GroupObject &ko, ThermostatBridge* thermostatBridge)
{
    if (isKo(ko, KO_TARGET_TEMPERATURE_FEEDBACK))
    {
        double temperature = koGet(KO_TARGET_TEMPERATURE_FEEDBACK);
        logDebugP("Received ko target temperature: %f", temperature);

        if (thermostatBridge != nullptr)
            thermostatBridge->setTargetTemperature(temperature);
        else
        {
            for (auto it = thermostatBridges.begin(); it != thermostatBridges.end(); ++it)
            {
                (*it)->setTargetTemperature(temperature);
            }
            mainFunctionValueChanged();
        }
    }
    else if (isKo(ko, KO_CURRENT_TEMPERATUR_FEEDBACK))
    {
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
            mainFunctionValueChanged();
        }
    }
    else if (isKo(KoBRI_KO1_, KO_COOLING_FEEDBACK) || isKo(ko, KO_HEADING_FEEDBACK))
    {
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
 
        if (thermostatBridge != nullptr)
            thermostatBridge->setMode(mode);
        else
        {
            for (auto it = thermostatBridges.begin(); it != thermostatBridges.end(); ++it)
            {
                (*it)->setMode(mode);
            }
            mainFunctionValueChanged();
        }
    }
    else if (isKo(ko, KO_HEADING_ACTIVE_FEEDBACK) || isKo(ko, KO_COOLING_ACTIVE_FEEDBACK))
    {
        bool heading = ParamBRI_CHThemostateHeatingFeedbackKoType == 0 ? (boolean) koGet(KO_HEADING_ACTIVE_FEEDBACK) : 0 < (uint8_t)koGet(KO_HEADING_ACTIVE_PERCENT_FEEDBACK);
        if (heading && isKo(ko, KO_HEADING_ACTIVE_FEEDBACK))
            koSetWithoutSend(KO_COOLING_FEEDBACK, false);
        bool cooling = ParamBRI_CHThemostateCoolingFeedbackKoType == 0 ? (boolean) koGet(KO_COOLING_ACTIVE_FEEDBACK) : 0 < (uint8_t)koGet(KO_COOLING_ACTIVE_PERCENT_FEEDBACK);
        if (cooling && isKo(ko, KO_COOLING_ACTIVE_FEEDBACK))
            koSetWithoutSend(KO_HEADING_FEEDBACK, false);
        
        ThermostatCurrentState state = ThermostatCurrentState::ThermostatCurrentStateOff;
        if (heading)
            state = ThermostatCurrentState::ThermostatCurrentStateHeating;
        if (cooling)
            state = ThermostatCurrentState::ThermostatCurrentStateCooling;

        if (thermostatBridge != nullptr)
            thermostatBridge->setCurrentState(state);
        else
        {
            for (auto it = thermostatBridges.begin(); it != thermostatBridges.end(); ++it)
            {
                (*it)->setCurrentState(state);
            }
            mainFunctionValueChanged();
        }
    }
}

std::string KnxChannelThermostat::currentValueAsString()
{
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%0.1f°", (double) koGet(KO_TARGET_TEMPERATURE_FEEDBACK));
    return std::string(buffer);
}

bool KnxChannelThermostat::mainFunctionValue()
{
    return koGet(KO_HEADING_FEEDBACK) || koGet(KO_COOLING_FEEDBACK);
}

MainFunctionStateImage KnxChannelThermostat::mainFunctionImage()
{
    if (koGet(KO_HEADING_FEEDBACK))
        return calculateMainFunctionImage(100, LIMIT_NOT_USED, 100, LIMIT_NOT_USED);
    if (koGet(KO_COOLING_FEEDBACK))
        return calculateMainFunctionImage(100, LIMIT_NOT_USED, LIMIT_NOT_USED, 100);
   return calculateMainFunctionImage(0, 0, LIMIT_NOT_USED, LIMIT_NOT_USED);
}
