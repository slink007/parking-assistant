#include "switch.h"

void switch_init(void)
{
	// Switch is attached to PB1

	// Enable clock for PORTB
	RCC->AHB1ENR |= (1U << 1);

	// PORTB.1 is input
    GPIOB->MODER &= ~(1U << 0);
    GPIOB->MODER &= ~(1U << 1);

    // Internal pull-up for PORTB.1
    GPIOB->PUPDR |= (1U << 1);
    GPIOB->PUPDR &= ~(1U << 0);

}
