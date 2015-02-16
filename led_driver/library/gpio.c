#include "gpio.h"

bool gpio_port_enable(uint8_t ports)
{
	if((ports & INVALID_PORTS) != 0x00)
		return false;
	
	SYSCTL->RCGCGPIO |= ports;
	
	while((SYSCTL->PRGPIO & ports)!= ports)
		; // Wait
	
	if((ports & PORT_D_CGC) == PORT_D_CGC) {
		PORT_D->LOCK = UNLOCK_CODE;
		PORT_D->CR = COMMIT_ALL;
	}
	
	if((ports & PORT_F_CGC) == PORT_F_CGC) {
		PORT_F->LOCK = UNLOCK_CODE;
		PORT_F->CR = COMMIT_ALL;
	}
	
	return true;
}

bool gpio_digital_enable(GPIOA_Type *port, uint8_t pins)
{
	port->DEN |= pins;
	return true;
}

bool gpio_digital_disable(GPIOA_Type *port, uint8_t pins)
{
	port->DEN &= ~pins;
	return true;
}

bool gpio_analog_enable(GPIOA_Type *port, uint8_t pins)
{
	port->AMSEL |= pins;
	return true;
}

bool gpio_analog_disable(GPIOA_Type *port, uint8_t pins)
{
	port->AMSEL &= ~pins;
	return true;
}

bool gpio_pin_direction(GPIOA_Type *port, uint8_t direction, uint8_t pins)
{
	if(direction == DIRECTION_INPUT) {
		port->DIR &= ~pins;
		return true;
	} else if(direction == DIRECTION_OUTPUT) {
		port->DIR |= pins;
		return true;
	}
	
	return false;
}

bool gpio_alternate_function_enable(GPIOA_Type *port, uint8_t pins)
{
	port->AFSEL |= pins;
	
	return true;
}
