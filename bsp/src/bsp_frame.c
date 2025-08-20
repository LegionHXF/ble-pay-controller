#include "app.h"
#include "bsp.h"
/*
** 帧结构
** 帧头            2byte
** 功能码          2byte
** 数据长度        2byte
** 数据内容        0-512Byte四字节对齐
** CRC/BCC校验     4Byte
** 帧尾            2Byte             rx_msg->data = (uint8_t*)malloc(rx_msg->msg_len + head_size + len_size + fuc_code_size);
*/

#define head_size 2
#define fuc_code_size 2
#define crc_size 4
#define len_size 2
#define end_size 2

extern timer_state_t timer_state;
extern uint8_t  usart_rx_fifo_buf[USART_RX_FIFO_SIZE];
extern volatile uint16_t usart_rx_fifo_start;
extern volatile uint16_t usart_rx_fifo_end;
extern volatile uint32_t time_sec;
/*
* @brief: 添加帧数据的帧头和功能码  蓝牙发送给上位机时的处理  
*/
uint32_t bsp_create_frame(uint8_t *msg_buf, uint8_t *tx_buf, uint32_t tx_data_len, func_code_t fuc_code)
{   
// | 帧头(2) | 功能码(2) | 数据长度(2) | 数据(n) | CRC32校验(4) | 帧尾(2) |

    assert_param(msg_buf != NULL);
    uint32_t data_point = 0;

    msg_buf[data_point++] = frame_head_1;
    msg_buf[data_point++] = frame_head_2;

    msg_buf[data_point++] = (fuc_code >> 8) & 0xFF;
    msg_buf[data_point++] = fuc_code & 0xFF;

    //添加帧数据长度
    msg_buf[data_point++] = (tx_data_len >> 8) & 0xFF;
    msg_buf[data_point++] = tx_data_len & 0xFF;

    //添加帧数据
    for(uint32_t i = 0; i < tx_data_len; i++)
    {
        msg_buf[data_point++] = *tx_buf;
        NS_LOG_INFO("time set 0x%x\r\n",*tx_buf++);
    }

    /*crc校验值：4byte*/
	uint32_t crc32 = drv_crc32_calculate(msg_buf, data_point);
	msg_buf[data_point++] = (uint8_t)(crc32 >> 24) & 0xFF;
	msg_buf[data_point++] = (uint8_t)(crc32 >> 16) & 0xFF;
	msg_buf[data_point++] = (uint8_t)(crc32 >>  8) & 0xFF;
	msg_buf[data_point++] = (uint8_t)(crc32 >>  0) & 0xFF;

    // 帧尾
    msg_buf[data_point++] = frame_end_1;
    msg_buf[data_point++]   = frame_end_2;

    return data_point;
}


/**
 * @brief 状态机 蓝牙接收数据时的处理 
 * 
 * @param data 
 * @param rx_msg 
 * @return true 
 * @return false 
 */
bool resolve_frame(uint8_t data, rx_msg_frame_t *rx_msg)
{
    static frame_state_t frame_state = frame_state_head1;   //接收状态机
    static uint16_t rx_index = 0;

   // NS_LOG_INFO("recv state :%d data: 0x%x ...\r\n", frame_state, data);

    switch(frame_state)
    {
        case frame_state_head1:
            if(data == frame_head_1)
            {
                frame_state = frame_state_head2;
            }
        break;

        case frame_state_head2:
            if(data == frame_head_2)
            {
                frame_state = frame_state_fuc1;
            }
            else
            {
                frame_state = frame_state_head1;
            }
        break;

        case frame_state_fuc1:
            rx_msg->fuc_code |= (data << 8) & 0xff00;
            frame_state = frame_state_fuc2;
        break;

        case frame_state_fuc2:
            rx_msg->fuc_code |= data & 0xff;
            frame_state = frame_state_len1;
        break;
        
        case frame_state_len1:
            rx_msg->msg_len |= (data << 8) & 0xff00;
            frame_state = frame_state_len2;
        break;
        
        case frame_state_len2:
        {
            rx_msg->msg_len |= data & 0xff;
            rx_msg->count = rx_msg->msg_len;
            
            if( (rx_msg->msg_len <= 0) || (rx_msg->msg_len > 512) )
            {
                frame_state = frame_state_head1;
            }

            rx_msg->data = (uint8_t*)malloc(rx_msg->msg_len + head_size + len_size + fuc_code_size);
            if(rx_msg->data == NULL)  // 尝试为数据帧分配内存失败了。
            {
                frame_state = frame_state_head1;
                return false;
            }

            //添加帧头和功能码和长度值到内存里面，方便crc校验
            rx_index = 0;
            rx_msg->data[rx_index++] = frame_head_1;
            rx_msg->data[rx_index++] = frame_head_2;
            rx_msg->data[rx_index++] = (rx_msg->fuc_code >> 8) & 0xff;
            rx_msg->data[rx_index++] = rx_msg->fuc_code& 0xff;
            rx_msg->data[rx_index++] = (rx_msg->msg_len >> 8) & 0xff;
            rx_msg->data[rx_index++] = rx_msg->msg_len & 0xff;

            frame_state = frame_state_data;
        }
        break;

        case frame_state_data:
        {
            rx_msg->data[rx_index++] = data;

            rx_msg->count--;
            if(rx_msg->count == 0)
            {
                frame_state = frame_state_crc1;
                rx_index = 0;
            }
                
        }
        break;

        case frame_state_crc1:
        {
            rx_msg->msg_crc |= (data & 0xff) << 24;
            frame_state = frame_state_crc2;
        }
        break;

        case frame_state_crc2:
        {
            rx_msg->msg_crc |= (data & 0xff) << 16;
            frame_state = frame_state_crc3;
        }
        break;

        case frame_state_crc3:
        {
            rx_msg->msg_crc |= (data & 0xff) << 8;
            frame_state = frame_state_crc4;
        }
        break;

        case frame_state_crc4:
        {
            uint32_t l_crc32 = 0;
            rx_msg->msg_crc |= data & 0xff;
            frame_state = frame_state_end1;
            l_crc32 = drv_crc32_calculate(rx_msg->data, rx_msg->msg_len + head_size + fuc_code_size + len_size);

            if(l_crc32 != rx_msg->msg_crc)
            {
                NS_LOG_INFO("crc1: 0x%x crc2 :0x%x ...\r\n",l_crc32 ,rx_msg->msg_crc);
                free(rx_msg->data);
                frame_state = frame_state_head1;
            }
        }
        break;

        case frame_state_end1:
        {
            if(data != frame_end_1)
            {
                free(rx_msg->data);
                frame_state = frame_state_head1;
                break;
            }

            frame_state = frame_state_end2;
            break;
        }

        case frame_state_end2:
        {
            if(data != frame_end_2)
            {
                free(rx_msg->data);
                frame_state = frame_state_head1;
                break;
            }

            frame_state = frame_state_success;
            break;
        }

        default:
            frame_state = frame_state_head1;    
            break;
    }

    if(frame_state != frame_state_success)
    {
        return false;
    }

    frame_state = frame_state_head1;
    return true;
}

