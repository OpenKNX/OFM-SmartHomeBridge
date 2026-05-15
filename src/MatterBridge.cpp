#include "MatterBridge.h"

#include "SmartHomeBridgeModule.h"

#include "Matter/MatterBridgeDeviceBase.h"

#include "Switch/MatterSwitch.h"
#include "Dimmer/MatterDimmer.h"
#include "RGB/MatterRGB.h"
#include "Jalousie/MatterJalousie.h"
#include "Rolladen/MatterRolladen.h"
#include "Thermostat/MatterThermostat.h"
#include "Display/MatterDisplay.h"
#include "Alarm/MatterAlarm.h"
#include "Fan/MatterFan.h"
#include "DoorWindow/MatterDoorWindow.h"
#include "Scene/MatterScene.h"
#include "Lock/MatterLock.h"
#include "Media/MatterMedia.h"

#include <esp_matter.h>
#include <esp_matter_bridge.h>

#include <cstring>

namespace
{
esp_err_t addSwitchEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::on_off_switch::config_t config;
    return esp_matter::endpoint::on_off_switch::add(endpoint, &config);
}

esp_err_t addOutletEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::on_off_plugin_unit::config_t config;
    return esp_matter::endpoint::on_off_plugin_unit::add(endpoint, &config);
}

esp_err_t addOnOffLightEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::on_off_light::config_t config;
    return esp_matter::endpoint::on_off_light::add(endpoint, &config);
}

esp_err_t addDimmerEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::dimmable_plugin_unit::config_t config;
    return esp_matter::endpoint::dimmable_plugin_unit::add(endpoint, &config);
}

esp_err_t addDimmableLightEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::dimmable_light::config_t config;
    return esp_matter::endpoint::dimmable_light::add(endpoint, &config);
}

esp_err_t addRGBEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::extended_color_light::config_t config;
    return esp_matter::endpoint::extended_color_light::add(endpoint, &config);
}

esp_err_t addRolladenEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::window_covering_device::config_t config;
    return esp_matter::endpoint::window_covering_device::add(endpoint, &config);
}

esp_err_t addThermostatEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::thermostat::config_t config;
    return esp_matter::endpoint::thermostat::add(endpoint, &config);
}

esp_err_t addFanEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::fan::config_t config;
    return esp_matter::endpoint::fan::add(endpoint, &config);
}

esp_err_t addLockEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::door_lock::config_t config;
    return esp_matter::endpoint::door_lock::add(endpoint, &config);
}

esp_err_t addTemperatureEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::temperature_sensor::config_t config;
    return esp_matter::endpoint::temperature_sensor::add(endpoint, &config);
}

esp_err_t addHumidityEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::humidity_sensor::config_t config;
    return esp_matter::endpoint::humidity_sensor::add(endpoint, &config);
}

esp_err_t addLightEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::light_sensor::config_t config;
    return esp_matter::endpoint::light_sensor::add(endpoint, &config);
}

esp_err_t addOccupancyEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::occupancy_sensor::config_t config;
    return esp_matter::endpoint::occupancy_sensor::add(endpoint, &config);
}

esp_err_t addContactEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::contact_sensor::config_t config;
    return esp_matter::endpoint::contact_sensor::add(endpoint, &config);
}

esp_err_t addLeakEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::water_leak_detector::config_t config;
    return esp_matter::endpoint::water_leak_detector::add(endpoint, &config);
}

esp_err_t addSmokeEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::smoke_co_alarm::config_t config;
    return esp_matter::endpoint::smoke_co_alarm::add(endpoint, &config);
}

esp_err_t addAirQualityEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::air_quality_sensor::config_t config;
    return esp_matter::endpoint::air_quality_sensor::add(endpoint, &config);
}

esp_err_t addRainEndpoint(esp_matter::endpoint_t *endpoint)
{
    esp_matter::endpoint::rain_sensor::config_t config;
    return esp_matter::endpoint::rain_sensor::add(endpoint, &config);
}

esp_err_t addForDeviceType(esp_matter::endpoint_t *endpoint, uint32_t deviceTypeId)
{
    if (deviceTypeId == esp_matter::endpoint::on_off_switch::get_device_type_id())
        return addSwitchEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::on_off_plugin_unit::get_device_type_id())
        return addOutletEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::on_off_light::get_device_type_id())
        return addOnOffLightEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::dimmable_plugin_unit::get_device_type_id())
        return addDimmerEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::dimmable_light::get_device_type_id())
        return addDimmableLightEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::extended_color_light::get_device_type_id())
        return addRGBEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::window_covering_device::get_device_type_id())
        return addRolladenEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::thermostat::get_device_type_id())
        return addThermostatEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::fan::get_device_type_id())
        return addFanEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::door_lock::get_device_type_id())
        return addLockEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::temperature_sensor::get_device_type_id())
        return addTemperatureEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::humidity_sensor::get_device_type_id())
        return addHumidityEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::light_sensor::get_device_type_id())
        return addLightEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::occupancy_sensor::get_device_type_id())
        return addOccupancyEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::contact_sensor::get_device_type_id())
        return addContactEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::water_leak_detector::get_device_type_id())
        return addLeakEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::smoke_co_alarm::get_device_type_id())
        return addSmokeEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::air_quality_sensor::get_device_type_id())
        return addAirQualityEndpoint(endpoint);
    if (deviceTypeId == esp_matter::endpoint::rain_sensor::get_device_type_id())
        return addRainEndpoint(endpoint);

    return ESP_OK;
}

