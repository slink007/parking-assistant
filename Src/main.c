#include <stdint.h>
#include "stm32f4xx.h"
#include "indicator.h"
#include "switch.h"


int main(void)
{
	light_init();
	switch_init();
	light_t light = OFF;

    /* Loop forever */
	for(;;)
	{
		set_light(light);
		for(int i=0;i<1000000;i++){}

		light = GREEN;
		set_light(light);
		for(int i=0;i<1000000;i++){}

		light = YELLOW;
		set_light(light);
		for(int i=0;i<1000000;i++){}

		light = RED;
		set_light(light);
		for(int i=0;i<1000000;i++){}

		light = OFF;
	}
}