bool ble_rx_msg(void)
{
    static rx_msg_frame_t rx_msg = {0};

    //相等代表未接收到数据   不相等代表有数据待处理
    if(usart_rx_fifo_end == usart_rx_fifo_start) 
    {
        return false;
    }

    if(usart_rx_fifo_start == USART_RX_FIFO_SIZE)
    {
        usart_rx_fifo_start = 0;  // 环形缓冲区回绕操作。  缓冲区大小是 USART_RX_FIFO_SIZE，当 start 指针增加到末尾（等于最大值）时，就重置为 0，从头开始。 
    } 
    
    bsp_beep_on();
    //处理接收到的数据
    if(resolve_frame(usart_rx_fifo_buf[usart_rx_fifo_start++], &rx_msg) == true)
    {
        NS_LOG_INFO("recv frame data ...\r\n");
        uint8_t msg_buf[16] = {0};
        uint16_t send_size = 0;

        switch(rx_msg.fuc_code)
        {      
            // case FUNC_CODE_START: 接受数据内容（定时时间），开启定时器 ，完成继电器的控制 并返回应答包
            case FUNC_CODE_TIMING:
            {
                uint16_t data_index = head_size + len_size + fuc_code_size;
                uint8_t tx_buf[2]   = {0xff,0xff};   //FUNC_CODE_TIMING_ACK：固定返回数据

                time_sec = (rx_msg.data[data_index] << 8) | rx_msg.data[data_index + 1]; //接收数据为分钟
                time_sec *= 60; //转化为秒处理
                // 控制继电器 
                bsp_DigitronDisplayTime(time_sec);
                bsp_relay_on();
                timer_state = TIMER_STATE_RUN;
                
                send_size = bsp_create_frame(msg_buf, tx_buf, sizeof(tx_buf), FUNC_CODE_TIMING_ACK);
                for(int i = 0; i < send_size; i++)
                {
                    NS_LOG_INFO("send : 0x%x ",msg_buf[i]);
                }
                rdtss_send_notify(msg_buf, send_size);
            }
            break;
            
            //  关闭定时器 ，并返回应答包 
            case FUNC_CODE_STOP:
			{
                uint8_t tx_buf[2]   = {0xff,0xff};   //FUNC_CODE_STOP_ACK：固定返回数据
                timer_state = TIMER_STATE_STOP;

                bsp_relay_off();
                send_size = bsp_create_frame(msg_buf, tx_buf, sizeof(tx_buf), FUNC_CODE_STOP_ACK);
                rdtss_send_notify(msg_buf, send_size);
			}
            break;
            //  获取当前定时器剩余时间，并返回应答包  
            case FUNC_CODE_GET_TIME:
			{
                uint8_t tx_buf[2] = {0}; // 当前定时器剩余时间是 125 秒

                //转换为分钟并向上取整 
                uint16_t time_s = time_sec/60; // 125 / 60 = 2
                if((float)time_sec/60 - time_s) // 2.08 - 2 = 0.08 ≠ 0
                {
                    time_s++;  // 因为还有小数，所以向上取整 → time_s = 3 
                }


                // 打包为 2 个字节（大端格式）
                tx_buf[0] = (time_s >> 8) & 0xff;
                tx_buf[1] = time_s & 0xFF;

                send_size = bsp_create_frame(msg_buf, &tx_buf[0], sizeof(tx_buf), FUNC_CODE_GET_TIME_ACK);
               
                rdtss_send_notify(msg_buf, send_size);
                timer_state = TIMER_STATE_RUN;
                
				NS_LOG_INFO("send :  ");
                for(int i = 0; i < send_size; i++)
                {
                    NS_LOG_INFO("  0x%x ",msg_buf[i]);
                }
				NS_LOG_INFO(" size: 0x%x ",send_size);
   
			}
            break;

            default:
                NS_LOG_INFO("not expected function code\r\n");
                return false;
        }
        
        free(rx_msg.data);
        memset(&rx_msg, 0, sizeof(rx_msg_frame_t));
        bsp_beep_off();
        return true;
    }
    return false;
}










