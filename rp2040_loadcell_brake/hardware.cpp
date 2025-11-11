#include <Arduino.h>
#include "consts.h"
#include "common.h"

struct {
  uint8_t brake; // Только ось тормоза 0-255
} hid_report;

void pinsInit()
{
  analogReadResolution(12); // Явно устанавливаем 12 бит
  pinMode(AD620_OUT_PIN, INPUT_PULLDOWN);
  pinMode(MOTOR0_PIN, OUTPUT);
  pinMode(MOTOR1_PIN, OUTPUT);
  pinMode(MOTOR2_PIN, OUTPUT);

  analogWrite(MOTOR0_PIN, 0);
  analogWrite(MOTOR1_PIN, 0);
  analogWrite(MOTOR2_PIN, 0);

  //analogWriteFreq(1000);
}

void ad620Init()
{
    // Калибровка нуля (лучше вынести в отдельную процедуру)
  float sum = 0;
  for(int i=0; i<100; i++) {
    sum += analogRead(AD620_OUT_PIN) * (MAX_SENSOR_VOLTAGE / 4095.0f);
    delay(10);
  }
  ZERO_VOLTAGE = sum / 100.0f;
}

void usbInit()
{
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));  
  usb_hid.begin(); 

  usb_serial.begin(115200);
  while (!USBDevice.mounted());

  // Working

  // // Настраиваем HID дескриптор
  // usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  
  // // Начинаем работу USB устройств
  // usb_hid.begin();
  // usb_serial.begin(115200);
  
  // // Ждем подключения к ПК
  // while (!USBDevice.mounted());
}

void repotBrakeLevel(uint8_t brake_out)
{
    // Отправка отчета
  hid_report.brake = brake_out;
  if (usb_hid.ready())
    usb_hid.sendReport(1, &hid_report, sizeof(hid_report));
}
