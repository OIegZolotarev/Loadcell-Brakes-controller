#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/adc.h"
#include "loadcell_pedal.h"

#include "bsp/board.h"

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


int main()
{
    adc_init();

    brakePedal.initHardware();
    
    board_init();
    tusb_init();
    

    while (1)
    {
        hid_task();
        tud_task(); // tinyusb device task
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