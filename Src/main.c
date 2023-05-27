#include <stdint.h>
#include "stm32f4xx.h"
#include "indicator.h"
#include "switch.h"
#include "sensor.h"

/* Added solely for testing over serial port */
#include <stdio.h>

#define GPIOAEN			(1U << 0)
#define UART2EN			(1U << 17)
#define CR1_TE			(1U << 3)
#define CR1_UE			(1U << 13)
#define CR1_M			(1U << 12)
#define CR1_PCE			(1U << 10)
#define CR2_STOP_1		(1U << 13)
#define CR2_STOP_0		(1U << 12)
#define SR_TXE			(1U << 7)
#define SYS_FREQ		16000000
#define APB1_CLK		SYS_FREQ
#define UART_BAUDRATE	19200

void uart2_tx_init(void);
void uart2_write(int ch);
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
int __io_putchar(int ch);
/* End of first section for serial port debug */



/* Added for 1 second delay */
// #include "systick.h"
#define SYSTICK_LOAD_VAL          16000
#define CTRL_ENABLE               (1U << 0)
#define CTRL_CLKSRC               (1U << 2)
#define CTRL_COUNTFLAG            (1U << 16)

void systickDelayMs(int delay);
/*****************************/



int main(void)
{
	light_init();
	switch_init();
	sensor_init();

	struct reading distance;

	/* Needed to send debug info out serial port */
	RCC->AHB1ENR |= (1U << 0);

	/*Set PA5 as output pin*/
	GPIOA->MODER |=(1U << 10);
	GPIOA->MODER &=~(1U << 11);

	uart2_tx_init();
	/* End serial port debug section */

	while (1)
	{
		//if ( switch_is_opened() )
		if ( 1 == 1 )
		{
			systickDelayMs(100);
			distance = get_distance();
			printf("A-to-D count is %d\r\n", distance.count);  // debug
			set_light(distance);
		}
		else
			light_off();
	}
}


/* Functions for writing data over serial port */

void uart2_tx_init(void)
{
	/****************Configure uart gpio pin***************/
	// Enable clock access to gpioa
	RCC->AHB1ENR |= GPIOAEN;

	// Set PA2 mode to alternate function mode
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	// Set PA2 alternate function type to UART_TX (AF07)
	GPIOA->AFR[0] |= (1U << 8);
	GPIOA->AFR[0] |= (1U << 9);   // Header doesn't provide AFRH
	GPIOA->AFR[0] |= (1U << 10);  // or AFRL.  It uses an array
	GPIOA->AFR[0] &= ~(1U << 11); // w/ 0 = AFRL and 1 = AFRH.

	/****************Configure uart module ***************/
	// Enable clock access to uart2
	RCC->APB1ENR |= UART2EN;

	// Set UART2 to 19200, N, 8, 1
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	USART2->CR1 &= ~(CR1_M);      // 8 bits
	USART2->CR1 &= ~(CR1_PCE);    // no parity
	USART2->CR2 &= ~(CR2_STOP_1); // 1 stop bit
	USART2->CR2 &= ~(CR2_STOP_0);

	// Configure the transfer direction
	 USART2->CR1 |= CR1_TE;

	// Enable uart module
	 USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch)
{
  // Make sure the transmit data register is empty
	while( !(USART2->SR & SR_TXE) ){}

  // Write to transmit data register
	USART2->DR = (ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate)
{
	USARTx->BRR =  compute_uart_bd(PeriphClk,BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

/* End of serial port functions */



/* for 1 second delay */
void systickDelayMs(int delay)
{
	SysTick->LOAD = SYSTICK_LOAD_VAL;  // reload with clocks per millisecond
	SysTick->VAL = 0;  // clear current count value
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;  // enable timer and use internal clock source

	for(int i = 0; i < delay; i++)
	{
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){}  // wait for COUNTFLAG to be set
	}
	SysTick->CTRL = 0;
	SysTick->CTRL &= ~(CTRL_ENABLE);
}
