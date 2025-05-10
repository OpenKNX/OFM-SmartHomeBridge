#ifndef SMARTHOMEBRIDGE_DEVICESONLY  

#include "HueMedia.h"

HueMedia::HueMedia(HueBridge* hueBridge)
: hueBridge(hueBridge)
{
    
}

void HueMedia::setup(uint8_t _channelIndex)
{
    espalexaDevice = new EspalexaDevice(_channel->getNameInUTF8(), [this](EspalexaDevice* d){update();}, EspalexaDeviceType::dimmable, 0);
    espalexaDevice->setState(false);
    hueBridge->espalexa.addDevice(_channel->channelIndex(), espalexaDevice);
}

boolean HueMedia::update()
{
    switch (espalexaDevice->getLastChangedProperty())
    {
        case EspalexaDeviceProperty::on:
            _channel->commandPower(this, true);
            break;
        case EspalexaDeviceProperty::off:
            _channel->commandPower(this, false);
            break;
         case EspalexaDeviceProperty::bri:
            _channel->commandBrightness(this, espalexaDevice->getPercent());
            break;
    }
    return (true);
}

void HueMedia::setBrightness(uint8_t brightness)
{
    espalexaDevice->setPercent(brightness);
    if (brightness == 0)
    {
        espalexaDevice->setState(false);
    }
    else
    {
        espalexaDevice->setState(true);
    }
}

#endif
