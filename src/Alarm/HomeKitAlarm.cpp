#ifndef SMARTHOMEBRIDGE_DEVICESONLY  
#include "HomeKitAlarm.h"

HomeKitAlarm::HomeKitAlarm(int device) :
    device(device)
{
}

void HomeKitAlarm::createAccessory()
{
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
}

void HomeKitAlarm::setup(uint8_t _channelIndex)
{
    switch (_channel->getAlarmType())
    {
        case AlarmType::AlarmTypeContact:
            createAccessory();
            new Service::ContactSensor();
            currentValue = new Characteristic::ContactSensorState(0);
            break;
        case AlarmType::AlarmTypeMotion:
            createAccessory();
            new Service::MotionSensor();
            currentValue = new Characteristic::MotionDetected();
            break;
        case AlarmType::AlarmTypeOccupancy:
            createAccessory();
            new Service::OccupancySensor();
            currentValue = new Characteristic::OccupancyDetected();
            break;
        case AlarmType::AlarmTypeLeak:
            createAccessory();
            new Service::LeakSensor();
            currentValue = new Characteristic::LeakDetected();
            break;
        case AlarmType::AlarmTypeSmoke:
            createAccessory();
            new Service::SmokeSensor();
            currentValue = new Characteristic::SmokeDetected();
            break;
        case AlarmType::AlarmTypeCarbonDioxid:
            createAccessory();
            new Service::CarbonDioxideSensor();
            currentValue = new Characteristic::CarbonDioxideDetected();
            break;
        case AlarmType::AlarmTypeCarbonMonoxid:
            createAccessory();
            new Service::CarbonMonoxideSensor();
            currentValue = new Characteristic::CarbonMonoxideDetected();
            break;
        default:
            Serial.print("Unkown Sensor Type ");
            Serial.print(_channel->getAlarmType());
        break;
    }
}

void HomeKitAlarm::setDetected(bool value)
{
    if (currentValue != nullptr)
        currentValue->setVal(value ? 1 : 0);
}

#endif