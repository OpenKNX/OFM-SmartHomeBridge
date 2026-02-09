#ifndef SMARTHOMEBRIDGE_DEVICESONLY

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
        lockCurrentState->setVal(Characteristic::LockCurrentState::JAMMED);
    }
    else if (_unlocking)
    {
        lockCurrentState->setVal(Characteristic::LockCurrentState::LOCKED);
        _ignoreUpdate = true;
        lockTargetState->setVal(Characteristic::LockTargetState::UNLOCK);
        _ignoreUpdate = false;
    }
    else if (_locking)
    {
        lockCurrentState->setVal(Characteristic::LockCurrentState::UNLOCKED);
        _ignoreUpdate = true;
        lockTargetState->setVal(Characteristic::LockTargetState::LOCK);
        _ignoreUpdate = false;
    }
    else if (_locked)
    {
        _ignoreUpdate = true;
        lockTargetState->setVal(Characteristic::LockTargetState::LOCK);
        _ignoreUpdate = false;
        lockCurrentState->setVal(Characteristic::LockCurrentState::LOCKED);
    }
    else
    {
        _ignoreUpdate = true;
        lockTargetState->setVal(Characteristic::LockTargetState::UNLOCK);
        _ignoreUpdate = false;
        lockCurrentState->setVal(Characteristic::LockCurrentState::UNLOCKED);
    }
}
#endif
