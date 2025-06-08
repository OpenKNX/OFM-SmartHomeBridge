#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HomeKitMedia.h"
#if 0
HomeKitMedia::HomeKitMedia(int device) :
    device(device)
{
}

void HomeKitMedia::setup(uint8_t _channelIndex)
{
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
    new ServiceImplementation(this);
        power = new Characteristic::On();
        level = new Characteristic::Brightness();
}

boolean HomeKitMedia::update()
{
    if (level->updated())
        _channel->commandBrightness(this, level->getNewVal());
    else if (power->updated())
        _channel->commandPower(this, power->getNewVal());
    return true;
}


void HomeKitMedia::setBrightness(uint8_t brightness)
{
    if (brightness < 0)
        brightness = 0;
    if (brightness > 100)
        brightness = 100;

    if (brightness == 0)
    {
        power->setVal(false);
    }
    else
    {
        level->setVal(brightness);
        power->setVal(true);
    }
}

#endif
#endif
