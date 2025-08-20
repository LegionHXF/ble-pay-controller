#include "bsp_smg.h"

static uint8_t g_digitron_num[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
static uint8_t g_digitron_address[4] = {0x68, 0x6a, 0x6c, 0x6e};
/**
 * @brief 初始化IIC   引脚
 * 
 * @return true 
 * @return false 
 */
bool drv_soft_iic_init(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(SOFT_IIC_SDA_PORT));
    assert_param(IS_GPIO_ALL_PERIPH(SOFT_IIC_SCL_PORT));
    assert_param(IS_GPIO_PIN(SOFT_IIC_SDA_PIN));
    assert_param(IS_GPIO_PIN(SOFT_IIC_SCL_PIN));

    // 设置SCL和SDA引脚为高电平
    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);  
    GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);  


    // 使能SCL和SDA引脚的时钟
    RCC_EnableAPB2PeriphClk(SOFT_IIC_SDA_GPIO_CLK, ENABLE);
    RCC_EnableAPB2PeriphClk(SOFT_IIC_SCL_GPIO_CLK, ENABLE);

    // 初始化GPIO_InitStructure结构体
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = SOFT_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = SOFT_IIC_PIN_MODE;
    // 初始化SDA引脚
    GPIO_InitPeripheral(SOFT_IIC_SDA_PORT, &GPIO_InitStructure);    

    // 初始化GPIO_InitStructure结构体
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = SOFT_IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = SOFT_IIC_PIN_MODE;
    // 初始化SCL引脚
    GPIO_InitPeripheral(SOFT_IIC_SCL_PORT, &GPIO_InitStructure);  


    return fuc_back_ok;
}


/**
 * @brief IIC 的開始信号
 * 
 */
void drv_soft_iic_start(void)
{
    //发送start信号
    GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN); //设置SDA为高电平
    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN); //设置SCL为高电平
    delay_n_10us(1); //延时1个10微秒
    GPIO_ResetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN); //设置SDA为低电平
    delay_n_10us(1); //延时1个10微秒
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN); //设置SCL为低电平
    delay_n_10us(1); //延时1个10微秒
}


/**
 * @brief IIC 的關閉信号
 * 
 */
void drv_soft_iic_stop(void)
{
    //发送stop信号
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    GPIO_ResetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    delay_n_10us(1);
    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    delay_n_10us(1);
    GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    delay_n_10us(1);
}

/**
 * @brief 等待ACK应答，超时时间250个周期
 * 
 * @return bool 
 */
bool drv_soft_iic_wait_ack(void)
{
    uint8_t error_time = 0;
    GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    delay_n_10us(1);
    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    delay_n_10us(1);

    while (GPIO_ReadInputDataBit(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN))
    {
        error_time++;
        if (error_time > 250)
        {
            return false;
        }
    }
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);

    return true;
}

void drv_soft_iic_ack(void)
{
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    GPIO_ResetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    delay_n_10us(1);
    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    delay_n_10us(1);
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
}

void drv_soft_iic_nack(void)
{
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    delay_n_10us(1);
    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    delay_n_10us(1);
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
}
bool drv_soft_iic_send_byte(uint8_t byt)
{
    uint8_t i = 0;
    //拉低IIC时钟总线开始传输数据
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);

    for (i=0; i<8; i++)
    {
        if (byt & 0x80)
        {
            GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
        }
        else
        {
            GPIO_ResetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
        }
        byt <<= 1;
        delay_n_10us(1);
        GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
        delay_n_10us(1);
        GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
        delay_n_10us(1);
    }
    return true;
}

bool drv_soft_iic_read_byte(uint8_t ack, uint8_t *byt)
{
    uint8_t i = 0;
    
    for (i=0; i<8; i++)
    {
        GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
        delay_n_10us(1);
        GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
        *byt <<= 1;
        if (GPIO_ReadInputDataBit(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN))
        {
            *byt++;
        }
        delay_n_10us(1);
    }
    if (ack)
    {
        drv_soft_iic_ack();
    }
    else
    {
        drv_soft_iic_nack();
    }
    return true;
}

/**
 * @brief 设置电源和亮度
 * 
 * @param power 电源
 * @param brightness 亮度 
 */
void bsp_DigitronPowerBrightnessSet(bsp_DigitronPower_t power,bsp_DigitronBrightness_t brightness)
{
    uint8_t data = 0x00;

    data |= (brightness << 4);
    data |= (power << 0);
    
    drv_soft_iic_start();
    drv_soft_iic_send_byte(0x48);   //发送控制命令
    drv_soft_iic_wait_ack();
    drv_soft_iic_send_byte(data);   //设置电源和亮度
    drv_soft_iic_wait_ack();
    drv_soft_iic_stop();
}


/**
 * @brief 显示数字和点
 * 
 * @param first 
 * @param second 
 * @param third 
 * @param fourth 上述取值范围 0~15
 * @param point  0：不显示时钟 1：显示时钟点
 */
void bsp_DigitronDisplay(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth, uint8_t point)
{
    uint8_t i = 0;
    uint8_t data_array[4] = {0};
    
    data_array[0] = first  ;
    data_array[1] = second ;
    data_array[2] = third  ;
    data_array[3] = fourth ;

    for (i=0; i<4; i++)
    {
        drv_soft_iic_start();
        drv_soft_iic_send_byte(g_digitron_address[i]);   //发送显存地址
        drv_soft_iic_wait_ack();
        if (point == 0)
        {
            drv_soft_iic_send_byte(g_digitron_num[data_array[i]]);   //设置数据
        }
        else
        {
            drv_soft_iic_send_byte(g_digitron_num[data_array[i]] | 0x80);   //设置数据
        }
        
        drv_soft_iic_wait_ack();
        drv_soft_iic_stop();
    } 
}
/**
 * @brief 将秒数转换为小时、分钟、秒
 * 
 * @param second 
 * @param hour 
 * @param miniute 
 * @param second1 
 */
void  bsp_timer_second2hms(int32_t second, uint8_t *hour, uint8_t *miniute, uint8_t *second1)
{
    *hour = second / 3600;
    *miniute = (second % 3600) / 60;
    *second1 = (second % 3600) % 60;
}

// 函数：bsp_DigitronDisplayTime
// 功能：将时间显示在数码管上
// 参数：time：时间，单位为秒
void bsp_DigitronDisplayTime(uint32_t time)
{
    // 定义小时、分钟、秒
    uint8_t hour, miniute, second;  
    // 将时间转换为小时、分钟、秒
    bsp_timer_second2hms(time, &hour, &miniute, &second);
    // 如果时间小于3600秒，即小于1小时，则显示分钟和秒
    if (time < 3600)
    {
        bsp_DigitronDisplay(miniute/10, miniute%10, second/10, second%10, 1);  
    }
    // 否则，显示小时和分钟
    else
    {
        bsp_DigitronDisplay(hour/10, hour%10, miniute/10, miniute%10, 1);          
    }
}



