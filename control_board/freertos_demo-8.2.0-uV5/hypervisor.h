#ifndef __HYPERVISOR_H__
#define __HYPERVISOR_H__

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "tasks.h"
#include "bloom.h"
//#include "pong.h"
#include "snake.h"

#define PRIORITY_HYPERVISOR 5

bool init_hypervisor_task(void);

#endif