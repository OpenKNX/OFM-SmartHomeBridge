#ifndef SMARTHOMEBRIDGE_DEVICESONLY

#include "WebVisuBridge.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <vector>

#include <NetworkModule.h>

#include "Switch/KnxChannelSwitch.h"
#include "Switch/WebVisuSwitch.h"
#include "Dimmer/KnxChannelDimmer.h"
#include "Dimmer/WebVisuDimmer.h"
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
}

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
        if (baseChannel->supportMainFunctionClick())
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

    if (action == "setDimmerPower" && kind == "Dimmer")
    {
        bool power = false;
        if (!parseBoolField(message, "power", power))
        {
            return;
        }
        ((KnxChannelDimmer*)baseChannel)->commandPower(nullptr, power);
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
                /* Original JS before minification:
                     (function(){
                         const grid = document.getElementById('webvisu-grid');
                         const meta = document.getElementById('webvisu-meta');
                         const devices = {};
                         let ws = null;
                         let reconnectTimer = null;
                         let imageLoadGeneration = 0;

                         function send(payload){
                             if (ws && ws.readyState === 1){
                                 ws.send(JSON.stringify(payload));
                             }
                         }

                         function loadImagesSequentially(container){
                             imageLoadGeneration += 1;
                             const generation = imageLoadGeneration;
                             const images = Array.from(container.querySelectorAll('img[data-src]'));

                             function loadNext(index){
                                 if (generation !== imageLoadGeneration || index >= images.length){
                                     return;
                                 }

                                 const image = images[index];
                                 const src = image.getAttribute('data-src');
                                 if (!src){
                                     loadNext(index + 1);
                                     return;
                                 }

                                 image.addEventListener('load', () => loadNext(index + 1), { once: true });
                                 image.addEventListener('error', () => loadNext(index + 1), { once: true });
                                 image.setAttribute('src', src);
                                 image.removeAttribute('data-src');
                             }

                             loadNext(0);
                         }

                         function render(){
                             const entries = Object.values(devices).sort((a,b) => Number(a.channel) - Number(b.channel));
                             meta.textContent = ws && ws.readyState === 1 ? 'Live verbunden' : 'Nicht verbunden';
                             if (entries.length === 0){
                                 grid.innerHTML = '<div class="webvisu-empty">Noch keine Ger\u00e4te gefunden.</div>';
                                 return;
                             }

                             grid.innerHTML = entries.map(device => device.html || '').join('');
                             loadImagesSequentially(grid);
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
                             ws = new WebSocket(proto + location.host + '/devices/ws');

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
                                 return;
                             }

                             if (action === 'setDimmerPower'){
                                 const power = target.getAttribute('data-power') === 'true';
                                 send({ action: 'setDimmerPower', channel: channel, power: power });
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
                */
        html += R"HTML(
    <h1>Ger&auml;te</h1>
  <div id='webvisu-meta' class='meta'>Verbinde...</div>
  <div id='webvisu-grid' class='webvisu-grid'></div>
                <script>(function(){const g=document.getElementById('webvisu-grid'),m=document.getElementById('webvisu-meta'),d={};let w=null,r=null,l=0;function s(p){if(w&&w.readyState===1)w.send(JSON.stringify(p))}function q(c){l+=1;const n=l,i=Array.from(c.querySelectorAll('img[data-src]'));function x(j){if(n!==l||j>=i.length)return;const e=i[j],u=e.getAttribute('data-src');if(!u){x(j+1);return}e.addEventListener('load',()=>x(j+1),{once:true});e.addEventListener('error',()=>x(j+1),{once:true});e.setAttribute('src',u);e.removeAttribute('data-src')}x(0)}function v(){const a=Object.values(d).sort((a,b)=>Number(a.channel)-Number(b.channel));m.textContent=w&&w.readyState===1?'Live verbunden':'Nicht verbunden';if(a.length===0){g.innerHTML='<div class="webvisu-empty">Noch keine Ger\u00e4te gefunden.</div>';return}g.innerHTML=a.map(e=>e.html||'').join('');q(g)}function t(){if(r)return;r=setTimeout(()=>{r=null;o()},1500)}function o(){const p=location.protocol==='https:'?'wss://':'ws://';w=new WebSocket(p+location.host+'/devices/ws');w.onopen=()=>{v()};w.onclose=()=>{v();t()};w.onerror=()=>{v()};w.onmessage=e=>{let p=null;try{p=JSON.parse(e.data)}catch(_){return}if(p.type==='snapshot'&&Array.isArray(p.devices)){Object.keys(d).forEach(k=>delete d[k]);p.devices.forEach(e=>{d[String(e.channel)]=e});v();return}if(p.type==='update'&&p.device&&p.device.channel){d[String(p.device.channel)]=p.device;v()}}}g.addEventListener('click',e=>{const t=e.target;if(!(t instanceof HTMLElement))return;const a=t.getAttribute('data-action'),c=Number(t.getAttribute('data-channel'));if(!a||!c)return;if(a==='toggle'){s({action:'toggle',channel:c});return}if(a==='setDimmerPower')s({action:'setDimmerPower',channel:c,power:t.getAttribute('data-power')==='true'})});g.addEventListener('change',e=>{const t=e.target;if(!(t instanceof HTMLInputElement))return;const a=t.getAttribute('data-action'),c=Number(t.getAttribute('data-channel'));if(a!=='setDimmer'||!c)return;s({action:'setDimmer',channel:c,brightness:Number(t.value)})});o();v()})();</script>
</div>)HTML";
        return html;
}

