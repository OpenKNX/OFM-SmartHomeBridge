#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HueScene.h"

HueScene::HueScene(HueBridge* hueBridge)
: hueBridge(hueBridge)
{
    
}

void HueScene::setup(uint8_t _channelIndex)
{
    espalexaDevice = new EspalexaDevice(_channel->getNameInUTF8(), [this](EspalexaDevice* d){update();}, EspalexaDeviceType::onoff);
    espalexaDevice->setState(false);
    hueBridge->espalexa.addDevice(_channel->channelIndex(), espalexaDevice);
}

boolean HueScene::update()
{
    switch (espalexaDevice->getLastChangedProperty())
    {
        case EspalexaDeviceProperty::on:
            _channel->commandActivate(this);
            break;
        case EspalexaDeviceProperty::off:
            break;
    }
    return true;
}

void HueScene::setActivating(bool activating)
{
    espalexaDevice->setState(activating);
}

#endif
