/*
 * indicator.c
 *
 */

#include "indicator.h"


void light_init(void)
{
	// Indicator will be connected to PORTB pins 15, 14, and 13

	// Enable clock for PORTB
	RCC->AHB1ENR |= (1U << 1);

	// PORTB.15 is output
	GPIOB->MODER &= ~(1U << 31);
	GPIOB->MODER |= (1U << 30);

	// PORTB.14 is output
	GPIOB->MODER &= ~(1U << 29);
	GPIOB->MODER |= (1U << 28);

	// PORTB.13 is output
	GPIOB->MODER &= ~(1U << 27);
	GPIOB->MODER |= (1U << 26);

}


void set_light(light_t light)
{
	uint32_t temp = GPIOB->ODR & 0x1FFF;  // all bits for light are 0, so this is OFF condition
	if (light == GREEN)
	{
		temp |= GREEN_LED;
	}
	else if (light == YELLOW)
	{
		temp |= YELLOW_LED;
	}
	else if (light == RED)
	{
		temp |= RED_LED;
	}
	GPIOB->ODR = temp;
}
