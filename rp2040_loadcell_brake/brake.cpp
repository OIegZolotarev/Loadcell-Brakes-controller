#include <Adafruit_TinyUSB.h>
#include <Adafruit_TinyUSB.h>
#include "consts.h"
#include <math.h>
#include "common.h"

float ZERO_VOLTAGE = 1.65f;    // Напряжение без нагрузки (middle of 3.3V)
float MAX_FORCE_KG = 100.0f;    // Макс. усилие (кг)

float _err_measure = 0.8;  // примерный шум измерений
float _q = 0.25;           // скорость изменения значений 0.001-1, варьировать самому

bool trace_brake = false;

float simpleKalman(float newVal) {
  float _kalman_gain, _current_estimate;
  static float _err_estimate = _err_measure;
  static float _last_estimate;

  _kalman_gain = (float)_err_estimate / (_err_estimate + _err_measure);
  _current_estimate = _last_estimate + (float)_kalman_gain * (newVal - _last_estimate);
  _err_estimate = (1.0 - _kalman_gain) * _err_estimate + fabs(_last_estimate - _current_estimate) * _q;
  _last_estimate = _current_estimate;
  return _current_estimate;
}


void loadCellThink() {
  // Чтение с фильтрацией
  float voltage = analogRead(AD620_OUT_PIN) * (MAX_SENSOR_VOLTAGE / 4095.0f);
  // filter.Filter(voltage);
  voltage = simpleKalman(voltage);

  // Преобразование в усилие
  float force_kg = ((voltage - ZERO_VOLTAGE) / 1.65f) * MAX_FORCE_KG;
  force_kg = constrain(force_kg, 0, MAX_FORCE_KG);

  // Масштабирование с нелинейностью (опционально)
  uint8_t brake_out = pow(force_kg / MAX_FORCE_KG, 0.7) * 255;
  //uint8_t brake_out = force_kg / MAX_FORCE_KG * 255;

  if (trace_brake)
    usb_serial.printf("raw voltage=%f , force_kg = %f, brake_output = %d\n", voltage, force_kg, brake_out);
  //Serial.printf("usb_hid.ready() == %d\n", usb_hid.ready());
  //analogWrite(CLUTCH_PIN, brake_out);

  repotBrakeLevel(brake_out);
}
