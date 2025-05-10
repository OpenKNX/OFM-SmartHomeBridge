#include <knx.h>
#include "knxprod.h"
#include "KnxChannelMedia.h"
#include "BridgeBase.h"

#define KO_VOLUME           KoBRI_KO1_, DPT_Scaling
#define KO_VOLUME_FEEDBACK  KoBRI_KO2_, DPT_Scaling
#define KO_PLAY             KoBRI_KO3_, DPT_Switch
#define KO_PLAY_FEEDBACK    KoBRI_KO4_, DPT_Switch
#define KO_NEXT_PREVIOUS    KoBRI_KO5_, DPT_UpDown
#define KO_TITLE            KoBRI_KO6_, DPT_String_8859_1

KnxChannelMedia::KnxChannelMedia(uint16_t _channelIndex)
    : KnxChannelBase(_channelIndex),
      mediaBridges()
{
}

ChannelBridge* KnxChannelMedia::createBridgeDevice(BridgeBase &bridge)
{
    return bridge.createMedia(*this, _channelIndex, ParamBRI_CHDeviceType);
}

void KnxChannelMedia::deleteBridgeDevice(ChannelBridge *device)
{
    remove((MediaBridge *)device);
}

void KnxChannelMedia::add(MediaBridge *mediaBridge)
{
    mediaBridges.push_back(mediaBridge);
    mediaBridge->initialize(this);
    mediaBridge->setPlay(koGet(KO_PLAY_FEEDBACK));
    mediaBridge->setVolume(koGet(KO_VOLUME_FEEDBACK));
    mediaBridge->setTitle(koGet(KO_TITLE));
}

void KnxChannelMedia::remove(MediaBridge *mediaBridge)
{
    mediaBridges.remove(mediaBridge);
    delete mediaBridge;
}

const std::string KnxChannelMedia::name()
{
    return std::string("Media");
}

void KnxChannelMedia::commandVolume(MediaBridge* mediaBridge, uint8_t volume)
{
    logDebugP("Received changed. Volume: %d", (int) volume);
    uint8_t knxValue = volume;
    for (auto it = mediaBridges.begin(); it != mediaBridges.end(); ++it)
    {
        if ((*it) != mediaBridge)
        {
            (*it)->setVolume(volume);
            (*it)->mainFunctionValueChanged();
        }
    }
    koSetWithoutSend(KO_VOLUME_FEEDBACK, knxValue);
    koSet(KO_VOLUME, knxValue, true);
    mainFunctionValueChanged();
}

void KnxChannelMedia::commandPlay(MediaBridge* mediaBridge, bool play)
{
    logDebugP("Received changed. Play: %d", (int) play);
    uint8_t knxValue = play;
    for (auto it = mediaBridges.begin(); it != mediaBridges.end(); ++it)
    {
        if ((*it) != mediaBridge)
        {
            (*it)->setPlay(play);
            (*it)->mainFunctionValueChanged();
        }
    }
    koSetWithoutSend(KO_PLAY_FEEDBACK, knxValue);
    koSet(KO_PLAY, knxValue, true);
    mainFunctionValueChanged();
}

void KnxChannelMedia::commandPrevious(MediaBridge* mediaBridge)
{
    logDebugP("Received changed. Previous");
    koSet(KO_NEXT_PREVIOUS, true, true);
}

void KnxChannelMedia::commandNext(MediaBridge* mediaBridge)
{
    logDebugP("Received changed. Next");
    koSet(KO_NEXT_PREVIOUS, false, true);
}

void KnxChannelMedia::commandMainFunctionClick()
{
    if (koGet(KO_PLAY_FEEDBACK))
    {
        commandPlay(nullptr, false);
    }
    else
    {
        commandPlay(nullptr, true);
    }
}


void KnxChannelMedia::setup()
{
    koSetWithoutSend(KO_VOLUME, (uint8_t) 0);
    koSetWithoutSend(KO_VOLUME_FEEDBACK,(uint8_t) 0);
    koSendReadRequest(KO_VOLUME_FEEDBACK);

    koSetWithoutSend(KO_PLAY, false);
    koSetWithoutSend(KO_PLAY_FEEDBACK, false);
    koSendReadRequest(KO_PLAY_FEEDBACK);

    if (ParamBRI_CHMediaTextInput)
    {
        koSetWithoutSend(KO_TITLE, "");
        koSendReadRequest(KO_TITLE);
    }
}

void KnxChannelMedia::processInputKo(GroupObject &groupObject)
{
    if (isKo(groupObject, KO_VOLUME_FEEDBACK))
    {
        uint8_t volume = koGet(KO_VOLUME_FEEDBACK);
        koSetWithoutSend(KO_VOLUME, volume);
        for (auto it = mediaBridges.begin(); it != mediaBridges.end(); ++it)
        {
            (*it)->setVolume(volume);
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();
    }
    else if (isKo(groupObject, KO_PLAY_FEEDBACK))
    {
        bool play = koGet(KO_PLAY_FEEDBACK);
        koSetWithoutSend(KO_PLAY, play);
        for (auto it = mediaBridges.begin(); it != mediaBridges.end(); ++it)
        {
            (*it)->setPlay(play);
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();
    }
    else if (isKo(groupObject, KO_TITLE))
    {
        const char* title = (const char*) groupObject.valueRef();
        for (auto it = mediaBridges.begin(); it != mediaBridges.end(); ++it)
        {
            (*it)->setTitle(title);
            (*it)->mainFunctionValueChanged();
        }
        mainFunctionValueChanged();
    }
}

std::string KnxChannelMedia::currentValueAsString()
{
    if (ParamBRI_CHMediaTextInput)
    {
        auto title =  (const char*) KoBRI_KO6_.valueRef();
        if (strlen(title) > 0)
            return std::string(title);   
    }
    return std::string(koGet(KO_PLAY_FEEDBACK) ? "Spielt" : "-");
}

bool KnxChannelMedia::mainFunctionValue()
{
    return koGet(KO_PLAY_FEEDBACK);
}

MainFunctionStateImage KnxChannelMedia::mainFunctionImage()
{
   return calculateMainFunctionImage(mainFunctionValue() ? 0 : 100, 0, LIMIT_NOT_USED, 100);
}
