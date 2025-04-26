#include <knx.h>
#include "knxprod.h"
#include "KnxChannelRGB.h"
#include "BridgeBase.h"
#include "rgbconvert.h"
#define KO_RGB             KoBRI_KO1_, DPT_Colour_RGB
#define KO_RGB_FEEDBACK    KoBRI_KO2_, DPT_Colour_RGB
#define KO_POWER           KoBRI_KO3_, DPT_Switch
#define KO_POWER_FEEDBACK  KoBRI_KO4_, DPT_Switch


enum RGBSwitchBehavior
{
    White = 0,
    Dark = 1,
    Red = 2,
    Green = 3,
    Blue = 4,
    Yellow = 5,
    Magenta = 6,
    Cyan = 7,
    SendPower = 200,
    LastBrightness = 201,
    LastBrightnessLessOtherThenWhite = 202
};


KnxChannelRGB::KnxChannelRGB(uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      RGBBridges()
{
    
}

ChannelBridge* KnxChannelRGB::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createRGB(*this, _channelIndex, ParamBRI_CHDeviceType);
}

void KnxChannelRGB::deleteBridgeDevice(ChannelBridge *device)
{
    remove((RGBBridge *)device);
}

void KnxChannelRGB::add(RGBBridge* RGBBridge)
{
    RGBBridges.push_back(RGBBridge);
    RGBBridge->initialize(this);
    RGBBridge->setRGB(lastColor);
    if (ParamBRI_CHLightRGBUseSwitchKO)
        RGBBridge->setPower(koGet(KO_POWER_FEEDBACK));
    else
        RGBBridge->setPower((uint32_t) koGet(KO_RGB_FEEDBACK) > 0);
}

void KnxChannelRGB::remove(RGBBridge* RGBBridge)
{
    RGBBridges.remove(RGBBridge);
    delete RGBBridge;
}

const std::string KnxChannelRGB::name()
{
    return std::string("RGB");
}

void KnxChannelRGB::commandMainFunctionClick()
{
    if (mainFunctionValue())
    {
        uint32_t targetColor = getColorForBehavior(ParamBRI_CHLightRGBSwitchOn2Behavior);
        logErrorP("Target Color: %lu Last Color: %d", (unsigned long) targetColor, (unsigned long) lastColor);
        if (targetColor == 0 || targetColor == lastColor)
        {
            commandPower(nullptr, false);
        }
        else
        {
            commandRGB(nullptr, targetColor);
        }
    }
    else
    {
        commandPower(nullptr, true);
    }
}

