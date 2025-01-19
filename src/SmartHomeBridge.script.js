// OFM-FunctionBlocks --
// SPDX-License-Identifier: AGPL-3.0-only

function briAddChannel(device, online, progress, context) {
    var numberOfChannelds = device.getParameterByName('BRI_VisibleChannels');
    var newNumberOfChannels = numberOfChannelds.value + 1;
    var channelType = device.getParameterByName('BRI_CH' + newNumberOfChannels + 'DeviceType');
    var notAllowed = device.getParameterByName('BRI_AddNotAllowed');
    if (channelType == 0)
    {
        notAllowed.value = 1;
    }
    else
    {
        notAllowed.value = 0;
        numberOfChannelds.value = numberOfChannelds.value + 1;
    }
}

// -- OFM-FunctionBlocks //
