#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuBridge.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <NetworkModule.h>

#include "Switch/KnxChannelSwitch.h"
#include "Switch/WebVisuSwitch.h"
#include "Dimmer/KnxChannelDimmer.h"
#include "Dimmer/WebVisuDimmer.h"
#include "RGB/KnxChannelRGB.h"
#include "RGB/WebVisuRGB.h"
#include "Scene/KnxChannelScene.h"
#include "Scene/WebVisuScene.h"
#include "Rolladen/KnxChannelRolladen.h"
#include "Rolladen/WebVisuRolladen.h"
#include "Jalousie/KnxChannelJalousie.h"
#include "Jalousie/WebVisuJalousie.h"
#include "Thermostat/KnxChannelThermostat.h"
#include "Thermostat/WebVisuThermostat.h"
#include "Display/KnxChannelDisplay.h"
#include "Display/WebVisuDisplay.h"
#include "Alarm/KnxChannelAlarm.h"
#include "Alarm/WebVisuAlarm.h"
#include "Fan/KnxChannelFan.h"
#include "Fan/WebVisuFan.h"
#include "DoorWindow/KnxChannelDoorWindow.h"
#include "DoorWindow/WebVisuDoorWindow.h"
#include "Media/KnxChannelMedia.h"
#include "Media/WebVisuMedia.h"
#include "Lock/KnxChannelLock.h"
#include "Lock/WebVisuLock.h"
#include "ImageLoader.h"
#include "WebVisuWidgetBase.h"

namespace
{
bool decodeBase64Char(char c, uint8_t& value)
{
    if (c >= 'A' && c <= 'Z')
    {
        value = (uint8_t)(c - 'A');
        return true;
    }
    if (c >= 'a' && c <= 'z')
    {
        value = (uint8_t)(c - 'a' + 26);
        return true;
    }
    if (c >= '0' && c <= '9')
    {
        value = (uint8_t)(c - '0' + 52);
        return true;
    }
    if (c == '+')
    {
        value = 62;
        return true;
    }
    if (c == '/')
    {
        value = 63;
        return true;
    }
    return false;
}

bool decodeBase64(const std::string& input, std::vector<uint8_t>& output)
{
    output.clear();
    output.reserve((input.size() * 3) / 4);

    uint8_t quartet[4] = {0, 0, 0, 0};
    int count = 0;
    int padding = 0;

    for (char c : input)
    {
        if (c == '=')
        {
            quartet[count++] = 0;
            ++padding;
        }
        else if (c == '\r' || c == '\n' || c == ' ' || c == '\t')
        {
            continue;
        }
        else
        {
            uint8_t value = 0;
            if (!decodeBase64Char(c, value))
                return false;
            quartet[count++] = value;
        }

        if (count == 4)
        {
            output.push_back((uint8_t)((quartet[0] << 2) | (quartet[1] >> 4)));
            if (padding < 2)
            {
                output.push_back((uint8_t)((quartet[1] << 4) | (quartet[2] >> 2)));
            }
            if (padding == 0)
            {
                output.push_back((uint8_t)((quartet[2] << 6) | quartet[3]));
            }

            count = 0;
            padding = 0;
        }
    }

    return count == 0;
}

std::string headerIgnoreCase(const OpenKNX::Network::WebRequest& req, const char* name)
{
    std::string target(name);
    std::transform(target.begin(), target.end(), target.begin(), [](unsigned char c) {
        return (char)std::tolower(c);
    });

    for (const auto& entry : req.headers)
    {
        std::string current = entry.first;
        std::transform(current.begin(), current.end(), current.begin(), [](unsigned char c) {
            return (char)std::tolower(c);
        });
        if (current == target)
            return entry.second;
    }
    return std::string();
}

uint32_t fnv1a32(const std::string& value)
{
    uint32_t hash = 2166136261u;
    for (unsigned char c : value)
    {
        hash ^= (uint32_t)c;
        hash *= 16777619u;
    }
    return hash;
}

std::string stripQueryString(const std::string& value)
{
    const size_t queryPos = value.find('?');
    if (queryPos == std::string::npos)
    {
        return value;
    }
    return value.substr(0, queryPos);
}

std::string imageVersionTokenFromDataUri(const std::string& dataUri)
{
    return std::to_string((unsigned)fnv1a32(dataUri));
}
}

