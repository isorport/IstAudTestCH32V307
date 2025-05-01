#ifndef _TIMERS_CONF_H
#define _TIMERS_CONF_H

#include "ch32v30x.h"

void TIM4_PWMOut_init ();
void TIM9_PWMOut_Init (u16 arr, u16 psc, u16 ccp);
#endif //_TIMERS_CONF_H
