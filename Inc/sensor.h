#ifndef SENSOR_H_
#define SENSOR_H_

#include "stm32f4xx.h"

#define TRIGGER_PIN		(1U << 10)

void sensor_init(void);
void send_trigger(void);


#endif /* SENSOR_H_ */
