#pragma once
#include "KnxChannelBase.h"

class KnxChannelScene;

class SceneBridge : public ChannelBridgeBase<KnxChannelScene>
{
    public:
        virtual void setActivating(bool activating) = 0;
};

class KnxChannelScene : public KnxChannelBase
{
    private:
        unsigned long _lastActivatiation = 0;
        DynamicPointerArray<SceneBridge> sceneBridges; 
        void reportActivation(SceneBridge* ignore);
    public:
        KnxChannelScene(uint16_t channelIndex);
        void add(SceneBridge* sceneBridge);
        void remove(SceneBridge* sceneBridge);
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) override;
        virtual void deleteBridgeDevice(ChannelBridge* device) override;
    protected:
        virtual void setup() override;
        virtual void loop() override;
        virtual void processInputKo(GroupObject& ko) override;
   
    public:
        void commandActivate(SceneBridge* sceneBridge);
        virtual void commandMainFunctionClick() override;
        virtual const std::string name() override;
        virtual std::string currentValueAsString() override;
        virtual bool mainFunctionValue() override;
        virtual MainFunctionStateImage mainFunctionImage() override;
};
