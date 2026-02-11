#include <knx.h>
#include "knxprod.h"
#include "KnxChannelLock.h"
#include "BridgeBase.h"

#define KO_UNLOCK KoBRI_KO1_, DPT_Switch
#define KO_UNLOCK_FEEDBACK KoBRI_KO2_, DPT_Switch
#define KO_BLOCKED_FEEDBACK KoBRI_KO3_, DPT_Switch
#define KO_UNLOCKING_FEEDBACK KoBRI_KO4_, DPT_Switch
#define KO_LOCKING_FEEDBACK KoBRI_KO5_, DPT_Switch

KnxChannelLock::KnxChannelLock(uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      lockBridges()
{
}

ChannelBridge *KnxChannelLock::createBridgeDevice(BridgeBase &bridge)
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
    if (isLocked())
    {
        logDebugP("Received click. Unlocking.");
        commandLock(nullptr, false);
    }
    else
    {
        logDebugP("Received click. Locking.");
        commandLock(nullptr, true);
    }
}


void KnxChannelLock::commandLock(LockBridge *lockBridge, bool lock)
{
    bool value;
    // <Enumeration Value="0" Id="%ENID%" Text="Verriegeln=0 / Entriegeln=1"  />
    // <Enumeration Value="1" Id="%ENID%" Text="Entriegeln=0 / Verriegeln=1"     />
    if (ParamBRI_CHLockLocking)
        value = lock;
    else
        value = !lock;
    koSet(KO_UNLOCK, value, true);
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
    koSetWithoutSend(KO_UNLOCK, ParamBRI_CHLockLocking ? false : true);
    // <Enumeration Value="0" Id="%ENID%" Text="Verriegelt=0 / Entriegelt=1"  />
    // <Enumeration Value="1" Id="%ENID%" Text="Entriegelt=0 / Verriegelt=1"     />
    koSendReadRequest(KO_UNLOCK_FEEDBACK);
    koSetWithoutSend(KO_UNLOCK_FEEDBACK, ParamBRI_CHLockLocked ? false : true);
    koSendReadRequest(KO_BLOCKED_FEEDBACK);
    koSetWithoutSend(KO_BLOCKED_FEEDBACK, false);
    if (ParamBRI_CHLockOperation)
    {
        koSendReadRequest(KO_UNLOCKING_FEEDBACK);
        koSendReadRequest(KO_LOCKING_FEEDBACK);
    }
}

bool KnxChannelLock::isLocked()
{
    bool unlocked = koGet(KO_UNLOCK_FEEDBACK);
    // <Enumeration Value="0" Id="%ENID%" Text="Verriegelt=0 / Entriegelt=1"  />
    // <Enumeration Value="1" Id="%ENID%" Text="Entriegelt=0 / Verriegelt=1"     />
    if (ParamBRI_CHLockLocked)
        unlocked = !unlocked;
    return !unlocked;
}

void KnxChannelLock::processInputKo(GroupObject &ko)
{
    if (isKo(ko, KO_UNLOCK_FEEDBACK))
    {
        auto locked = isLocked();
        _tempUnlockedUntil = 0;
        _tempLockedUntil = 0;
        koSetWithoutSend(KO_UNLOCK, ParamBRI_CHLockLocking ? locked : !locked);
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
        for (auto it = lockBridges.begin(); it != lockBridges.end(); ++it)
        {
            (*it)->setBlocked(blocked);
        }
    }
    else if (isKo(ko, KO_UNLOCKING_FEEDBACK))
    {
        bool unlocking = koGet(KO_UNLOCKING_FEEDBACK);
        if (unlocking != _unlocking)
        {
             _unlocking = unlocking;
            if (!_unlocking && isLocked())
            {
                _tempUnlockedUntil = max(1UL, millis());
                for (auto it = lockBridges.begin(); it != lockBridges.end(); ++it)
                {
                    (*it)->setLocked(false);
                }
            }
            for (auto it = lockBridges.begin(); it != lockBridges.end(); ++it)
            {
                (*it)->setUnlocking(unlocking);
                (*it)->mainFunctionValueChanged();
            }
            mainFunctionValueChanged();
        }
    }
    else if (isKo(ko, KO_LOCKING_FEEDBACK))
    {
        bool locking = koGet(KO_LOCKING_FEEDBACK);
        if (locking != _locking)
        {
             _locking = locking;
            if (!_locking && !isLocked())
            {
                _tempLockedUntil = max(1UL, millis());
                for (auto it = lockBridges.begin(); it != lockBridges.end(); ++it)
                {
                    (*it)->setLocked(false);
                }
            }
            for (auto it = lockBridges.begin(); it != lockBridges.end(); ++it)
            {
                (*it)->setLocking(locking);
                (*it)->mainFunctionValueChanged();
            }
            mainFunctionValueChanged();
        }
    }
}

std::string KnxChannelLock::currentValueAsString()
{
    // <Enumeration Value="0" Id="%ENID%" Text="Entriegelt / Verriegelt"  />
    // <Enumeration Value="1" Id="%ENID%" Text="Entsperrt / Versperrt"  />
    // <Enumeration Value="2" Id="%ENID%" Text="Offen / Zu"  />
    switch (ParamBRI_CHLockDisplay)
    {
        case 1:
            if (_locking)
                return "Versperren";
            if (_unlocking)           
                return "Entsperren";
            return isLocked() ? "Versperrt" : "Entsperrt";
        case 2:
            if (_locking)
                return "Schließen";
            if (_unlocking)
                return "Öffnen";
            return isLocked() ? "Zu" : "Offen";
        default:
            if (_locking)
                return "Verriegeln";
            if (_unlocking)           
                return "Entriegeln";
            return isLocked() ? "Verriegelt" : "Entriegelt";
    }
}

bool KnxChannelLock::mainFunctionValue()
{
    return !isLocked();
}

void KnxChannelLock::loop()
{
    KnxChannelBase::loop();
    if (_tempUnlockedUntil != 0 && millis() - _tempUnlockedUntil > 2000)
    {
        _tempUnlockedUntil = 0;
        for (auto it = lockBridges.begin(); it != lockBridges.end(); ++it)
        {
            (*it)->setLocked(isLocked());
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();
    }
    if (_tempLockedUntil != 0 && millis() - _tempLockedUntil > 2000)
    {
        _tempLockedUntil = 0;
        for (auto it = lockBridges.begin(); it != lockBridges.end(); ++it)
        {
            (*it)->setLocked(isLocked());
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();
    }
}

MainFunctionStateImage KnxChannelLock::mainFunctionImage()
{
   return calculateMainFunctionImage((_locking || _unlocking) ? 50 : isLocked() ? 0 : 100, 0, 50, 100);
}

MainFunctionStateImage KnxChannelLock::mainFunctionTypeImage(uint8_t value)
{
    if (value > 0)
    {
        std::string image = "Type";
        image += std::to_string(ParamBRI_CHDeviceType);
        image += "_A.png";
        return {true, image};
    }
    return KnxChannelBase::mainFunctionTypeImage(value);
}