#include <knx.h>
#include "knxprod.h"
#include "KnxChannelScene.h"
#include "BridgeBase.h"

#define KO_SCENE KoBRI_KO1_, DPT_SceneNumber

KnxChannelScene::KnxChannelScene(uint16_t channelIndex)
    : KnxChannelBase(channelIndex),
      sceneBridges()
{
}

ChannelBridge* KnxChannelScene::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createScene(*this, _channelIndex, ParamBRI_CHDeviceType);
}

void KnxChannelScene::deleteBridgeDevice(ChannelBridge *device)
{
    remove((SceneBridge *)device);
}

void KnxChannelScene::add(SceneBridge *sceneBridge)
{
    sceneBridges.push_back(sceneBridge);
    sceneBridge->initialize(this);
    sceneBridge->setActivating(false);

}

void KnxChannelScene::remove(SceneBridge *sceneBridge)
{
    sceneBridges.remove(sceneBridge);
    delete sceneBridge;
}

const std::string KnxChannelScene::name()
{
    return std::string("Scene");
}

void KnxChannelScene::commandMainFunctionClick()
{
    commandActivate(nullptr);
}

MainFunctionStateImage KnxChannelScene::mainFunctionImage()
{
   return calculateMainFunctionImage(_lastActivatiation == 0 ? 0 : 100, 0, LIMIT_NOT_USED, 100);
}

void KnxChannelScene::commandActivate(SceneBridge *sceneBridge)
{
    _lastActivatiation = max(1L, millis());
    koSet(KO_SCENE, (uint8_t) (ParamBRI_CHSceneNumber - 1), true);
    reportActivation(sceneBridge);
}

void KnxChannelScene::reportActivation(SceneBridge* ignore)
{
    for (auto it = sceneBridges.begin(); it != sceneBridges.end(); ++it)
    {
        if (*it != ignore)
            (*it)->setActivating(_lastActivatiation != 0);
        (*it)->mainFunctionValueChanged();
    }
    mainFunctionValueChanged();
}

void KnxChannelScene::loop()
{
    if (_lastActivatiation != 0 && millis() - _lastActivatiation > 1000)
    {
        _lastActivatiation = 0;
        reportActivation(nullptr);
    }
}


void KnxChannelScene::processInputKo(GroupObject &ko)
{
    if (isKo(ko, KO_SCENE))
    {
        uint8_t scene = koGet(KO_SCENE);
        if (scene == (uint8_t) (ParamBRI_CHSceneNumber - 1))
        {
            _lastActivatiation = max(1L, millis());
            reportActivation(nullptr);
        }
    }  
}

void KnxChannelScene::setup()
{
    koSetWithoutSend(KO_SCENE, (uint32_t) 0);
}

std::string KnxChannelScene::currentValueAsString()
{
    return _lastActivatiation != 0 ? "Starten" : "";
}

bool KnxChannelScene::mainFunctionValue()
{
    return _lastActivatiation != 0;
}