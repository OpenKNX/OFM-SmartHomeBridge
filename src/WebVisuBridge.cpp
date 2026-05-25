#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuBridge.h"

#include <algorithm>
#include <cctype>

#include <NetworkModule.h>

#include "Switch/KnxChannelSwitch.h"
#include "Switch/WebVisuSwitch.h"
#include "Dimmer/KnxChannelDimmer.h"
#include "Dimmer/WebVisuDimmer.h"
#include "WebVisuWidgetBase.h"

SwitchBridge* WebVisuBridge::createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuSwitch(this);
    channel.add(bridge);
    return bridge;
}

DimmerBridge* WebVisuBridge::createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuDimmer(this);
    channel.add(bridge);
    return bridge;
}

const std::string WebVisuBridge::name()
{
    return "WebVisuBridge";
}

void WebVisuBridge::initialize(SmartHomeBridgeModule* bridge)
{
    _bridge = bridge;
}

void WebVisuBridge::loop()
{
    processPendingCommands();
}

void WebVisuBridge::registerWebPages()
{
#ifdef OPENKNX_WEBSERVER
    openknxNetwork.webserver.addMenuItem("Ger\xC3\xA4te", MENU_URI, 52);
    openknxNetwork.webserver.addRoute(OpenKNX::Network::WEB_GET, MENU_URI,
                                      [this](OpenKNX::Network::WebRequest&, OpenKNX::Network::WebResponse& res) {
                                          std::string html = buildPageHtml();
                                          res.setLayout(true);
                                          res.setActiveMenu(MENU_URI);
                                          res.send(html.c_str());
                                      });

    openknxNetwork.webserver.addSocket(
        SOCKET_URI,
        [this](int clientId, OpenKNX::Network::WebSocketFrame* frame) {
            queueCommand(frame->data, frame->length);
        },
        [this](int clientId, bool connected) {
            if (connected)
            {
                sendSnapshotToClient(clientId);
            }
        });
#endif
}

bool WebVisuBridge::processCommand(const std::string cmd, bool diagnoseKo)
{
    return false;
}

void WebVisuBridge::showHelp()
{
}

void WebVisuBridge::queueCommand(const uint8_t* data, int length)
{
    if (data == nullptr || length <= 0)
        return;

    int safeLen = std::min(length, 512);
    std::string message;
    message.reserve(safeLen);

    for (int i = 0; i < safeLen; ++i)
    {
        uint8_t c = data[i];
        if ((c >= 0x20 && c <= 0x7E) || c == '\t' || c == '\n' || c == '\r')
        {
            message.push_back((char)c);
        }
    }

    if (!message.empty())
    {
        _pendingCommands.push_back(message);
    }
}

void WebVisuBridge::processPendingCommands()
{
    if (_pendingCommands.empty())
        return;

    std::vector<std::string> commands;
    commands.swap(_pendingCommands);

    for (const std::string& command : commands)
    {
        processCommandMessage(command);
    }
}

void WebVisuBridge::processCommandMessage(const std::string& message)
{
    std::string action;
    if (!parseStringField(message, "action", action))
    {
        return;
    }

    int channelOneBased = 0;
    if (!parseIntField(message, "channel", channelOneBased))
    {
        return;
    }

    if (channelOneBased <= 0)
    {
        return;
    }

    if (_bridge == nullptr)
    {
        return;
    }

    const uint8_t channelIndex = (uint8_t)(channelOneBased - 1);
    KnxChannelBase* baseChannel = _bridge->getChannel(channelIndex);
    if (baseChannel == nullptr)
    {
        return;
    }

    const std::string kind = baseChannel->name();

    if (action == "toggle")
    {
        if (kind == "Switch" || kind == "Dimmer")
        {
            baseChannel->commandMainFunctionClick();
        }
        return;
    }

    if (action == "setSwitch" && kind == "Switch")
    {
        bool power = false;
        if (!parseBoolField(message, "power", power))
        {
            return;
        }
        ((KnxChannelSwitch*)baseChannel)->commandPower(nullptr, power);
        return;
    }

    if (action == "setDimmer" && kind == "Dimmer")
    {
        int brightness = 0;
        if (!parseIntField(message, "brightness", brightness))
        {
            return;
        }

        if (brightness < 0)
            brightness = 0;
        if (brightness > 100)
            brightness = 100;

        ((KnxChannelDimmer*)baseChannel)->commandBrightness(nullptr, (uint8_t)brightness);
        return;
    }
}

