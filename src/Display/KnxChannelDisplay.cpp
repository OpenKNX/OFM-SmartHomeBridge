#include <knx.h>
#include "knxprod.h"
#include "KnxChannelDisplay.h"
#include "BridgeBase.h"

#define KO_TEMPERATURE_FEEDBACK KoBRI_KO1_, DPT_Value_Temp
#define KO_HUMIDITY_FEEDBACK    KoBRI_KO1_, DPT_Value_Humidity
#define KO_LUX_FEEDBACK         KoBRI_KO1_, DPT_Value_Lux

KnxChannelDisplay::KnxChannelDisplay(uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      displayBridges()
{
}

ChannelBridge *KnxChannelDisplay::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createDisplay(*this, _channelIndex, ParamBRI_CHDisplayType);
}

void KnxChannelDisplay::deleteBridgeDevice(ChannelBridge *device)
{
    remove((DisplayBridge *)device);
}

void KnxChannelDisplay::add(DisplayBridge *displayBridge)
{
    displayBridges.push_back(displayBridge);
    displayBridge->initialize(this);
    switch (getDisplayType())
    {
        case DisplayType::DisplayTypeTemperature:
            if (koInitialized(KO_TEMPERATURE_FEEDBACK))
                displayBridge->setValue(koGet(KO_TEMPERATURE_FEEDBACK));
            break;
        case DisplayType::DisplayTypeHumidity:
            if (koInitialized(KO_HUMIDITY_FEEDBACK))
                displayBridge->setValue(koGet(KO_HUMIDITY_FEEDBACK));
            break;
            case DisplayType::DisplayTypeLux:
            if (koInitialized(KO_LUX_FEEDBACK))
                displayBridge->setValue(koGet(KO_LUX_FEEDBACK));
            break;
    }
}

void KnxChannelDisplay::commandMainFunctionClick()
{
}

void KnxChannelDisplay::remove(DisplayBridge *displayBridge)
{
    displayBridges.remove(displayBridge);
    delete displayBridge;
}

const std::string KnxChannelDisplay::name()
{
    return std::string("Display");
}

DisplayType KnxChannelDisplay::getDisplayType()
{
    return (DisplayType) ParamBRI_CHDisplayType;
}

void KnxChannelDisplay::setup()
{
    switch (getDisplayType())
    {
        case DisplayType::DisplayTypeTemperature:
            koSetWithoutSend(KO_TEMPERATURE_FEEDBACK, 0.F);
            koSendReadRequest(KO_TEMPERATURE_FEEDBACK);
            break;
        case DisplayType::DisplayTypeHumidity:
            koSetWithoutSend(KO_HUMIDITY_FEEDBACK, 0.F);
            koSendReadRequest(KO_HUMIDITY_FEEDBACK);
            break;
            case DisplayType::DisplayTypeLux:
            koSetWithoutSend(KO_LUX_FEEDBACK,  0.F);
            koSendReadRequest(KO_LUX_FEEDBACK);
            break;
    }
}

void KnxChannelDisplay::processInputKo(GroupObject &groupObject)
{
    if (isKo(groupObject, KoBRI_KO1_))
    {
        double value = 0;
        switch (getDisplayType())
        {
            case DisplayType::DisplayTypeTemperature:
                value = koGet(KO_TEMPERATURE_FEEDBACK);
                break;
            case DisplayType::DisplayTypeHumidity:
                value = koGet(KO_HUMIDITY_FEEDBACK);
                break;
            case DisplayType::DisplayTypeLux:
                value = koGet(KO_LUX_FEEDBACK);
                break;
        }
        for (auto it = displayBridges.begin(); it != displayBridges.end(); ++it)
        {
            (*it)->setValue(value);
        }
        mainFunctionValueChanged();
    }
}

std::string KnxChannelDisplay::currentValueAsString()
{
    switch (getDisplayType())
    {
        case DisplayType::DisplayTypeTemperature:
            return std::to_string((double) koGet(KO_TEMPERATURE_FEEDBACK)) + "°C";
        case DisplayType::DisplayTypeHumidity:
            return std::to_string((double) koGet(KO_HUMIDITY_FEEDBACK)) + "%";
        case DisplayType::DisplayTypeLux:
            return std::to_string((double) koGet(KO_LUX_FEEDBACK)) + "Lux";
    }
    return "";
}

bool KnxChannelDisplay::mainFunctionValue()
{
    return koInitialized(KO_TEMPERATURE_FEEDBACK);
}
