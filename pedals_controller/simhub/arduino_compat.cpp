#include <pico/stdlib.h>
#include "arduino_compat.h"

SerialImpl Serial;

void delay(uint32_t ms)
{
    sleep_ms(ms);
}

uint32_t millis()
{
    absolute_time_t end = get_absolute_time();
    return us_to_ms(end);
}

void interrupts()
{
}

void noInterrupts()
{

}

