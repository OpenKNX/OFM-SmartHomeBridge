#pragma once
#include "KnxChannelBase.h"

class KnxChannelMedia;

class MediaBridge : public ChannelBridgeBase<KnxChannelMedia>
{
public:
    virtual void setPlay(bool play) = 0;
    virtual void setVolume(uint8_t volume) = 0;
    virtual void setTitle(const char* text) = 0;
};

class KnxChannelMedia : public KnxChannelBase
{
      DynamicPointerArray<MediaBridge> mediaBridges;
    public:
        KnxChannelMedia(uint16_t channelIndex);
        void add(MediaBridge* mediaBridge);
        void remove(MediaBridge* mediaBridge);
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;
        virtual void syncBridgeState(ChannelBridge* bridge) override;
        virtual void syncAllBridgeStates() override;

    protected:
        virtual void setup() override;
        virtual void processInputKo(GroupObject& ko) override;

    public:
        void commandPlay(MediaBridge* mediaBridge, bool play);
        void commandVolume(MediaBridge* mediaBridge, uint8_t volume);
        void commandNext(MediaBridge* mediaBridge);
        void commandPrevious(MediaBridge* mediaBridge);
        virtual void commandMainFunctionClick() override;
        virtual const std::string name() override;
        virtual std::string currentValueAsString() override;
        virtual bool mainFunctionValue() override;
        virtual MainFunctionStateImage mainFunctionImage() override;
};
