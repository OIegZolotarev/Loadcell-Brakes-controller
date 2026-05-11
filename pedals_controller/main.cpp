#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/adc.h"
#include "loadcell_pedal.h"

#include "motors.h"
#include "bsp/board.h"
#include "cdc_proto.h"
#include "pico/time.h"
#include "pico/multicore.h"

#include "simhub/simhub.h"
#include "simhub/arduino_compat.h"

extern "C" {
  #include "tusb.h"
}

LoadCellPedal brakePedal(2, 100); // 0-й канал ADC, тензодатчик на 100 кг

void hid_task(void)
{
    // Опрос с частотой 250 гц
    const uint32_t interval_ms = 1000 / 250;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    

    typedef struct report_s
    {
        uint16_t brakeLevel;
    }report_t;
      
    report_t report;

    start_ms += interval_ms;
    
    report.brakeLevel = brakePedal.update() - 32768;
    
    
    // Remote wakeup
    if (tud_suspended())
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }

    if (tud_hid_ready())
    {
        tud_hid_n_report(0x00, 0x01, &report, sizeof(report));
    }

}

static void echo_serial_port(uint8_t itf, uint8_t buf[], uint32_t count) 
{

    for (uint32_t i = 0; i < count; i++) {
        tud_cdc_n_write_char(itf, buf[i]);
        tud_cdc_n_write_char(itf, 'F');
    }
    tud_cdc_n_write_flush(itf);
}

static void cdc_task(void) {
    uint8_t itf = 0;

    if (tud_cdc_n_available(itf)) {
                
        uint8_t buf[CFG_TUD_CDC_RX_BUFSIZE];
        uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

        for (size_t i = 0 ; i < count; i++)
            Serial.core0_pushRX(buf[i]);
        
        // printf("cdc_task(): pushed %d bytes\n", count);
    }    

    Serial.core0_popTX(itf);
}

int main()
{
    stdio_init_all();
    
    printf("\033[2J\033[H");
    printf("===================\n");
    printf("Starting board...\n");
    printf("===================\n");

    adc_init();
    board_init();
    
    brakePedal.initHardware();
    motorsInit();

    multicore_launch_core1(simhub_entry);
    
    tusb_init();
    
    absolute_time_t start = get_absolute_time();
    // 60 hz обновление моторов
    const uint motorsUpdateRate = (1000 / 60) * 1000;

    while (1)
    {        
        tud_task(); // tinyusb device task
        hid_task();
        cdc_task();        
        
        absolute_time_t end = get_absolute_time();
        int64_t elapsed_us = absolute_time_diff_us(start, end);

        if (elapsed_us > motorsUpdateRate)
        {
            start = end;
            motorsUpdate();
        }

    }
}



//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    // if (report_id == 2 && report_type == HID_REPORT_TYPE_OUTPUT && buffer[0] == 2 && bufsize >= sizeof(light_data)) //light data
    // {
    //     size_t i = 0;
    //     for (i; i < sizeof(light_data); i++)
    //     {
    //         light_data.raw[i] = buffer[i + 1];
    //     }
    // }

    // echo back anything we received from host
    tud_hid_report(0, buffer, bufsize);
}