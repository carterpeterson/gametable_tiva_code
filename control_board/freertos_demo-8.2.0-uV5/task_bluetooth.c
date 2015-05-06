#include "task_bluetooth.h"

//*****************************************************************************
//
// The stack size for the Consoletask.
//
//*****************************************************************************
#define BLUETOOTH_TASKSTACKSIZE        128         // Stack size in words

//*****************************************************************************
//
// The item size and queue size for the LED message queue.
//
//*****************************************************************************
#define BLUETOOTH_ITEM_SIZE           sizeof(uint8_t)
#define BLUETOOTH_QUEUE_SIZE          32

uint8_t readsome(void);
char uartRxPoll(uint32_t base, bool block);
void uartTxPoll(uint32_t base, char *data);

//*****************************************************************************
//
// The queue that holds messages sent to the BTUART task.
//
//*****************************************************************************
/*
xQueueHandle UartTxQueue;
xQueueHandle UartRxQueue;
*/
//*****************************************************************************
//
// Bluetooth commands/buttons
//
//*****************************************************************************


//*****************************************************************************
//This task contains the switch interpreting commands
//
//*****************************************************************************
static void
BluetoothTask(void *pvParameters)
{
	uint32_t count;
	char msg;
	
    gpio_port_enable(PORT_C_CGC);
	gpio_port_enable(PORT_F_CGC);
		
	//BT_UART GPIO Config (C), alternate function UART
	gpio_digital_enable(PORT_C, (PIN_4 | PIN_5));
	gpio_alternate_function_enable(PORT_C, (PIN_4 | PIN_5));
	gpio_config_port_ctl(PORT_C, (PIN_4 | PIN_5), 2);
	
	//BT_UART GPIO Config (F), alternate functions CTS and RTS
	gpio_digital_enable(PORT_F, (PIN_0 | PIN_1 | PIN_2 | PIN_3 | PIN_4));
	gpio_alternate_function_enable(PORT_F, (PIN_0 | PIN_1));
	gpio_config_port_ctl(PORT_F, (PIN_0 | PIN_1), 1);
	
	//BT_UART GPIO Config (F) normal Outputs
	gpio_pin_direction(PORT_F, DIRECTION_OUTPUT, (PIN_2 | PIN_3 | PIN_4));

	// Turn BT WAKE_SW Off
	GPIOF->DATA &= ~(1<<3);
	// Turn WAKE_SW ON
	GPIOF->DATA |= (1<<3);
	// HW Wake ON probably not necessary
	GPIOF->DATA |= (1<<4);
	// CMD Mode ON (PIN LOW)
	GPIOF->DATA &= ~(1<<2);

	
		//BT_UART_XX configuration
	uart_clock_enable(UART1_CGC);
	uart_channel_disable(UART1);
	uart_config_baud(UART1, 115200);
	uart_config_line_control(UART1, (UART_CTL_WORD_LENGTH_8 | UART_LCRH_FEN));

	UART1->CTL|=(3<<14); //rts/cts flow control

	uart_channel_enable(UART1, UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE);
	
	readsome();
	uartTxPoll(UART1_BASE,"SF,1\n");
	readsome();
	uartTxPoll(UART1_BASE,"SN,Tobias\n");
	readsome();
	uartTxPoll(UART1_BASE,"SS,F0000000\n");
	readsome();
	uartTxPoll(UART1_BASE,"SR,32000000\n");
	readsome();
	while(count<10)
	{
		readsome();
	  count++;
	}	
	uartTxPoll(UART1_BASE,"R,1\n");
	
	
	while(!readsome())
	{
		
	}
	//uartTxPoll(UART0_BASE,"Reset done\n\r");
	count = 0;
	while(count<10)
	{
		readsome();
		count++;
	}
	
	count = 0;
	while(count<100000)
	{
	count++;
	}
	// MLDP Mode ON (PIN HIGH)
	GPIOF->DATA |= (1<<2);
	count = 0;

    //portTickType ui32WakeTime;
    //uint32_t count = 0;

    while(1)   {
			msg = uartRxPoll(UART1_BASE,false);
			switch(msg)		{
				case 0:
					break;
				//Basic Buttons
				case 'M':
					center_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				case 'U':
					up_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				case 'R':
					right_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				case 'D':
					down_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				case 'L':
					left_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				case 'S':
					center_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				case 'N':
					// snake
					viewing_games = true;
					current_game_selected = 0;
					center_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				case 'P':
					// pong
					viewing_games = true;
					current_game_selected = 1;
					center_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				case 'B':
					// bloom
					viewing_games = false;
					current_animation_selected = 1;
					center_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				case 'O':
					// plot
					viewing_games = false;
					current_animation_selected = 2;
					center_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				case 'F':
					// scroll
					viewing_games = false;
					current_animation_selected = 3;
					center_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				case 'E':
					viewing_games = false;
					current_animation_selected = 0;
					center_pressed = true;
					xTaskResumeFromISR(hypervisor_task);
					break;
				//case '???' tochangeprogram=true; programid=(the ID for ???)

				///////////////////////////////////////////////////////////////////
				default:
					break;
			}
			//Also add an appropriate delay here////////////////////////
			
			vTaskDelay(50);
    }
    
}

