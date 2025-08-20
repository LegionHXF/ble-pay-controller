#ifndef __BSP_RELAY_H__
#define __BSP_RELAY_H__

#define BSP_RELAY_GPIO_PORT  GPIOB
#define BSP_RELAY_GPIO_PIN   GPIO_PIN_11

void bsp_relay_init(void);
void bsp_relay_on(void);
void bsp_relay_off(void);


#endif // __BSP_RELAY_H__
