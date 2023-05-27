/*
 * indicator.h
 *
 */

#ifndef INDICATOR_H_
#define INDICATOR_H_

#include "stm32f4xx.h"
#include "sensor.h"
#include <stdint.h>


void light_init(void);
void set_light(struct reading);
void light_off(void);

#endif /* INDICATOR_H_ */
