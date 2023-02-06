/*
 * indicator.h
 *
 */

#ifndef INDICATOR_H_
#define INDICATOR_H_

#include "stm32f4xx.h"
#include <stdint.h>

#define GREEN_LED		(1U << 15)
#define YELLOW_LED		(1U << 14)
#define RED_LED			(1U << 13)

typedef enum {OFF = 0, RED, YELLOW, GREEN} light_t;

void light_init(void);
void set_light(light_t light);

#endif /* INDICATOR_H_ */
