#include "sensor.h"

void trigger_timer_init(void);


void sensor_init(void)
{
	/*  Set up pins for interfacing with HC-SR04 sensor */

	// Enable clock for PORTB
	RCC->AHB1ENR |= (1U << 1);

	// PORTB.10 is output for trigger to sesnsor
	GPIOB->MODER &= ~(1U << 21);
	GPIOB->MODER |= (1U << 20);
    
    // Trigger output is low
    GPIOB->ODR  &= ~TRIGGER_PIN;

	trigger_timer_init();
}


// Keep default 16 MHz speed
void trigger_timer_init(void)
{
    // Enable clock access to TIM2
	RCC->APB1ENR |= (1U << 0);
    
    // Need 16 counts for a 10 uS pulse
    TIM2->ARR = 16 - 1;
    
    // Clear current count to ensure we get a correct count
	TIM2->CNT = 0;
}


void send_trigger(void)
{
    // Enable TIM2
	TIM2->CR1 |= (1U << 0);
    
    // Drive trigger output high for 10 uS then bring it back low again.
    GPIOB->ODR |= TRIGGER_PIN;
    while (!(TIM2->SR & SR_UIF)) {}
    GPIOB->ODR  &= ~TRIGGER_PIN;
    
    // Disable TIM2
	TIM2->CR1 &= ~(1U << 0);
}
