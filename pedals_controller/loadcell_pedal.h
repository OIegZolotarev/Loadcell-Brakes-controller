
class LoadCellPedal
{
    // Данные калибровки
    float minVoltage;
    float maxVoltage;

    uint8_t adcChannel;
    uint8_t cellMaxWeight;

    // Фильтрация по Калману
    float err_measure = 0.8f;
    float err_estimate = 0.8f;
    float last_estimate;
    float q_coeff = 0.5f;

    float simpleKalman(float newVal);
    
public:
    LoadCellPedal(uint8_t adcChannel, uint8_t cellMaxWeightRating, float max_sensor_voltage = 2.8f);
    
    void initHardware(uint8_t calibrationIterations = 100);
    uint16_t update();
};