#include <stdio.h>
#include "pico/stdlib.h"

#include "loadcell_pedal.h"
#include <cmath>
#include "hardware/adc.h"
#include "utils.h"

int adcChannelsPins[4] = {26, 27, 28, 29};

LoadCellPedal::LoadCellPedal(uint8_t _adcChannel, uint8_t _cellMaxWeightRating, float _max_sensor_voltage)
{
    cellMaxWeight = _cellMaxWeightRating;
    maxVoltage = _max_sensor_voltage;
    adcChannel = _adcChannel;    
}

void LoadCellPedal::initHardware(uint8_t calibrationIterations)
{
    adc_gpio_init(adcChannelsPins[adcChannel]);

    adc_select_input(adcChannel);
    float sum = 0;

    for (int i = 0; i < 100; i++)
    {
        uint16_t volttage = adc_read();
        sum += volttage * (maxVoltage / 4095.0f);
        sleep_ms(10);
    }

    minVoltage = sum / 100.0f;

    printf("Pedal #%d: detected range %f - %f volts\n", adcChannel, minVoltage, maxVoltage);
}
float LoadCellPedal::simpleKalman(float newVal)
{
    float gain, current_estimate;

    gain = (float)err_estimate / (err_estimate + err_measure);
    current_estimate = last_estimate + (float)gain * (newVal - last_estimate);
    err_estimate = (1.0 - gain) * err_estimate + fabs(last_estimate - current_estimate) * q_coeff;
    last_estimate = current_estimate;
    return current_estimate;
}

uint16_t LoadCellPedal::update()
{
    // Чтение с фильтрацией

    adc_select_input(adcChannel);
    volatile uint16_t raw = adc_read();
    float voltage = raw * (maxVoltage / 4095.0f);

    voltage = simpleKalman(voltage);

    // Преобразование в усилие
    float force_kg = ((voltage - minVoltage) / 1.65f) * cellMaxWeight;
    force_kg = clamp(force_kg, 0, cellMaxWeight);

    float range = 65535.f;

    // Масштабирование с нелинейностью (опционально)
    uint16_t brake_out = pow(force_kg / cellMaxWeight, 0.7) * range;
    brake_out = force_kg / cellMaxWeight * range;

    return brake_out;
}