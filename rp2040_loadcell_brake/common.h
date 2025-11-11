

void ad620Init();
void usbInit();
void pinsInit();

void repotBrakeLevel(uint8_t level);
void loadCellThink();

#include <Adafruit_TinyUSB.h>

extern Adafruit_USBD_CDC usb_serial;
extern Adafruit_USBD_HID usb_hid;

extern float _q;
extern bool trace_brake;