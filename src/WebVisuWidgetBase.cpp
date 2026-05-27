#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuWidgetBase.h"

std::string WebVisuWidgetBase::widgetStyles()
{
    return R"CSS(<style>
.webvisu{padding:0.5rem 0;}
.webvisu h1{margin:0 0 0.75rem 0;}
.webvisu .meta{margin-bottom:1rem;color:var(--muted,#555);}
.webvisu-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(260px,1fr));gap:0.75rem;}
.webvisu-card{border:1px solid #d5d5d5;border-radius:8px;padding:0.9rem;background:#fff;display:flex;flex-direction:column;gap:0.6rem;box-shadow:0 1px 2px rgba(0,0,0,0.04);}
.webvisu-title{display:flex;justify-content:space-between;align-items:center;gap:0.75rem;font-weight:600;}
.webvisu-type{font-size:0.8rem;color:#666;background:#f1f1f1;border-radius:999px;padding:0.1rem 0.5rem;}
.webvisu-main{display:flex;align-items:center;gap:0.75rem;}
.webvisu-icon-wrap{width:52px;height:52px;border-radius:10px;background:transparent;display:flex;align-items:center;justify-content:center;border:none;flex:0 0 auto;}
.webvisu-icon{max-width:36px;max-height:36px;}
.webvisu-icon-recolor.webvisu-icon-state-on{filter:brightness(0) saturate(100%) invert(54%) sepia(74%) saturate(558%) hue-rotate(79deg) brightness(98%) contrast(96%);}
.webvisu-icon-recolor.webvisu-icon-state-off{filter:brightness(0) saturate(100%) opacity(1);}
.webvisu-value{font-size:1.25rem;font-weight:700;}
.webvisu-controls{display:flex;align-items:center;gap:0.6rem;flex-wrap:wrap;}
.webvisu-btn{border:1px solid #999;background:#fafafa;border-radius:6px;padding:0.35rem 0.75rem;cursor:pointer;font-size:0.95rem;}
.webvisu-btn:hover{background:#efefef;}
.webvisu-link{color:#215d8f;text-decoration:none;font-size:0.9rem;font-weight:600;}
.webvisu-link:hover{text-decoration:underline;}
.webvisu-slider{width:100%;accent-color:#3b6ea8;}
.webvisu-empty{padding:0.75rem;border:1px dashed #bbb;border-radius:8px;color:#666;background:#fafafa;}
@media (max-width: 640px){
    .webvisu-grid{grid-template-columns:1fr;}
}
    </style>)CSS";
}

std::string WebVisuWidgetBase::webVisuKind() const
{
    return "generic";
}

void WebVisuWidgetBase::setWebVisuName(const std::string& name)
{
    (void)name;
}

std::string WebVisuWidgetBase::webVisuOverviewHtml(uint8_t channelIndex) const
{
    (void)channelIndex;
    return "";
}

std::string WebVisuWidgetBase::webVisuDetailHtml(uint8_t channelIndex) const
{
    return webVisuOverviewHtml(channelIndex);
}

std::string WebVisuWidgetBase::webVisuJson(uint8_t channelIndex) const
{
    (void)channelIndex;
    return "";
}

bool WebVisuWidgetBase::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    (void)action;
    (void)message;
    return false;
}

std::string WebVisuWidgetBase::escapeHtml(const std::string& value)
{
    std::string escaped;
    escaped.reserve(value.size());
    for (char c : value)
    {
        switch (c)
        {
        case '&':
            escaped += "&amp;";
            break;
        case '<':
            escaped += "&lt;";
            break;
        case '>':
            escaped += "&gt;";
            break;
        case '"':
            escaped += "&quot;";
            break;
        default:
            escaped += c;
            break;
        }
    }
    return escaped;
}

std::string WebVisuWidgetBase::jsonEscape(const std::string& input)
{
    std::string escaped;
    escaped.reserve(input.size());

    for (char c : input)
    {
        switch (c)
        {
        case '\\':
            escaped += "\\\\";
            break;
        case '"':
            escaped += "\\\"";
            break;
        case '\n':
            escaped += "\\n";
            break;
        case '\r':
            escaped += "\\r";
            break;
        case '\t':
            escaped += "\\t";
            break;
        default:
            escaped += c;
            break;
        }
    }

    return escaped;
}

bool WebVisuWidgetBase::parseStringField(const std::string& message, const char* key, std::string& value)
{
    const std::string pattern = std::string("\"") + key + "\":";
    const size_t keyPos = message.find(pattern);
    if (keyPos == std::string::npos)
    {
        return false;
    }

    size_t valuePos = keyPos + pattern.size();
    while (valuePos < message.size() && (message[valuePos] == ' ' || message[valuePos] == '\t'))
    {
        valuePos++;
    }

    if (valuePos >= message.size() || message[valuePos] != '"')
    {
        return false;
    }

    valuePos++;
    std::string parsed;
    bool escaped = false;
    for (size_t i = valuePos; i < message.size(); ++i)
    {
        const char c = message[i];
        if (escaped)
        {
            switch (c)
            {
            case 'n': parsed += '\n'; break;
            case 'r': parsed += '\r'; break;
            case 't': parsed += '\t'; break;
            case '"': parsed += '"'; break;
            case '\\': parsed += '\\'; break;
            default: parsed += c; break;
            }
            escaped = false;
            continue;
        }

        if (c == '\\')
        {
            escaped = true;
            continue;
        }
        if (c == '"')
        {
            value = parsed;
            return true;
        }
        parsed += c;
    }

    return false;
}

bool WebVisuWidgetBase::parseIntField(const std::string& message, const char* key, int& value)
{
    const std::string pattern = std::string("\"") + key + "\":";
    const size_t keyPos = message.find(pattern);
    if (keyPos == std::string::npos)
    {
        return false;
    }

    size_t valuePos = keyPos + pattern.size();
    while (valuePos < message.size() && (message[valuePos] == ' ' || message[valuePos] == '\t'))
    {
        valuePos++;
    }

    size_t endPos = valuePos;
    while (endPos < message.size() && (message[endPos] == '-' || (message[endPos] >= '0' && message[endPos] <= '9')))
    {
        endPos++;
    }

    if (endPos == valuePos)
    {
        return false;
    }

    try
    {
        value = std::stoi(message.substr(valuePos, endPos - valuePos));
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool WebVisuWidgetBase::parseDoubleField(const std::string& message, const char* key, double& value)
{
    const std::string pattern = std::string("\"") + key + "\":";
    const size_t keyPos = message.find(pattern);
    if (keyPos == std::string::npos)
    {
        return false;
    }

    size_t valuePos = keyPos + pattern.size();
    while (valuePos < message.size() && (message[valuePos] == ' ' || message[valuePos] == '\t'))
    {
        valuePos++;
    }

    size_t endPos = valuePos;
    while (endPos < message.size() && (message[endPos] == '-' || message[endPos] == '+' || message[endPos] == '.' || (message[endPos] >= '0' && message[endPos] <= '9') || message[endPos] == 'e' || message[endPos] == 'E'))
    {
        endPos++;
    }

    if (endPos == valuePos)
    {
        return false;
    }

    try
    {
        value = std::stod(message.substr(valuePos, endPos - valuePos));
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool WebVisuWidgetBase::parseBoolField(const std::string& message, const char* key, bool& value)
{
    const std::string pattern = std::string("\"") + key + "\":";
    const size_t keyPos = message.find(pattern);
    if (keyPos == std::string::npos)
    {
        return false;
    }

    size_t valuePos = keyPos + pattern.size();
    while (valuePos < message.size() && (message[valuePos] == ' ' || message[valuePos] == '\t'))
    {
        valuePos++;
    }

    if (message.compare(valuePos, 4, "true") == 0)
    {
        value = true;
        return true;
    }
    if (message.compare(valuePos, 5, "false") == 0)
    {
        value = false;
        return true;
    }
    return false;
}

std::string WebVisuWidgetBase::renderCard(uint8_t channelIndex,
                                          const std::string& name,
                                          const std::string& typeLabel,
                                          const std::string& valueText,
                                          const std::string& controlsHtml,
                                          const std::string& detailHtml)
{
    const int channelOneBased = (int)channelIndex + 1;

    std::string html;
    html += "<article class=\"webvisu-card\">";
    html += "<div class=\"webvisu-title\"><span>" + escapeHtml(name) + "</span><span class=\"webvisu-type\">";
    html += typeLabel + " #" + std::to_string(channelOneBased);
    html += "</span></div>";
    html += "<div class=\"webvisu-value\">" + valueText + "</div>";
    html += "<div class=\"webvisu-controls\">" + controlsHtml + "</div>";
    html += detailHtml;
    html += "</article>";
    return html;
}

std::string WebVisuWidgetBase::renderGenericCard(uint8_t channelIndex,
                                                 const std::string& name,
                                                 const std::string& typeLabel,
                                                 const std::string& valueText,
                                                 const std::string& imageUrl,
                                                 bool allowMainAction,
                                                 const std::string& detailUrl,
                                                 bool state,
                                                 bool allowRecolor)
{
    const int channelOneBased = (int)channelIndex + 1;

    std::string controls;
    if (allowMainAction)
    {
        controls += "<button class=\"webvisu-btn webvisu-mainaction\" data-wv-payload='{\"action\":\"toggle\",\"channel\":";
        controls += std::to_string(channelOneBased);
        controls += "}'>Aktion</button>";
    }
    if (!detailUrl.empty())
    {
        controls += "<a class=\"webvisu-link\" href=\"";
        controls += escapeHtml(detailUrl);
        controls += "\">Details</a>";
    }

    std::string imageClass = "webvisu-icon";
    if (allowRecolor)
    {
        imageClass += " webvisu-icon-recolor ";
        imageClass += state ? "webvisu-icon-state-on" : "webvisu-icon-state-off";
    }

    std::string mainHtml;
    mainHtml += "<div class=\"webvisu-main\">";
    mainHtml += "<div class=\"webvisu-icon-wrap\"><img class=\"" + escapeHtml(imageClass) + "\" data-src=\"" + escapeHtml(imageUrl) + "\" alt=\"" + escapeHtml(name) + "\" loading=\"lazy\"></div>";
    mainHtml += "<div class=\"webvisu-value\">" + escapeHtml(valueText) + "</div>";
    mainHtml += "</div>";

    return renderCard(channelIndex, name, typeLabel, mainHtml, controls);
}

#endif