std::string WebVisuBridge::buildPageHtml() const
{
        std::string html = "<div class='webvisu'>";
        html += WebVisuWidgetBase::widgetStyles();
        html += R"HTML(
    <h1>Ger&auml;te</h1>
  <div id='webvisu-meta' class='meta'>Verbinde...</div>
  <div id='webvisu-grid' class='webvisu-grid'></div>
  <script>
    (function(){
      const grid = document.getElementById('webvisu-grid');
      const meta = document.getElementById('webvisu-meta');
      const devices = {};
      let ws = null;
      let reconnectTimer = null;

      function send(payload){
        if (ws && ws.readyState === 1){
          ws.send(JSON.stringify(payload));
        }
      }

      function render(){
        const entries = Object.values(devices).sort((a,b) => Number(a.channel) - Number(b.channel));
        meta.textContent = ws && ws.readyState === 1 ? 'Live verbunden' : 'Nicht verbunden';
        if (entries.length === 0){
          grid.innerHTML = '<div class="webvisu-empty">Noch keine unterst\u00fctzten Ger\u00e4te gefunden. V1 zeigt Switch und Dimmer.</div>';
          return;
        }

                grid.innerHTML = entries.map(device => device.html || '').join('');
      }

      function scheduleReconnect(){
        if (reconnectTimer){
          return;
        }
        reconnectTimer = setTimeout(() => {
          reconnectTimer = null;
          connect();
        }, 1500);
      }

      function connect(){
        const proto = location.protocol === 'https:' ? 'wss://' : 'ws://';
        ws = new WebSocket(proto + location.host + '/geraete/ws');

        ws.onopen = () => {
          render();
        };

        ws.onclose = () => {
          render();
          scheduleReconnect();
        };

        ws.onerror = () => {
          render();
        };

        ws.onmessage = (event) => {
          let payload = null;
          try {
            payload = JSON.parse(event.data);
          } catch (e) {
            return;
          }

          if (payload.type === 'snapshot' && Array.isArray(payload.devices)){
            Object.keys(devices).forEach(k => delete devices[k]);
            payload.devices.forEach(device => {
              devices[String(device.channel)] = device;
            });
            render();
            return;
          }

          if (payload.type === 'update' && payload.device && payload.device.channel){
            devices[String(payload.device.channel)] = payload.device;
            render();
          }
        };
      }

      grid.addEventListener('click', (event) => {
        const target = event.target;
        if (!(target instanceof HTMLElement)) return;
        const action = target.getAttribute('data-action');
        const channel = Number(target.getAttribute('data-channel'));
        if (!action || !channel) return;

        if (action === 'toggle'){
          send({ action: 'toggle', channel: channel });
        }
      });

      grid.addEventListener('change', (event) => {
        const target = event.target;
        if (!(target instanceof HTMLInputElement)) return;
        const action = target.getAttribute('data-action');
        const channel = Number(target.getAttribute('data-channel'));
        if (action !== 'setDimmer' || !channel) return;

        send({ action: 'setDimmer', channel: channel, brightness: Number(target.value) });
      });

      connect();
      render();
    })();
  </script>
</div>)HTML";
        return html;
}

