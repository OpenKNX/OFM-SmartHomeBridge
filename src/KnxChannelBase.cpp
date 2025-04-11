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

MainFunctionStateImage KnxChannelBase::mainFunctionTypeImage()
{
    std::string image = "Type";
    image += std::to_string(ParamBRI_CHDeviceType);
    image += ".png";
    return {true, image};
}

MainFunctionStateImage KnxChannelBase::calculateMainFunctionImage(GroupObject& feedbackKo, const Dpt& dpt, uint8_t limit0, uint8_t limit50, uint8_t limit100)
{
    return calculateMainFunctionImage(koGet(feedbackKo, dpt), limit0, limit50, limit100);
}
MainFunctionStateImage KnxChannelBase::calculateMainFunctionImage(uint8_t value, uint8_t limit0, uint8_t limit50, uint8_t limit100)
{
    if (!ParamBRI_CHIcon)
        return mainFunctionTypeImage();
    int parameterIndex = -1;
    bool allowRecolor;
    if (limit0 != LIMIT_NOT_USED && value <= limit0)
    {
        parameterIndex = BRI_CHIcon0;
        allowRecolor = ParamBRI_CHIconCol0;
    }
    else if (limit50 != LIMIT_NOT_USED && value <= limit50)
    {
        parameterIndex = BRI_CHIcon50;
        allowRecolor = ParamBRI_CHIconCol50;
    }
    else if (limit100 != LIMIT_NOT_USED && value <= limit100)
    {
        parameterIndex = BRI_CHIcon100;
        allowRecolor = ParamBRI_CHIconCol100;
    }
    else
    {
        return mainFunctionTypeImage();
    }
    return {allowRecolor, getImageFileName(parameterIndex)};
}