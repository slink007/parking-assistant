#ifndef SENSOR_H_
#define SENSOR_H_

#include "stm32f4xx.h"

typedef enum {GOOD, BAD} result_t;

struct reading
{
	int count;        // A-to-D count of the echo pulse width
	result_t result;  // Indicator of whether or not the sensor responded
};

void sensor_init(void);
struct reading get_distance(void);
//int get_distance(void);

#endif /* SENSOR_H_ */
