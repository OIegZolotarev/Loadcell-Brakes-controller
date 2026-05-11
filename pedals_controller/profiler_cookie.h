#pragma once
class ProfilingCookie
{
    const char* mTag;
    uint32_t time;
public:
    ProfilingCookie(const char* tag);
    ~ProfilingCookie();
};

#define PROFILE_SCOPE(name) ProfilingCookie timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)