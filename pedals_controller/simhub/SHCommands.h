#define MESSAGE_HEADER 0x03

// Эмуляция подписи ATmega328P (стандарт для Arduino Uno/Nano)
#define SIGNATURE_0 0x1E
#define SIGNATURE_1 0x95
#define SIGNATURE_2 0x0F

void Command_Hello();
void Command_SetBaudrate();
void Command_ButtonsCount();
void Command_TM1638Count();
void Command_SimpleModulesCount();
void Command_Acq();
void Command_DeviceName();
void Command_UniqueId();
void Command_MCUType();
void Command_EncodersCount() ;
void Command_SpeedoData();
void Command_TachData() ;
void Command_BoostData();
void Command_TempData();
void Command_ConsData();
void Command_FuelData() ;
void Command_GLCDData();
void Command_ExpandedCommandsList() ;
void Command_TM1638Data() ;
void Command_Features();
void Command_Motors() ;
void Command_Shutdown();
void Command_7SegmentsData() ;
void Command_RGBLEDSCount() ;
void Command_RGBLEDSData();
void Command_RGBMatrixData();
void Command_MatrixData() ;
void Command_GearData() ;
void Command_I2CLCDData();
void Command_CustomProtocolData();


#include "SHShakeItImpl.h"