#include <stdio.h>

enum Motor
{
    CLUTCH = 0,
    BRAKE,
    THROTTLE
};

void motorsInit();
void motorsUpdate();
void motorsSet(Motor index, uint8_t value);
void motorsDisable();

#define TEST_SEQUENCE_DURATION_MS 50
#define TEST_SEQUENCE_PAUSE_MS 200