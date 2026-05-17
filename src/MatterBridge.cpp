#include "MatterBridge.h"

#include "SmartHomeBridgeModule.h"

#include "Matter/MatterBridgeCommon.h"
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
#include <zap_common/zap-generated/endpoint_config.h>
#include <platform/PlatformManager.h>
#include <platform/CommissionableDataProvider.h>
#include <platform/ESP32/ESP32Config.h>
#include <crypto/CHIPCryptoPAL.h>
#include <lib/support/CHIPMem.h>
#include <setup_payload/SetupPayload.h>
#include <ESPmDNS.h>
#include <esp_system.h>

#include <cctype>
#include <cstring>

namespace
{
constexpr uint32_t kDefaultMatterSetupPasscode = 20202021;

void logMatterEndpointBudget()
{
#ifdef CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT
    constexpr unsigned dynamicCount = CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT;
#else
    constexpr unsigned dynamicCount = 0;
#endif

#ifdef FIXED_ENDPOINT_COUNT
    constexpr unsigned fixedCount = FIXED_ENDPOINT_COUNT;
#else
    constexpr unsigned fixedCount = 0;
#endif

#ifdef MAX_ENDPOINT_COUNT
    constexpr unsigned maxCount = MAX_ENDPOINT_COUNT;
#else
    constexpr unsigned maxCount = dynamicCount + fixedCount;
#endif

#ifdef CONFIG_ESP_MATTER_MAX_DYNAMIC_ENDPOINT_COUNT
    constexpr int sdkconfigDynamic = CONFIG_ESP_MATTER_MAX_DYNAMIC_ENDPOINT_COUNT;
#else
    constexpr int sdkconfigDynamic = -1;
#endif

    logInfo("MatterBridge",
            "Matter endpoint budget: dynamic=%u fixed=%u max=%u sdkconfig_dynamic=%d",
            dynamicCount, fixedCount, maxCount, sdkconfigDynamic);
}

class RuntimeCommissionableDataProvider : public chip::DeviceLayer::CommissionableDataProvider
{
public:
    CHIP_ERROR SetBackingProvider(chip::DeviceLayer::CommissionableDataProvider *provider)
    {
        _backingProvider = provider;
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR SetRuntimePasscode(uint32_t passcode)
    {
        if (!chip::PayloadContents::IsValidSetupPIN(passcode))
            return CHIP_ERROR_INVALID_ARGUMENT;

        _passcode = passcode;
        for (size_t i = 0; i < sizeof(_salt); ++i)
            _salt[i] = static_cast<uint8_t>(esp_random() & 0xFF);

        chip::Crypto::Spake2pVerifier verifier;
        CHIP_ERROR err = verifier.Generate(_iterationCount, chip::ByteSpan(_salt, sizeof(_salt)), _passcode);
        if (err != CHIP_NO_ERROR)
            return err;

        chip::MutableByteSpan serialized(_verifier);
        err = verifier.Serialize(serialized);
        if (err != CHIP_NO_ERROR)
            return err;

        _verifierLen = serialized.size();

        _isInitialized = true;
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR GetSetupDiscriminator(uint16_t &setupDiscriminator) override
    {
        if (_backingProvider != nullptr)
            return _backingProvider->GetSetupDiscriminator(setupDiscriminator);

        setupDiscriminator = 3840;
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR SetSetupDiscriminator(uint16_t setupDiscriminator) override
    {
        if (_backingProvider != nullptr)
            return _backingProvider->SetSetupDiscriminator(setupDiscriminator);

        (void)setupDiscriminator;
        return CHIP_ERROR_NOT_IMPLEMENTED;
    }

    CHIP_ERROR GetSpake2pIterationCount(uint32_t &iterationCount) override
    {
        VerifyOrReturnError(_isInitialized, CHIP_ERROR_INCORRECT_STATE);
        iterationCount = _iterationCount;
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR GetSpake2pSalt(chip::MutableByteSpan &saltBuf) override
    {
        VerifyOrReturnError(_isInitialized, CHIP_ERROR_INCORRECT_STATE);
        if (saltBuf.size() < sizeof(_salt))
            return CHIP_ERROR_BUFFER_TOO_SMALL;

        memcpy(saltBuf.data(), _salt, sizeof(_salt));
        saltBuf.reduce_size(sizeof(_salt));
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR GetSpake2pVerifier(chip::MutableByteSpan &verifierBuf, size_t &outVerifierLen) override
    {
        VerifyOrReturnError(_isInitialized, CHIP_ERROR_INCORRECT_STATE);
        outVerifierLen = _verifierLen;
        if (verifierBuf.size() < outVerifierLen)
            return CHIP_ERROR_BUFFER_TOO_SMALL;

        memcpy(verifierBuf.data(), _verifier, outVerifierLen);
        verifierBuf.reduce_size(outVerifierLen);
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR GetSetupPasscode(uint32_t &setupPasscode) override
    {
        VerifyOrReturnError(_isInitialized, CHIP_ERROR_INCORRECT_STATE);
        setupPasscode = _passcode;
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR SetSetupPasscode(uint32_t setupPasscode) override
    {
        return SetRuntimePasscode(setupPasscode);
    }

private:
    chip::DeviceLayer::CommissionableDataProvider *_backingProvider = nullptr;
    uint32_t _passcode = kDefaultMatterSetupPasscode;
    uint32_t _iterationCount = 10000;
    uint8_t _salt[16]{};
    uint8_t _verifier[chip::Crypto::kSpake2p_VerifierSerialized_Length]{};
    size_t _verifierLen = 0;
    bool _isInitialized = false;
};

RuntimeCommissionableDataProvider sRuntimeCommissionableDataProvider;

bool tryParseMatterPasscode(const std::string &text, uint32_t &out)
{
    if (text.size() != 8)
        return false;

    for (unsigned char c : text)
    {
        if (!std::isdigit(c))
            return false;
    }

    try
    {
        size_t consumed = 0;
        unsigned long parsed = std::stoul(text, &consumed, 10);
        if (consumed != text.size())
            return false;
        out = static_cast<uint32_t>(parsed);
        return true;
    }
    catch (...)
    {
        return false;
    }
}


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
    return _aggregatorEndpointId;
}

const std::string MatterBridge::name()
{
    return "MatterBridge";
}

esp_err_t MatterBridge::configureNode(esp_matter::node_t *node)
{
    esp_matter::attribute::set_callback(attributeCallback);

    esp_matter::endpoint::aggregator::config_t aggregatorConfig{};
    esp_matter::endpoint_t *aggregatorEndpoint =
        esp_matter::endpoint::aggregator::create(node, &aggregatorConfig, esp_matter::ENDPOINT_FLAG_NONE, nullptr);
    if (aggregatorEndpoint == nullptr)
    {
        logErrorP("Matter aggregator endpoint creation failed");
        return ESP_FAIL;
    }

    _aggregatorEndpointId = esp_matter::endpoint::get_id(aggregatorEndpoint);
    return esp_matter_bridge::initialize(node, deviceTypeCallback);
}

void MatterBridge::initialize(SmartHomeBridgeModule *bridge)
{
    BridgeBase::initialize(bridge);
    logMatterEndpointBudget();

    // Resolve intended passcode
    uint32_t matterSetupPasscode = kDefaultMatterSetupPasscode;
    if (!tryParseMatterPasscode(ParamBRI_PairingCodeMatterStr, matterSetupPasscode))
    {
        logErrorP("Matter setup code invalid ('%s'), using fallback %u",
                  ParamBRI_PairingCodeMatterStr.c_str(), (unsigned)kDefaultMatterSetupPasscode);
        matterSetupPasscode = kDefaultMatterSetupPasscode;
    }
    else if (!chip::PayloadContents::IsValidSetupPIN(matterSetupPasscode))
    {
        logErrorP("Matter setup code not allowed by Matter rules (%u), using fallback %u",
                  (unsigned)matterSetupPasscode, (unsigned)kDefaultMatterSetupPasscode);
        matterSetupPasscode = kDefaultMatterSetupPasscode;
    }
    _matterSetupPasscode = matterSetupPasscode;

    // NVS must be updated BEFORE InitChipStack(), because the
    // LegacyTemporaryCommissionableDataProvider reads and caches salt/verifier
    // from NVS during ConfigurationManagerImpl::Init() (called by InitChipStack).
    // Clearing the old verifier here forces CHIP to recompute it from the new passcode.
    {
        using cfg = chip::DeviceLayer::Internal::ESP32Config;
        uint32_t nvsPasscode = 0;
        bool nvsHasPin = (cfg::ReadConfigValue(cfg::kConfigKey_SetupPinCode, nvsPasscode) == CHIP_NO_ERROR);
        if (!nvsHasPin || nvsPasscode != _matterSetupPasscode)
        {
            logInfoP("Matter passcode changed (%u -> %u): updating NVS before CHIP init",
                     (unsigned)nvsPasscode, (unsigned)_matterSetupPasscode);
            cfg::WriteConfigValue(cfg::kConfigKey_SetupPinCode, _matterSetupPasscode);
            cfg::ClearConfigValue(cfg::kConfigKey_Spake2pVerifier);
            cfg::ClearConfigValue(cfg::kConfigKey_Spake2pSalt);
            cfg::ClearConfigValue(cfg::kConfigKey_Spake2pIterationCount);
        }
        else
        {
            logInfoP("Matter passcode unchanged (%u), NVS kept", (unsigned)_matterSetupPasscode);
        }
    }

    // Pre-init CHIP before node::create() can register connectivity handlers.
    // This avoids PostEventOrDie() on a NULL chip event queue when WiFi events
    // arrive between initialize() and start().
    CHIP_ERROR chipErr = chip::Platform::MemoryInit();
    if (chipErr != CHIP_NO_ERROR && chipErr != CHIP_ERROR_INCORRECT_STATE)
    {
        logErrorP("Matter MemoryInit failed: %" CHIP_ERROR_FORMAT, chipErr.Format());
        return;
    }

    chipErr = chip::DeviceLayer::PlatformMgr().InitChipStack();
    if (chipErr != CHIP_NO_ERROR && chipErr != CHIP_ERROR_INCORRECT_STATE)
    {
        logErrorP("Matter InitChipStack failed: %" CHIP_ERROR_FORMAT, chipErr.Format());
        return;
    }

    // Belt-and-suspenders: also set the in-memory passcode on the provider
    // that was registered by InitChipStack.
    if (chip::DeviceLayer::CommissionableDataProvider *provider = chip::DeviceLayer::GetCommissionableDataProvider();
        provider != nullptr)
    {
        chipErr = provider->SetSetupPasscode(_matterSetupPasscode);
        if (chipErr != CHIP_NO_ERROR)
            logInfoP("Matter SetSetupPasscode (in-memory) not supported: %" CHIP_ERROR_FORMAT, chipErr.Format());
        else
            logInfoP("Matter setup code set: %u", (unsigned)_matterSetupPasscode);
    }
    else
    {
        logErrorP("Matter CommissionableDataProvider missing");
    }

    esp_matter::node::config_t config{};
    _bridgeNodeLabel = bridge != nullptr ? bridge->getNameInUTF8() : "";
    if (_bridgeNodeLabel.empty())
        _bridgeNodeLabel = "SmartHomeBridge";
    logInfoP("Matter bridge node label: '%s'", _bridgeNodeLabel.c_str());
    std::strncpy(config.root_node.basic_information.node_label, _bridgeNodeLabel.c_str(),
                 sizeof(config.root_node.basic_information.node_label) - 1);

    _node = esp_matter::node::create(&config, attributeCallback, nullptr, this);
    if (_node == nullptr)
    {
        logErrorP("Matter node creation failed");
        return;
    }

    if (configureNode(_node) != ESP_OK)
        logErrorP("Matter bridge initialization failed");
    else
    {
        logInfoP("Matter bridge initialized (aggregator endpoint=%u)", (unsigned)_aggregatorEndpointId);
    }
}

void MatterBridge::start(SmartHomeBridgeModule *bridge)
{
    BridgeBase::start(bridge);
    if (startMatter() != ESP_OK)
        logErrorP("Matter start failed");
}

esp_err_t MatterBridge::startMatter()
{
    // Arduino/OpenKNX may already run mDNS. ESP-Matter discovery initializes
    // its own advertiser and fails if mDNS is already active.
    MDNS.end();

    esp_err_t err = esp_matter::start(nullptr);
    if (err != ESP_OK)
        return err;

    // After start(), set NodeLabel and ProductName on ep 0.
    // esp_matter::start() restores persistent attribute values from NVS and may
    // overwrite what was set via set_val before start. Calling update() after
    // start() ensures Apple Home reads the correct bridge name during commissioning.
    matterbridge::updateText(0, matterbridge::basicInformationClusterId,
                             matterbridge::nodeLabelAttrId, _bridgeNodeLabel.c_str());
    matterbridge::updateText(0, matterbridge::basicInformationClusterId,
                             matterbridge::productNameAttrId, _bridgeNodeLabel.c_str());

    if (chip::DeviceLayer::CommissionableDataProvider *provider = chip::DeviceLayer::GetCommissionableDataProvider();
        provider != nullptr)
    {
        CHIP_ERROR chipErr = sRuntimeCommissionableDataProvider.SetBackingProvider(provider);
        if (chipErr != CHIP_NO_ERROR)
            logErrorP("Matter runtime provider backing set failed: %" CHIP_ERROR_FORMAT, chipErr.Format());

        chipErr = sRuntimeCommissionableDataProvider.SetRuntimePasscode(_matterSetupPasscode);
        if (chipErr != CHIP_NO_ERROR)
            logErrorP("Matter runtime provider passcode setup failed: %" CHIP_ERROR_FORMAT, chipErr.Format());
        else
        {
            chip::DeviceLayer::SetCommissionableDataProvider(&sRuntimeCommissionableDataProvider);
            logInfoP("Matter runtime commissionable provider installed");
        }

        uint32_t effectivePasscode = 0;
        chipErr = chip::DeviceLayer::GetCommissionableDataProvider()->GetSetupPasscode(effectivePasscode);
        if (chipErr == CHIP_NO_ERROR)
            logInfoP("Matter effective setup code after start: %u", (unsigned)effectivePasscode);
        else
            logInfoP("Matter GetSetupPasscode not supported after start: %" CHIP_ERROR_FORMAT, chipErr.Format());
    }

    return ESP_OK;
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
        logInfoP("Matter factory reset triggered");
        openknx.restart();
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
