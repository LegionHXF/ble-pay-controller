#ifndef __DRV_SMG_H__
#define __DRV_SMG_H__

#include "bsp.h"

#define SOFT_IIC_SDA_GPIO_CLK RCC_APB2_PERIPH_GPIOB
#define SOFT_IIC_SCL_GPIO_CLK RCC_APB2_PERIPH_GPIOB
#define SOFT_IIC_SDA_PORT GPIOB
#define SOFT_IIC_SDA_PIN  GPIO_PIN_6
#define SOFT_IIC_SCL_PORT GPIOB
#define SOFT_IIC_SCL_PIN  GPIO_PIN_7

#define SOFT_IIC_PIN_MODE  GPIO_MODE_OUTPUT_OD


typedef enum
{
    BSP_DIGITRON_POWER_OFF = 0,
    BSP_DIGITRON_POWER_ON
}bsp_DigitronPower_t;

typedef enum
{
    BSP_DIGITRON_BRIGHTNESS_8 = 0,
    BSP_DIGITRON_BRIGHTNESS_1,
    BSP_DIGITRON_BRIGHTNESS_2,
    BSP_DIGITRON_BRIGHTNESS_3,
    BSP_DIGITRON_BRIGHTNESS_4,
    BSP_DIGITRON_BRIGHTNESS_5,
    BSP_DIGITRON_BRIGHTNESS_6,
    BSP_DIGITRON_BRIGHTNESS_7
}bsp_DigitronBrightness_t;

bool drv_soft_iic_init(void);
bool drv_soft_iic_read_byte(uint8_t ack, uint8_t *byt);
void bsp_DigitronDisplayTime(uint32_t time);
void bsp_DigitronPowerBrightnessSet(bsp_DigitronPower_t power,bsp_DigitronBrightness_t brightness);
#endif







