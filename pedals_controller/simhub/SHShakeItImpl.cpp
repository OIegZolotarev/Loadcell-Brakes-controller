#include "SHShakeItImpl.h"
#include "../motors.h"

SHShakeItImpl shShakeitL298N;

uint8_t SHShakeItImpl::motorCount()
{
    return 3;
}

String SHShakeItImpl::providerName()
{
    return "RP2040 Custom motors driver";
}

void SHShakeItImpl::setMotorOutput(uint8_t motorIdx, uint8_t value)
{
    motorsSet((Motor)motorIdx, value);
}