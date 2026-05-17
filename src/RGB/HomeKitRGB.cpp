#if !defined(SMARTHOMEBRIDGE_DEVICESONLY) && defined(SMARTHOMEBRIDGE_HOMEKIT)

#include "HomeKitRGB.h"
#include "rgbconvert.h"

HomeKitRGB::HomeKitRGB(int device) :
    device(device)
{
}

void HomeKitRGB::setup(uint8_t _channelIndex)
{
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
    new ServiceImplementation(this);
        power = new Characteristic::On();
        brighness = new Characteristic::Brightness();
        saturation = new Characteristic::Saturation();
        hue = new Characteristic::Hue();
}

boolean HomeKitRGB::update()
{
    if (brighness->updated() || saturation->updated() || hue->updated())
    {
        hsv hsv;
        hsv.h = hue->getNewVal();
        hsv.s = ((double) saturation->getNewVal()) / 100.;
        hsv.v = ((double) brighness->getNewVal()) / 100.;

        auto rgb = hsv2rgb(hsv);
        auto r = (uint32_t)(rgb.r * 255.);
        auto g = (uint32_t)(rgb.g * 255.);
        auto b = (uint32_t)(rgb.b * 255.);

        uint32_t rgbValue = (r << 16) | (g << 8) | (b);
        _channel->commandRGB(this, rgbValue);
    }
    else if (power->updated())
        _channel->commandPower(this, power->getNewVal());
    return true;
}

void HomeKitRGB::setPower(bool on)
{
    power->setVal(on);
}

void HomeKitRGB::setRGB(uint32_t rgbValue)
{
    if (rgbValue == 0)
    {
        power->setVal(true);
    }
    else
    {
        rgb rgb;
        rgb.r = ((rgbValue & 0xFF0000) >> 16) / 255.;
        rgb.g = ((rgbValue & 0x00FF00) >> 8) / 255.;
        rgb.b = ((rgbValue & 0x0000FF)) / 255.;

        auto hsv = rgb2hsv(rgb);

        hue->setVal(hsv.h);
        saturation->setVal(hsv.s * 100.);
        brighness->setVal(hsv.v * 100.);

        power->setVal(true);
    }
}

#endif