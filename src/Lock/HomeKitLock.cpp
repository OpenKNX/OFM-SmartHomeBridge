#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HomeKitLock.h"

HomeKitLock::HomeKitLock(int device) :
    device(device)
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
        logDebugP("HomeKit Lock target state changed to %s", lockTargetState->getNewVal() == Characteristic::LockTargetState::LOCK ? "LOCK" : "UNLOCK");
        _channel->commandLock(this, lockTargetState->getNewVal() == Characteristic::LockTargetState::LOCK);
    }
    return (true);
}

void HomeKitLock::setLocked(bool locked)
{
    _locked = locked;
    _ignoreUpdate = true;
    lockTargetState->setVal(locked ? Characteristic::LockTargetState::LOCK : Characteristic::LockTargetState::UNLOCK);
    _ignoreUpdate = false;

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
void HomeKitLock::updateState()
{
    if (_blocked)
    {
        logDebugP("Lock is blocked");
        lockCurrentState->setVal(Characteristic::LockCurrentState::JAMMED);
    }
    else if (_locked)
    {
        logDebugP("Lock is locked");   
        lockCurrentState->setVal(Characteristic::LockCurrentState::LOCKED);
    }
    else        
    {
        logDebugP("Lock is unlocked"); 
        lockCurrentState->setVal(Characteristic::LockCurrentState::UNLOCKED);
    }
}
#endif
