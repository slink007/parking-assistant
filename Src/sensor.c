#include "sensor.h"

#define CR1_CEN		    (1U << 0)
#define TRIGGER_PIN		(1U << 10)
#define ECHO_PIN        (1U << 6)
#define SR_UIF			(1U << 0)
#define GPIOAEN		    (1U << 0)
#define TIM3EN		    (1U << 1)


void trigger_timer_init(void);
void echo_timer_init(void);


/*  Set up pins for interfacing with HC-SR04 sensor */
void sensor_init(void)
{
	trigger_timer_init();
	echo_timer_init();
}


// PORTB.10 is output to trigger
// Keep default 16 MHz speed for trigger timer
void trigger_timer_init(void)
{
	// Enable clock for PORTB
	RCC->AHB1ENR |= (1U << 1);

	// PORTB.10 into output mode
	// I did not use alternate mode because the timer is not tied directly to the pin.
	// It is only a control used to direct the normal GPIO functionality.
	GPIOB->MODER &= ~(1U << 21);
	GPIOB->MODER |= (1U << 20);

    // Trigger output is low
    GPIOB->ODR  &= ~TRIGGER_PIN;

    // Enable clock access to TIM2
	RCC->APB1ENR |= (1U << 0);

    // 10 uS / (1 / 16 MHz) = count of 160
	// Experimentation shows that 162 needed to ensure a 10 uS pulse
	TIM2->ARR = 162;
    
    // Clear current count to ensure we get a correct count
	TIM2->CNT = 0;

	// Enable TIM2
	TIM2->CR1 |= (1U << 0);
}


// PORTA.6 is input from sensor echo pin
void echo_timer_init(void)
{
	RCC->AHB1ENR |= GPIOAEN;      // Enable clock access to GPIOA

	GPIOA->MODER &= ~(1U << 12);  // Set PA6 to alternate function
	GPIOA->MODER |= (1U << 13);
	
	GPIOA->AFR[0] &= ~(1U << 24);  // Set PA6 alternate type to TIM3_CH1 (AF02)
	GPIOA->AFR[0] |= (1U << 25);
	GPIOA->AFR[0] &= ~(1U << 26);
	GPIOA->AFR[0] &= ~(1U << 27);


	RCC->APB1ENR |= TIM3EN;        // Enable clock access to TIM3
	TIM3->PSC = 8 - 1;             // 16 MHz / 8 = 2 MHz, so 0.5 uS per tick

	// Timer counts UP
	TIM3->CR1 |= (1U << 4);

	TIM3->CCMR1 &= ~(1U << 1);     // TIM3 set for input capture
	TIM3->CCMR1 |= (1U << 0);

	TIM3->CCER |= (1U << 1);      // Input capture enabled and triggered
	TIM3->CCER |= (1U << 0);      // on rising and falling edges.
	TIM3->CCER |= (1U << 3);
	
	TIM3->CR1 |= CR1_CEN;          // TIM3 enabled
}


void send_trigger(void)
{
    // Reset the timer
	TIM2->CNT = 0;
	TIM2->SR &= ~(1U << 0);
    
    // Drive trigger output high for 10 uS then bring it back low again.
    GPIOB->ODR |= TRIGGER_PIN;
    while (!(TIM2->SR & SR_UIF)) {}
    GPIOB->ODR  &= ~TRIGGER_PIN;
}


// int get_distance(void)
struct reading get_distance(void)
{
	int rising_edge = 0;
	int falling_edge = 0;
	struct reading echo = {.count = 0, .result = GOOD};
	
	send_trigger();
	TIM3->CNT = 0;
	
	// wait for, and capture, rising edge of echo
	while(!(TIM3->SR & (1U<<1))){}
	rising_edge = TIM3->CCR1;
	
	// wait for, and capture, falling edge of echo
	while(!(TIM3->SR & (1U<<1))){}
	falling_edge = TIM3->CCR1;
	
	echo.count = (falling_edge - rising_edge);

	/*
	 * For reasons unknown, the STM32F does not apply the command to make TIM3 an up counter
	 * when it is powered on.  This results in negative count values.  If a manual reset is
	 * performed then the counter will operate as expected until the device experiences another
	 * power cycle.  The following deals with those negative counts until a reason can be found
	 * for why it behaves one way on power up and another way following a reset.
	 *
	 * I tried working around this by just letting the timer be a down counter and reversing the
	 * order of the counts when calculating the difference.  This did give me a positive count
	 * on power up but then all counts go negative on a reset.  It seems there is no escaping
	 * this hack.
	 */
	if (echo.count < 0)
	{
		echo.count *= -1;
	}

	return echo;
}
