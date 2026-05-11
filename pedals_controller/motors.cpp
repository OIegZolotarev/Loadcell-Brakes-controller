#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "motors.h"

uint motorPins[3] = {17,16,18};
uint pwm_slices[3] = {0};

uint motorLevels[3];
uint channels[3];

const uint PWM_WRAP = 6249;

void motorsInit()
{
        
    for (int i = 0; i < 3; i++)
    {
        uint pin = motorPins[i];

        gpio_init(pin);
        gpio_set_function(pin, GPIO_FUNC_PWM);

        pwm_slices[i] = pwm_gpio_to_slice_num(pin);

        pwm_set_clkdiv(pwm_slices[i], 1.0f);
        pwm_set_wrap(pwm_slices[i], PWM_WRAP);

        channels[i] = pwm_gpio_to_channel(pin);

        pwm_set_chan_level(pwm_slices[i], channels[i], PWM_WRAP - 1);

        pwm_set_enabled(pwm_slices[i], true);
        sleep_ms(TEST_SEQUENCE_DURATION_MS);
        pwm_set_chan_level(pwm_slices[i], channels[i], 0);
        sleep_ms(TEST_SEQUENCE_PAUSE_MS);        
    }

    printf("motorsInit(): done\n");
}

void motorsSet(Motor index, uint8_t value)
{
    motorLevels[index] = value;
}


void motorsUpdate()
{
    for (int i = 0 ; i < 3; i++)
    {
        auto slice = pwm_slices[i];
        auto channel =  channels[i];

        auto pwm_value = (PWM_WRAP * motorLevels[i]) / 255;

        pwm_set_chan_level(slice, channel, pwm_value);
    }
}

void motorsDisable()
{
    for (int i = 0 ; i < 3; i++)
    {
        auto slice = pwm_slices[i];
        auto channel =  pwm_gpio_to_channel(motorPins[i]);

        pwm_set_chan_level(slice, channel, 0);
    }
}