//*****************************************************************************
//
// Initializes the LED task.
//
//*****************************************************************************
uint32_t
BluetoothTaskInit(void)
{
    BaseType_t return_val;
	
    // Create the Bluetooth task.
    //
    return_val = xTaskCreate(
                              BluetoothTask,                            // Function Pointer
                              (signed portCHAR *)"Bluetooth",           // Task Name
                              BLUETOOTH_TASKSTACKSIZE,                  // Stack Depth in Words
                              NULL,                                   // Parameters to Function
                              tskIDLE_PRIORITY + PRIORITY_LED_TASK,   // Task Priority
                              NULL);                                  // Task Handle

    if(return_val != pdTRUE)
    {
        return(1);
    }

    // Success.
    return(0);
}

/****************************************************************************
 * This routine transmits a single character out the UART / COM port.
 * Only the lower 8 bits of the 'data' variable are transmitted.
 ****************************************************************************/
void uartTxPollChar(uint32_t base, char data)
{
  UART0_Type *myUart =((UART0_Type *) base);

  if ( data != 0)
  {
    while( ((myUart->FR)&(UART_FR_TXFF)) != 0 );
    myUart->DR = data;
  }
  return;
}

/****************************************************************************
 * This routine transmits a character string out the UART / COM port.
 * Only the lower 8 bits of the 'data' variable are transmitted.
 ****************************************************************************/
void uartTxPoll(uint32_t base, char *data)
{
  UART0_Type *myUart =((UART0_Type *) base);


  if ( data != 0)
  {
    while(*data != '\0')
    {
      while( ((myUart->FR)&(UART_FR_TXFF)) != 0 );
      myUart->DR = *data;
      data++;
    }
  }
  return;
}

/****************************************************************************
 * This routine returns a character received from the UART/COM port.
 * If blocking is enabled, this routine should not return until data
 * is available. If blocking is disabled and no data is available,
 * this function should return 0.
 ****************************************************************************/
char uartRxPoll(uint32_t base, bool block)
{
  UART0_Type *myUart =((UART0_Type *) base);


  if( (block == false) && !(myUart->FR & UART_FR_RXFE))
  {
       return myUart->DR;
  }
  else if((block == false) && (myUart->FR & UART_FR_RXFE))
  {
    return 0;
  }

  while(myUart->FR & UART_FR_RXFE && block)
  {
    // Wait
  }

   return myUart->DR;
}


uint8_t readsome(void)
{
		char c;
		uint8_t toreturn=0;
	  uint32_t count=0;
		
		c= uartRxPoll(UART1_BASE,false);
//		c= bt_uart_receive();
		while(c!=0)
		{		
			if(c=='R')
				toreturn= 1;
//			uartTxPollChar(UART0_BASE, c); //This line is for debug
			c=uartRxPoll(UART1_BASE,false);

		}
		while(count<100000)
		{
			count++;
		}
		return toreturn;
}


