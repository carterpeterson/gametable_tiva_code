#include "timer.h"

int get_timer_num(TIMER0_Type *timer)
{
	switch((uint32_t)timer) {
		case TIMER0_BASE: 
			return 0;
		case TIMER1_BASE:
			return 1;
		case TIMER2_BASE:
			return 2;
		case TIMER3_BASE:
			return 3;
		case TIMER4_BASE:
			return 4;
		case TIMER5_BASE:
			return 5;
		default:
			return -1;
	}
}

bool timer_init(TIMER0_Type *timer)
{
	int timer_num;
	uint8_t mask;
	timer_num	= get_timer_num(timer);
	mask = (0x01 << timer_num);
	
	if(timer_num < 0)
		return false;
	
	SYSCTL->RCGCTIMER |= mask;
	
	while((SYSCTL->PRTIMER & mask) != mask)
		;	// Wait
	
	return true;
}

bool timer_disable(TIMER0_Type *timer)
{
	if(get_timer_num(timer) < 0)
		return false;
	
	timer->CTL &= ~(TIMER_CTL_A_EN | TIMER_CTL_B_EN);
	return true;
}

bool timer_enable(TIMER0_Type *timer, uint8_t enable_mask)
{
	if(get_timer_num(timer) < 0)
		return false;
	
	timer->CTL |= enable_mask;	
	return true;
}

bool timer_mode(TIMER0_Type *timer, uint8_t mode)
{
	if(get_timer_num(timer) < 0)
		return false;
	
	timer->TAMR &= ~TIMER_MODE_CLEAR;
	timer->TAMR |= mode;
	
	timer->TBMR &= ~TIMER_MODE_CLEAR;
	timer->TBMR |= mode;
	
	return true;
}

