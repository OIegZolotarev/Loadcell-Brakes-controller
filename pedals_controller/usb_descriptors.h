#include "common/tusb_common.h"
#include "device/usbd.h"

#define GAMECON_REPORT_DESC_GAMEPAD(...)                   \
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),                \
        HID_USAGE(HID_USAGE_DESKTOP_GAMEPAD),              \
        HID_COLLECTION(HID_COLLECTION_APPLICATION),        \
        __VA_ARGS__                                        \        
        HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),            \
            HID_LOGICAL_MIN_N(-32768, 2),                             \
            HID_LOGICAL_MAX_N(32767, 2),                      \
            HID_USAGE(HID_USAGE_DESKTOP_Z),                    \
            HID_REPORT_COUNT(1),                               \
            HID_REPORT_SIZE(16),                                \
            HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
        HID_COLLECTION_END

