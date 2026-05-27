#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <cstdint>
#include <string>

class WebVisuWidgetBase
{
    public:
        static std::string widgetStyles();
        virtual ~WebVisuWidgetBase() = default;

        virtual std::string webVisuKind() const;
        virtual void setWebVisuName(const std::string& name);
        virtual std::string webVisuOverviewHtml(uint8_t channelIndex) const;
        virtual std::string webVisuDetailHtml(uint8_t channelIndex) const;
        virtual std::string webVisuJson(uint8_t channelIndex) const;
        virtual bool webVisuHandleCommand(const std::string& action, const std::string& message);

    protected:
        static std::string escapeHtml(const std::string& value);

        static std::string jsonEscape(const std::string& input);

        static bool parseStringField(const std::string& message, const char* key, std::string& value);
        static bool parseIntField(const std::string& message, const char* key, int& value);
        static bool parseDoubleField(const std::string& message, const char* key, double& value);
        static bool parseBoolField(const std::string& message, const char* key, bool& value);

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
