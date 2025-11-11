
#pragma once

// Кастомный дескриптор (только ось тормоза)
uint8_t const desc_hid_report[] = {
  0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
  0x09, 0x04,        // USAGE (Joystick)
  0xA1, 0x01,        // COLLECTION (Application)
  0x85, 0x01,        // REPORT_ID (1)
  0x15, 0x00,        // LOGICAL_MINIMUM (0)
  0x26, 0xFF, 0x00,  // LOGICAL_MAXIMUM (255)
  0x09, 0x33,        // USAGE (Rx) - ось тормоза
  0x75, 0x08,        // REPORT_SIZE (8)
  0x95, 0x01,        // REPORT_COUNT (1)
  0x81, 0x02,        // INPUT (Data,Var,Abs)
  0xC0               // END_COLLECTION
};

// Калибровка
extern float ZERO_VOLTAGE;
extern float MAX_FORCE_KG;

const float MAX_SENSOR_VOLTAGE = 2.8f;

// Настройки Load Cell
const int AD620_OUT_PIN = 28;


const int MOTOR0_PIN = 16;
const int MOTOR1_PIN = 17;
const int MOTOR2_PIN = 18;

#define GAS_PIN MOTOR2_PIN
#define BRAKE_PIN MOTOR0_PIN
#define CLUTCH_PIN MOTOR1_PIN