void KnxChannelRGB::commandRGB(RGBBridge* RGBBridge, uint32_t rgb)
{
    logErrorP("Received changed. RGB: %lu", (unsigned long) rgb);
    if (rgb > 0)
    {
        lastColor = rgb;
        if (rgb != 0xFFFFFF)
            lastColorLessOtherThanWhite = rgb;
    }
    uint32_t knxValue = rgb;
    for (auto it = RGBBridges.begin(); it != RGBBridges.end(); ++it)
    {
        if ((*it) != RGBBridge)
        {
            if (rgb > 0)
                (*it)->setRGB(rgb);
            else
                (*it)->setPower(false);
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();
    }
    if (knxValue == 0)
    {
        if (ParamBRI_CHLightRGBUseSwitchKO)
        {
            koSetWithoutSend(KO_POWER_FEEDBACK, false);
            koSet(KO_POWER, false, true);
        }
        else
        {
            koSetWithoutSend(KO_RGB_FEEDBACK, (uint32_t) 0x000000);
            koSet(KO_RGB, (uint32_t) 0x000000, true);
        };
    }
    else
    {
        koSetWithoutSend(KO_RGB_FEEDBACK, knxValue);
        koSet(KO_RGB, knxValue, true);
        if (ParamBRI_CHLightRGBUseSwitchKO)
        {
            koSetWithoutSend(KO_POWER_FEEDBACK, true);
            koSetWithoutSend(KO_POWER, true);
        }
    }
}

uint32_t KnxChannelRGB::getColorForBehavior(uint8_t behavior)
{
    logErrorP("getColorForBehavior: %d", (int) behavior);
    switch((RGBSwitchBehavior) behavior)
    {
        case RGBSwitchBehavior::SendPower:      
            return 0;
        case RGBSwitchBehavior::White:
            return 0xFFFFFF;
        case RGBSwitchBehavior::Dark:
            return 0x808080;
            break;
        case RGBSwitchBehavior::Red:
            return 0xFF0000;
        case RGBSwitchBehavior::Green:
            return 0x00FF00;
        case RGBSwitchBehavior::Blue:
            return 0x0000FF;
        case RGBSwitchBehavior::Yellow:
            return 0xFFFF00;
        case RGBSwitchBehavior::Magenta:
            return 0xFF00FF;
        case RGBSwitchBehavior::Cyan:
            return 0x00FFFF;
        case RGBSwitchBehavior::LastBrightness:
            return lastColor;
        case RGBSwitchBehavior::LastBrightnessLessOtherThenWhite:
            return lastColorLessOtherThanWhite;
    }
    return 0;
}

void KnxChannelRGB::commandPower(RGBBridge* RGBBridge, bool power)
{
    for (auto it = RGBBridges.begin(); it != RGBBridges.end(); ++it)
    {
        if ((*it) != RGBBridge)
        {
            (*it)->setPower(power);
        }
    }
    if (power)
    {
        uint32_t configValue = (mainFunctionValue())
            ? ParamBRI_CHLightRGBSwitchOn2Behavior 
            : ParamBRI_CHLightRGBSwitchOnBehavior;
        logErrorP("Switch on: %d behavior: %d", (int) mainFunctionValue(), (int) configValue);
        switch((RGBSwitchBehavior) configValue)
        {
            case RGBSwitchBehavior::SendPower:      
                if (ParamBRI_CHLightRGBUseSwitchKO)
                {
                    koSetWithoutSend(KO_POWER_FEEDBACK, true);
                    koSet(KO_POWER, true, true);    
                }
                else
                {
                    commandRGB(nullptr, 0xFFFFFF);
                }
                break;
            case RGBSwitchBehavior::White:
                commandRGB(nullptr, 0xFFFFFF);
                break;
            case RGBSwitchBehavior::Dark:
                commandRGB(nullptr, 0x808080);
                break;
            case RGBSwitchBehavior::Red:
                commandRGB(nullptr, 0xFF0000);
                break;
            case RGBSwitchBehavior::Green:
                commandRGB(nullptr, 0x00FF00);
                break;
            case RGBSwitchBehavior::Blue:
                commandRGB(nullptr, 0x0000FF);
                break;
            case RGBSwitchBehavior::Yellow:
                commandRGB(nullptr, 0xFFFF00);
                break;
            case RGBSwitchBehavior::Magenta:
                commandRGB(nullptr, 0xFF00FF);
                break;
            case RGBSwitchBehavior::Cyan:
                commandRGB(nullptr, 0x00FFFF);
                break;
            case RGBSwitchBehavior::LastBrightness:
                commandRGB(nullptr, lastColor);
                break;
            case RGBSwitchBehavior::LastBrightnessLessOtherThenWhite:
                commandRGB(nullptr, lastColorLessOtherThanWhite);
                break;
        }
    }
    else
    {
        if (ParamBRI_CHLightRGBUseSwitchKO)
        {
            koSetWithoutSend(KO_POWER_FEEDBACK, false);
            koSet(KO_POWER, false, true);    
        }
        else
        {
            commandRGB(nullptr, 0x000000);
        }
   }
   for (auto it = RGBBridges.begin(); it != RGBBridges.end(); ++it)
   {
       if ((*it) != RGBBridge)
       {
           (*it)->mainFunctionValueChanged();
       }
       mainFunctionValueChanged();
   }
}

void KnxChannelRGB::setup()
{
    koSetWithoutSend(KO_RGB, (uint32_t) 0x000000);
    koSetWithoutSend(KO_RGB_FEEDBACK, (uint32_t) 0x000000);
    koSendReadRequest(KO_RGB_FEEDBACK);
    if (ParamBRI_CHLightRGBUseSwitchKO)
    {
        koSetWithoutSend(KO_POWER, false);
        koSetWithoutSend(KO_POWER_FEEDBACK, false);
    }
    koSendReadRequest(KO_RGB_FEEDBACK);
}

void KnxChannelRGB::processInputKo(GroupObject &groupObject)
{
    if (isKo(groupObject, KO_RGB_FEEDBACK))
    {
        uint32_t rgb = koGet(KO_RGB_FEEDBACK);
        if (rgb > 0)
        {
            lastColor = rgb;
            if (rgb != 0xFFFFFF)
                lastColorLessOtherThanWhite = rgb;
        }    
        koSetWithoutSend(KO_RGB, rgb);
        for (auto it = RGBBridges.begin(); it != RGBBridges.end(); ++it)
        {
            if (rgb > 0)
                (*it)->setRGB(rgb);
            if (rgb == 0)
            {
                (*it)->setPower(false);
            }
            else
            {
                if (ParamBRI_CHLightRGBUseSwitchKO)
                {
                    (*it)->setPower(koGet(KO_POWER_FEEDBACK));
                }
                else
                {
                    (*it)->setPower(true);
                }
            }
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();
    }
    if (isKo(groupObject, KO_POWER_FEEDBACK))
    {
        bool power = koGet(KO_POWER_FEEDBACK);
        koSetWithoutSend(KO_RGB, power);
        for (auto it = RGBBridges.begin(); it != RGBBridges.end(); ++it)
        {
            (*it)->setPower(power);
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();
    }
}

std::string KnxChannelRGB::currentValueAsString()
{
    // rgb rgb;
    // rgb.r = (lastColor >> 16) & 0xFF;
    // rgb.g = (lastColor >> 8) & 0xFF;
    // rgb.b = (lastColor) & 0xFF;
    // auto hsv = rgb2hsv(rgb);
    if (ParamBRI_CHLightRGBUseSwitchKO)
    {
        return koGet(KO_POWER_FEEDBACK) ? "Ein" : "Aus";
    }
    else
    {
        auto rgb = lastColor;
        if (rgb != 0x000000)
        {
            return "Ein";// std::to_string(hsv.v) + "%";
        }
        else
        {
            return "Aus";
        }
    }            
}

bool KnxChannelRGB::mainFunctionValue()
{
    if (ParamBRI_CHLightRGBUseSwitchKO)
        return koGet(KO_POWER_FEEDBACK);
    else
        return (uint32_t) koGet(KO_RGB_FEEDBACK) > 0;
}

MainFunctionStateImage KnxChannelRGB::mainFunctionImage()
{
   return calculateMainFunctionImage(KO_POWER_FEEDBACK, 0, LIMIT_NOT_USED, 100);
}