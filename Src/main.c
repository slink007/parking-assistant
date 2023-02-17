#include <stdint.h>
#include "stm32f4xx.h"
#include "indicator.h"
#include "switch.h"


int main(void)
{
	light_init();
	switch_init();
	light_t light = OFF;

	for(;;)
	{
		if ( button_is_pressed() )
			light = RED;
		else
			light = GREEN;

		set_light(light);
	}
}
