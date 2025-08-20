#include "bsp_relay.h"
#include "n32wb03x_gpio.h"


void bsp_relay_init(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(BSP_RELAY_GPIO_PORT));

    /* Enable the GPIO Clock */
    if (BSP_RELAY_GPIO_PORT == GPIOA)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    }
    else if (BSP_RELAY_GPIO_PORT == GPIOB)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    }

    /* Configure the GPIO pin */
    if (BSP_RELAY_GPIO_PIN <= GPIO_PIN_ALL)
    {
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin = BSP_RELAY_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitPeripheral(BSP_RELAY_GPIO_PORT, &GPIO_InitStructure);
    }    

    bsp_relay_off();
}

void bsp_relay_on(void)
{
    GPIO_SetBits(BSP_RELAY_GPIO_PORT, BSP_RELAY_GPIO_PIN);    
}

void bsp_relay_off(void)
{
    GPIO_ResetBits(BSP_RELAY_GPIO_PORT, BSP_RELAY_GPIO_PIN);    
}

// Path: BSP/bsp_led.h

