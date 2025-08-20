#ifndef __BSP_BEEP_H__
#define __BSP_BEEP_H__

#include <stdint.h>

#define BSP_BEEP_GPIO_PORT  GPIOA
#define BSP_BEEP_GPIO_PIN   GPIO_PIN_6

typedef enum
{
    BSP_BEEP_MODE_RUNNING = 0,
    BSP_BEEP_MODE_STOP,
}bsp_beep_mode_t;

typedef enum 
{
    BSP_BEEP_STATUS_OK = 0,
    BSP_BEEP_STATUS_ERROR,
}bsp_beep_status_t;

void bsp_beep_init(void);
void bsp_beep_on(void);
void bsp_beep_off(void);


void bsp_beep_process(void);
bsp_beep_status_t bsp_beep_on_nonblock(uint32_t time);

#endif // !__BSP_BEEP_H__

