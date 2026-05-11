#pragma once

#define FlowSerialBegin Serial.begin
#define FlowSerialFlush Serial.flush

#include "ArqSerial.h"
extern ARQSerial arqserial;

#define FlowSerialAvailable() arqserial.Available()
#define FlowSerialTimedRead() arqserial.read()
#define  FlowSerialWrite(data) arqserial.Write(data)

inline String FlowSerialReadStringUntil(char terminator) { return arqserial.ReadStringUntil(terminator); }
inline String FlowSerialReadStringUntil(char terminator1, char terminator2) { return arqserial.ReadStringUntil(terminator1, terminator2); }
inline void FlowSerialReadStringUntil(char buffer[], char terminator){ arqserial.ReadStringUntil(buffer, terminator); }

inline void FlowSerialPrint(String& data) { arqserial.WriteString(data); }
inline void FlowSerialPrint(char data){	arqserial.Print(data);}
inline void FlowSerialPrint(const char str[]) {	arqserial.Print(str);}
inline void FlowSerialDebugPrintLn(String& data){	arqserial.DebugPrintLn(data);}
inline void FlowSerialDebugPrintLn(const char str[]) {	arqserial.DebugPrintLn(str);}
inline void FlowSerialPrintLn(String& data){	arqserial.PrintLn(data);}
inline void FlowSerialPrintLn(const char str[]) {	arqserial.PrintLn(str);}
inline void FlowSerialPrintLn() { arqserial.PrintLn();}

inline void SetBaudrate() {
    // В CDC не используются baudRate, по сути не нужно.
}
