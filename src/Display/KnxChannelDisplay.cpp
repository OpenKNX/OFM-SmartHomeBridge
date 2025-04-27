#include <knx.h>
#include "knxprod.h"
#include "KnxChannelDisplay.h"
#include "BridgeBase.h"

#define KO_TEMPERATURE_FEEDBACK KoBRI_KO1_, DPT_Value_Temp
#define KO_HUMIDITY_FEEDBACK    KoBRI_KO1_, DPT_Value_Humidity
#define KO_LUX_FEEDBACK         KoBRI_KO1_, DPT_Value_Lux
#define KO_RAIN_INPUT           KoBRI_KO1_, DPT_Rain_Amount
#define KO_SNOW_INPUT           KoBRI_KO1_, DPT_Length_mm
#define KO_WIND_INPUT           KoBRI_KO1_, DPT_Value_Wsp_kmh
#define KO_TEXT_INPUT           KoBRI_KO1_, DPT_String_8859_1

KnxChannelDisplay::KnxChannelDisplay(uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      DeviceBridges()
{
}

ChannelBridge *KnxChannelDisplay::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createDisplay(*this, _channelIndex, ParamBRI_CHDisplayType);
}

void KnxChannelDisplay::deleteBridgeDevice(ChannelBridge *device)
{
    remove((DeviceBridge *)device);
}

void KnxChannelDisplay::add(DeviceBridge *DeviceBridge)
{
    DeviceBridges.push_back(DeviceBridge);
    DeviceBridge->initialize(this);
    if (getDisplayType() == DisplayType::DisplayTypeText)
    {
        DeviceBridge->setValue(lastStringValue);
    }
    else
    {
        DeviceBridge->setValue(lastValue);
    }
}

void KnxChannelDisplay::commandMainFunctionClick()
{
}

void KnxChannelDisplay::remove(DeviceBridge *DeviceBridge)
{
    DeviceBridges.remove(DeviceBridge);
    delete DeviceBridge;
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
        case DisplayType::DisplayTyppeRain:
            koSetWithoutSend(KO_RAIN_INPUT, 0.F);
            koSendReadRequest(KO_RAIN_INPUT);
            break;
        case DisplayType::DisplayTypeSnow:
            koSetWithoutSend(KO_SNOW_INPUT, 0.F);
            koSendReadRequest(KO_SNOW_INPUT);
            break;
        case DisplayType::DisplayTypeWind:
            koSetWithoutSend(KO_WIND_INPUT, 0.F);
            koSendReadRequest(KO_WIND_INPUT);
            break;
        case DisplayType::DisplayTypeText:
            koSetWithoutSend(KO_TEXT_INPUT, "");
            koSendReadRequest(KO_TEXT_INPUT);
            break;

    }
}

void KnxChannelDisplay::processInputKo(GroupObject &groupObject)
{
    if (isKo(groupObject, KoBRI_KO1_))
    {
        hasValue = true;
        switch (getDisplayType())
        {
            case DisplayType::DisplayTypeTemperature:
                lastValue = koGet(KO_TEMPERATURE_FEEDBACK);
                break;
            case DisplayType::DisplayTypeHumidity:
                lastValue = koGet(KO_HUMIDITY_FEEDBACK);
                break;
            case DisplayType::DisplayTypeLux:
                lastValue = koGet(KO_LUX_FEEDBACK);
                break;
            case DisplayType::DisplayTyppeRain:
                lastValue = koGet(KO_RAIN_INPUT);
                break;
            case DisplayType::DisplayTypeSnow:
                lastValue = koGet(KO_SNOW_INPUT);
                break;
            case DisplayType::DisplayTypeWind:
                lastValue = koGet(KO_WIND_INPUT);
                break;
            case DisplayType::DisplayTypeText:  
                lastStringValue = (const char*) KoBRI_KO1_.valueRef();
                break;
        }
        if (getDisplayType() == DisplayType::DisplayTypeText)
        {
            for (auto it = DeviceBridges.begin(); it != DeviceBridges.end(); ++it)
            {
                (*it)->setValue(lastStringValue);
                (*it)->mainFunctionValueChanged();
            }
        }
        else
        {
            for (auto it = DeviceBridges.begin(); it != DeviceBridges.end(); ++it)
            {
                (*it)->setValue(lastValue);
                (*it)->mainFunctionValueChanged();
            }
        }
        mainFunctionValueChanged();
    }
}

