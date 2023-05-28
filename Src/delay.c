#include "delay.h"

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
