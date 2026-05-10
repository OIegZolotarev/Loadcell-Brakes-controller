#include "FlowSerial.h"

#define MESSAGE_HEADER 0x03


char loop_opt;
unsigned long lastSerialActivity = 0;

void simhub_loop()
{
	// Wait for data
	if (FlowSerialAvailable() > 0) {
		if (FlowSerialTimedRead() == MESSAGE_HEADER)
		{
			lastSerialActivity = millis();
			// Read command
			loop_opt = FlowSerialTimedRead();

			if (loop_opt == '1') Command_Hello();
			else if (loop_opt == '8') Command_SetBaudrate();
			else if (loop_opt == 'J') Command_ButtonsCount();
			else if (loop_opt == '2') Command_TM1638Count();
			else if (loop_opt == 'B') Command_SimpleModulesCount();
			else if (loop_opt == 'A') Command_Acq();
			else if (loop_opt == 'N') Command_DeviceName();
			else if (loop_opt == 'I') Command_UniqueId();
			else if (loop_opt == '0') Command_Features();
			else if (loop_opt == '3') Command_TM1638Data();
			else if (loop_opt == 'V') Command_Motors();
			else if (loop_opt == 'S') Command_7SegmentsData();
			else if (loop_opt == '4') Command_RGBLEDSCount();
			else if (loop_opt == '6') Command_RGBLEDSData();
			else if (loop_opt == 'R') Command_RGBMatrixData();
			else if (loop_opt == 'M') Command_MatrixData();
			else if (loop_opt == 'G') Command_GearData();
			else if (loop_opt == 'L') Command_I2CLCDData();
			else if (loop_opt == 'K') Command_GLCDData(); // Nokia | OLEDS
			else if (loop_opt == 'P') Command_CustomProtocolData();
			else if (loop_opt == 'X')
			{
				String xaction = FlowSerialReadStringUntil(' ', '\n');
				if (xaction == F("list")) Command_ExpandedCommandsList();
				else if (xaction == F("mcutype")) Command_MCUType();
				else if (xaction == F("tach")) Command_TachData();
				else if (xaction == F("speedo")) Command_SpeedoData();
				else if (xaction == F("boost")) Command_BoostData();
				else if (xaction == F("temp")) Command_TempData();
				else if (xaction == F("fuel")) Command_FuelData();
				else if (xaction == F("cons")) Command_ConsData();
				else if (xaction == F("encoderscount")) Command_EncodersCount();
			}
		}
	}

	if (millis() - lastSerialActivity > 5000) {
		Command_Shutdown();
	}
}