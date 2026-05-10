#pragma once

#include <stdio.h>
#include "pico/stdlib.h"

#include <string.h>
#include <stdint.h>

typedef uint8_t byte;

uint32_t millis();

class String
{
public:
    String operator +=(char c);
    size_t length();
};


class SerialImpl
{
public:
    void write(char c);
    void write(const char* str);

    void print(char c);
    void print(const char*);
    void print(String & str);


    void flush();
    void begin(uint baudRate);

    char read();

    size_t available();
};

extern SerialImpl Serial;

void interrupts();
void noInterrupts();