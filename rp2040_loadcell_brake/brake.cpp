#include <Adafruit_TinyUSB.h>
#include "consts.h"
#include <math.h>
#include "common.h"

ad620_settings_t ad620;
bool trace_brake = false;

float simpleKalman(float newVal) {
  float _kalman_gain, _current_estimate;
  static float _err_estimate = ad620.err_measure;
  static float _last_estimate;

  _kalman_gain = (float)_err_estimate / (_err_estimate + ad620.err_measure);
  _current_estimate = _last_estimate + (float)_kalman_gain * (newVal - _last_estimate);
  _err_estimate = (1.0 - _kalman_gain) * _err_estimate + fabs(_last_estimate - _current_estimate) * ad620.q;
  _last_estimate = _current_estimate;
  return _current_estimate;
}


void loadCellThink() {
  // Чтение с фильтрацией
  float voltage = analogRead(AD620_OUT_PIN) * (ad620.max_voltage / 4095.0f);
  // filter.Filter(voltage);
  voltage = simpleKalman(voltage);

  // Преобразование в усилие
  float force_kg = ((voltage - ad620.min_voltage) / 1.65f) * ad620.max_force;
  force_kg = constrain(force_kg, 0, ad620.max_force);

  // Масштабирование с нелинейностью (опционально)
  uint8_t brake_out = pow(force_kg / ad620.max_force, 0.7) * 255;
  //uint8_t brake_out = force_kg / ad620.max_force * 255;

  if (trace_brake)
    Con_Printf("raw voltage=%f , force_kg = %f, brake_output = %d\n", voltage, force_kg, brake_out);
  
  if (ad620.calibrating)
  {
      if (voltage < ad620.min_voltage)
        ad620.min_voltage = voltage;
      if (voltage > ad620.max_voltage)
        ad620.max_voltage = voltage;

      Con_Printf("Calibration: min=%f\tmax=%f\n", ad620.min_voltage, ad620.max_voltage);
  }

  reportBrakeLevel(brake_out);
}
