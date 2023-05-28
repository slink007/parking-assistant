/*
 * indicator.c
 *
 */

#include "indicator.h"


#define GREEN_LED		(1U << 15)
#define YELLOW_LED		(1U << 14)
#define RED_LED			(1U << 13)
#define ERROR_LED       (GREEN_LED | YELLOW_LED | RED_LED)
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


void set_light(struct reading sensor)
{
	uint32_t temp = GPIOB->ODR;
	static int red_confidence = 0;
	static int yellow_confidence = 0;
	static int green_confidence = 0;

	if (sensor.result == BAD)
	{
		temp |= ERROR_LED;
	}
	else
	{
		/* The sensor tends to be noisy, especially when the target is moving. The noise is severe enough that
		 * you wind up briefly changing the indicator state.  To deal with this problem I am using a version
		 * of the switch debounce code from sensor.c.  Now we must get 4 counts for a given indicator state
		 * before any changes are made to the sensor.
		 */
		if (sensor.count < RED_COUNT)
		{
			red_confidence += 1;
		}
		else if (sensor.count < YELLOW_COUNT)
		{
			yellow_confidence += 1;
		}
		else
		{
			green_confidence += 1;
		}

		if ( (red_confidence > 3) || (yellow_confidence > 3) || (green_confidence > 3) )
		{
			if (red_confidence > 3)
			{
				temp |= RED_LED;
				temp &= ~YELLOW_LED;
				temp &= ~GREEN_LED;
			}
			else if (yellow_confidence > 3)
			{
				temp &= ~RED_LED;
				temp |= YELLOW_LED;
				temp &= ~GREEN_LED;
			}
			else
			{
				temp &= ~RED_LED;
				temp &= ~YELLOW_LED;
				temp |= GREEN_LED;
			}
			red_confidence = 0;
			yellow_confidence = 0;
			green_confidence = 0;
		}

	}
	GPIOB->ODR = temp;
}


void test_lights(void)
{
	struct reading test = {.count = 0, .result = BAD};
	set_light(test);
	systickDelayMs(1000);
	light_off();
}
