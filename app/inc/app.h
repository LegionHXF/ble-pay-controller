
#ifndef __APP_H__
#define __APP_H__

#include "n32wb03x.h"
#include "rwip.h"
#include "ns_ble.h"
#include "ns_sleep.h"
#include "ns_delay.h"
#include "ns_log.h"
#include "app_ble.h"
#include "app_rdtss.h"
#if  (CFG_APP_NS_IUS)
#include "ns_dfu_boot.h"
#endif




int borad_init(void);

#endif



