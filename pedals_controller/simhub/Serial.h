
#include "pico/util/queue.h"
#include <rigtorp/SPSCQueue.h>

#include "tusb_config.h"

class SerialImpl
{
    uint32_t lastNonEmptyRXTime = 0;    
    
    rigtorp::SPSCQueue<uint8_t> rx;
    rigtorp::SPSCQueue<uint8_t> tx;

public:
    SerialImpl();

    void write(char c);
    void write(const char* str);

    void print(char c);
    void print(const char*);
    void print(String & str);


    void flush();
    void begin(uint baudRate);

    int read();

    size_t available();

    void core0_pushRX(char c);
    void core0_popTX(uint8_t);

    uint32_t getLastNoneEmptyRXTime()
    {
        return lastNonEmptyRXTime;
    }
};