std::string KnxChannelDisplay::currentValueAsString()
{
    
    char buffer[50] = {0};
    switch (getDisplayType())
    {
        case DisplayType::DisplayTypeTemperature:
            snprintf(buffer, sizeof(buffer), "%.1lf °C", (double) lastValue);
            break;
        case DisplayType::DisplayTypeHumidity:
            snprintf(buffer, sizeof(buffer), "%.0lf %%", (double) lastValue);
            break;
        case DisplayType::DisplayTypeLux:
            snprintf(buffer, sizeof(buffer), "%.0lf Lux", (double) lastValue);
            break;
        case DisplayType::DisplayTyppeRain:
            snprintf(buffer, sizeof(buffer), "%.1lf l/h", (double) lastValue);
            break;
        case DisplayType::DisplayTypeSnow:
            snprintf(buffer, sizeof(buffer), "%.0lf mm", (double) lastValue);
            break;
        case DisplayType::DisplayTypeWind:
            snprintf(buffer, sizeof(buffer), "%.1lf km/h", (double) lastValue);
            break;
        case DisplayType::DisplayTypeText:
            return std::string(lastStringValue);
        default:
            return std::string("?");
    }
    return std::string(buffer);
}

bool KnxChannelDisplay::mainFunctionValue()
{
    if (!hasValue)
        return false;
    if (!ParamBRI_CHIcon)
        return lastValue >= ParamBRI_CHDisplayUpper;
        
    // <Enumeration Value="0" Id="%ENID%" Text="Keine Einfärbung"       />
    // <Enumeration Value="1" Id="%ENID%" Text="Als AUS darstellen"     />
    // <Enumeration Value="2" Id="%ENID%" Text="Als EIN darstellen "    />
    switch (getIconState())
    {
        case 0:
            if (getDisplayType() == DisplayType::DisplayTypeText)
                return lastStringValue[0] != 0;
            else
                return true;
        case 1:
            return false;
        case 2:
            return true;
    }
    return false;
}

bool KnxChannelDisplay::mainFunctionPreferValueDisplay()
{
    return true;
}

int KnxChannelDisplay::getIconState()
{
    int iconState = 0;
    if (getDisplayType() == DisplayType::DisplayTypeText)
    {
        if (lastStringValue[0] != 0)
            iconState = ParamBRI_CHDisplayIconState3;
        else
            iconState = ParamBRI_CHDisplayIconState1;
    }
    else
    {
        if (lastValue <= ParamBRI_CHDisplayLower)
            iconState = ParamBRI_CHDisplayIconState1;
        else if (lastValue >= ParamBRI_CHDisplayUpper)
            iconState = ParamBRI_CHDisplayIconState3;
        else
            iconState = ParamBRI_CHDisplayIconState2;
    }
    return iconState;
}

MainFunctionStateImage KnxChannelDisplay::mainFunctionImage()
{
    if (!ParamBRI_CHIcon)
    {
        std::string image = "Type";
        image += std::to_string(ParamBRI_CHDeviceType);
        image += "_";
        image += std::to_string(ParamBRI_CHDisplayType);
        image += ".png";
        return {true, image};
    }

    bool allowRecolor = getIconState() != 0;
    if (getDisplayType() == DisplayType::DisplayTypeText)
    {
        bool textSet = lastStringValue[0] != 0;
        return { allowRecolor, textSet ? getImageFileName(BRI_CHIcon100) : getImageFileName(BRI_CHIcon0)};
    }
    if (lastValue <= ParamBRI_CHDisplayLower)
        return { allowRecolor, getImageFileName(BRI_CHIcon0)};
    if (lastValue >= ParamBRI_CHDisplayUpper)
        return { allowRecolor, getImageFileName(BRI_CHIcon100)};
    return { allowRecolor, getImageFileName(BRI_CHIcon50) };
}