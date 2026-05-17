#include "RGB/MatterRGB.h"

#include "MatterBridge.h"
#include "Matter/MatterBridgeCommon.h"

#include <esp_matter_bridge.h>

namespace
{
uint8_t clampHue(uint32_t rgb)
{
    auto red = static_cast<float>((rgb >> 16) & 0xFF) / 255.0f;
    auto green = static_cast<float>((rgb >> 8) & 0xFF) / 255.0f;
    auto blue = static_cast<float>(rgb & 0xFF) / 255.0f;
    auto maxValue = std::max({red, green, blue});
    auto minValue = std::min({red, green, blue});
    auto delta = maxValue - minValue;

    if (delta == 0.0f)
        return 0;

    float hue = 0.0f;
    if (maxValue == red)
        hue = 60.0f * std::fmod(((green - blue) / delta), 6.0f);
    else if (maxValue == green)
        hue = 60.0f * (((blue - red) / delta) + 2.0f);
    else
        hue = 60.0f * (((red - green) / delta) + 4.0f);

    if (hue < 0.0f)
        hue += 360.0f;

    return static_cast<uint8_t>(std::clamp(static_cast<int>(std::lround(hue * 254.0f / 360.0f)), 0, 254));
}

uint8_t clampSaturation(uint32_t rgb)
{
    auto red = static_cast<float>((rgb >> 16) & 0xFF) / 255.0f;
    auto green = static_cast<float>((rgb >> 8) & 0xFF) / 255.0f;
    auto blue = static_cast<float>(rgb & 0xFF) / 255.0f;
    auto maxValue = std::max({red, green, blue});
    auto minValue = std::min({red, green, blue});
    auto saturation = maxValue == 0.0f ? 0.0f : ((maxValue - minValue) / maxValue);
    return static_cast<uint8_t>(std::clamp(static_cast<int>(std::lround(saturation * 254.0f)), 0, 254));
}

uint32_t rgbFromHueSaturation(uint8_t hue, uint8_t saturation, uint8_t brightness)
{
    auto h = static_cast<float>(hue) * 360.0f / 254.0f;
    auto s = static_cast<float>(saturation) / 254.0f;
    auto v = static_cast<float>(brightness) / 254.0f;

    auto c = v * s;
    auto x = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
    auto m = v - c;

    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;

    if (h < 60.0f) { red = c; green = x; }
    else if (h < 120.0f) { red = x; green = c; }
    else if (h < 180.0f) { green = c; blue = x; }
    else if (h < 240.0f) { green = x; blue = c; }
    else if (h < 300.0f) { red = x; blue = c; }
    else { red = c; blue = x; }

    auto toByte = [m](float component) {
        return static_cast<uint8_t>(std::clamp(static_cast<int>(std::lround((component + m) * 255.0f)), 0, 255));
    };

    return (static_cast<uint32_t>(toByte(red)) << 16) |
           (static_cast<uint32_t>(toByte(green)) << 8) |
           static_cast<uint32_t>(toByte(blue));
}
}

MatterRGBBridge::MatterRGBBridge(MatterBridge *bridge) : MatterBridgeDeviceBase(bridge)
{
}

void MatterRGBBridge::setup(uint8_t _channelIndex)
{
    if (_bridge == nullptr || _bridge->node() == nullptr)
        return;

    _device = esp_matter_bridge::create_device(_bridge->node(), _bridge->parentEndpointId(),
                                               esp_matter::endpoint::extended_color_light::get_device_type_id(),
                                               static_cast<MatterBridgeDeviceBase *>(this));
    if (_device != nullptr)
    {
        matterbridge::setDeviceName(_device, _channel->getNameInUTF8());
        setPower(_channel->mainFunctionValue());
    }
}

void MatterRGBBridge::setPower(bool on)
{
    if (_device == nullptr)
        return;

    matterbridge::reportBool(_device->persistent_info.device_endpoint_id, matterbridge::onOffClusterId,
                             matterbridge::onOffAttrId, on);
}

void MatterRGBBridge::setRGB(uint32_t rgb)
{
    if (_device == nullptr)
        return;

    _hue = clampHue(rgb);
    _saturation = clampSaturation(rgb);
    _brightness = std::max<uint8_t>(1, static_cast<uint8_t>((((rgb >> 16) & 0xFF) + ((rgb >> 8) & 0xFF) + (rgb & 0xFF)) / 3));

    matterbridge::reportBool(_device->persistent_info.device_endpoint_id, matterbridge::onOffClusterId,
                             matterbridge::onOffAttrId, rgb != 0);
    matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::levelControlClusterId,
                           matterbridge::currentLevelAttrId, _brightness);
    matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::colorControlClusterId,
                           matterbridge::currentHueAttrId, _hue);
    matterbridge::reportU8(_device->persistent_info.device_endpoint_id, matterbridge::colorControlClusterId,
                           matterbridge::currentSaturationAttrId, _saturation);
}

void MatterRGBBridge::handleMatterAttribute(esp_matter::attribute::callback_type_t, uint32_t clusterId,
                                            uint32_t attributeId, esp_matter_attr_val_t *val)
{
    if (clusterId == matterbridge::onOffClusterId && attributeId == matterbridge::onOffAttrId)
    {
        _channel->commandPower(this, val->val.b);
    }
    else if (clusterId == matterbridge::levelControlClusterId && attributeId == matterbridge::currentLevelAttrId)
    {
        _brightness = val->val.u8;
        _channel->commandRGB(this, rgbFromHueSaturation(_hue, _saturation, _brightness));
    }
    else if (clusterId == matterbridge::colorControlClusterId)
    {
        if (attributeId == matterbridge::currentHueAttrId)
            _hue = val->val.u8;
        else if (attributeId == matterbridge::currentSaturationAttrId)
            _saturation = val->val.u8;

        _channel->commandRGB(this, rgbFromHueSaturation(_hue, _saturation, _brightness));
    }
}