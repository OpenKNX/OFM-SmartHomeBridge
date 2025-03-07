#pragma once
#include "component.h"
#include "OpenKNX.h"
#include "DynamicPointerArray.h"
#include "MemoryAllocator.h"

class BridgeBase;

class ChannelBridge
{
    public:
    static void* operator new(size_t size)
    {
        return HS_MALLOC(size);
    }  
    virtual ~ChannelBridge() {};
    virtual void mainFunctionValueChanged() {};
  
};

template<class T> 
class ChannelBridgeBase : public ChannelBridge
{
protected:
    T* _channel = nullptr;
public:
    virtual void initialize(T* channel)
    {
        _channel = channel;
        setup(_channel->channelIndex());
    }

   
protected:
    virtual void setup(uint8_t _channelIndex)
    {
    }

    const std::string logPrefix()
    {
        if (_channel == nullptr)
            return "Not initialized ChannelBridge";
        return _channel->logPrefix();
    }
};

class KnxChannelBase;
using MainFunctionChangedHandler = std::function<void(KnxChannelBase&)>; // Funktionszeiger-Typ


class KnxChannelBase : public OpenKNX::Channel, public Component
{
    private:
  
        DynamicPointerArray<MainFunctionChangedHandler> mainFunctionChangedHandlers;
 
        const char* utf8Name = nullptr;
    protected:
        void mainFunctionValueChanged();
    public:
        ~KnxChannelBase();
        KnxChannelBase(uint16_t channelIndex);
        void addChangedHandler(MainFunctionChangedHandler& mainFunctionChangedHandle);
        void removeChangedHandler(MainFunctionChangedHandler& mainFunctionChangedHandler);
        virtual const std::string logPrefix() override;
        virtual const std::string name() = 0;
        const char* getNameInUTF8();
        virtual ChannelBridge* createBridgeDevice(BridgeBase& bridge) = 0;
        virtual void deleteBridgeDevice(ChannelBridge* device) = 0;
        virtual void commandMainFunctionClick() = 0;
        virtual bool supportMainFunctionClick() { return true; }

};