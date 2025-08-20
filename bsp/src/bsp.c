#include "bsp.h"


#define DEMO_STRING  "\r\n device init : \r\n"

#define SYSTICK_1MS ((uint32_t)1000)
#define SYSTICK_100MS ((uint32_t)100000)
#define SYSTICK_1S  ((uint32_t)1000000)

#define BSP_SYSTICK_TICK       SYSTICK_100MS

timer_state_t timer_state = TIMER_STATE_STOP;
volatile uint32_t time_sec = 0;
volatile uint32_t Tick_num = 0;
void SysTick_Handler(void)
{
    Tick_num++;
}


int bsp_init(void)
{
    // 打印初始化信息
    NS_LOG_INFO(DEMO_STRING);

    drv_soft_iic_init();      // 初始化软件IIC
    bsp_DigitronPowerBrightnessSet(BSP_DIGITRON_POWER_ON,BSP_DIGITRON_BRIGHTNESS_7); //启动数码管、设置显示亮度为8级
	bsp_DigitronDisplayTime(0); 	//数码管默认显示00:00

    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / BSP_SYSTICK_TICK);  //默认64MHz

    bsp_beep_init();   //蜂鸣器初始化  
    bsp_relay_init();  // 继电器初始化  
    all_led_init();    // LED

    return true;
}

void bsp_run(void)
{
	static uint32_t old_time = 0;
    static uint32_t old_sec_time = 0;

   //数码管更新显示数据
   if((Tick_num - old_time) >= (BSP_SYSTICK_TICK / 2) )  //0.5S 执行一次
   {  
       LedBlink(LED3_PORT,LED3_PIN);
       bsp_DigitronDisplayTime(time_sec);
       old_time = Tick_num;
   }

    //计时开关
    if(timer_state == TIMER_STATE_RUN)
    {
        // 这里应该直接写 BSP_SYSTICK_TICK = 10 就可以  
        if( (time_sec > 0) && (Tick_num - old_sec_time >= BSP_SYSTICK_TICK) ) //1S 执行一次
        {
            time_sec--;
            old_sec_time = Tick_num;
            // NS_LOG_INFO("time_sec-- = %d !\r\n",time_sec);
        }
    }
    else if(timer_state == TIMER_STATE_STOP)
    {
        time_sec = 0;
    }

//    //到时间关闭开关
//    if(time_sec == 0)
//    {
//        bsp_relay_off();
//    }

//    if(Tick_num >= (1000 * BSP_SYSTICK_TICK))  //1000秒复位一次，防止Tick_num大小溢出，导致错误
//    {
//        Tick_num     = Tick_num % (1000 * BSP_SYSTICK_TICK);
//        old_time     = old_time % (10000 * BSP_SYSTICK_TICK);
//        old_sec_time = old_sec_time % (10000 * BSP_SYSTICK_TICK);
//    }

}




