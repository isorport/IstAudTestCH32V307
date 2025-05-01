// ========== настройка портов ввода-выода
#ifndef _ADC_CONF_H
#define _ADC_CONF_H

#include "ch32v30x.h"

void ADC_init(void);
void Start_ADC_conv();
u16 Get_ADC_Val(u8);

#endif // _ADC_CONF_H

