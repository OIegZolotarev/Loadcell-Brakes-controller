#include <pico/stdlib.h>
#include "arduino_compat.h"

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

void SerialImpl::write(char c)
{
}

void SerialImpl::write(const char *str)
{
}

void SerialImpl::print(char c)
{
}

void SerialImpl::print(const char *)
{
}

void SerialImpl::print(String &str)
{
}

void SerialImpl::flush()
{
}

void SerialImpl::begin(uint baudRate)
{
}

char SerialImpl::read()
{
    return 0;
}

size_t SerialImpl::available()
{
    return size_t();
}

String String::operator+=(char c)
{
    return String();
}

size_t String::length()
{
    return size_t();
}
