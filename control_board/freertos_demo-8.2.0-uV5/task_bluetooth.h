#ifndef __TASK_BLUETOOTH_H__
#define __TASK_BLUETOOTH_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "TM4C123.h"

#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "button_input.h"

#include "../drivers/uart.h"
#include "../drivers/gpio.h"
#include "hypervisor.h"

uint32_t BluetoothTaskInit(void);


#endif