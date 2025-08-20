#include "app.h"
#include "bsp.h"
#include "bsp_usart.h"
int borad_init(void)
{
    //for hold the SWD before sleep
    //delay_n_10us(300*1000);
    NS_LOG_INIT(); //打印初始化  

    #if  (CFG_APP_NS_IUS)
    //判断当前应用程序的起始地址，输出相应的信息
    if(CURRENT_APP_START_ADDRESS == NS_APP1_START_ADDRESS){
        NS_LOG_INFO("application 1 start new ...\r\n");
    }else if(CURRENT_APP_START_ADDRESS == NS_APP2_START_ADDRESS){
        NS_LOG_INFO("application 2 start new ...\r\n");
    }
    #endif
     
    //初始化应用程序
    app_ble_init();   // 蓝牙协议   GATT协议  的初始化  
    NS_LOG_INFO("board init ...\r\n");

	return true;
}






