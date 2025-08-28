#include <knx.h>
#include "knxprod.h"
#include "KnxChannelDisplay.h"
#include "BridgeBase.h"
#include <ISO8859_15ToUTF8.h>

#define KO_TEMPERATURE_FEEDBACK KoBRI_KO1_, DPT_Value_Temp
#define KO_HUMIDITY_FEEDBACK    KoBRI_KO1_, DPT_Value_Humidity
#define KO_LUX_FEEDBACK         KoBRI_KO1_, DPT_Value_Lux
#define KO_RAIN_INPUT           KoBRI_KO1_, DPT_Rain_Amount
#define KO_SNOW_INPUT           KoBRI_KO1_, DPT_Length_mm
#define KO_WIND_INPUT           KoBRI_KO1_, DPT_Value_Wsp_kmh
#define KO_PERCENT_INPUT        KoBRI_KO1_, DPT_Scaling
#define KO_CUSTOM_INPUT         KoBRI_KO1_, _customDpt
#define KO_TEXT_INPUT           KoBRI_KO1_, DPT_String_8859_1

KnxChannelDisplay::KnxChannelDisplay(uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      DeviceBridges()
{
    if (ParamBRI_CHDisplayType == DisplayType::DisplayTypeCustom)
    {
        // "DPST-1-1"   <Enumeration Text="DPT 1.xxx (Schalten)" Value="0" Id="%ENID%" />
        // "DPST-5-5"   <Enumeration Text="DPT 5.xxx (1-Byte-Wert)" Value="2" Id="%ENID%" />
        // "DPST-5-1"   <Enumeration Text="DPT 5.001 (Prozentwert)" Value="3" Id="%ENID%" />
        // "DPST-6-10"  <Enumeration Text="DPT 6.xxx (1-Byte-Wert-Vorzeichen)" Value="4" Id="%ENID%" />
        // "DPST-7-1"   <Enumeration Text="DPT 7.xxx (2-Byte-Wert)" Value="5" Id="%ENID%" />
        // "DPST-8-1"   <Enumeration Text="DPT 8.xxx (2-Byte-Wert-Vorzeichen)" Value="6" Id="%ENID%" />
        // "DPST-9-1"   <Enumeration Text="DPT 9.xxx (2-Byte-Fließkomma)" Value="7" Id="%ENID%" />
        // "DPST-12-1"  <Enumeration Text="DPT 12.xxx (4-Byte-Wert)" Value="13" Id="%ENID%" />
        // "DPST-13-1"  <Enumeration Text="DPT 13.xxx (4-Byte-Wert-Vorzeichen)" Value="14" Id="%ENID%" />
        // "DPST-14-68" <Enumeration Text="DPT 14.xxx (4-Byte-Fließkomma)" Value="15" Id="%ENID%" />
        switch (ParamBRI_CHDisplayInput)
        {
            case 0:
                _customDpt = DPT_Switch;
                break;
            case 2:
                _customDpt = DPT_DecimalFactor;
                break;
            case 3:
                _customDpt = DPT_Scaling;
                break;
            case 4:
                _customDpt = DPT_Value_1_Count;
                break;
            case 5:
                _customDpt = DPT_Value_2_Ucount;
                break;
            case 6:
                _customDpt = DPT_Value_2_Count;
                break;
            case 7:
                _customDpt = DPT_Value_Temp;
                break;
            case 13:
                _customDpt = DPT_Value_4_Ucount;
                break;
            case 14:
                _customDpt = DPT_Value_4_Count;
                break;
            case 15:
                _customDpt = DPT_Value_Common_Temperature;
                break;
            default:
                _customDpt = Dpt();
                break;
        }
    }
    else
    {
        _customDpt = Dpt();
    }
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
        DeviceBridge->setValue(lastStringValue.c_str());
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
        case DisplayType::DisplayTypePercent:
            koSetWithoutSend(KO_PERCENT_INPUT, (uint8_t) 0);
            koSendReadRequest(KO_PERCENT_INPUT);
            break;
        case DisplayType::DisplayTypeCustom:
            koSetWithoutSend(KO_CUSTOM_INPUT, 0.F);
            koSendReadRequest(KO_CUSTOM_INPUT);
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
            case DisplayType::DisplayTypePercent:
                lastValue = koGet(KO_PERCENT_INPUT);
                break;
            case DisplayType::DisplayTypeCustom:
                lastValue = koGet(KO_CUSTOM_INPUT);
                break;
            case DisplayType::DisplayTypeText:  
                lastStringValue = convertISO8859_15ToUTF8_string((const char*) KoBRI_KO1_.valueRef());
                break;
        }
        if (getDisplayType() == DisplayType::DisplayTypeText)
        {
            for (auto it = DeviceBridges.begin(); it != DeviceBridges.end(); ++it)
            {
                (*it)->setValue(lastStringValue.c_str());
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
            snprintf(buffer, sizeof(buffer), u8"%.1lf °C", lastValue);
            break;
        case DisplayType::DisplayTypeHumidity:
            snprintf(buffer, sizeof(buffer), u8"%.0lf %%", lastValue);
            break;
        case DisplayType::DisplayTypeLux:
            snprintf(buffer, sizeof(buffer), u8"%.0lf Lux", lastValue);
            break;
        case DisplayType::DisplayTyppeRain:
            snprintf(buffer, sizeof(buffer), u8"%.1lf l/h", lastValue);
            break;
        case DisplayType::DisplayTypeSnow:
            snprintf(buffer, sizeof(buffer), u8"%.0lf mm", lastValue);
            break;
        case DisplayType::DisplayTypeWind:
            snprintf(buffer, sizeof(buffer), u8"%.1lf km/h", lastValue);
            break;
        case DisplayType::DisplayTypePercent:
            snprintf(buffer, sizeof(buffer), u8"%.0lf %%", lastValue);
            break;
        case DisplayType::DisplayTypeCustom:
            if (ParamBRI_CHDisplayType == DisplayType::DisplayTypeCustom)
            {
                // "DPST-1-1"   <Enumeration Text="DPT 1.xxx (Schalten)" Value="0" Id="%ENID%" />
                // "DPST-5-5"   <Enumeration Text="DPT 5.xxx (1-Byte-Wert)" Value="2" Id="%ENID%" />
                // "DPST-5-1"   <Enumeration Text="DPT 5.001 (Prozentwert)" Value="3" Id="%ENID%" />
                // "DPST-6-10"  <Enumeration Text="DPT 6.xxx (1-Byte-Wert-Vorzeichen)" Value="4" Id="%ENID%" />
                // "DPST-7-1"   <Enumeration Text="DPT 7.xxx (2-Byte-Wert)" Value="5" Id="%ENID%" />
                // "DPST-8-1"   <Enumeration Text="DPT 8.xxx (2-Byte-Wert-Vorzeichen)" Value="6" Id="%ENID%" />
                // "DPST-9-1"   <Enumeration Text="DPT 9.xxx (2-Byte-Fließkomma)" Value="7" Id="%ENID%" />
                // "DPST-12-1"  <Enumeration Text="DPT 12.xxx (4-Byte-Wert)" Value="13" Id="%ENID%" />
                // "DPST-13-1"  <Enumeration Text="DPT 13.xxx (4-Byte-Wert-Vorzeichen)" Value="14" Id="%ENID%" />
                // "DPST-14-68" <Enumeration Text="DPT 14.xxx (4-Byte-Fließkomma)" Value="15" Id="%ENID%" />
                const char* unit = (const char*) ParamBRI_CHDisplayUnit;
                auto unitUtf8 = convertISO8859_15ToUTF8(unit);
                switch (ParamBRI_CHDisplayInput)
                {
                    case 0:
                        snprintf(buffer, sizeof(buffer), "%.s %.5s", lastValue == 0 ? "Aus" : "Ein", unitUtf8);
                        break;
                    case 2:
                        snprintf(buffer, sizeof(buffer), "%.0lf %.5s", lastValue, unitUtf8);
                        break;
                    case 3:
                        snprintf(buffer, sizeof(buffer), "%.0lf %.5s", lastValue, unitUtf8);
                        break;
                    case 4:
                        snprintf(buffer, sizeof(buffer), "%.0lf %.5s", lastValue, unitUtf8);
                        break;
                    case 5:
                        snprintf(buffer, sizeof(buffer), "%.0lf %.5s", lastValue, unitUtf8);
                        break;
                    case 6:
                        snprintf(buffer, sizeof(buffer), "%.0lf %.5s", lastValue, unitUtf8);
                        break;
                    case 7:
                        snprintf(buffer, sizeof(buffer), "%.1lf %.5s", lastValue, unitUtf8);
                        break;
                    case 13:
                        snprintf(buffer, sizeof(buffer), "%.0lf %.5s", lastValue, unitUtf8);
                        break;
                    case 14:
                        snprintf(buffer, sizeof(buffer), "%.0lf %.5s", lastValue, unitUtf8);
                        break;
                    case 15:
                        snprintf(buffer, sizeof(buffer), "%.1lf %.5s", lastValue, unitUtf8);
                        break;
                    default:
                        break;
                }
                if (unitUtf8 != unit)
                {
                    free((void *) unitUtf8);
                }   
            }
            break;
        case DisplayType::DisplayTypeText:
            return lastStringValue;
        default:
            return std::string("?");
    }
    return buffer;
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