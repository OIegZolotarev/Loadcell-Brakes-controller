#include "arduino_compat.h"
#include "ArqSerial.h"
#include "FlowSerial.h"


#define DEVICE_NAME "LoadCell Pedals with vibration" //{"Group":"General","Name":"DEVICE_NAME","Title":"Device name,\r\n make sure to use a unique name when using multiple arduinos","DefaultValue":"SimHub Dash","Type":"string","Template":"#define DEVICE_NAME \"{0}\""}
#define DEVICE_UNIQUE_ID "9c72b833-2070-463b-be46-f1e01ba42742" //{"UniqueId":"86f28d79-7f53-4c1f-9dd1-0a862ff9a14e","Name":"DEVICE_UNIQUE_ID","Type":"uniqueid"}

#define VERSION 'j'
#define MESSAGE_HEADER 0x03

#define WS2812B_RGBLEDCOUNT 0
#define PL9823_RGBLEDCOUNT 0
#define WS2801_RGBLEDCOUNT 0
#define ENABLED_OLEDLCD 0 
#define ENABLED_NOKIALCD 0
#define MAX7221_MATRIX_ENABLED 0
#define WS2812B_MATRIX_ENABLED 0
#define DM163_MATRIX_ENABLED 0
#define SUNFOUNDERSH104P_MATRIX_ENABLED 0
#define I2CLCD_enabled 0
#define GAMEPAD_AXIS_01_ENABLED 0
#define GAMEPAD_AXIS_02_ENABLED 0
#define GAMEPAD_AXIS_03_ENABLED 0
#define DUALVNH5019_ENABLED 0
#define MOTOMONSTER_ENABLED 0
#define ENABLE_ADA_HT16K33_BiColorMatrix 0 
#define ENABLE_ADA_HT16K33_SingleColorMatrix 0

#define ENABLED_BUTTONS_COUNT 0
#define ENABLED_BUTTONMATRIX 0 

#define BMATRIX_COLS 0
#define BMATRIX_ROWS 0

#define TM1638_ENABLEDMODULES 0
#define MAX7221_ENABLEDMODULES 0
#define TM1637_ENABLEDMODULES 0
#define TM1637_6D_ENABLEDMODULES 0
#define ENABLE_ADA_HT16K33_7SEGMENTS 0


void simhub_entry();

#define ADAMOTORS_SHIELDSCOUNT 0
#define DKMOTOR_SHIELDSCOUNT 0
#define L98NMOTORS_ENABLED 3
#define SHAKEITPWM_ENABLED_MOTORS 0
#define SHAKEITPWMFANS_ENABLED_MOTORS 0


#define INCLUDE_SHAKEITL298N