std::string WebVisuBridge::buildDetailPageHtml(uint8_t channelIndex) const
{
        std::string initialDetailHtml;
        if (_bridge != nullptr)
        {
            KnxChannelBase* channel = _bridge->getChannel(channelIndex);
            if (channel != nullptr)
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

        std::string html = "<div class='webvisu'>";
        html += WebVisuWidgetBase::widgetStyles();
        html += R"HTML(
        <h1>Ger&auml;tedetails</h1>
        <div class='meta'><a class='webvisu-link' href='/devices'>&larr; Zur&uuml;ck zur &Uuml;bersicht</a></div>
        <div id='webvisu-meta' class='meta'>Verbinde...</div>
        <div id='webvisu-detail' class='webvisu-grid'>)HTML";
        html += initialDetailHtml;
        /* Original JS before minification:
                     const detail = document.getElementById('webvisu-detail');
                     const meta = document.getElementById('webvisu-meta');
                     let ws = null;
                     let reconnectTimer = null;
                     let current = null;

                     function send(payload){
                         if (ws && ws.readyState === 1){
                             ws.send(JSON.stringify(payload));
                         }
                     }

                     function render(){
                         meta.textContent = ws && ws.readyState === 1 ? 'Live verbunden' : 'Nicht verbunden';
                         if (!current){
                             detail.innerHTML = '<div class="webvisu-empty">Ger&auml;t nicht gefunden.</div>';
                             return;
                         }
                         detail.innerHTML = current.detailHtml || current.html || '';
                     }

                     function updateFromPayload(payload){
                         if (payload && Number(payload.channel) === channel){
                             current = payload;
                             render();
                         }
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
                         ws = new WebSocket(proto + location.host + '/devices/ws');

                         ws.onopen = () => { render(); };
                         ws.onclose = () => { render(); scheduleReconnect(); };
                         ws.onerror = () => { render(); };
                         ws.onmessage = (event) => {
                             let payload = null;
                             try { payload = JSON.parse(event.data); } catch (e) { return; }

                             if (payload.type === 'snapshot' && Array.isArray(payload.devices)){
                                 const found = payload.devices.find(d => Number(d.channel) === channel);
                                 if (found) current = found;
                                 render();
                                 return;
                             }

                             if (payload.type === 'update' && payload.device){
                                 updateFromPayload(payload.device);
                             }
                         };
                     }

                     detail.addEventListener('click', (event) => {
                         const target = event.target;
                         if (!(target instanceof HTMLElement)) return;
                         const action = target.getAttribute('data-action');
                         const actionChannel = Number(target.getAttribute('data-channel'));
                         if (!action || !actionChannel) return;

                         if (action === 'toggle'){
                             send({ action: 'toggle', channel: actionChannel });
                             return;
                         }

                         if (action === 'setDimmerPower'){
                             const power = target.getAttribute('data-power') === 'true';
                             send({ action: 'setDimmerPower', channel: actionChannel, power: power });
                         }
                     });

                     detail.addEventListener('change', (event) => {
                         const target = event.target;
                         if (!(target instanceof HTMLInputElement)) return;
                         const action = target.getAttribute('data-action');
                         const actionChannel = Number(target.getAttribute('data-channel'));
                         if (action !== 'setDimmer' || !actionChannel) return;

                         send({ action: 'setDimmer', channel: actionChannel, brightness: Number(target.value) });
                     });

                     connect();
                     render();
    */
    html += R"HTML(</div>
        <script>(function(){const c=)HTML";
        html += std::to_string((int)channelIndex + 1);
                html += R"HTML(;const d=document.getElementById('webvisu-detail'),m=document.getElementById('webvisu-meta');let w=null,r=null,p=null;function s(a){if(w&&w.readyState===1)w.send(JSON.stringify(a))}function v(){m.textContent=w&&w.readyState===1?'Live verbunden':'Nicht verbunden';if(!p){d.innerHTML='<div class="webvisu-empty">Ger&auml;t nicht gefunden.</div>';return}d.innerHTML=p.detailHtml||p.html||''}function u(a){if(a&&Number(a.channel)===c){p=a;v()}}function t(){if(r)return;r=setTimeout(()=>{r=null;o()},1500)}function o(){const x=location.protocol==='https:'?'wss://':'ws://';w=new WebSocket(x+location.host+'/devices/ws');w.onopen=()=>{v()};w.onclose=()=>{v();t()};w.onerror=()=>{v()};w.onmessage=e=>{let a=null;try{a=JSON.parse(e.data)}catch(_){return}if(a.type==='snapshot'&&Array.isArray(a.devices)){const f=a.devices.find(e=>Number(e.channel)===c);if(f)p=f;v();return}if(a.type==='update'&&a.device)u(a.device)}}d.addEventListener('click',e=>{const t=e.target;if(!(t instanceof HTMLElement))return;const a=t.getAttribute('data-action'),h=Number(t.getAttribute('data-channel'));if(!a||!h)return;if(a==='toggle'){s({action:'toggle',channel:h});return}if(a==='setDimmerPower')s({action:'setDimmerPower',channel:h,power:t.getAttribute('data-power')==='true'})});d.addEventListener('change',e=>{const t=e.target;if(!(t instanceof HTMLInputElement))return;const a=t.getAttribute('data-action'),h=Number(t.getAttribute('data-channel'));if(a!=='setDimmer'||!h)return;s({action:'setDimmer',channel:h,brightness:Number(t.value)})});o();v()})();</script>
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

        std::string deviceJson = buildDeviceJson(*baseChannel, (uint8_t)idx);

        if (!first)
            json += ",";
        first = false;
        json += deviceJson;
    }
    json += "]}";
    return json;
}

