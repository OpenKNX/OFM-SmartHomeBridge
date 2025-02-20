#include <knx.h>
#include "knxprod.h"
#include "KnxChannelSwitch.h"

#define KO_SWITCH           KoBRI_KO1_, DPT_Switch
#define KO_SWITCH_FEEDBACK  KoBRI_KO2_, DPT_Switch

KnxChannelSwitch::KnxChannelSwitch(uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      switchBridges()
{
}

void KnxChannelSwitch::add(SwitchBridge *switchBridge)
{
    switchBridges.push_back(switchBridge);
    switchBridge->initialize(this);
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

void KnxChannelSwitch::commandPower(SwitchBridge *switchBridge, bool power)
{
    logDebugP("Received changed. Power %s", power ? "true" : "false");

    koSet(KO_SWITCH, power, true);
    for (auto it = switchBridges.begin(); it != switchBridges.end(); ++it)
    {
        if ((*it) != switchBridge)
            (*it)->setPower(power);
    }
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
    }
}