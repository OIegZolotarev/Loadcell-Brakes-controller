#include <Adafruit_TinyUSB.h>
#include "consts.h"
// #include <Filters.h>
#include <pico/bootrom.h>
#include "common.h"


bool debug = false;
uint8_t debug_level;
int debug_cycle;

// Создаём интерфейсы на этом устройстве
Adafruit_USBD_CDC usb_serial;
Adafruit_USBD_HID usb_hid;



void setup() {
  pinsInit();
  usbInit();  
  ad620Init();



}

void serialThink()
{
    if (usb_serial.available()) {
    String cmd = usb_serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.startsWith("M1=")) {
      int val = constrain(cmd.substring(3).toInt(),0,255);
      analogWrite(GAS_PIN, val);
      usb_serial.printf("Setting throttle motor to %d\n", val);
    } else if (cmd.startsWith("M2=")) {
      int val = constrain(cmd.substring(3).toInt(),0,255);
      analogWrite(BRAKE_PIN, val);
      usb_serial.printf("Setting brake motor to %d\n", val);
    } else if (cmd.startsWith("M3=")) {
      int val = constrain(cmd.substring(3).toInt(),0,255);
      analogWrite(CLUTCH_PIN, val);
      usb_serial.printf("Setting clutch motor to %d\n", val);    
    } else if (cmd.startsWith("Q=")) {
      float val = constrain(cmd.substring(3).toFloat(),0,1);
      _q = val;      
      usb_serial.printf("Set Q to %d\n", _q);      
    } else if (cmd.startsWith("update_firmware")){
      usb_serial.printf("Rebooting in UF2 mode in a moment...\n");
      delay(200);
      reset_usb_boot(0, 0);    
    }else if (cmd.startsWith("debug")) {       
        debug = !debug;
        debug_level = 0;
        debug_cycle = 0;
        usb_serial.printf("debug flag = %d\n", debug);
    }
    else if (cmd.startsWith("cycle")) {       
        
        analogWrite(GAS_PIN, 255);
        usb_serial.printf("Setting throttle motor to %d\n", 255);
        delay(1000);
        analogWrite(GAS_PIN, 0);
        delay(1000);

        analogWrite(BRAKE_PIN, 255);
        usb_serial.printf("Setting brake motor to %d\n", 255);
        delay(1000);       
        analogWrite(BRAKE_PIN, 0);
        delay(1000); 

        analogWrite(CLUTCH_PIN, 255);
        usb_serial.printf("Setting clutch motor to %d\n", 255);
        delay(1000);
        analogWrite(CLUTCH_PIN, 0);

    }
    else if (cmd.startsWith("trace_brake")) {       
        trace_brake = !trace_brake;
    }


    else{
      usb_serial.printf("Unknown command!\n");
    }

    
  }
}


void loop() {
  serialThink();
  
  if (!debug)
  {
    loadCellThink();  
    delay(4); // ~250Hz опрос
  }
  else
  {
    
      debug_level += 1;

      if (debug_level == 0)  
      {
          analogWrite(GAS_PIN, 0);
          analogWrite(BRAKE_PIN, 0);
          analogWrite(CLUTCH_PIN, 0);
          debug_cycle += 1;
      }
      
      
      delay(10);
      usb_serial.printf("debug_level=%d, cycl=%d\n", debug_level, debug_cycle);
      analogWrite(GAS_PIN + debug_cycle %3, debug_level);
  }


}