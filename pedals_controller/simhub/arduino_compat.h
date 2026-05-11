#pragma once

#include <string>
#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/mutex.h"

#include <string.h>
#include <stdint.h>

#include "tusb_config.h"

typedef uint8_t byte;

uint32_t millis();

inline char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}



// Как васе просто оказалось, лол :)
using String = std::string;

#include "Serial.h"

extern SerialImpl Serial;

void interrupts();
void noInterrupts();


void delay(uint32_t ms);

