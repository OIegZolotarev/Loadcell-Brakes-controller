#include "arduino_compat.h"
#include "SHShakeItBase.h"
#include "FlowSerial.h"

void SHShakeitBase::safetyStop() 
{
    uint8_t motorcount = motorCount();
    for (int m = 0; m < motorcount; m++) 
    {
        setMotorOutput(m, 0);
    }
}

void SHShakeitBase::safetyCheck() 
{
    if (millis() - lastRead > SHShakeitBaseSafetyDelay && lastRead > 0) {
        safetyStop();
        lastRead = 0;
    }
}

void SHShakeitBase::read() 
{
    uint8_t motorcount = motorCount();

    for (int motorIdx = 0; motorIdx < motorcount; motorIdx++) 
    {
        int value = FlowSerialTimedRead();
                
        if (value != -1) 
            setMotorOutput(motorIdx, value);
        else 
            return;        
    }
    lastRead = millis();
}

