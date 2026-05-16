#if !defined(SMARTHOMEBRIDGE_DEVICESONLY) && defined(SMARTHOMEBRIDGE_HOMEKIT)

#include "HomeKitLock.h"

HomeKitLock::HomeKitLock(int device) : device(device)
{
}

void HomeKitLock::setup(uint8_t _channelIndex)
{
    new SpanAccessory(device);
    new Service::AccessoryInformation();
    new Characteristic::Identify();
    new Characteristic::Name(_channel->getNameInUTF8());
    new ServiceImplementationLock(this);

    lockCurrentState = new Characteristic::LockCurrentState();
    lockCurrentState->setVal(Characteristic::LockCurrentState::UNLOCKED);
    lockTargetState = new Characteristic::LockTargetState();
    lockCurrentState->setVal(Characteristic::LockCurrentState::UNLOCKED);
}

boolean HomeKitLock::update()
{
    if (lockTargetState->updated() && !_ignoreUpdate)
    {
        _channel->commandLock(this, lockTargetState->getNewVal() == Characteristic::LockTargetState::LOCK);
    }
    return (true);
}

void HomeKitLock::setLocked(bool locked)
{
    _locked = locked;
    updateState();
    if (!_targetStateInitialized)
    {
        // Initialize target state to current state on first update
        _targetStateInitialized = true;
    }
}
void HomeKitLock::setBlocked(bool blocked)
{
    _blocked = blocked;
    updateState();
}

void HomeKitLock::setUnlocking(bool unlocking)
{
    _unlocking = unlocking;
    updateState();
}

void HomeKitLock::setLocking(bool locking)
{
    _locking = locking;
    updateState();
}

void HomeKitLock::updateState()
{

    if (_blocked)
    {
        logDebug("Lock", "Lock is blocked");
        lockCurrentState->setVal(Characteristic::LockCurrentState::JAMMED);
    }
    else if (_unlocking)
    {
        logDebug("Lock", "Lock is unlocking");
        updateTargetState(Characteristic::LockTargetState::UNLOCK);
        lockCurrentState->setVal(Characteristic::LockCurrentState::LOCKED);
    }
    else if (_locking)
    {
        logDebug("Lock", "Lock is locking");
        updateTargetState(Characteristic::LockTargetState::LOCK);
        lockCurrentState->setVal(Characteristic::LockCurrentState::UNLOCKED);
       
    }
    else if (_locked)
    {
        logDebug("Lock", "Lock is locked");
        updateTargetState(Characteristic::LockTargetState::LOCK);
        lockCurrentState->setVal(Characteristic::LockCurrentState::LOCKED);
    }
    else
    {
        logDebug("Lock", "Lock is unlocked");
        updateTargetState(Characteristic::LockTargetState::UNLOCK);
        lockCurrentState->setVal(Characteristic::LockCurrentState::UNLOCKED);
    }
}

void HomeKitLock::updateTargetState(Characteristic::LockTargetState::Value_t targetState)
{
     if (lockTargetState->getVal<Characteristic::LockTargetState::Value_t>() != targetState)
     {
        _ignoreUpdate = true;
        lockTargetState->setVal(targetState);
        _ignoreUpdate = false;
     }
}

#endif
