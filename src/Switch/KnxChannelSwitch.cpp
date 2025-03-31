#include <knx.h>
#include "knxprod.h"
#include "KnxChannelSwitch.h"
#include "BridgeBase.h"

#define KO_SWITCH           KoBRI_KO1_, DPT_Switch
#define KO_SWITCH_FEEDBACK  KoBRI_KO2_, DPT_Switch

KnxChannelSwitch::KnxChannelSwitch(uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      switchBridges()
{
}

ChannelBridge* KnxChannelSwitch::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createSwitch(*this, _channelIndex, ParamBRI_CHDeviceType);
}

void KnxChannelSwitch::deleteBridgeDevice(ChannelBridge *device)
{
    remove((SwitchBridge *)device);
}

void KnxChannelSwitch::add(SwitchBridge *switchBridge)
{
    switchBridges.push_back(switchBridge);
    switchBridge->initialize(this);
    switchBridge->setPower(koGet(KO_SWITCH_FEEDBACK));
}

void KnxChannelSwitch::remove(SwitchBridge *switchBridge)
{
    switchBridges.remove(switchBridge);
    delete switchBridge;
}

const std::string KnxChannelSwitch::name()
{
    return std::string("Switch");
}

void KnxChannelSwitch::commandMainFunctionClick()
{
    if (koGet(KO_SWITCH_FEEDBACK))
    {
        commandPower(nullptr, false);
    }
    else
    {
        commandPower(nullptr, true);
    }
}
std::string KnxChannelSwitch::mainFunctionImage()
{
    if (!ParamBRI_CHIcon)
        return KnxChannelBase::mainFunctionImage();
    return getImageFileName(koGet(KO_SWITCH_FEEDBACK) ? BRI_CHIcon100  : BRI_CHIcon0);
}

void KnxChannelSwitch::commandPower(SwitchBridge *switchBridge, bool power)
{
    logDebugP("Received changed. Power %s", power ? "true" : "false");

    koSet(KO_SWITCH, power, true);
    for (auto it = switchBridges.begin(); it != switchBridges.end(); ++it)
    {
        if ((*it) != switchBridge)
            (*it)->setPower(power);
    }
    mainFunctionValueChanged();
}


void KnxChannelSwitch::setup()
{
    koSetWithoutSend(KO_SWITCH, (uint32_t) 0);
    koSetWithoutSend(KO_SWITCH_FEEDBACK, (uint32_t) 0);
    koSendReadRequest(KO_SWITCH_FEEDBACK);
}

void KnxChannelSwitch::processInputKo(GroupObject &ko)
{
    if (isKo(ko, KO_SWITCH_FEEDBACK))
    {
        bool power = koGet(KO_SWITCH_FEEDBACK);
        koSetWithoutSend(KO_SWITCH, power);
        for (auto it = switchBridges.begin(); it != switchBridges.end(); ++it)
        {
            (*it)->setPower(power);
        }
        mainFunctionValueChanged();
    }
}

std::string KnxChannelSwitch::currentValueAsString()
{
    return koGet(KO_SWITCH_FEEDBACK) ? "Ein" : "Aus";
}

bool KnxChannelSwitch::mainFunctionValue()
{
    return koGet(KO_SWITCH_FEEDBACK);
}