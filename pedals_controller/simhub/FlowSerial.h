#define FlowSerialBegin Serial.begin
#define FlowSerialFlush Serial.flush

#include "ArqSerial.h"
ARQSerial arqserial;

#define FlowSerialAvailable() arqserial.Available()
#define FlowSerialTimedRead() arqserial.read()
#define  FlowSerialWrite(data) arqserial.Write(data)

String FlowSerialReadStringUntil(char terminator) { return arqserial.ReadStringUntil(terminator); }
String FlowSerialReadStringUntil(char terminator1, char terminator2) { return arqserial.ReadStringUntil(terminator1, terminator2); }
void FlowSerialReadStringUntil(char buffer[], char terminator){ arqserial.ReadStringUntil(buffer, terminator); }

void FlowSerialPrint(String& data) { arqserial.WriteString(data); }
void FlowSerialPrint(char data){	arqserial.Print(data);}
void FlowSerialPrint(const char str[]) {	arqserial.Print(str);}
void FlowSerialDebugPrintLn(String& data){	arqserial.DebugPrintLn(data);}
void FlowSerialDebugPrintLn(const char str[]) {	arqserial.DebugPrintLn(str);}
void FlowSerialPrintLn(String& data){	arqserial.PrintLn(data);}
void FlowSerialPrintLn(const char str[]) {	arqserial.PrintLn(str);}
void FlowSerialPrintLn() { arqserial.PrintLn();}

void SetBaudrate() {
    // В CDC не используются baudRate, по сути не нужно.
}
