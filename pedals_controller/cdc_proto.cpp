#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "cdc_proto.h"
#include "motors.h"

extern "C" {
  #include "tusb.h"
}

uint8_t checksum8(const uint8_t *data, size_t length) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < length; i++) {
        checksum += data[i]; // Sums up all bytes, overflowing to stay in 8-bit
    }
    return checksum;
}

size_t tryReadPacket(uint8_t itf, uint8_t* buffer, size_t remainingLength)
{
    packet_s * packet = (packet_s*)buffer;

    if (packet->size < sizeof(packet_s))
        return 1; // Конец буфера
    
    if (packet->size > remainingLength)
        return 1; // Битые данные, смещаемся на 1 байт дальше.

    if (packet->packet_id >= PacketID::MAX_PACKETS)
        return 1;  // Некорректный идентификатор пакета, смещаемся на 1 байт дальше

    // uint8_t receievedCheckSum = packet->checkSum;

    // packet->checkSum = 0;

    // uint8_t calculatedChecksum = 0;
    // //checksum8(buffer, remainingLength);

    // if (calculatedChecksum != receievedCheckSum)
    // {
    //     // Несоответствия контрольной суммы.

    // }

    PacketID id = (PacketID)packet->packet_id;

    switch(id)
    {
        case PacketID::DisableAllMotors:
            motorsDisable();
            break;            
        case PacketID::SetClutchMotor:
            motorsSet(Motor::CLUTCH, packet->data[0]);
            break;
        case PacketID::SetBrakeMotor:
            motorsSet(Motor::BRAKE, packet->data[0]);           
            break;
        case PacketID::SetThrottleMotor:
            motorsSet(Motor::THROTTLE, packet->data[0]);
            break;
        case PacketID::SetPedalQCoeff:
            break;       
    }

    return packet->size;

}

void cdcParseData(uint8_t itf, uint8_t * buffer, size_t len)
{
    size_t offset = 0;

    while(offset < len)
    {
        offset += tryReadPacket(itf, &buffer[offset], len - offset);
    }

    tud_cdc_n_read_flush(itf);
}
