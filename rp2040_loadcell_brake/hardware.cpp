#include <Arduino.h>
#include "consts.h"
#include "common.h"

uint32_t lastReport = 0;
const uint32_t REPORT_INTERVAL = 10; // мс, 100Hz


volatile bool hid_report_ready = true;

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
    sum += analogRead(AD620_OUT_PIN) * (ad620.max_voltage / 4095.0f);
    delay(10);
  }
  ad620.min_voltage = sum / 100.0f;
}

// Callback для GetReport (Input Report)
void hidReportCallback(uint8_t report_id, hid_report_type_t report_type,
                        uint8_t const* buffer, uint16_t bufsize) {
  if (report_type != HID_REPORT_TYPE_INPUT)
    return;

  
  usb_hid.sendReport(1, &hid_report, sizeof(hid_report));

}

void usbInit()
{
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));  
  usb_hid.setReportCallback(0, hidReportCallback);
  usb_hid.begin(); 

  usb_serial.begin(115200);
  // while (!USBDevice.mounted());

}



void reportBrakeLevel(uint8_t brake_out)
{
    if (!usb_hid.ready())
      return;

    uint32_t now = millis();
    if (now - lastReport < REPORT_INTERVAL)
      return; // ещё рано отправлять

    // Не отправляем, если предыдущий отчёт ещё не принят хостом
    if (!hid_report_ready) 
      return;


    // Отправка отчета
    hid_report.brake = brake_out;  

    // Пробуем отправить
    if (usb_hid.sendReport(1, &hid_report, sizeof(hid_report))) {
      hid_report_ready = false;  // Ждём подтверждения в callback'е
      lastReport = now;
    } else {
      // Если sendReport вернул false — канал занят, сбрасываем флаг
      hid_report_ready = true;
    }

}
