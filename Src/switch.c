#include "switch.h"

volatile unsigned int closed = 0;

void switch_init(void)
{
    __disable_irq();                  // disable global interrupts

	RCC->AHB1ENR |= (1U << 1);        // enable clock for PORTB

	// PORTB.1 is input
	GPIOB->MODER &= ~(1U << 3);
	GPIOB->MODER &= ~(1U << 2);

	// Internal pull-up for PORTB.1
	GPIOB->PUPDR &= ~(1U << 3);
	GPIOB->PUPDR |=  (1U << 2);

	RCC->APB2ENR |= (1U << 14);       // enable clock for SYSCFG

	// Use EXTICR1 for pins 0 to 3.  PORTB interrupt pattern is 0001.  Pin 1 is bits 7:4.
	SYSCFG->EXTICR[0] &= ~(1U << 7);
	SYSCFG->EXTICR[0] &= ~(1U << 6);
	SYSCFG->EXTICR[0] &= ~(1U << 5);
	SYSCFG->EXTICR[0] |=  (1U << 4);

	EXTI->FTSR  |= (1U << 1);        // falling edge trigger for pin 1
	EXTI->RTSR  |= (1U << 1);        // also rising edge trigger for pin 1
	EXTI->IMR   |= (1U << 1);        // unmask EXTI1

	NVIC_EnableIRQ(EXTI1_IRQn);
	__enable_irq();                   // enable global interrupts
}


unsigned int switch_is_opened(void)
{
    return closed;
}


static void exti_callback(void)
{
    int closed_confidence = 0;
    int opened_confidence = 0;

    while(1)
    {
        // switch not closed
        if(GPIOB->IDR & (1U << 1))
        {
            opened_confidence++;
            closed_confidence = 0;
            if (opened_confidence > 500)
            {
                closed = 0;
                break;
            }
        }
        // switch closed
        else
        {
            closed_confidence++;
            opened_confidence = 0;
            if (closed_confidence > 500)
            {
                closed = 1;
                break;
            }
        }
    }
    __enable_irq();
}


void EXTI1_IRQHandler(void)
{
	if ( (EXTI->PR & (1U << 1)) != 0)
	{
		EXTI->PR |= (1U << 1);  // Clear PR flag - per the manual we write a '1' to clear it.
        __disable_irq();
		exti_callback();     // Do whatever it is we want to happen when interrupt occurs.
	}
}
