#include "SmartHomeBridgeModule.h"
#include "KnxChannelBase.h"
#include "CP1252ToUTF8.h"

KnxChannelBase::KnxChannelBase(uint16_t _channelIndex) // Do not rename the parameter name, it is used in the ParamBRI_CHDeviceName macro
    : Component((const char*) ParamBRI_CHDeviceName)
{
    this->_channelIndex = _channelIndex;
    utf8Name = convert1252ToUTF8(getName());
}

const std::string KnxChannelBase::logPrefix() 
{
    return OpenKNX::Channel::logPrefix();
}

const char* KnxChannelBase::getNameInUTF8()
{
    return utf8Name;
}


void KnxChannelBase::addChangedHandler(MainFunctionChangedHandler& mainFunctionChangedHandler)
{
    mainFunctionChangedHandlers.push_back(&mainFunctionChangedHandler);
    mainFunctionChangedHandler(*this);
}

void KnxChannelBase::removeChangedHandler(MainFunctionChangedHandler& mainFunctionChangedHandler)
{
    mainFunctionChangedHandlers.remove(&mainFunctionChangedHandler);
}
   


void KnxChannelBase::mainFunctionValueChanged()
{
    for (auto it = mainFunctionChangedHandlers.begin(); it != mainFunctionChangedHandlers.end(); ++it)
    {
        MainFunctionChangedHandler& handler = **it;
        handler(*this);
    }
}


KnxChannelBase::~KnxChannelBase()
{
    if (utf8Name != nullptr)
    {
        delete utf8Name;
        utf8Name = nullptr;
    }
}

std::string KnxChannelBase::getImageFileName(int channelParameterIndex)
{
    auto fileName = (const char*) (knx.paramData(BRI_ParamCalcIndex(channelParameterIndex)));
    int len = 0;
    while (fileName[len] != 0 && len < IconNameParameterLength)
    {
        len++;
    }
    return std::string(fileName, len) + ".png";
}

std::string KnxChannelBase::mainFunctionImage()
{
    std::string image = "Type";
    image += std::to_string(ParamBRI_CHDeviceType);
    image += ".png";
    return image;
}