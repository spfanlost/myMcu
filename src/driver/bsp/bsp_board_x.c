

#include "common.h"
#include "mcu.h"
#include "drv_xxx.h"

dword_t bsp_myf407_led_init(void);
dword_t bsp_myf407_led_on(dword_t pin);
dword_t bsp_myf407_led_off(dword_t pin);

struct led_dev bsp_myf407_led =
{
    bsp_myf407_led_init,
    bsp_myf407_led_on,
    bsp_myf407_led_off,
};

dword_t bsp_myf407_led_init(void)
{
    return SUCCESS;
}

dword_t bsp_myf407_led_on( dword_t pin)
{
    return SUCCESS;
}

dword_t bsp_myf407_led_off(dword_t pin)
{
    return SUCCESS;
}


