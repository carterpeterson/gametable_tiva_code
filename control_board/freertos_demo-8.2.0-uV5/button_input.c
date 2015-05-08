#include "button_input.h"

bool left_pressed = false;
bool up_pressed = false;
bool down_pressed = false;
bool right_pressed = false;
bool center_pressed = false;

void init_button_input_gpio(void)
{
	// Enable left and center button
	gpio_port_enable(PORT_A_CGC);
	gpio_digital_enable(PORT_A, (BUTTON_LEFT | BUTTON_CENTER));
	gpio_pin_direction(PORT_A, DIRECTION_INPUT, (BUTTON_LEFT | BUTTON_CENTER));
	gpio_pull_up_enable(PORT_A, (BUTTON_LEFT | BUTTON_CENTER));
	// enable interrupts (lazy and dont want to make driver function)
	PORT_A->IM |= (BUTTON_LEFT | BUTTON_CENTER);
	NVIC_SetPriority(GPIOA_IRQn, 5);
	NVIC_EnableIRQ(GPIOA_IRQn);
	
	// enable up and down button
	gpio_port_enable(PORT_C_CGC);
	gpio_digital_enable(PORT_C, (BUTTON_UP | BUTTON_DOWN));
	gpio_pin_direction(PORT_C, DIRECTION_INPUT, (BUTTON_UP | BUTTON_DOWN));
	gpio_pull_up_enable(PORT_C, (BUTTON_UP | BUTTON_DOWN));
	// enable interrupts (lazy and dont want to make driver function)
	PORT_C->IM |= (BUTTON_UP | BUTTON_DOWN);
	NVIC_SetPriority(GPIOC_IRQn, 5);
	NVIC_EnableIRQ(GPIOC_IRQn);
	
	// enable right button
	gpio_port_enable(PORT_E_CGC);
	gpio_digital_enable(PORT_E, BUTTON_RIGHT);
	gpio_pin_direction(PORT_E, DIRECTION_INPUT, BUTTON_RIGHT);
	gpio_pull_up_enable(PORT_E, BUTTON_RIGHT);
	// enable interrupts (lazy and dont want to make driver function)
	PORT_E->IM |= BUTTON_RIGHT;
	NVIC_SetPriority(GPIOE_IRQn, 5);
	NVIC_EnableIRQ(GPIOE_IRQn);
}

void init_button_input(void)
{
	init_button_input_gpio();
}