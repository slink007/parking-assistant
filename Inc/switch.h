#ifndef SWITCH_H_
#define SWITCH_H_

#include "stm32f4xx.h"

void switch_init(void);
unsigned int button_is_pressed(void);

#endif /* SWITCH_H_ */
