#ifndef __SHSHAKEITBASE_H__
#define __SHSHAKEITBASE_H__

#include "pico/stdlib.h"

#define SHShakeitBaseSafetyDelay 1000

class SHShakeitBase {
private:
	uint32_t lastRead = 0;

public:
	virtual uint8_t motorCount();
	virtual const char* providerName();

	virtual void safetyStop();
	void safetyCheck();
	void read();
	
protected:
	virtual void setMotorOutput(uint8_t motorIdx, uint8_t value);
    
};

#endif
