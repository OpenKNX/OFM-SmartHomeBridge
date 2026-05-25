#pragma once
#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include <string>

class WebVisuWidgetBase
{
    public:
        static std::string widgetStyles()
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
        .webvisu-icon-wrap{width:52px;height:52px;border-radius:10px;background:#f7f7f7;display:flex;align-items:center;justify-content:center;border:1px solid #e1e1e1;flex:0 0 auto;}
        .webvisu-icon{max-width:36px;max-height:36px;}
        .webvisu-icon-recolor.webvisu-icon-state-on{filter:brightness(0) saturate(100%) invert(36%) sepia(55%) saturate(1873%) hue-rotate(103deg) brightness(94%) contrast(96%);}
        .webvisu-icon-recolor.webvisu-icon-state-off{filter:grayscale(1) opacity(0.72);}
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

    protected:
    static std::string escapeHtml(const std::string& value)
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

    static std::string jsonEscape(const std::string& input)
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

    static std::string renderCard(uint8_t channelIndex,
                                  const std::string& name,
                                  const std::string& typeLabel,
                                  const std::string& valueText,
                                  const std::string& controlsHtml,
                                  const std::string& detailHtml = "")
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

    public:
    static std::string renderGenericCard(uint8_t channelIndex,
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
            controls += "<button class=\"webvisu-btn webvisu-mainaction\" data-action=\"toggle\" data-channel=\"";
            controls += std::to_string(channelOneBased);
            controls += "\">Aktion</button>";
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
        mainHtml += "<div class=\"webvisu-icon-wrap\"><img class=\"" + escapeHtml(imageClass) + "\" src=\"" + escapeHtml(imageUrl) + "\" alt=\"" + escapeHtml(name) + "\" loading=\"lazy\"></div>";
        mainHtml += "<div class=\"webvisu-value\">" + escapeHtml(valueText) + "</div>";
        mainHtml += "</div>";

        return renderCard(channelIndex, name, typeLabel, mainHtml, controls);
    }
};

#endif
