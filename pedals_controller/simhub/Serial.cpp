#include "arduino_compat.h"

extern "C" {
  #include "tusb.h"
}

//#define DEBUG_TX
//#define DEBUG_RX

SerialImpl::SerialImpl() : rx(CFG_TUD_CDC_RX_BUFSIZE), tx(CFG_TUD_CDC_TX_BUFSIZE)
{
}

void SerialImpl::write(char c)
{    
    tx.push(c);
}

void SerialImpl::write(const char *str)
{
    char* p = (char*)str;

    while(*p)
    {
        tx.push(*p);
        p++;
    }
}

void SerialImpl::print(char c)
{
    tx.push(c);
}

void SerialImpl::print(const char * str)
{
    write(str);
}

void SerialImpl::print(String &str)
{
    write(str.c_str());
}

void SerialImpl::flush()
{
    // TODO
}

void SerialImpl::begin(uint baudRate)
{
    printf("[STUB] SerialImpl::begin\n");
}

int SerialImpl::read()
{
    char * p = (char*)rx.front();

    if (!p)
        return -1;

    char c = *p;
    rx.pop();
    return c;
}

size_t SerialImpl::available()
{   
    size_t sz = rx.size();
#ifdef DEBUG_RX
    if (sz)
       {
            printf("SerialImpl::available(%d): ", sz);
            uint8_t * data = rx.front();

            for (int i = 0 ; i < sz; i++)
            {
                printf("0x%.2X ", data[i]);                
            }
            printf("\n");
       }
       
#endif

    if (sz)
        lastNonEmptyRXTime = millis();

    return sz;
}

void SerialImpl::core0_pushRX(char c)
{
    rx.push((uint8_t)c);
}

void SerialImpl::core0_popTX(uint8_t itf)
{

#ifdef DEBUG_TX
    if (tx.size() > 0)
       printf("tx.size() > 0 (%d)\n", tx.size());
#endif
    size_t dataSize = tx.size();

    for (size_t i = 0; i < tx.size(); i++)
    {
        tud_cdc_n_write_char(itf, (char)*tx.front());
        tx.pop();
    }

    if (dataSize)
        tud_cdc_n_write_flush(itf);
}