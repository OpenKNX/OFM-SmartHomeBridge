#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuScene.h"

#include "../WebVisuBridge.h"

WebVisuScene::WebVisuScene(WebVisuBridge* webVisuBridge)
    : _webVisuBridge(webVisuBridge)
{
}

void WebVisuScene::setWebVisuName(const std::string& name)
{
    _name = name;
}

std::string WebVisuScene::webVisuKind() const
{
    return "scene";
}

void WebVisuScene::setActivating(bool activating)
{
    _activating = activating;
    if (_channel != nullptr && _webVisuBridge != nullptr)
    {
        _webVisuBridge->broadcastChannelUpdate(_channel->channelIndex());
    }
}

std::string WebVisuScene::webVisuOverviewHtml(uint8_t channelIndex) const
{
    return webVisuDetailHtml(channelIndex);
}

std::string WebVisuScene::webVisuDetailHtml(uint8_t channelIndex) const
{
    const std::string stateText = _activating ? "Aktiv" : "Bereit";
    std::string controls;
    controls += "<button class=\"webvisu-btn\" data-wv-payload='{";
    controls += "\"action\":\"activateScene\",\"channel\":" + std::to_string((int)channelIndex + 1);
    controls += "}'>Auslösen</button>";
    return renderCard(channelIndex, _name, "Scene", stateText, controls);
}

std::string WebVisuScene::webVisuJson(uint8_t channelIndex) const
{
    std::string json = "{";
    json += "\"kind\":\"scene\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(_name) + "\",";
    json += "\"activating\":" + std::string(_activating ? "true" : "false") + ",";
    json += "\"html\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(webVisuDetailHtml(channelIndex)) + "\"";
    json += "}";
    return json;
}

bool WebVisuScene::webVisuHandleCommand(const std::string& action, const std::string& message)
{
    (void)message;
    if (action != "activateScene" || _channel == nullptr)
    {
        return false;
    }

    _channel->commandActivate(this);
    return true;
}

#endif