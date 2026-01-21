#include <knx.h>
#include "knxprod.h"
#include "KnxChannelLock.h"
#include "BridgeBase.h"

#define KO_LOCK           KoBRI_KO1_, DPT_Switch
#define KO_LOCK_FEEDBACK  KoBRI_KO2_, DPT_Switch
#define KO_BLOCKED_FEEDBACK  KoBRI_KO3_, DPT_Switch

KnxChannelLock::KnxChannelLock(uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      lockBridges()
{
}

ChannelBridge* KnxChannelLock::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createLock(*this, _channelIndex, ParamBRI_CHDeviceType);
}

void KnxChannelLock::deleteBridgeDevice(ChannelBridge *device)
{
    remove((LockBridge *)device);
}

void KnxChannelLock::add(LockBridge *lockBridge)
{
    lockBridges.push_back(lockBridge);
    lockBridge->initialize(this);
    lockBridge->setLocked(isLocked());
    lockBridge->setBlocked(koGet(KO_BLOCKED_FEEDBACK));
}

void KnxChannelLock::remove(LockBridge *lockBridge)
{
    lockBridges.remove(lockBridge);
    delete lockBridge;
}

const std::string KnxChannelLock::name()
{
    return std::string("Lock");
}

void KnxChannelLock::commandMainFunctionClick()
{
    if (koGet(KO_LOCK_FEEDBACK))
    {
        commandLock(nullptr, false);
    }
    else
    {
        commandLock(nullptr, true);
    }
}

MainFunctionStateImage KnxChannelLock::mainFunctionImage()
{
   return calculateMainFunctionImage(isLocked() ? 0 : 100, 0, LIMIT_NOT_USED, 100);
}

void KnxChannelLock::commandLock(LockBridge *lockBridge, bool lock)
{
    logDebugP("Received changed. Lock %s", lock ? "true" : "false");
    bool value;
    // <Enumeration Value="0" Id="%ENID%" Text="Verriegeln=0 / Entriegeln=1"  />
    // <Enumeration Value="1" Id="%ENID%" Text="Entriegeln=0 / Verriegeln=1"     />
    if (ParamBRI_CHLockLocking)
        value = lock;
    else
        value = !lock;
    koSet(KO_LOCK, value, true);
    for (auto it = lockBridges.begin(); it != lockBridges.end(); ++it)
    {
        if ((*it) != lockBridge)
        {
            (*it)->mainFunctionValueChanged();
        }
    }
    mainFunctionValueChanged();
}


void KnxChannelLock::setup()
{
    // <Enumeration Value="0" Id="%ENID%" Text="Verriegeln=0 / Entriegeln=1"  />
    // <Enumeration Value="1" Id="%ENID%" Text="Entriegeln=0 / Verriegeln=1"     />
    koSetWithoutSend(KO_LOCK, ParamBRI_CHLockLocking ? false: true);
    // <Enumeration Value="0" Id="%ENID%" Text="Verriegelt=0 / Entriegelt=1"  />
    // <Enumeration Value="1" Id="%ENID%" Text="Entriegelt=0 / Verriegelt=1"     />
    koSetWithoutSend(KO_LOCK_FEEDBACK, ParamBRI_CHLockLocked ? false : true);
    koSendReadRequest(KO_LOCK_FEEDBACK);
    koSetWithoutSend(KO_BLOCKED_FEEDBACK, false);
    koSendReadRequest(KO_BLOCKED_FEEDBACK);
}

bool KnxChannelLock::isLocked()
{
    bool locked = koGet(KO_LOCK_FEEDBACK);
    // <Enumeration Value="0" Id="%ENID%" Text="Verriegelt=0 / Entriegelt=1"  />
    // <Enumeration Value="1" Id="%ENID%" Text="Entriegelt=0 / Verriegelt=1"     />
    if (!ParamBRI_CHLockLocked)
        locked = !locked;
    return locked;
}

void KnxChannelLock::processInputKo(GroupObject &ko)
{
    if (isKo(ko, KO_LOCK_FEEDBACK))
    {
        bool locked = isLocked();
        koSetWithoutSend(KO_LOCK, ParamBRI_CHLockLocking ? locked : !locked);
        for (auto it = lockBridges.begin(); it != lockBridges.end(); ++it)
        {
            (*it)->setLocked(locked);
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();
    }
    else if (isKo(ko, KO_BLOCKED_FEEDBACK))
    {
        bool blocked = koGet(KO_BLOCKED_FEEDBACK);
        logDebugP("Blocked state changed to %s", blocked ? "true" : "false");
    
        for (auto it = lockBridges.begin(); it != lockBridges.end(); ++it)
        {
            (*it)->setBlocked(blocked);
        }
    }
}

std::string KnxChannelLock::currentValueAsString()
{
    return  isLocked() ? "Versperrt" : "Entsperrt";
}

bool KnxChannelLock::mainFunctionValue()
{
    return !isLocked();
}