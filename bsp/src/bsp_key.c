#include "bsp_key.h"
#include "n32wb03x_gpio.h"
#include "ns_log.h"

extern uint32_t SystickGetime(void);

void bsp_key_init(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(BSP_BEEP_GPIO_PORT));

    /* Enable the GPIO Clock */
    if (BSP_KEY_GPIO_PORT == GPIOA)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    }
    else if (BSP_KEY_GPIO_PORT == GPIOB)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    }   

    /* Configure the GPIO pin */
    if (BSP_KEY1_GPIO_PIN <= GPIO_PIN_ALL)
    {
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin = BSP_KEY1_GPIO_PIN | BSP_KEY2_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_MODE_INPUT;
        GPIO_InitPeripheral(BSP_KEY_GPIO_PORT, &GPIO_InitStructure);
    }    
}


void bsp_key_scan(void)
{
    static uint8_t key1_status = 0;
    static uint8_t key2_status = 0;
    static uint32_t key1_time = 0;
    static uint32_t key2_time = 0;
    uint32_t time_now = 0;

    time_now = SystickGetime();

    if (time_now - key1_time > 10)
    {
        key1_time = time_now;
        if (GPIO_ReadInputDataBit(BSP_KEY_GPIO_PORT, BSP_KEY1_GPIO_PIN))
        {
            if (key1_status == 0)
            {
                key1_status = 1;
                NS_LOG_INFO("key1 down\r\n");
            }
        }
        else
        {
            if (key1_status == 1)
            {
                key1_status = 0;
                NS_LOG_INFO("key1 up\r\n");
            }
        }
    }

    if (time_now - key2_time > 10)
    {
        key2_time = time_now;
        if (GPIO_ReadInputDataBit(BSP_KEY_GPIO_PORT, BSP_KEY2_GPIO_PIN))
        {
            if (key2_status == 0)
            {
                key2_status = 1;
                NS_LOG_INFO("key2 down\r\n");
            }
        }
        else
        {
            if (key2_status == 1)
            {
                key2_status = 0;
                NS_LOG_INFO("key2 up\r\n");
            }
        }
    }
}
