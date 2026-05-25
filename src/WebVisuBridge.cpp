#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuBridge.h"

#include <algorithm>
#include <cctype>

#include <NetworkModule.h>

#include "Switch/KnxChannelSwitch.h"
#include "Switch/WebVisuSwitch.h"
#include "Dimmer/KnxChannelDimmer.h"
#include "Dimmer/WebVisuDimmer.h"

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

void WebVisuBridge::reportSwitchState(uint8_t channelIndex, const char* name, bool power)
{
    DeviceState& state = _devices[channelIndex];
    state.kind = DeviceKind::Switch;
    state.channelIndex = channelIndex;
    state.name = name == nullptr ? "Unbenannt" : std::string(name);
    state.power = power;
    state.brightness = power ? 100 : 0;
    broadcastUpdate(state);
}

void WebVisuBridge::reportDimmerState(uint8_t channelIndex, const char* name, uint8_t brightness)
{
    DeviceState& state = _devices[channelIndex];
    state.kind = DeviceKind::Dimmer;
    state.channelIndex = channelIndex;
    state.name = name == nullptr ? "Unbenannt" : std::string(name);
    state.brightness = brightness;
    state.power = brightness > 0;
    broadcastUpdate(state);
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

    DeviceState* device = findDeviceByChannelOneBased(channelOneBased);
    if (device == nullptr || _bridge == nullptr)
    {
        return;
    }

    KnxChannelBase* baseChannel = _bridge->getChannel(device->channelIndex);
    if (baseChannel == nullptr)
    {
        return;
    }

    if (action == "toggle")
    {
        if (device->kind == DeviceKind::Switch)
        {
            ((KnxChannelSwitch*)baseChannel)->commandPower(nullptr, !device->power);
        }
        else if (device->kind == DeviceKind::Dimmer)
        {
            ((KnxChannelDimmer*)baseChannel)->commandMainFunctionClick();
        }
        return;
    }

    if (action == "setSwitch" && device->kind == DeviceKind::Switch)
    {
        bool power = false;
        if (!parseBoolField(message, "power", power))
        {
            return;
        }
        ((KnxChannelSwitch*)baseChannel)->commandPower(nullptr, power);
        return;
    }

    if (action == "setDimmer" && device->kind == DeviceKind::Dimmer)
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

WebVisuBridge::DeviceState* WebVisuBridge::findDeviceByChannelOneBased(int channel)
{
    if (channel <= 0)
        return nullptr;

    uint8_t channelIndex = (uint8_t)(channel - 1);
    auto it = _devices.find(channelIndex);
    if (it == _devices.end())
        return nullptr;

    return &it->second;
}

std::string WebVisuBridge::buildPageHtml() const
{
    return R"HTML(<div class='webvisu'>
  <style>
    .webvisu{padding:0.5rem 0;}
    .webvisu h1{margin:0 0 0.75rem 0;}
    .webvisu .meta{margin-bottom:1rem;color:var(--muted,#555);}
    .webvisu-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(260px,1fr));gap:0.75rem;}
    .webvisu-card{border:1px solid #d5d5d5;border-radius:8px;padding:0.9rem;background:#fff;display:flex;flex-direction:column;gap:0.6rem;box-shadow:0 1px 2px rgba(0,0,0,0.04);}
    .webvisu-title{display:flex;justify-content:space-between;align-items:center;gap:0.75rem;font-weight:600;}
    .webvisu-type{font-size:0.8rem;color:#666;background:#f1f1f1;border-radius:999px;padding:0.1rem 0.5rem;}
    .webvisu-value{font-size:1.25rem;font-weight:700;}
    .webvisu-controls{display:flex;align-items:center;gap:0.6rem;flex-wrap:wrap;}
    .webvisu-btn{border:1px solid #999;background:#fafafa;border-radius:6px;padding:0.35rem 0.75rem;cursor:pointer;font-size:0.95rem;}
    .webvisu-btn:hover{background:#efefef;}
    .webvisu-slider{width:100%;accent-color:#3b6ea8;}
    .webvisu-empty{padding:0.75rem;border:1px dashed #bbb;border-radius:8px;color:#666;background:#fafafa;}
    @media (max-width: 640px){
      .webvisu-grid{grid-template-columns:1fr;}
    }
  </style>
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

      function cardHeader(device){
        const typeLabel = device.kind === 'dimmer' ? 'Dimmer' : 'Switch';
        return '<div class="webvisu-title"><span>' + escapeHtml(device.name) + '</span><span class="webvisu-type">' + typeLabel + ' #' + device.channel + '</span></div>';
      }

      function escapeHtml(value){
        return String(value)
          .replace(/&/g, '&amp;')
          .replace(/</g, '&lt;')
          .replace(/>/g, '&gt;')
          .replace(/\"/g, '&quot;');
      }

      function renderSwitch(device){
        const value = device.power ? 'Ein' : 'Aus';
        const next = device.power ? 'Aus' : 'Ein';
        return cardHeader(device) +
          '<div class="webvisu-value">' + value + '</div>' +
          '<div class="webvisu-controls">' +
            '<button class="webvisu-btn" data-action="toggle" data-channel="' + device.channel + '">' + next + '</button>' +
          '</div>';
      }

      function renderDimmer(device){
        const brightness = Number(device.brightness || 0);
        return cardHeader(device) +
          '<div class="webvisu-value">' + brightness + '%</div>' +
          '<div class="webvisu-controls">' +
            '<button class="webvisu-btn" data-action="toggle" data-channel="' + device.channel + '">' + (brightness > 0 ? 'Aus' : 'Ein') + '</button>' +
          '</div>' +
          '<input class="webvisu-slider" type="range" min="0" max="100" step="1" value="' + brightness + '" data-action="setDimmer" data-channel="' + device.channel + '">';
      }

      function render(){
        const entries = Object.values(devices).sort((a,b) => Number(a.channel) - Number(b.channel));
        meta.textContent = ws && ws.readyState === 1 ? 'Live verbunden' : 'Nicht verbunden';
        if (entries.length === 0){
          grid.innerHTML = '<div class="webvisu-empty">Noch keine unterst\u00fctzten Ger\u00e4te gefunden. V1 zeigt Switch und Dimmer.</div>';
          return;
        }

        grid.innerHTML = entries.map(device => {
          const body = device.kind === 'dimmer' ? renderDimmer(device) : renderSwitch(device);
          return '<article class="webvisu-card">' + body + '</article>';
        }).join('');
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
}

std::string WebVisuBridge::buildSnapshotMessage() const
{
    std::string json = "{\"type\":\"snapshot\",\"devices\":[";
    bool first = true;
    for (const auto& entry : _devices)
    {
        if (!first)
            json += ",";
        first = false;
        json += buildDeviceJson(entry.second);
    }
    json += "]}";
    return json;
}

std::string WebVisuBridge::buildUpdateMessage(const DeviceState& device) const
{
    return std::string("{\"type\":\"update\",\"device\":") + buildDeviceJson(device) + "}";
}

std::string WebVisuBridge::buildDeviceJson(const DeviceState& device) const
{
    std::string json = "{";
    json += "\"kind\":\"";
    json += (device.kind == DeviceKind::Dimmer) ? "dimmer" : "switch";
    json += "\",";
    json += "\"channel\":" + std::to_string((int)device.channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(device.name) + "\",";
    json += "\"power\":";
    json += device.power ? "true" : "false";
    json += ",";
    json += "\"brightness\":" + std::to_string((int)device.brightness);
    json += "}";
    return json;
}

void WebVisuBridge::sendSnapshotToClient(int clientId)
{
#ifdef OPENKNX_WEBSERVER
    std::string snapshot = buildSnapshotMessage();
    openknxNetwork.webserver.sendWebsocketMessage(SOCKET_URI, snapshot.c_str(), clientId);
#endif
}

void WebVisuBridge::broadcastUpdate(const DeviceState& device)
{
#ifdef OPENKNX_WEBSERVER
    std::string update = buildUpdateMessage(device);
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

std::string WebVisuBridge::jsonEscape(const std::string& input)
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

#endif
