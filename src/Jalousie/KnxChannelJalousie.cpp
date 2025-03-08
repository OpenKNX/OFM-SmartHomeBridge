#include <knx.h>
#include "knxprod.h"
#include "KnxChannelJalousie.h"
#include "BridgeBase.h"

#define KO_SLAT_POSITION          KoBRI_KO7_, DPT_Scaling
#define KO_SLAT_POSITION_FEEDBACK KoBRI_KO8_, DPT_Scaling

KnxChannelJalousie::KnxChannelJalousie(uint16_t channelIndex)
    : KnxChannelRolladen(channelIndex)
{  

}

ChannelBridge* KnxChannelJalousie::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createJalousien(*this, _channelIndex, ParamBRI_CHDeviceType);
}

void KnxChannelJalousie::deleteBridgeDevice(ChannelBridge *device)
{
    remove((RolladenBridge *)device);
}

void KnxChannelJalousie::add(RolladenBridge* interface)
{
    interfaces.push_back(interface);
    interface->initialize(this);
    if (koInitialized(KO_SLAT_POSITION_FEEDBACK))
        interface->setSlatPosition(koGet(KO_SLAT_POSITION_FEEDBACK));
}

void KnxChannelJalousie::remove(RolladenBridge* interface)
{
    interfaces.remove(interface);
    delete interface;
}

const std::string KnxChannelJalousie::name()
{
    return std::string("Jalousie");
}

BlindsHandling KnxChannelJalousie::getBlindsHandling()
{
    return (BlindsHandling) ParamBRI_CHJalousieUpDownHandling;
}

bool KnxChannelJalousie::useStop()
{
    return ParamBRI_CHJalousieUseStop == 1;
}

void KnxChannelJalousie::commandMainFunctionClick()
{
    if ((uint8_t) koGet(KO_SLAT_POSITION_FEEDBACK) > 0)
    {
        commandPosition(nullptr, 0);
    }
    else
    {
        commandPosition(nullptr, 100);
    }
}

bool KnxChannelJalousie::commandPosition(RolladenBridge* interface, uint8_t position)
{
    uint8_t currentPosition = KnxChannelRolladen::currentPosition();
    bool result = KnxChannelRolladen::commandPosition(interface, position);
    if (result)
    {
        switch (ParamBRI_CHSlatHandling)
        {
            case 1: // Send 100% if close from 0
                if (position > 0 && currentPosition == 0)
                    commandSlatPosition(nullptr, 100);
            break;
            case 2: // Send 100% for closing, 0% for opening
                if (position > currentPosition)
                    commandSlatPosition(nullptr, 100);
                else if (position < currentPosition)
                    commandSlatPosition(nullptr, 0);
            break;
        }
    }
    return result;
}

void KnxChannelJalousie::commandSlatPosition(RolladenBridge* interface, uint8_t slatPosition)
{
    logDebugP("Received changed. Slat Position: %d", slatPosition);
    koSet(KO_SLAT_POSITION, slatPosition, true);
    for (auto it = interfaces.begin(); it != interfaces.end(); ++it)
    {
        if ((*it) != (RolladenBridge*)interface)
            (*it)->setSlatPosition(slatPosition);
    }
    mainFunctionValueChanged();
}

void KnxChannelJalousie::setup()
{
    KnxChannelRolladen::setup();
    koSetWithoutSend(KO_SLAT_POSITION, (uint8_t) 0);
    koSetWithoutSend(KO_SLAT_POSITION_FEEDBACK, (uint8_t) 0);
    koSendReadRequest(KO_SLAT_POSITION_FEEDBACK);
}

void KnxChannelJalousie::processInputKo(GroupObject &ko)
{
    KnxChannelRolladen::processInputKo(ko);
    if (isKo(ko, KO_SLAT_POSITION_FEEDBACK))
    {
        uint8_t slatPosition = koGet(KO_SLAT_POSITION_FEEDBACK);
        koSetWithoutSend(KO_SLAT_POSITION, slatPosition);
        for (auto it = interfaces.begin(); it != interfaces.end(); ++it)
        {
            (*it)->setSlatPosition(slatPosition);
        }
        mainFunctionValueChanged();
    }
}
