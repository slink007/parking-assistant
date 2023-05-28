#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx.h"

#define SYSTICK_LOAD_VAL          16000
#define CTRL_ENABLE               (1U << 0)
#define CTRL_CLKSRC               (1U << 2)
#define CTRL_COUNTFLAG            (1U << 16)

void systickDelayMs(int delay);


#endif /* DELAY_H_ */
