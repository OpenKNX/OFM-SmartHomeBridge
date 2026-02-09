#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HueLock.h"

HueLock::HueLock(HueBridge* hueBridge)
: hueBridge(hueBridge)
{
    
}

void HueLock::setup(uint8_t _channelIndex)
{
    espalexaDevice = new EspalexaDevice(_channel->getNameInUTF8(), [this](EspalexaDevice* d){update();}, EspalexaDeviceType::onoff);
    espalexaDevice->setState(false);
    hueBridge->espalexa.addDevice(_channel->channelIndex(), espalexaDevice);
}

boolean HueLock::update()
{
    switch (espalexaDevice->getLastChangedProperty())
    {
        case EspalexaDeviceProperty::on:
            _channel->commandLock(this, true);
            break;
        case EspalexaDeviceProperty::off:
            _channel->commandLock(this, false);
            break;
    }
    return true;
}

void HueLock::setLocked(bool locked)
{
    espalexaDevice->setState(locked);
}

void HueLock::setBlocked(bool blocked)
{
    // Not supported
}

void HueLock::setUnlocking(bool unlocking)
{
    // Not supported
}

void HueLock::setLocking(bool locking)
{
    // Not supported
}

#endif
