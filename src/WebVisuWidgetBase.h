#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <cstdint>
#include <string>

class WebVisuWidgetBase
{
    public:
        static std::string widgetStyles();

    protected:
    static std::string escapeHtml(const std::string& value);

    static std::string jsonEscape(const std::string& input);

    static std::string renderCard(uint8_t channelIndex,
                                  const std::string& name,
                                  const std::string& typeLabel,
                                  const std::string& valueText,
                                  const std::string& controlsHtml,
                                  const std::string& detailHtml = "");

    public:
    static std::string renderGenericCard(uint8_t channelIndex,
                                         const std::string& name,
                                         const std::string& typeLabel,
                                         const std::string& valueText,
                                         const std::string& imageUrl,
                                         bool allowMainAction,
                                         const std::string& detailUrl,
                                         bool state,
                                         bool allowRecolor);
};

#endif
