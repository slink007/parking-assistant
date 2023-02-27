#include "sensor.h"

void timer_init(void);

void sensor_init(void)
{
	/*  Set up pins for interfacing with HC-SR04 sensor */

	// Enable clock for PORTB
	RCC->AHB1ENR |= (1U << 1);

	// PORTB.10 is output for trigger to sesnsor
	GPIOB->MODER &= ~(1U << 21);
	GPIOB->MODER |= (1U << 20);

	timer_init();
}


void timer_init(void)
{

}