std::string WebVisuBridge::buildDeviceJson(KnxChannelBase& channel, uint8_t channelIndex) const
{
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

    const std::string detailHtml = buildDetailWidgetHtml(channel,
                                                         channelIndex,
                                                         name,
                                                         type,
                                                         value,
                                                         state,
                                                         imageUrl,
                                                         supportMainAction,
                                                         image.allowRecolor);

    std::string json = "{";
    json += "\"kind\":\"generic\",";
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
    json += "\"html\":\"" + jsonEscape(html) + "\",";
    json += "\"detailHtml\":\"" + jsonEscape(detailHtml) + "\"";
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
    (void)channel;

    if (type == "Switch")
    {
        return WebVisuSwitch::renderWidgetHtml(channelIndex, name, state);
    }

    if (type == "Dimmer")
    {
        int brightnessInt = atoi(value.c_str());
        if (brightnessInt < 0)
            brightnessInt = 0;
        if (brightnessInt > 100)
            brightnessInt = 100;
        return WebVisuDimmer::renderWidgetHtml(channelIndex, name, (uint8_t)brightnessInt);
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
    return "/devices/image/" + urlEncode(imageFile.empty() ? "missing_file.png" : imageFile);
}

void WebVisuBridge::sendSnapshotToClient(int clientId)
{
#ifdef OPENKNX_WEBSERVER
    ensureChangeHandlersRegistered();
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

    const uint16_t channels = _bridge->getNumberOfUsedChannels();
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

    std::string imageName = uri.substr(prefix.size());
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

    const std::string etag = std::string("\"") + imageName + "\"";
    const std::string ifNoneMatch = headerIgnoreCase(req, "if-none-match");

    res.setHeader("Cache-Control", "private, no-cache, max-age=0, must-revalidate");
    res.setHeader("Pragma", "no-cache");
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
