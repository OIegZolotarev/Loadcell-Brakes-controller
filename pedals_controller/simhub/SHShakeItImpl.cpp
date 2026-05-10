#include "SHShakeItImpl.h"
#include "../motors.h"

uint8_t SHShakeItImpl::motorCount()
{
    return 3;
}

const char *SHShakeItImpl::providerName()
{
    //  TODO: проверить возможность изменения
    return "L298N";
}

void SHShakeItImpl::setMotorOutput(uint8_t motorIdx, uint8_t value)
{
    motorsSet((Motor)motorIdx, value);
}