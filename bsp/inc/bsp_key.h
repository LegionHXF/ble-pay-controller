#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#define BSP_KEY_GPIO_PORT  GPIOA

#define BSP_KEY1_GPIO_PIN   GPIO_PIN_0
#define BSP_KEY2_GPIO_PIN   GPIO_PIN_1

void bsp_key_init(void);
void bsp_key_scan(void);



#endif // !__BSP_KEY_H__
