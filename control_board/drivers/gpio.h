#ifndef __GPIO_H__
#define __GPIO_H__

// C
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// MCU
#include "TM4C123.h"

// Project
#include "./config.h"

// GPIO Related Defines
#define PIN_0 0x01
#define PIN_1 0x02
#define PIN_2 0x04
#define PIN_3 0x08
#define PIN_4 0x10
#define PIN_5 0x20
#define PIN_6 0x40
#define PIN_7 0x80

// Unlocking the CR register
#define UNLOCK_CODE 	0x4C4F434B
#define COMMIT_ALL 		0xFF

// Clock gating pin assignments
#define PORT_A_CGC		0x01
#define PORT_B_CGC		0x02
#define PORT_C_CGC		0x04
#define PORT_D_CGC		0x08
#define PORT_E_CGC		0x10
#define PORT_F_CGC		0x20
#define INVALID_PORTS 	0xC0

// Setup pointers to allow for one reference in code
#ifndef _FORCE_GPIO_APB
	#define PORT_A_BASE GPIOA_AHB_BASE
	#define PORT_B_BASE GPIOB_AHB_BASE
	#define PORT_C_BASE GPIOC_AHB_BASE
	#define PORT_D_BASE GPIOD_AHB_BASE
	#define PORT_E_BASE GPIOE_AHB_BASE
	#define PORT_F_BASE GPIOF_AHB_BASE
	
	#define PORT_A 		GPIOA_AHB
	#define PORT_B 		GPIOB_AHB
	#define PORT_C 		GPIOC_AHB
	#define PORT_D 		GPIOD_AHB
	#define PORT_E 		GPIOE_AHB
	#define PORT_F 		GPIOF_AHB
#else
	#define PORT_A_BASE GPIOA_BASE
	#define PORT_B_BASE GPIOB_BASE
	#define PORT_C_BASE GPIOC_BASE
	#define PORT_D_BASE GPIOD_BASE
	#define PORT_E_BASE GPIOE_BASE
	#define PORT_F_BASE GPIOF_BASE
	
	#define PORT_A 		GPIOA
	#define PORT_B 		GPIOB
	#define PORT_C 		GPIOC
	#define PORT_D 		GPIOD
	#define PORT_E 		GPIOE
	#define PORT_F 		GPIOF
#endif

// Port direction
#define DIRECTION_INPUT 	0
#define DIRECTION_OUTPUT 	1

/*
* 	Enable the specified GPIO ports. Preferably use the AHB bus so it can
* 	be faster.
*		
* 	Parameters:
*		base_addr - Base address of the GPIO port
*			
* 	Returns:
*		True if port was enabled
*/
bool gpio_port_enable(uint8_t ports);

/*
* 	Configure the specified pins to be digital pins.
*		
* 	Parameters:
*		base_addr - Base address of the GPIO port
*		pins - Pins to be enabled as digital pins
*			
* 	Returns:
*		True if port was digital enabled
*/
bool gpio_digital_enable(GPIOA_Type *port, uint8_t pins);
bool gpio_digital_disable(GPIOA_Type *port, uint8_t pins);

/*
* 	Configure the specified pins to be analog pins.
*		
*	Parameters:
*		base_addr - Base address of the GPIO port
*		pins - Pins to be enabled as analog pins
*
*	Returns:
*		True if port was analog enabled
*/
bool gpio_analog_enable(GPIOA_Type *port, uint8_t pins);
bool gpio_analog_disable(GPIOA_Type *port, uint8_t pins);

/*
*	Configure the direction (input/output) of the specified pins
*	
*	Parameters:
*		base_addr - Base address of the GPIO port
*		pins - Pins to be have direction changed
*			
*	Returns:
*		True if port direction was changed
*/
bool gpio_pin_direction(GPIOA_Type *port, uint8_t direction, uint8_t pins);

/*
*	Enable the alternate function of the specified pins
*	
*	Parameters:
*		base_addr - Base address of the GPIO port
*		pins - Pins to be have direction changed
*			
*	Returns:
*		True if port direction was changed
*/
bool gpio_alternate_function_enable(GPIOA_Type *port, uint8_t pins);

/*
*	Enable / Disable Open Drain for the specified pins
*/
bool gpio_open_drain_enable(GPIOA_Type *port, uint8_t pins);
bool gpio_open_drain_disable(GPIOA_Type *port, uint8_t pins);


/*
*		Set the port control on the specified pins of the port
*/
void gpio_config_port_ctl(GPIOA_Type *port, uint8_t pins, uint8_t pctl);

#endif
