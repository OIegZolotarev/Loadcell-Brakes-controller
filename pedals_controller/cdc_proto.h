
enum PacketID
{
    DisableAllMotors = 0,
    SetClutchMotor,
    SetBrakeMotor,
    SetThrottleMotor,
    SetPedalQCoeff,    
    MAX_PACKETS
};

void cdcParseData(uint8_t itf, uint8_t * buffer, size_t len);

typedef struct packet_s
{
    uint8_t size;
    uint8_t packet_id;
    uint8_t checkSum;
    uint8_t data[0]; /* Переменная длина*/
}packet_t;


