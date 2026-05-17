#include <knx.h>
#include "knxprod.h"
#include "KnxChannelDimmer.h"
#include "BridgeBase.h"

#define KO_DIMMER           KoBRI_KO1_, DPT_Scaling
#define KO_DIMMER_FEEDBACK  KoBRI_KO2_, DPT_Scaling


enum DimmerSwitchBehavior
{
    LastBrightness = 201,
    LastBrightnessLessThan100 = 200,
    Custom = 255
};


KnxChannelDimmer::KnxChannelDimmer(uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      dimmerBridges()
{
}

ChannelBridge* KnxChannelDimmer::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createDimmer(*this, _channelIndex, ParamBRI_CHDeviceType);
}

void KnxChannelDimmer::deleteBridgeDevice(ChannelBridge *device)
{
    remove((DimmerBridge *)device);
}

void KnxChannelDimmer::add(DimmerBridge *dimmerBridge)
{
    dimmerBridges.push_back(dimmerBridge);
    dimmerBridge->initialize(this);
}

void KnxChannelDimmer::syncBridgeState(ChannelBridge *bridge)
{
    if (bridge == nullptr)
        return;

    auto dimmerBridge = static_cast<DimmerBridge *>(bridge);
    dimmerBridge->setBrightness(koGet(KO_DIMMER_FEEDBACK));
}

void KnxChannelDimmer::syncAllBridgeStates()
{
    for (auto it = dimmerBridges.begin(); it != dimmerBridges.end(); ++it)
    {
        syncBridgeState(*it);
    }
}

void KnxChannelDimmer::remove(DimmerBridge *dimmerBridge)
{
    dimmerBridges.remove(dimmerBridge);
    delete dimmerBridge;
}

const std::string KnxChannelDimmer::name()
{
    return std::string("Dimmer");
}

void KnxChannelDimmer::commandBrightness(DimmerBridge* dimmerBridge, uint8_t brightness)
{
    logDebugP("Received changed. Brightness: %d", (int) brightness);
    if (brightness > 0)
        lastBrighness = brightness;
    uint8_t knxValue = brightness;
    for (auto it = dimmerBridges.begin(); it != dimmerBridges.end(); ++it)
    {
        if ((*it) != dimmerBridge)
        {
            (*it)->setBrightness(brightness);
        }
    }
    koSetWithoutSend(KO_DIMMER_FEEDBACK, knxValue);
    koSet(KO_DIMMER, knxValue, true);
    mainFunctionValueChanged();
}

void KnxChannelDimmer::commandMainFunctionClick()
{
    uint8_t brightness = koGet(KO_DIMMER_FEEDBACK);
    if (brightness > 0)
    {
        uint8_t targetValue = 0;
        switch (ParamBRI_CHLightSwitchOn2Behavior)
        {
        case DimmerSwitchBehavior::LastBrightness:
            targetValue = lastBrighness;
            break;
        case DimmerSwitchBehavior::LastBrightnessLessThan100:
            targetValue = lastBrighnessLessThan100;
            break;
        case DimmerSwitchBehavior::Custom:
            targetValue = ParamBRI_CHLightSwitchOn2Custom;
            break;
        default:
            targetValue = ParamBRI_CHLightSwitchOn2Behavior; // Direct percentage value
        }
        if (brightness == targetValue)
        {
            commandBrightness(nullptr, 0);
        }
        else
        {
            commandBrightness(nullptr, targetValue);
        }
    }
    else
    {
        commandPower(nullptr, true);
    }
}

void KnxChannelDimmer::commandPower(DimmerBridge* dimmerBridge, bool power)
{
    if (power)
    {
        bool first = 0 == (uint8_t) koGet(KO_DIMMER_FEEDBACK);
        uint8_t configValue = first
            ? ParamBRI_CHLightSwitchOnBehavior 
            : ParamBRI_CHLightSwitchOn2Behavior;
        switch((DimmerSwitchBehavior) configValue)
        {
            case DimmerSwitchBehavior::LastBrightness:
                commandBrightness(nullptr, lastBrighness);
                break;
            case DimmerSwitchBehavior::LastBrightnessLessThan100:
                commandBrightness(nullptr, lastBrighnessLessThan100);
                break;
            case DimmerSwitchBehavior::Custom:
                commandBrightness(nullptr, first ? ParamBRI_CHLightSwitchOnCustom : ParamBRI_CHLightSwitchOn2Custom);
                break;
            default:
                commandBrightness(nullptr, configValue); // Direct percentage value
        }
    }
    else
    {
        commandBrightness(nullptr, 0);
    }
}

void KnxChannelDimmer::setup()
{
    koSetWithoutSend(KO_DIMMER, (uint8_t) 0);
    koSetWithoutSend(KO_DIMMER_FEEDBACK,(uint8_t) 0);
    koSendReadRequest(KO_DIMMER_FEEDBACK);
   
}

void KnxChannelDimmer::processInputKo(GroupObject &groupObject)
{
    if (isKo(groupObject, KO_DIMMER_FEEDBACK))
    {
        uint8_t brightness = koGet(KO_DIMMER_FEEDBACK);
        if (brightness > 0)
        {
            lastBrighness = brightness;
            if (brightness < 100)
                lastBrighnessLessThan100 = brightness;
        }    
        koSetWithoutSend(KO_DIMMER, brightness);
        for (auto it = dimmerBridges.begin(); it != dimmerBridges.end(); ++it)
        {
            (*it)->setBrightness(brightness);
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();

    }
}

std::string KnxChannelDimmer::currentValueAsString()
{
    return std::to_string((uint8_t) koGet(KO_DIMMER_FEEDBACK)) + "%";
}

bool KnxChannelDimmer::mainFunctionValue()
{
    return (uint8_t) koGet(KO_DIMMER_FEEDBACK) > 0;
}

MainFunctionStateImage KnxChannelDimmer::mainFunctionImage()
{
   return calculateMainFunctionImage(KO_DIMMER_FEEDBACK, 0, 99, 100);
}
