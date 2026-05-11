#include <pico/stdlib.h>
#include "ArqSerial.h"
#include "profiler_cookie.h"

size_t consumedBytes = 0;

int ARQSerial::Arq_TimedRead()
{
	int c;
	unsigned long fsr_startMillis = millis();
	do {
		if (idleFunction != 0) idleFunction(true);
		c = Serial.read();
		if (c >= 0) {
#ifdef TESTFAIL
			testfailidx = (testfailidx + 1) % 5000;
			if (testfailidx == 500)
				return random(255);

			if (testfailidx == 1000)
				return -1;
#endif
			consumedBytes++;
			return c;
		}
	} while (millis() - fsr_startMillis < 100);
	return -1;
}

void ARQSerial::ProcessIncomingData()
{
	int packetID, length, header, res, i, crc, nextpacketid;
	byte currentCrc;

	consumedBytes = 0;

	while (Serial.available() > 0) {
		header = Arq_TimedRead();
		//DebugPrintLn("hello1");
		currentCrc = 0;

		if (header == 0x01) {
			byte reason = 0x00;

			header = Arq_TimedRead();
			if (header != 0x01) {
				return;
			}

			if (reason == 0) {
				packetID = Arq_TimedRead();
				if (packetID < 0) {
					reason = 0x01;
				}
			}

			if (reason == 0) {
				length = Arq_TimedRead();
				if (length <= 0 || length > 32) {
					reason = 0x02;
				}
			}

			if (reason == 0)
			{
				for (i = 0; i < length && !reason; i++) {
					res = Arq_TimedRead();
					partialdatabuffer[i] = res;
					if (res < 0) reason = 0x05;
				}
			}

			if (reason == 0) {
				crc = Arq_TimedRead();
				if (crc < 0) {
					reason = 0x03;
				}
			}

			if (reason == 0) {
				currentCrc = updateCrc(currentCrc, packetID);
				currentCrc = updateCrc(currentCrc, length);
				for (i = 0; i < length; i++) {
					currentCrc = updateCrc(currentCrc, partialdatabuffer[i]);
				}

				if (crc != currentCrc) {
					reason = 0x04;
				}
			}

			

			if (reason == 0) {
				nextpacketid = Arq_LastValidPacket > 127 ? 0 : Arq_LastValidPacket + 1;
				
				//printf("\t\t\t(packetID = %d, nextpacketid = %d)\n", packetID, nextpacketid);

				if (packetID == nextpacketid || packetID == 255) {
					for (i = 0; i < length; i++) {
						DataBuffer.push(partialdatabuffer[i]);
					}
					Arq_LastValidPacket = packetID;
				}
#ifdef TESTFAIL
				testfailidx = (testfailidx + 1) % 5000;
				if (testfailidx != 788) {
					SendAcq(packetID);
				}
#else
				SendAcq(packetID);
#endif
			}

			if (reason > 0) {
				SendNAcq(Arq_LastValidPacket, reason);
			}
		}
	}
}

void ARQSerial::SendAcq(uint8_t packetId)
{
	Serial.write(0x03);
	Serial.write(packetId);
	Serial.flush();
}

void ARQSerial::SendNAcq(uint8_t lastKnownValidPacket, byte reason)
{
	Serial.write(0x04);
	Serial.write(lastKnownValidPacket);
	Serial.write(reason);
	Serial.flush();	
}

void ARQSerial::setIdleFunction(IdleFunction function)
{
	idleFunction = function;
}

void ARQSerial::CustomPacketStart(byte packetType, uint8_t length)
{
	Serial.write(0x09);
	Serial.write(packetType);
	Serial.write(length);
}

void ARQSerial::CustomPacketSendByte(byte data)
{
	Serial.write(data);
}

void ARQSerial::CustomPacketEnd()
{
	//Serial.write(0x00);
}

int ARQSerial::read()
{
	// PROFILE_FUNCTION();

	unsigned long fsr_startMillis = millis();
	do {
		if (idleFunction != 0) idleFunction(false);

		if (DataBuffer.size() > 0) {
			uint8_t res = 0;
			DataBuffer.pop(res);
			return (int)res;
		}

		ProcessIncomingData();
		// if (consumedBytes > 0)
		// 	printf("Consumed bytes(read): %d\n", consumedBytes);
		
	} while (millis() - fsr_startMillis < 400 || DataBuffer.size() > 0);

	//DebugPrintLn("Read timeout !");
	return -1;
}

int ARQSerial::Available()
{
	if (idleFunction != 0) idleFunction(false);
	if (DataBuffer.size() == 0) {		
		ProcessIncomingData();		
		// if (consumedBytes)
		// 	printf("Consumed bytes (available): %d, Databuffer.size(): %d\n", consumedBytes, DataBuffer.size());
	}
	return DataBuffer.size();
}

void ARQSerial::Write(byte data)
{
	Serial.write(0x08);
	Serial.write(data);
	Serial.flush();
}

void ARQSerial::Print(const char str[])
{
	int len = strlen(str);
	Serial.write(0x06);
	Serial.write(len);
	Serial.write(str);
	Serial.write(0x20);
	Serial.flush();
}

void ARQSerial::Print(char data)
{
	Write((byte)data);
}

void ARQSerial::WriteString(String& data)
{
	int len = data.length();
	Serial.write(0x06);
	Serial.write(len);
	Serial.print(data);
	Serial.write(0x20);
	Serial.flush();
}

void ARQSerial::PrintString(const char str[])
{
	int len = strlen(str);
	Serial.write(0x06);
	Serial.write(len);
	Serial.write(str);
	Serial.write(0x20);
	Serial.flush();
}

void ARQSerial::PrintLn()
{
	Write('\n');
}

void ARQSerial::PrintLn(String& data)
{
	Serial.write(0x06);
	Serial.write(data.length() + 1);
	Serial.print(data);
	Serial.print('\n');
	Serial.write(0x20);
	Serial.flush();
}

void ARQSerial::PrintLn(const char str[])
{
	int len = strlen(str);
	Serial.write(0x06);
	Serial.write(len + 1);
	Serial.write(str);
	Serial.write('\n');
	Serial.write(0x20);
	Serial.flush();
}

String ARQSerial::ReadStringUntil(char terminator1)
{
	String ret;
	int c = read();
	while (c >= 0 && c != terminator1)
	{
		ret += (char)c;
		c = read();
	}
	return ret;
}

void ARQSerial::ReadStringUntil(char buffer[], char terminator)
{
	int pos = 0;

	int c = read();
	while (c >= 0 && c != terminator)
	{
		buffer[pos] = (char)c;
		c = read();
		pos++;
	}
	buffer[pos] = 0;
}

String ARQSerial::ReadStringUntil(char terminator1, char terminator2)
{
	String ret;
	
	int c = read();
	while (c >= 0 && c != terminator1 && c != terminator2)
	{
		ret += (char)c;
		c = read();
	}
	return ret;
}

void ARQSerial::DebugPrintLn(const char str[])
{
	Serial.write(0x07);
	Serial.write((byte)(strlen(str) + 1));
	Serial.print(str);
	Serial.print('\n');
	Serial.write(0x20);
	Serial.flush();
}

void ARQSerial::DebugPrintLn(String& data)
{
	Serial.write(0x07);
	Serial.write(data.length() + 1);
	Serial.print(data);
	Serial.print('\n');
	Serial.write(0x20);
	Serial.flush();
}

void ARQSerial::DebugPrint(char data)
{
	Serial.write(0x07);
	Serial.write(1);
	Serial.print(data);
	Serial.write(0x20);
	Serial.flush();
}
