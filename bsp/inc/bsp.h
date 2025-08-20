#ifndef __DEV_H
#define __DEV_H

#include "n32wb03x.h"

#include "bsp_usart.h"
#include "bsp_gpio.h"
#include "ns_log.h"
#include "bsp_smg.h"
#include "ns_delay.h"
#include "bsp_frame.h"
#include "bsp_crc.h"
#include "bsp_relay.h"
#include "bsp_beep.h"

typedef enum{
    fuc_back_ok,
    fuc_back_err
}fuc_back_t;

typedef enum{
    TIMER_STATE_RUN,
    TIMER_STATE_STOP,
}timer_state_t;



int bsp_init(void);
void bsp_run(void);
#endif

