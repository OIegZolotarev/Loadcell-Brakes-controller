#include "hardware/watchdog.h"
#include <Adafruit_TinyUSB.h>
#include "consts.h"
// #include <Filters.h>

#include <pico/bootrom.h>
#include <pico/multicore.h>

#include "common.h"

#include "USBSerialCommand.h"

void applyRumble(int clutch, int brake, int throttle);

// Создаём интерфейсы на этом устройстве
Adafruit_USBD_CDC usb_serial;
Adafruit_USBD_HID usb_hid;

SerialCommand console;

// debug
int tick = 0;
int throttleRumble = 0;
int brakeRumble = 0;
int clutchRumble = 0;

bool trace_rumble = false;

extern volatile bool hid_report_ready;
extern "C" void tud_hid_report_complete_cb(uint8_t itf, uint8_t const* report, uint16_t len) {
  (void)itf;
  (void)report;
  (void)len;
  hid_report_ready = true;  // Теперь можно отправлять следующий отчёт
}

void setup() {
  pinsInit();
  usbInit();  
  ad620Init();


  Con_Printf("=======================\n");
  Con_Printf("Loadcell brakes controller ready!\n");
  Con_Printf("type help, to see avaible commands\n");
  
  console.addExecuteCommand((const char*)"help", []() 
  {
      console.printHelp();
  });

  console.addExecuteCommand((const char*)"trace_brake", []() 
  {
    trace_brake = !trace_brake;
  });

  console.addExecuteCommand((const char*)"update_firmware", []() 
  {
      usb_serial.printf("Rebooting in UF2 mode in a moment...\n");
      delay(200);
      reset_usb_boot(0, 0); 
  });

  console.addExecuteCommand((const char*)"trace_rumble", []() 
  {
      trace_rumble = !trace_rumble;
  });

  console.addExecuteCommand((const char*)"wd_test", []() 
  {      
      while (1); // намеренно зависаем
  });

  console.addExecuteCommand((const char*)"set_motor", []() 
  {
      auto motor_index = console.next();

      int val = console.nextInt();
      val = constrain(val,0,255);

      //Con_Printf("motor_index=%s, val = %d\n", motor_index, val);

      switch(*motor_index)
      {
        case '1':
        case 'T':
          {                            
              analogWrite(GAS_PIN, val);
              throttleRumble = val;
          }
          break;
        case '2':
        case 'B':
          {
              analogWrite(BRAKE_PIN, val);
              brakeRumble = val;
          }
          break;
        case '3':
        case 'C':
          {
              analogWrite(CLUTCH_PIN, val);
              clutchRumble = val;
          }      
          break;          
        default:
            Con_Printf("Unknown motor index! Use 1,2,3 or C,B,T (case sensitive)");
            break;
      }
  });

  console.addExecuteCommand((const char*)"pedal_rumble", []() 
  {
      clutchRumble = console.nextInt();
      clutchRumble = constrain(clutchRumble,0,255);

      brakeRumble = console.nextInt();
      brakeRumble = constrain(brakeRumble,0,255);

      throttleRumble = console.nextInt();
      throttleRumble = constrain(throttleRumble,0,255);

      applyRumble(clutchRumble, brakeRumble, throttleRumble);
  });

  console.addExecuteCommand((const char*)"tune", []() 
  {
      auto parameter = console.next();

      switch(toupper(*parameter))
      {          
          case 'Q':
            {
              float val = console.nextFloat();
              val = constrain(val,0,1);
              ad620.q = val;      
              Con_Printf("Set Q to %f\n", ad620.q);      
            }
            break;
      }
  });

  console.addExecuteCommand((const char*)"calibrate", []() 
  {
        ad620.calibrating = !ad620.calibrating;
  });

  watchdog_enable(1000, 1); // 1000 мс, reset on timeout
}

void applyRumble(int clutch, int brake, int throttle)
{
      clutchRumble = clutch;      
      brakeRumble = brake;      
      throttleRumble = throttle;
      

      analogWrite(GAS_PIN, throttleRumble);
      analogWrite(BRAKE_PIN, brakeRumble);
      analogWrite(CLUTCH_PIN, clutchRumble);
}

void processBinary() {
  static uint8_t state = 0;
  static uint8_t buf[4];
  static uint8_t idx = 0;

  while (usb_serial.available()) {
    uint8_t b = usb_serial.read();

    switch (state) {
      case 0: // ждём старт
        if (b == 0x01) {
          idx = 0;
          state = 1;
        }
        break;

      case 1: // читаем данные
        buf[idx++] = b;
        if (idx == 4) {
          uint8_t checksum = 0x01 ^ buf[0] ^ buf[1] ^ buf[2];

          if (checksum == buf[3]) {
            applyRumble(buf[0], buf[1], buf[2]);
          }

          state = 0;
        }
        break;
    }
  }
}


void loop() 
{
  uint32_t loopStart = millis();
  watchdog_update();
  tud_task();

  while (usb_serial.available()) 
  {
    uint8_t b = usb_serial.peek();

      if (b == 0x01) {
        processBinary();
      } else {
        //Con_Printf("Text\n");
        console.loop();
      }
  }      

  loadCellThink();  

  if (trace_rumble)
  {
    Con_Printf("Tick: %d, C = %d, B = %d, T = %d\n", tick, clutchRumble, brakeRumble, throttleRumble);
  }

}