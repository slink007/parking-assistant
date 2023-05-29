#include <stdint.h>
#include "stm32f4xx.h"
#include "indicator.h"
#include "switch.h"
#include "sensor.h"
#include "delay.h"


int main(void)
{
	light_init();
	switch_init();
	sensor_init();

	test_lights();

	struct reading distance;

	while (1)
	{
		if ( switch_is_opened() )
		{
			systickDelayMs(100);
			distance = get_distance();
			set_light(distance);
		}
		else
			light_off();
	}
}