MatterBridgeDeviceBase *asDeviceBase(void *privData)
{
    return reinterpret_cast<MatterBridgeDeviceBase *>(privData);
}
} // namespace

esp_matter::node_t *MatterBridge::node() const
{
    return _node;
}

uint16_t MatterBridge::parentEndpointId() const
{
    return 0;
}

const std::string MatterBridge::name()
{
    return "MatterBridge";
}

esp_err_t MatterBridge::configureNode(esp_matter::node_t *node)
{
    esp_matter::attribute::set_callback(attributeCallback);
    return esp_matter_bridge::initialize(node, deviceTypeCallback);
}

void MatterBridge::initialize(SmartHomeBridgeModule *bridge)
{
    BridgeBase::initialize(bridge);

    esp_matter::node::config_t config{};
    std::string nodeLabel = bridge != nullptr ? bridge->getNameInUTF8() : "SmartHomeBridge";
    std::strncpy(config.root_node.basic_information.node_label, nodeLabel.c_str(),
                 sizeof(config.root_node.basic_information.node_label) - 1);

    _node = esp_matter::node::create(&config, attributeCallback, nullptr, this);
    if (_node == nullptr)
    {
        logErrorP("Matter node creation failed");
        return;
    }

    if (configureNode(_node) != ESP_OK)
        logErrorP("Matter bridge initialization failed");
}

void MatterBridge::start(SmartHomeBridgeModule *bridge)
{
    BridgeBase::start(bridge);
    if (startMatter() != ESP_OK)
        logErrorP("Matter start failed");
}

esp_err_t MatterBridge::startMatter()
{
    return esp_matter::start(nullptr);
}

void MatterBridge::loop()
{
}

void MatterBridge::processInputKo(GroupObject &)
{
}

void MatterBridge::getInformation(String &result)
{
    result += "<h3>Matter</h3>";
    result += "Matter bridge aktiv";
}

bool MatterBridge::processCommand(const std::string cmd, bool)
{
    if (cmd == "mr")
    {
        factoryReset();
        return true;
    }
    return false;
}

void MatterBridge::showHelp()
{
    openknx.console.printHelpLine("mr", "Matter factory reset");
}

esp_err_t MatterBridge::factoryReset()
{
    return esp_matter_bridge::factory_reset();
}

esp_err_t MatterBridge::deviceTypeCallback(esp_matter::endpoint_t *endpoint, uint32_t deviceTypeId, void *)
{
    return addForDeviceType(endpoint, deviceTypeId);
}

esp_err_t MatterBridge::attributeCallback(esp_matter::attribute::callback_type_t type, uint16_t endpointId,
                                          uint32_t clusterId, uint32_t attributeId,
                                          esp_matter_attr_val_t *val, void *privData)
{
    auto device = asDeviceBase(privData);
    if (device == nullptr || val == nullptr || type == esp_matter::attribute::PRE_UPDATE)
        return ESP_OK;

    device->handleMatterAttribute(type, clusterId, attributeId, val);
    (void)endpointId;
    return ESP_OK;
}

SwitchBridge *MatterBridge::createSwitch(KnxChannelSwitch &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterSwitchBridge(this);
    channel.add(bridge);
    return bridge;
}

DimmerBridge *MatterBridge::createDimmer(KnxChannelDimmer &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterDimmerBridge(this);
    channel.add(bridge);
    return bridge;
}

RGBBridge *MatterBridge::createRGB(KnxChannelRGB &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterRGBBridge(this);
    channel.add(bridge);
    return bridge;
}

RolladenBridge *MatterBridge::createJalousien(KnxChannelJalousie &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterJalousieBridge(this);
    channel.add(bridge);
    return bridge;
}

RolladenBridge *MatterBridge::createRolladen(KnxChannelRolladen &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterRolladenBridge(this);
    channel.add(bridge);
    return bridge;
}

ThermostatBridge *MatterBridge::createThermostat(KnxChannelThermostat &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterThermostatBridge(this);
    channel.add(bridge);
    return bridge;
}

DeviceBridge *MatterBridge::createDisplay(KnxChannelDisplay &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterDisplayBridge(this);
    channel.add(bridge);
    return bridge;
}

AlarmBridge *MatterBridge::createSensor(KnxChannelAlarm &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterAlarmBridge(this);
    channel.add(bridge);
    return bridge;
}

FanBridge *MatterBridge::createFan(KnxChannelFan &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterFanBridge(this);
    channel.add(bridge);
    return bridge;
}

DoorWindowBridge *MatterBridge::createDoorWindow(KnxChannelDoorWindow &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterDoorWindowBridge(this);
    channel.add(bridge);
    return bridge;
}

SceneBridge *MatterBridge::createScene(KnxChannelScene &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterSceneBridge(this);
    channel.add(bridge);
    return bridge;
}

LockBridge *MatterBridge::createLock(KnxChannelLock &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterLockBridge(this);
    channel.add(bridge);
    return bridge;
}

MediaBridge *MatterBridge::createMedia(KnxChannelMedia &channel, uint8_t, uint8_t)
{
    auto bridge = new MatterMediaBridge(this);
    channel.add(bridge);
    return bridge;
}
