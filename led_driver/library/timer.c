// C

// MCU

// Project
#include "timer.h"

bool timer_enable(uint8_t timers)
{
	SYSCTL->RCGCTIMER |= timers;
	
	while((SYSCTL->PRTIMER & timers) != timers)
		;	// Wait
	
	return true;
}

bool timer_periodic_enable(TIMER0_Type *timer)
{
	timer->CTL &= ~(TIMER_A_ENABLE | TIMER_B_ENABLE);
	timer->CFG = TIMER_CFG_FULL;
	timer->TAMR |= TIMER_MODE_PERIODIC;
	timer->TAILR = 1000000;
	timer->IMR |= 0x1;
	timer->CTL |= TIMER_A_ENABLE;
	
	return true;
}
