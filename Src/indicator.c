/*
 * indicator.c
 *
 */

#include "indicator.h"


#define GREEN_LED		(1U << 15)
#define YELLOW_LED		(1U << 14)
#define RED_LED			(1U << 13)
#define ERROR_LED       (GREEN_LED | YELLOW_LED | RED_LED)
#define LIGHT_OFF		0x1FFF   // all bits for light are 0, so this is OFF condition
#define RED_COUNT		3552     // 12" at a echo clock of 2 MHz
#define YELLOW_COUNT	10656    // 36" at a echo clock of 2 MHz


void light_init(void)
{
	// Indicator will be connected to PORTB pins 15 (red), 14 (yellow), and 13 (green)

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


void light_off(void)
{
	GPIOB->ODR &= ~(GREEN_LED);
	GPIOB->ODR &= ~(YELLOW_LED);
	GPIOB->ODR &= ~(RED_LED);
}

//void set_light(light_t light)
void set_light(struct reading sensor)
{
	uint32_t temp = GPIOB->ODR & LIGHT_OFF;

	if (sensor.result == BAD)
	{
		temp |= ERROR_LED;
	}
	else
	{
		if (sensor.count < RED_COUNT)
		{
			temp |= RED_LED;
		}
		else if (sensor.count < YELLOW_COUNT)
		{
			temp |= YELLOW_LED;
		}
		else
		{
			temp |= GREEN_LED;
		}
	}

	GPIOB->ODR = temp;
}
