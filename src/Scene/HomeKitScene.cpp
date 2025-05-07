#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HomeKitScene.h"

HomeKitScene::HomeKitScene(int device) :
    device(device)
{
}


void HomeKitScene::setup(uint8_t _channelIndex)
{
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
    new ServiceImplementationSwitch(this);
    power = new Characteristic::On();
}

boolean HomeKitScene::update()
{
    if (power->updated() &&power->getNewVal())
        _channel->commandActivate(this);
    return (true);
}

void HomeKitScene::setActivating(bool activating)
{
    power->setVal(value);
}
#endif
