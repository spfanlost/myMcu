

#define LED1_PIN PIN9 //(PF9)
#define LED1_PORT GPIOF //

#define LED2_PIN PIN10 //(PF10)
#define LED2_PORT GPIOF //

struct led_dev
{
    dword_t (*led_init      )(void);
    dword_t (*led_on        )(dword_t);
    dword_t (*led_off       )(dword_t);
};

extern struct led_dev bsp_myf407_led;

