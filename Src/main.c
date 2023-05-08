#include <stdint.h>
#include "stm32f4xx.h"
#include "indicator.h"
#include "switch.h"
#include "sensor.h"


int main(void)
{
	light_init();
	switch_init();
	//light_t light = OFF;
	sensor_init();

	while (1)
	{
		send_trigger();
		for(int i = 0; i < 5000; i++){}
	}

    

	/*for(;;)
	{
		if ( button_is_pressed() )
			light = RED;
		else
			light = GREEN;

		set_light(light);
	}*/
    
    
}