std::string WebVisuBridge::buildSnapshotMessage() const
{
    std::string json = "{\"type\":\"snapshot\",\"devices\":[";
    if (_bridge == nullptr)
    {
        json += "]}";
        return json;
    }

    bool first = true;
    const uint16_t channels = _bridge->getNumberOfUsedChannels();
    for (uint16_t idx = 0; idx < channels; ++idx)
    {
        KnxChannelBase* baseChannel = _bridge->getChannel((uint8_t)idx);
        if (baseChannel == nullptr)
            continue;

        const char* channelName = baseChannel->getNameInUTF8();
        const std::string name = channelName == nullptr ? "Unbenannt" : std::string(channelName);
        std::string deviceJson;

        const std::string type = baseChannel->name();
        if (type == "Switch")
        {
            const bool power = baseChannel->mainFunctionValue();
            deviceJson = WebVisuSwitch::buildDeviceJson((uint8_t)idx, name, power);
        }
        else if (type == "Dimmer")
        {
            const std::string value = baseChannel->currentValueAsString();
            int brightnessInt = atoi(value.c_str());
            if (brightnessInt < 0)
                brightnessInt = 0;
            if (brightnessInt > 100)
                brightnessInt = 100;

            deviceJson = WebVisuDimmer::buildDeviceJson((uint8_t)idx, name, (uint8_t)brightnessInt);
        }
        else
        {
            continue;
        }

        if (!first)
            json += ",";
        first = false;
        json += deviceJson;
    }
    json += "]}";
    return json;
}

void WebVisuBridge::sendSnapshotToClient(int clientId)
{
#ifdef OPENKNX_WEBSERVER
    std::string snapshot = buildSnapshotMessage();
    openknxNetwork.webserver.sendWebsocketMessage(SOCKET_URI, snapshot.c_str(), clientId);
#endif
}

void WebVisuBridge::broadcastUpdate(const std::string& deviceJson)
{
#ifdef OPENKNX_WEBSERVER
    std::string update = std::string("{\"type\":\"update\",\"device\":") + deviceJson + "}";
    openknxNetwork.webserver.sendWebsocketMessage(SOCKET_URI, update.c_str());
#endif
}

bool WebVisuBridge::parseStringField(const std::string& message, const char* key, std::string& value)
{
    std::string token = "\"";
    token += key;
    token += "\"";

    size_t keyPos = message.find(token);
    if (keyPos == std::string::npos)
        return false;

    size_t colon = message.find(':', keyPos + token.size());
    if (colon == std::string::npos)
        return false;

    size_t begin = message.find('"', colon + 1);
    if (begin == std::string::npos)
        return false;

    size_t end = message.find('"', begin + 1);
    if (end == std::string::npos)
        return false;

    value = message.substr(begin + 1, end - begin - 1);
    return true;
}

bool WebVisuBridge::parseIntField(const std::string& message, const char* key, int& value)
{
    std::string token = "\"";
    token += key;
    token += "\"";

    size_t keyPos = message.find(token);
    if (keyPos == std::string::npos)
        return false;

    size_t colon = message.find(':', keyPos + token.size());
    if (colon == std::string::npos)
        return false;

    size_t pos = colon + 1;
    while (pos < message.size() && std::isspace((unsigned char)message[pos]))
        ++pos;

    size_t end = pos;
    if (end < message.size() && message[end] == '-')
        ++end;
    while (end < message.size() && std::isdigit((unsigned char)message[end]))
        ++end;

    if (end == pos)
        return false;

    value = atoi(message.substr(pos, end - pos).c_str());
    return true;
}

bool WebVisuBridge::parseBoolField(const std::string& message, const char* key, bool& value)
{
    std::string token = "\"";
    token += key;
    token += "\"";

    size_t keyPos = message.find(token);
    if (keyPos == std::string::npos)
        return false;

    size_t colon = message.find(':', keyPos + token.size());
    if (colon == std::string::npos)
        return false;

    size_t pos = colon + 1;
    while (pos < message.size() && std::isspace((unsigned char)message[pos]))
        ++pos;

    if (message.compare(pos, 4, "true") == 0)
    {
        value = true;
        return true;
    }
    if (message.compare(pos, 5, "false") == 0)
    {
        value = false;
        return true;
    }
    return false;
}

#endif
