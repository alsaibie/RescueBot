/**
 * \file
 * \brief FreeRTOS for Teensy 3.0 and Due
 */
#ifndef FreeRTOS_ARM_h
#define FreeRTOS_ARM_h

#ifndef __arm__
#error ARM Due or Teensy 3.0 required
#else  // __arm__
//------------------------------------------------------------------------------
/** FreeRTOS_ARM version YYYYMMDD */
#define FREE_RTOS_ARM_VERSION 20130713
//------------------------------------------------------------------------------
#include "../FreeRTOS_ARM/utility/FreeRTOS.h"
#include "../FreeRTOS_ARM/utility/task.h"
#include "../FreeRTOS_ARM/utility/queue.h"
#include "../FreeRTOS_ARM/utility/semphr.h"
#include "../FreeRTOS_ARM/utility/portmacro.h"
//#include "../FreeRTOS_ARM/utility/cmsis_os.h"
#endif  // __arm__
#endif  // FreeRTOS_ARM_h