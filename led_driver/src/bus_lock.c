#include "bus_lock.h"

#ifdef LED_BOARD
	bool bus_lock_top, bus_lock_bottom;
#else
	bool bus_lock;
#endif

void init_bus_lock_gpio(void)
{
	// Setup GPIO for UART6
	gpio_port_enable(PORT_D_CGC);
	gpio_digital_enable(PORT_D, (PIN_4 | PIN_5));
	gpio_pin_direction(PORT_D, DIRECTION_OUTPUT, PIN_5);
	gpio_pin_direction(PORT_D, DIRECTION_INPUT, PIN_4);
	gpio_alternate_function_enable(PORT_D, (PIN_4 | PIN_5));
	gpio_config_port_ctl(PORT_D, (PIN_4 | PIN_5), 1);	
	
	#ifdef LED_BOARD
		// Setup the GPIO for UART7
		gpio_port_enable(PORT_E_CGC);
		gpio_digital_enable(PORT_E, (PIN_0 | PIN_1));
		gpio_pin_direction(PORT_E, DIRECTION_OUTPUT, PIN_1);
		gpio_pin_direction(PORT_E, DIRECTION_INPUT, PIN_0);
		gpio_alternate_function_enable(PORT_E, (PIN_0 | PIN_1));
		gpio_config_port_ctl(PORT_E, (PIN_0 | PIN_1), 1);
	#endif
}

void init_bus_lock_uart(void)
{
	// Setup UART6 and make it do DMA requests
	uart_clock_enable(UART6_CGC);
	uart_channel_disable(UART6);
	uart_config_baud(UART6, UART_BAUDRATE);
	uart_config_line_control(UART6, (UART_CTL_WORD_LENGTH_8));
	uart_enable_interrupts(UART6, 5);
	uart_interrupt_mask_set(UART6, 0x10);
	uart_channel_enable(UART6, UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE);
	
	#ifdef LED_BOARD
		// Setup UART7 and make it do DMA requests
		uart_clock_enable(UART7_CGC);
		uart_channel_disable(UART7);
		uart_config_baud(UART7, UART_BAUDRATE);
		uart_config_line_control(UART7, (UART_CTL_WORD_LENGTH_8));
		uart_enable_interrupts(UART7, 5);
		uart_interrupt_mask_set(UART7, 0x10);
		uart_channel_enable(UART7, UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE);
	#endif
}

void give_lock(void)
{
	#ifdef LED_BOARD
		#ifdef ONE_I2C_BOARD
			if(bus_lock_top || bus_lock_bottom) {
				bus_lock_top = false;
				bus_lock_bottom = false;
				UART6->DR = 'l';
				UART7->DR = 'l';
			}
		#else	
			if(bus_lock_top && bus_lock_bottom) {
				bus_lock_top = false;
				bus_lock_bottom = false;
				UART2->DR = 'l';
				UART7->DR = 'l';
			}
		#endif
	#else
		if(bus_lock == true) {
			bus_lock = false;
			UART6->DR = 'l';
		}
	#endif
}

bool get_lock(uint8_t lock_code, bool top)
{
	#ifdef LED_BOARD
		if(lock_code == 'l') {
			if(top)
				bus_lock_top = true;
			else
				bus_lock_bottom = true;
			
			#ifdef ONE_I2C_BOARD
				return true;
			#endif
			
			if(bus_lock_top && bus_lock_bottom)
				return true;
			else
				return false;
		} else {
			return false;
		}
	#else
		if(lock_code == 'l') {
			bus_lock = true;
			return true;
		} else {
			return false;
		}
	#endif
}

void init_bus_lock(void)
{
	init_bus_lock_gpio();
	init_bus_lock_uart();
	
	#ifdef LED_BOARD
		bus_lock_top = false;
		bus_lock_bottom = false;
	#else
		bus_lock = true;
	#endif
}