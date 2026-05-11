#include "simhub.h"
#include "SHCommands.h"
#include "FlowSerial.h"
#include "profiler_cookie.h"

char loop_opt;
unsigned long lastSerialActivity = 0;

void simhub_loop()
{
	
	// Wait for data
	if (FlowSerialAvailable() > 0) {
		if (FlowSerialTimedRead() == MESSAGE_HEADER)
		{
			// printf("last cmd = %c, delta = %d ms\n", loop_opt,millis() - lastSerialActivity);

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
				if (xaction == "list") Command_ExpandedCommandsList();
				else if (xaction == "mcutype") Command_MCUType();
				else if (xaction == "tach") Command_TachData();
				else if (xaction == "speedo") Command_SpeedoData();
				else if (xaction == "boost") Command_BoostData();
				else if (xaction == "temp") Command_TempData();
				else if (xaction == "fuel") Command_FuelData();
				else if (xaction == "cons") Command_ConsData();
				else if (xaction == "encoderscount") Command_EncodersCount();
			}
		}
		else
		{
			printf("Bad header!!!\n");
		}
		
	}
	else
	{
		// printf("No data!!, Serial.avaible() = %d\n", Serial.available());
	}

	if (millis() - lastSerialActivity > 5000) {
		Command_Shutdown();
	}
}

#define LED_PIN PICO_DEFAULT_LED_PIN


void simhub_entry()
{
	
	uint32_t nextPinUpdate = millis() + 100;
	uint8_t pinState = 0;

	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	printf("Started simhub loop on core1\n");

	while(1)
	{

		simhub_loop();


		uint32_t time = millis();

		if (time < (Serial.getLastNoneEmptyRXTime() + 100))
		{
			if (time > nextPinUpdate)
			{
				pinState = !pinState;
				gpio_put(LED_PIN, pinState);
				nextPinUpdate = millis() + 100;			
			}
		}
		else
			gpio_put(LED_PIN, 0);
	}
}