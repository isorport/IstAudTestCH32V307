#ifndef _TIMERS_CONF_H
#define _TIMERS_CONF_H

#include "ch32v30x.h"

#define STP_PWM		80	// шагов 

void TIM4_PWMOut_init ();
void TIM9_PWMOut_init (u16 arr, u16 psc, u16 ccp);
#endif //_TIMERS_CONF_H
