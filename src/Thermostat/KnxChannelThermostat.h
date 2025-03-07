#pragma once
#include "KnxChannelBase.h"

class KnxChannelThermostat;

enum ThermostatMode
{
    ThermostatModeOff,
    ThermostatModeHeating,
    ThermostatModeCooling,
    ThermostatModeAutoHeatingCooling,
};

enum ThermostatCurrentState
{
    ThermostatCurrentStateOff,
    ThermostatCurrentStateHeating,
    ThermostatCurrentStateCooling,
};

enum ThermostatDisplayUnit
{
    ThermostatDisplayUnitCelsius,
    ThermostatDisplayUnitFahrenheit,
};

class ThermostatBridge : public ChannelBridgeBase<KnxChannelThermostat>
{
public:
    virtual void setTargetTemperature(double temperature) = 0;
    virtual void setCurrentTemperature(double temperature) = 0;
    virtual void setMode(ThermostatMode mode) = 0;
    virtual void setCurrentState(ThermostatCurrentState currentState) = 0;
};

class KnxChannelThermostat : public KnxChannelBase
{
        DynamicPointerArray<ThermostatBridge> thermostatBridges;
    public:
        static float DEFAULT_TEMPERATURE;
        KnxChannelThermostat(uint16_t channelIndex);
        void add(ThermostatBridge* thermostatBridge);
        void remove(ThermostatBridge* thermostatBridge);
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;
    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandTargetTemperature(ThermostatBridge* thermostatBridge, double temperature);
        
        bool commandMode(ThermostatBridge* thermostatBridge, ThermostatMode mode);
        virtual void commandMainFunctionClick() override;
         
        ThermostatDisplayUnit GetDisplayTemperaturUnit();
        virtual const std::string name() override;
};
