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

struct MainFunctionState
{
    bool state;
    std::string image;
    bool colorImage;
};

struct MainFunctionStateImage
{
    bool allowRecolor;
    std::string imageFile;
};

class KnxChannelBase : public OpenKNX::Channel, public Component
{
    private:
  
        DynamicPointerArray<MainFunctionChangedHandler> mainFunctionChangedHandlers;
        const int IconNameParameterLength = 8; 
        const char* utf8Name = nullptr;
    protected:
        const uint8_t LIMIT_NOT_USED = 255;
        std::string getImageFileName(int parameterIndex);
        virtual MainFunctionStateImage mainFunctionTypeImage();
        MainFunctionStateImage calculateMainFunctionImage(GroupObject& feedbackKo, const Dpt& dpt, uint8_t limit0, uint8_t limit50, uint8_t limit100);
        MainFunctionStateImage calculateMainFunctionImage(uint8_t currentValue, uint8_t limit0, uint8_t limit50, uint8_t limit100);
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
        virtual std::string currentValueAsString() = 0;
        virtual bool mainFunctionValue() = 0;
        virtual MainFunctionStateImage mainFunctionImage() = 0;
        virtual bool mainFunctionPreferValueDisplay();
};