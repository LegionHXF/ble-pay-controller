/**
 * @file main.c
 * @brief 蓝牙扫码支付控制器
 * @author HXF
 * @version 1.0.0
 * @date 2024
 *
 *  
 */

/*============================ INCLUDES ======================================*/
#include "app.h"
#include "bsp.h"
/*============================ MACROS ========================================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ INTERNAL IMPLEMENTATION =======================*/
/**
 * @brief  user handle before enter sleep mode
 * @param  
 * @return 
 * @note   
 */
void app_sleep_prepare_proc(void)
{
}

/**
 * @brief  user handle after wake up from sleep mode
 * @param  
 * @return 
 * @note   
 */
void app_sleep_resume_proc(void)
{
}
/*============================ EXTERNAL IMPLEMENTATION =======================*/

int main(void)
{
    borad_init();  //  初始话蓝牙  模块  
    bsp_init();   //   初始化外设模块    数码管 ， 继电器 ， LED ， 蜂鸣器   borad support package  板级支持包

    while (1)
    {
        /*schedule all pending events  此函数在他的pack里面*/
        rwip_schedule();
        //ns_sleep();
        
        ble_rx_msg();  // 数据的接受与处理 
        bsp_run();
    }
}



