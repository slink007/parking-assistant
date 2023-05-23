#ifndef SENSOR_H_
#define SENSOR_H_

#include "stm32f4xx.h"


void sensor_init(void);
int get_distance(void);
void send_trigger(void);

#endif /* SENSOR_H_ */
