#ifndef __BSP_FRAME_H__
#define __BSP_FRAME_H__

#include "bsp.h"


#define frame_head_1 0x50
#define frame_head_2 0x4C

#define frame_end_1 0xA5
#define frame_end_2 0x5A


//数据帧头、功能码等为2字节，1代表高八位，2代表低八位
typedef enum  
{
    frame_state_head1,      //接收帧头1
    frame_state_head2,      //接收帧头2
    frame_state_fuc1,       //接收功能码1
    frame_state_fuc2,       //接收功能码2
    frame_state_len1,       //接收数据长度1
    frame_state_len2,       //接收数据长度2
    frame_state_data,       //接收数据内容
    frame_state_crc1,        //接收校验码
    frame_state_crc2,        //接收校验码
    frame_state_crc3,        //接收校验码
    frame_state_crc4,        //接收校验码
    frame_state_end1,       //接收帧尾1
    frame_state_end2,       //接收帧尾2
    frame_state_success,    //接收成功
}frame_state_t;

typedef enum{
    FUNC_CODE_TIMING        = 0x0001,  //定时开启  
    FUNC_CODE_TIMING_ACK    = 0x0002, //定时开启应答
    FUNC_CODE_STOP          = 0x0003, //定时关闭 
    FUNC_CODE_STOP_ACK      = 0x0004, //定时关闭应答
    FUNC_CODE_GET_TIME      = 0x0005, //设备端定时时间获取
    FUNC_CODE_GET_TIME_ACK  = 0x0006, // 获取时间应答 
    FUNC_CODE_MAX
}func_code_t;

typedef struct{
    uint32_t msg_crc;
    uint16_t fuc_code;
    uint16_t msg_len;
    uint16_t count;
    uint8_t *data;
}rx_msg_frame_t;

bool ble_rx_msg(void);

#endif