SwitchBridge* WebVisuBridge::createSwitch(KnxChannelSwitch& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuSwitch(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

DimmerBridge* WebVisuBridge::createDimmer(KnxChannelDimmer& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuDimmer(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

RGBBridge* WebVisuBridge::createRGB(KnxChannelRGB& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuRGB(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

SceneBridge* WebVisuBridge::createScene(KnxChannelScene& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuScene(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

RolladenBridge* WebVisuBridge::createJalousien(KnxChannelJalousie& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuJalousie(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

RolladenBridge* WebVisuBridge::createRolladen(KnxChannelRolladen& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuRolladen(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

ThermostatBridge* WebVisuBridge::createThermostat(KnxChannelThermostat& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuThermostat(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

DeviceBridge* WebVisuBridge::createDisplay(KnxChannelDisplay& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuDisplay(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

AlarmBridge* WebVisuBridge::createSensor(KnxChannelAlarm& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuAlarm(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

FanBridge* WebVisuBridge::createFan(KnxChannelFan& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuFan(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

DoorWindowBridge* WebVisuBridge::createDoorWindow(KnxChannelDoorWindow& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuDoorWindow(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

MediaBridge* WebVisuBridge::createMedia(KnxChannelMedia& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuMedia(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
    return bridge;
}

LockBridge* WebVisuBridge::createLock(KnxChannelLock& channel, uint8_t _channelIndex, uint8_t deviceType)
{
    auto bridge = new WebVisuLock(this);
    bridge->setWebVisuName(channel.getNameInUTF8() == nullptr ? "Unbenannt" : std::string(channel.getNameInUTF8()));
    channel.add(bridge);
    registerWebVisuWidget(_channelIndex, bridge);
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
    processPendingSnapshots();
}

void WebVisuBridge::registerWebPages()
{
#ifdef OPENKNX_WEBSERVER
   // openknxNetwork.webserver.addMenuItem("Ger\xC3\xA4te", MENU_URI, 52);
    openknxNetwork.webserver.addRoute(OpenKNX::Network::WEB_GET, MENU_URI,
                                      [this](OpenKNX::Network::WebRequest&, OpenKNX::Network::WebResponse& res) {
                                          std::string html = buildPageHtml();
                                          res.setLayout(true);
                                          res.setActiveMenu(MENU_URI);
                                          res.send(html.c_str());
                                      });

    openknxNetwork.webserver.addRoute(OpenKNX::Network::WEB_GET, "/devices/image/*",
                                      [this](OpenKNX::Network::WebRequest& req, OpenKNX::Network::WebResponse& res) {
                                          handleImageRequest(req, res);
                                      });
    openknxNetwork.webserver.addRoute(OpenKNX::Network::WEB_GET, "/devices/*",
                                      [this](OpenKNX::Network::WebRequest& req, OpenKNX::Network::WebResponse& res) {
                                          handleDetailRequest(req, res);
                                      });

    openknxNetwork.webserver.addSocket(
        SOCKET_URI,
        [this](int clientId, OpenKNX::Network::WebSocketFrame* frame) {
            queueCommand(frame->data, frame->length);
        },
        [this](int clientId, bool connected) {
            if (connected)
            {
                queueSnapshotForClient(clientId);
            }
            else
            {
                _pendingSnapshotClients.erase(std::remove(_pendingSnapshotClients.begin(), _pendingSnapshotClients.end(), clientId),
                                              _pendingSnapshotClients.end());
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

void WebVisuBridge::queueSnapshotForClient(int clientId)
{
    if (clientId < 0)
        return;

    if (std::find(_pendingSnapshotClients.begin(), _pendingSnapshotClients.end(), clientId) != _pendingSnapshotClients.end())
        return;

    _pendingSnapshotClients.push_back(clientId);
}

void WebVisuBridge::processPendingSnapshots()
{
    if (_pendingSnapshotClients.empty())
        return;

    const int clientId = _pendingSnapshotClients.front();
    _pendingSnapshotClients.erase(_pendingSnapshotClients.begin());

    if (!isSocketClientConnected(clientId))
        return;

    sendSnapshotToClient(clientId);
}

bool WebVisuBridge::isSocketClientConnected(int clientId) const
{
#ifdef OPENKNX_WEBSERVER
    const auto clients = openknxNetwork.webserver.connectedClientFds(SOCKET_URI);
    return std::find(clients.begin(), clients.end(), clientId) != clients.end();
#else
    (void)clientId;
    return false;
#endif
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
    WebVisuWidgetBase* widget = webVisuWidget(channelIndex);
    if (widget == nullptr)
    {
        return;
    }

    widget->webVisuHandleCommand(action, message);
}

std::string WebVisuBridge::buildPageHtml() const
{
    std::string html = "<div class='webvisu'>";
    html += WebVisuWidgetBase::widgetStyles();
    html += R"HTML(
    <h1>Ger&auml;te</h1>
    <div id='webvisu-meta' class='meta'>Verbinde...</div>
    <div id='webvisu-grid' class='webvisu-grid'></div>
    <script>(function(){
        const grid=document.getElementById('webvisu-grid');
        const meta=document.getElementById('webvisu-meta');
        const devices={};
        const debugEnabled=(new URLSearchParams(location.search).get('wvdebug')||'1') !== '0';
        const tracePrefix='[WebVisu][Overview]';
        let ws=null;
        let reconnectTimer=null;
        let isClosing=false;
        let wsMessageCount=0;
        let snapshotActive=false;
        let snapshotDeviceCount=0;
        let snapshotStartedAt=0;
        let snapshotTimeoutTimer=null;

        function trace(level, message, extra){
            if(!debugEnabled){
                return;
            }
            const fn=(console[level]&&typeof console[level]==='function')?console[level]:console.log;
            if(extra!==undefined){
                fn(tracePrefix + ' ' + message, extra);
            } else {
                fn(tracePrefix + ' ' + message);
            }
        }

        function clearSnapshotTimeout(){
            if(snapshotTimeoutTimer){
                clearTimeout(snapshotTimeoutTimer);
                snapshotTimeoutTimer=null;
            }
        }

        function armSnapshotTimeout(){
            clearSnapshotTimeout();
            snapshotTimeoutTimer=setTimeout(() => {
               a if(snapshotActive){
                    trace('warn','snapshot timeout after 5000ms, devices=' + snapshotDeviceCount);
                }
            }, 5000);
        }

        function send(payload){
            if(ws&&ws.readyState===1){
                trace('debug','send payload action=' + String(payload.action || 'n/a') + ' channel=' + String(payload.channel || 'n/a'));
                ws.send(JSON.stringify(payload));
            }
        }

        function clearDevices(){
            Object.keys(devices).forEach(key => delete devices[key]);
            grid.innerHTML='';
        }

        function updateConnectionState(){
            meta.textContent = ws && ws.readyState === 1 ? 'Live verbunden' : 'Nicht verbunden';
        }

        function cardId(channel){
            return 'webvisu-card-' + String(channel);
        }

        function createCardFromHtml(html){
            if(!html){
                return null;
            }
            const template=document.createElement('template');
            template.innerHTML=String(html).trim();
            return template.content.firstElementChild;
        }

        function insertCardInOrder(element, channel){
            const children=Array.from(grid.children);
            for(const child of children){
                const childChannel=Number(child.getAttribute('data-channel-order') || '0');
                if(childChannel > channel){
                    grid.insertBefore(element, child);
                    return;
                }
            }
            grid.appendChild(element);
        }

        function showEmptyIfNeeded(){
            if(grid.children.length !== 0){
                return;
            }
            grid.innerHTML = '<div class="webvisu-empty">Noch keine Ger&auml;te gefunden.</div>';
        }

        function upsertDevice(device){
            if(!device || !device.channel){
                return;
            }

            const channel=Number(device.channel);
            if(!Number.isFinite(channel) || channel <= 0){
                return;
            }

            devices[String(channel)] = device;

            const card=createCardFromHtml(device.html || '');
            if(!card){
                return;
            }

            const id=cardId(channel);
            card.setAttribute('id', id);
            card.setAttribute('data-channel-order', String(channel));

            const emptyState=grid.querySelector('.webvisu-empty');
            if(emptyState){
                emptyState.remove();
            }

            const existing=document.getElementById(id);
            if(existing && existing.parentElement === grid){
                grid.replaceChild(card, existing);
            } else {
                insertCardInOrder(card, channel);
            }
        }

        function buildPayload(target){
            const template = target.getAttribute('data-wv-payload') || target.getAttribute('data-wv-payload-template');
            if(!template){
                return null;
            }

            let payloadText = template;
            const channel = target.getAttribute('data-channel');
            if(channel){
                payloadText = payloadText.replaceAll('__CHANNEL__', channel);
            }

            if(target instanceof HTMLInputElement || target instanceof HTMLTextAreaElement || target instanceof HTMLSelectElement){
                payloadText = payloadText.replaceAll('__VALUE__', target.value);
            }

            const container = target.closest('[data-wv-container]');
            if(container){
                container.querySelectorAll('[data-wv-role]').forEach(source => {
                    const role = source.getAttribute('data-wv-role');
                    if(role){
                        payloadText = payloadText.replaceAll(`__${role.toUpperCase()}__`, source.value);
                    }
                });
            }

            try {
                return JSON.parse(payloadText);
            } catch (error) {
                return null;
            }
        }

        function scheduleReconnect(){
            if(isClosing || reconnectTimer){
                return;
            }
            reconnectTimer = setTimeout(() => {
                reconnectTimer = null;
                connect();
            }, 1500);
        }

        function closeSocket(){
            isClosing = true;
            if(reconnectTimer){
                clearTimeout(reconnectTimer);
                reconnectTimer = null;
            }
            clearSnapshotTimeout();
            if(ws && (ws.readyState === 0 || ws.readyState === 1)){
                try {
                    ws.close(1000, 'page unload');
                } catch (error) {
                }
            }
        }

        function connect(){
            const proto = location.protocol === 'https:' ? 'wss://' : 'ws://';
            ws = new WebSocket(proto + location.host + '/devices/ws');
            trace('info','connecting to ' + (proto + location.host + '/devices/ws'));

            ws.onopen = () => {
                trace('info','ws open');
                updateConnectionState();
                showEmptyIfNeeded();
            };
            ws.onclose = () => {
                trace('warn','ws close');
                clearSnapshotTimeout();
                snapshotActive=false;
                ws = null;
                updateConnectionState();
                showEmptyIfNeeded();
                if(!isClosing){
                    scheduleReconnect();
                }
            };
            ws.onerror = () => {
                trace('error','ws error');
                updateConnectionState();
                showEmptyIfNeeded();
            };
            ws.onmessage = (event) => {
                wsMessageCount += 1;
                let payload = null;
                try {
                    payload = JSON.parse(event.data);
                } catch (error) {
                    const preview = String(event.data || '').replace(/\s+/g, ' ').slice(0, 140);
                    meta.textContent = 'WS JSON-Fehler: ' + preview;
                    trace('error','json parse failed #' + wsMessageCount + ' preview=' + preview);
                    return;
                }

                if(payload.type === 'snapshotBegin'){
                    snapshotActive=true;
                    snapshotDeviceCount=0;
                    snapshotStartedAt=Date.now();
                    armSnapshotTimeout();
                    trace('info','snapshotBegin #' + wsMessageCount);
                    clearDevices();
                    updateConnectionState();
                    return;
                }

                if(payload.type === 'snapshotDevice' && payload.device){
                    snapshotDeviceCount += 1;
                    armSnapshotTimeout();
                    trace('debug','snapshotDevice #' + snapshotDeviceCount + ' channel=' + String(payload.device.channel || 'n/a'));
                    upsertDevice(payload.device);
                    updateConnectionState();
                    return;
                }

                if(payload.type === 'snapshotEnd'){
                    const duration=Date.now()-snapshotStartedAt;
                    trace('info','snapshotEnd devices=' + snapshotDeviceCount + ' durationMs=' + duration);
                    snapshotActive=false;
                    clearSnapshotTimeout();
                    showEmptyIfNeeded();
                    updateConnectionState();
                    return;
                }

                if(payload.type === 'snapshot' && Array.isArray(payload.devices)){
                    clearDevices();
                    payload.devices.forEach(device => upsertDevice(device));
                    showEmptyIfNeeded();
                    updateConnectionState();
                    return;
                }

                if(payload.type === 'update' && payload.device && payload.device.channel){
                    trace('debug','update channel=' + String(payload.device.channel));
                    upsertDevice(payload.device);
                    showEmptyIfNeeded();
                    updateConnectionState();
                }
            };
        }

        grid.addEventListener('click', event => {
            const target = event.target;
            if(!(target instanceof HTMLElement)) return;
            const payloadTarget = target.closest('[data-wv-payload], [data-wv-payload-template]') || target;
            const payload = buildPayload(payloadTarget);
            if(payload){
                send(payload);
            }
        });

        grid.addEventListener('change', event => {
            const target = event.target;
            if(!(target instanceof HTMLElement)) return;
            const payloadTarget = target.closest('[data-wv-payload], [data-wv-payload-template]') || target;
            const payload = buildPayload(payloadTarget);
            if(payload){
                send(payload);
            }
        });

        window.addEventListener('pagehide', closeSocket);
        window.addEventListener('beforeunload', closeSocket);

        connect();
        updateConnectionState();
        showEmptyIfNeeded();
    })();</script>
</div>)HTML";
    return html;
}

std::string WebVisuBridge::buildDetailPageHtml(uint8_t channelIndex) const
{
    std::string initialDetailHtml;
    if (_bridge != nullptr)
    {
        KnxChannelBase* channel = _bridge->getChannel(channelIndex);
        WebVisuWidgetBase* widget = webVisuWidget(channelIndex);
        if (channel != nullptr)
        {
            if (widget != nullptr)
            {
                initialDetailHtml = widget->webVisuDetailHtml(channelIndex);
            }
            else
            {
                const char* channelName = channel->getNameInUTF8();
                const std::string name = channelName == nullptr ? "Unbenannt" : std::string(channelName);
                const std::string type = channel->name();
                const std::string value = channel->currentValueAsString();
                const bool state = channel->mainFunctionValue();
                const bool supportMainAction = channel->supportMainFunctionClick();
                const MainFunctionStateImage image = channel->mainFunctionImage();
                const std::string imageUrl = buildImageUrl(image.imageFile);
                initialDetailHtml = buildDetailWidgetHtml(*channel,
                                                          channelIndex,
                                                          name,
                                                          type,
                                                          value,
                                                          state,
                                                          imageUrl,
                                                          supportMainAction,
                                                          image.allowRecolor);
            }
        }
    }

    std::string html = "<div class='webvisu'>";
    html += WebVisuWidgetBase::widgetStyles();
    html += R"HTML(
    <h1>Ger&auml;tedetails</h1>
    <div class='meta'><a class='webvisu-link' href='/devices'>&larr; Zur&uuml;ck zur &Uuml;bersicht</a></div>
    <div id='webvisu-meta' class='meta'>Verbinde...</div>
    <div id='webvisu-detail' class='webvisu-grid'>)HTML";
    html += initialDetailHtml;
    html += R"HTML(</div>
    <script>(function(){
        const channel=)HTML";
    html += std::to_string((int)channelIndex + 1);
    html += R"HTML(;
        const detail=document.getElementById('webvisu-detail');
        const initialDetailHtml=detail ? detail.innerHTML : '';
        const meta=document.getElementById('webvisu-meta');
        const debugEnabled=(new URLSearchParams(location.search).get('wvdebug')||'1') !== '0';
        const tracePrefix='[WebVisu][Detail ch=' + String(channel) + ']';
        let ws=null;
        let reconnectTimer=null;
        let current=initialDetailHtml ? { channel: channel, detailHtml: initialDetailHtml } : null;
        let wsMessageCount=0;
        let snapshotDeviceCount=0;
        let snapshotStartedAt=0;
        let snapshotTimeoutTimer=null;
        let isClosing=false;

        function trace(level, message, extra){
            if(!debugEnabled){
                return;
            }
            const fn=(console[level]&&typeof console[level]==='function')?console[level]:console.log;
            if(extra!==undefined){
                fn(tracePrefix + ' ' + message, extra);
            } else {
                fn(tracePrefix + ' ' + message);
            }
        }

        function clearSnapshotTimeout(){
            if(snapshotTimeoutTimer){
                clearTimeout(snapshotTimeoutTimer);
                snapshotTimeoutTimer=null;
            }
        }

        function armSnapshotTimeout(){
            clearSnapshotTimeout();
            snapshotTimeoutTimer=setTimeout(() => {
                trace('warn','snapshot timeout after 5000ms, devices=' + snapshotDeviceCount);
            }, 5000);
        }

        function send(payload){
            if(ws&&ws.readyState===1){
                trace('debug','send payload action=' + String(payload.action || 'n/a') + ' channel=' + String(payload.channel || 'n/a'));
                ws.send(JSON.stringify(payload));
            }
        }

        function buildPayload(target){
            const template = target.getAttribute('data-wv-payload') || target.getAttribute('data-wv-payload-template');
            if(!template){
                return null;
            }

            let payloadText = template;
            const payloadChannel = target.getAttribute('data-channel');
            if(payloadChannel){
                payloadText = payloadText.replaceAll('__CHANNEL__', payloadChannel);
            }

            if(target instanceof HTMLInputElement || target instanceof HTMLTextAreaElement || target instanceof HTMLSelectElement){
                payloadText = payloadText.replaceAll('__VALUE__', target.value);
            }

            const container = target.closest('[data-wv-container]');
            if(container){
                container.querySelectorAll('[data-wv-role]').forEach(source => {
                    const role = source.getAttribute('data-wv-role');
                    if(role){
                        payloadText = payloadText.replaceAll(`__${role.toUpperCase()}__`, source.value);
                    }
                });
            }

            try {
                return JSON.parse(payloadText);
            } catch (error) {
                return null;
            }
        }

        function render(){
            meta.textContent = ws && ws.readyState === 1 ? 'Live verbunden' : 'Nicht verbunden';
            if(!current){
                detail.innerHTML = '<div class="webvisu-empty">Ger&auml;t nicht gefunden.</div>';
                return;
            }
            detail.innerHTML = current.detailHtml || current.html || '';
        }

        function updateFromPayload(payload){
            if(payload && Number(payload.channel) === channel){
                if(!payload.detailHtml || payload.detailHtml === ''){
                    if(current && current.detailHtml){
                        payload.detailHtml = current.detailHtml;
                    } else if(initialDetailHtml){
                        payload.detailHtml = initialDetailHtml;
                    }
                }
                current = payload;
                render();
            }
        }

        function scheduleReconnect(){
            if(isClosing || reconnectTimer){
                return;
            }
            reconnectTimer = setTimeout(() => {
                reconnectTimer = null;
                connect();
            }, 1500);
        }

        function closeSocket(){
            isClosing = true;
            if(reconnectTimer){
                clearTimeout(reconnectTimer);
                reconnectTimer = null;
            }
            clearSnapshotTimeout();
            if(ws && (ws.readyState === 0 || ws.readyState === 1)){
                try {
                    ws.close(1000, 'page unload');
                } catch (error) {
                }
            }
        }

        function connect(){
            const proto = location.protocol === 'https:' ? 'wss://' : 'ws://';
            ws = new WebSocket(proto + location.host + '/devices/ws');
            trace('info','connecting to ' + (proto + location.host + '/devices/ws'));

            ws.onopen = () => {
                trace('info','ws open');
                render();
            };
            ws.onclose = () => {
                trace('warn','ws close');
                clearSnapshotTimeout();
                ws = null;
                render();
                if(!isClosing){
                    scheduleReconnect();
                }
            };
            ws.onerror = () => {
                trace('error','ws error');
                render();
            };
            ws.onmessage = (event) => {
                wsMessageCount += 1;
                let payload = null;
                try {
                    payload = JSON.parse(event.data);
                } catch (error) {
                    const preview = String(event.data || '').replace(/\s+/g, ' ').slice(0, 140);
                    meta.textContent = 'WS JSON-Fehler: ' + preview;
                    trace('error','json parse failed #' + wsMessageCount + ' preview=' + preview);
                    return;
                }

                if(payload.type === 'snapshotBegin'){
                    snapshotDeviceCount=0;
                    snapshotStartedAt=Date.now();
                    armSnapshotTimeout();
                    trace('info','snapshotBegin #' + wsMessageCount);
                    render();
                    return;
                }

                if(payload.type === 'snapshotDevice' && payload.device){
                    snapshotDeviceCount += 1;
                    armSnapshotTimeout();
                    trace('debug','snapshotDevice #' + snapshotDeviceCount + ' channel=' + String(payload.device.channel || 'n/a'));
                    updateFromPayload(payload.device);
                    return;
                }

                if(payload.type === 'snapshotEnd'){
                    const duration=Date.now()-snapshotStartedAt;
                    trace('info','snapshotEnd devices=' + snapshotDeviceCount + ' durationMs=' + duration);
                    clearSnapshotTimeout();
                    render();
                    return;
                }

                if(payload.type === 'snapshot' && Array.isArray(payload.devices)){
                    const found = payload.devices.find(device => Number(device.channel) === channel);
                    if(found){
                        if((!found.detailHtml || found.detailHtml === '') && current && current.detailHtml){
                            found.detailHtml = current.detailHtml;
                        } else if(!found.detailHtml || found.detailHtml === ''){
                            found.detailHtml = initialDetailHtml;
                        }
                        current = found;
                    }
                    render();
                    return;
                }

                if(payload.type === 'update' && payload.device){
                    trace('debug','update channel=' + String(payload.device.channel || 'n/a'));
                    updateFromPayload(payload.device);
                }
            };
        }

        detail.addEventListener('click', event => {
            const target = event.target;
            if(!(target instanceof HTMLElement)) return;
            const payloadTarget = target.closest('[data-wv-payload], [data-wv-payload-template]');
            if(!(payloadTarget instanceof HTMLElement)) return;
            const payload = buildPayload(payloadTarget);
            if(payload){
                send(payload);
            }
        });

        detail.addEventListener('change', event => {
            const target = event.target;
            if(!(target instanceof HTMLElement)) return;
            const payloadTarget = target.closest('[data-wv-payload], [data-wv-payload-template]');
            if(!(payloadTarget instanceof HTMLElement)) return;
            const payload = buildPayload(payloadTarget);
            if(payload){
                send(payload);
            }
        });

        window.addEventListener('pagehide', closeSocket);
        window.addEventListener('beforeunload', closeSocket);

        connect();
        render();
    })();</script>
</div>)HTML";
    return html;
}

std::string WebVisuBridge::buildDeviceJson(KnxChannelBase& channel, uint8_t channelIndex) const
{
    WebVisuWidgetBase* widget = webVisuWidget(channelIndex);

    const char* channelName = channel.getNameInUTF8();
    const std::string name = channelName == nullptr ? "Unbenannt" : std::string(channelName);
    const std::string type = channel.name();
    const std::string value = channel.currentValueAsString();
    const bool state = channel.mainFunctionValue();
    const bool supportMainAction = channel.supportMainFunctionClick();
    const MainFunctionStateImage image = channel.mainFunctionImage();
    const std::string imageUrl = buildImageUrl(image.imageFile);
    const std::string detailUrl = std::string("/devices/") + std::to_string((int)channelIndex + 1);

    const std::string html = WebVisuWidgetBase::renderGenericCard(channelIndex,
                                                                   name,
                                                                   type,
                                                                   value,
                                                                   imageUrl,
                                                                   supportMainAction,
                                                                   detailUrl,
                                                                   state,
                                                                   image.allowRecolor);

    const std::string kind = widget != nullptr ? widget->webVisuKind() : "generic";

    std::string json = "{";
    json += "\"kind\":\"" + jsonEscape(kind) + "\",";
    json += "\"type\":\"" + jsonEscape(type) + "\",";
    json += "\"channel\":" + std::to_string((int)channelIndex + 1) + ",";
    json += "\"name\":\"" + jsonEscape(name) + "\",";
    json += "\"value\":\"" + jsonEscape(value) + "\",";
    json += "\"power\":";
    json += state ? "true" : "false";
    json += ",";
    json += "\"imageUrl\":\"" + jsonEscape(imageUrl) + "\",";
    json += "\"imageRecolor\":";
    json += image.allowRecolor ? "true" : "false";
    json += ",";
    json += "\"mainActionSupported\":";
    json += supportMainAction ? "true" : "false";
    json += ",";
    json += "\"detailUrl\":\"" + jsonEscape(detailUrl) + "\",";
    json += "\"html\":\"" + jsonEscape(html) + "\"";
    json += "}";
    return json;
}

std::string WebVisuBridge::buildDetailWidgetHtml(KnxChannelBase& channel,
                                                 uint8_t channelIndex,
                                                 const std::string& name,
                                                 const std::string& type,
                                                 const std::string& value,
                                                 bool state,
                                                 const std::string& imageUrl,
                                                 bool supportMainAction,
                                                 bool allowRecolor) const
{
    WebVisuWidgetBase* widget = webVisuWidget(channelIndex);
    if (widget != nullptr)
    {
        const std::string widgetHtml = widget->webVisuDetailHtml(channelIndex);
        if (!widgetHtml.empty())
        {
            return widgetHtml;
        }
    }

    const std::string detailUrl = std::string("/devices/") + std::to_string((int)channelIndex + 1);
    return WebVisuWidgetBase::renderGenericCard(channelIndex,
                                                name,
                                                type,
                                                value,
                                                imageUrl,
                                                supportMainAction,
                                                detailUrl,
                                                state,
                                                allowRecolor);
}

std::string WebVisuBridge::buildImageUrl(const std::string& imageFile) const
{
    const std::string normalizedImageName = imageFile.empty() ? "missing_file.png" : imageFile;
    const std::string dataUri = ImageLoader::loadImage(normalizedImageName);
    const std::string versionToken = imageVersionTokenFromDataUri(dataUri);
    return "/devices/image/" + urlEncode(normalizedImageName) + "?v=" + versionToken;
}

void WebVisuBridge::sendSnapshotToClient(int clientId)
{
#ifdef OPENKNX_WEBSERVER
    if (!isSocketClientConnected(clientId))
    {
        logDebug("WebVisu", "snapshot skip disconnected client=%d", clientId);
        return;
    }

    ensureChangeHandlersRegistered();
    const unsigned long startedAt = millis();
    uint16_t sentDevices = 0;
    uint16_t skippedChannels = 0;
    const uint16_t totalChannels = _bridge != nullptr ? _bridge->getNumberOfChannels() : 0;

    logDebug("WebVisu", "snapshot start client=%d channels=%u", clientId, (unsigned)totalChannels);

    if (!openknxNetwork.webserver.sendToClient(SOCKET_URI, clientId, "{\"type\":\"snapshotBegin\"}", strlen("{\"type\":\"snapshotBegin\"}")))
    {
        logError("WebVisu", "snapshot abort client=%d stage=snapshotBegin", clientId);
        return;
    }

    if (_bridge != nullptr)
    {
        const uint16_t channels = _bridge->getNumberOfChannels();
        for (uint16_t idx = 0; idx < channels; ++idx)
        {
            if (!isSocketClientConnected(clientId))
            {
                logDebug("WebVisu", "snapshot stop disconnected client=%d sent=%u", clientId, (unsigned)sentDevices);
                return;
            }

            KnxChannelBase* baseChannel = _bridge->getChannel((uint8_t)idx);
            if (baseChannel == nullptr)
            {
                ++skippedChannels;
                continue;
            }

            const std::string deviceJson = buildDeviceJson(*baseChannel, (uint8_t)idx);
            std::string message;
            message.reserve(deviceJson.size() + 40);
            message = "{\"type\":\"snapshotDevice\",\"device\":";
            message += deviceJson;
            message += "}";
            if (!openknxNetwork.webserver.sendToClient(SOCKET_URI, clientId, message.c_str(), message.size()))
            {
                logError("WebVisu", "snapshot abort client=%d stage=snapshotDevice channel=%u sent=%u",
                         clientId, (unsigned)(idx + 1), (unsigned)sentDevices);
                return;
            }

            ++sentDevices;
        }
    }

    if (!openknxNetwork.webserver.sendToClient(SOCKET_URI, clientId, "{\"type\":\"snapshotEnd\"}", strlen("{\"type\":\"snapshotEnd\"}")))
    {
        logError("WebVisu", "snapshot abort client=%d stage=snapshotEnd sent=%u", clientId, (unsigned)sentDevices);
        return;
    }

    const unsigned long duration = millis() - startedAt;
    logDebug("WebVisu", "snapshot done client=%d sent=%u skipped=%u durationMs=%u",
            clientId, (unsigned)sentDevices, (unsigned)skippedChannels, (unsigned)duration);
#endif
}

void WebVisuBridge::broadcastUpdate(const std::string& deviceJson)
{
#ifdef OPENKNX_WEBSERVER
    std::string update = std::string("{\"type\":\"update\",\"device\":") + deviceJson + "}";
    openknxNetwork.webserver.sendWebsocketMessage(SOCKET_URI, update.c_str());
#endif
}

void WebVisuBridge::broadcastChannelUpdate(uint8_t channelIndex)
{
    if (_bridge == nullptr)
        return;

    KnxChannelBase* channel = _bridge->getChannel(channelIndex);
    if (channel == nullptr)
        return;

    broadcastUpdate(buildDeviceJson(*channel, channelIndex));
}

void WebVisuBridge::ensureChangeHandlersRegistered()
{
    if (_bridge == nullptr)
        return;

    const uint16_t channels = _bridge->getNumberOfChannels();
    if (_channelChangedHandlers.size() < channels)
    {
        _channelChangedHandlers.resize(channels);
        _channelChangedHandlerRegistered.resize(channels, false);
    }

    for (uint16_t idx = 0; idx < channels; ++idx)
    {
        if (_channelChangedHandlerRegistered[idx])
            continue;

        KnxChannelBase* channel = _bridge->getChannel((uint8_t)idx);
        if (channel == nullptr)
            continue;

        _channelChangedHandlers[idx] = [this](KnxChannelBase& updatedChannel) {
            broadcastChannelUpdate(updatedChannel.channelIndex());
        };
        channel->addChangedHandler(_channelChangedHandlers[idx]);
        _channelChangedHandlerRegistered[idx] = true;
    }
}

void WebVisuBridge::registerWebVisuWidget(uint8_t channelIndex, WebVisuWidgetBase* widget)
{
    if (_webVisuWidgets.size() <= channelIndex)
    {
        _webVisuWidgets.resize((size_t)channelIndex + 1, nullptr);
    }
    _webVisuWidgets[channelIndex] = widget;
}

WebVisuWidgetBase* WebVisuBridge::webVisuWidget(uint8_t channelIndex) const
{
    if (channelIndex >= _webVisuWidgets.size())
    {
        return nullptr;
    }

    return _webVisuWidgets[channelIndex];
}

void WebVisuBridge::handleDetailRequest(const OpenKNX::Network::WebRequest& req,
                                        OpenKNX::Network::WebResponse& res) const
{
    if (req.getUri() == "/devices/")
    {
        res.setStatus(303);
        res.setHeader("Location", "/devices");
        res.send("");
        return;
    }

    uint8_t channelIndex = 0;
    if (!tryParseChannelOneBasedFromUri(req.getUri(), "/devices/", channelIndex) || _bridge == nullptr)
    {
        res.setStatus(404);
        res.setContentType("text/html");
        res.setLayout(true);
        res.setActiveMenu(MENU_URI);
        res.send("<h2>404 &ndash; Ger&auml;t nicht gefunden</h2>");
        return;
    }

    if (_bridge->getChannel(channelIndex) == nullptr)
    {
        res.setStatus(404);
        res.setContentType("text/html");
        res.setLayout(true);
        res.setActiveMenu(MENU_URI);
        res.send("<h2>404 &ndash; Ger&auml;t nicht gefunden</h2>");
        return;
    }

    std::string html = buildDetailPageHtml(channelIndex);
    res.setLayout(true);
    res.setActiveMenu(MENU_URI);
    res.send(html.c_str());
}

void WebVisuBridge::handleImageRequest(const OpenKNX::Network::WebRequest& req,
                                       OpenKNX::Network::WebResponse& res) const
{
    const std::string uri = req.getUri();
    const std::string prefix = "/devices/image/";
    if (!startsWith(uri, prefix) || uri.size() <= prefix.size())
    {
        res.setStatus(404);
        res.setContentType("text/plain");
        res.send("Image not found");
        return;
    }

    const std::string requestedImage = uri.substr(prefix.size());
    std::string imageName = stripQueryString(requestedImage);
    if (imageName.find("..") != std::string::npos || imageName.empty())
    {
        res.setStatus(400);
        res.setContentType("text/plain");
        res.send("Invalid image path");
        return;
    }

    std::string dataUri = ImageLoader::loadImage(imageName);
    const std::string dataPrefix = "data:";
    if (!startsWith(dataUri, dataPrefix))
    {
        res.setStatus(500);
        res.setContentType("text/plain");
        res.send("Invalid image payload");
        return;
    }

    const size_t mimeEnd = dataUri.find(';', dataPrefix.size());
    const size_t base64Start = dataUri.find(",", mimeEnd == std::string::npos ? dataPrefix.size() : mimeEnd);
    if (mimeEnd == std::string::npos || base64Start == std::string::npos)
    {
        res.setStatus(500);
        res.setContentType("text/plain");
        res.send("Invalid image payload");
        return;
    }

    const std::string mimeType = dataUri.substr(dataPrefix.size(), mimeEnd - dataPrefix.size());
    const std::string base64Payload = dataUri.substr(base64Start + 1);

    std::vector<uint8_t> binary;
    if (!decodeBase64(base64Payload, binary))
    {
        res.setStatus(500);
        res.setContentType("text/plain");
        res.send("Invalid image payload");
        return;
    }

    const std::string versionToken = imageVersionTokenFromDataUri(dataUri);
    const std::string etag = std::string("\"") + imageName + "-" + versionToken + "\"";
    const std::string ifNoneMatch = headerIgnoreCase(req, "if-none-match");

    res.setHeader("Cache-Control", "public, max-age=31536000, immutable");
    res.setHeader("ETag", etag.c_str());

    if (!ifNoneMatch.empty() && ifNoneMatch == etag)
    {
        res.setStatus(304);
        res.send("");
        return;
    }

    res.setContentType(mimeType.c_str());
    res.send(binary.data(), (int)binary.size());
}

bool WebVisuBridge::startsWith(const std::string& value, const std::string& prefix)
{
    return value.size() >= prefix.size() && value.compare(0, prefix.size(), prefix) == 0;
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

std::string WebVisuBridge::urlEncode(const std::string& input)
{
    static const char* hex = "0123456789ABCDEF";
    std::string encoded;
    encoded.reserve(input.size() * 3);

    for (unsigned char c : input)
    {
        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '-' || c == '_' || c == '.' || c == '~')
        {
            encoded += (char)c;
        }
        else
        {
            encoded += '%';
            encoded += hex[(c >> 4) & 0xF];
            encoded += hex[c & 0xF];
        }
    }

    return encoded;
}

bool WebVisuBridge::tryParseChannelOneBasedFromUri(const std::string& uri,
                                                   const char* prefix,
                                                   uint8_t& channelIndex)
{
    const std::string cleanUri = uri;
    const std::string p = prefix == nullptr ? std::string() : std::string(prefix);
    if (!startsWith(cleanUri, p))
        return false;

    const std::string tail = cleanUri.substr(p.size());
    if (tail.empty())
        return false;

    for (char c : tail)
    {
        if (!std::isdigit((unsigned char)c))
            return false;
    }

    int oneBased = atoi(tail.c_str());
    if (oneBased <= 0 || oneBased > 255)
        return false;

    channelIndex = (uint8_t)(oneBased - 1);
    return true;
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

