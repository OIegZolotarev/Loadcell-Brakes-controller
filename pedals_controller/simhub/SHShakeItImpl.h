#include "SHShakeItBase.h"

class SHShakeItImpl: public SHShakeitBase
{

public:
    uint8_t motorCount();
	String providerName();

    void setMotorOutput(uint8_t motorIdx, uint8_t value1);
};

extern SHShakeItImpl shShakeitL298N;