#include "arduino_compat.h"

extern "C" {
  #include "tusb.h"
}

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

char SerialImpl::read()
{
    char c = *rx.front();
    rx.pop();
    return c;
}

size_t SerialImpl::available()
{
    if (rx.size() > 0)
        printf("SerialImpl::available() > 0 (%d)\n", rx.size());

    return rx.size();
}

void SerialImpl::core0_pushRX(char c)
{
    rx.push((uint8_t)c);
}

void SerialImpl::core0_popTX(uint8_t itf)
{
    for (size_t i = 0; i < tx.size(); i++)
    {
        tud_cdc_n_write_char(itf, (char)*tx.front());
        tx.pop();
    }
}