#include "simhub/arduino_compat.h"
#include "profiler_cookie.h"

ProfilingCookie::ProfilingCookie(const char *tag): mTag(tag)
{
    time = millis();
}

ProfilingCookie::~ProfilingCookie()
{
    volatile uint32_t newTime = millis();
    printf("[ProfilingCookie]%s = %d ms\n", mTag, newTime - time);
}
