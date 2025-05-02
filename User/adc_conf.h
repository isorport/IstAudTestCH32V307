// ========== настройка портов ввода-выода
#ifndef _ADC_CONF_H
#define _ADC_CONF_H

#include "ch32v30x.h"

#define ADC_MAX 4095				// значений 12разр АЦП 0 .. 2^12-1

#define TRANS_RG	(1 * (ADC_MAX / 3))		// переход красный-зеленый
#define TRANS_GB	(2 * (ADC_MAX / 3))
#define TRANS_BR	(3 * (ADC_MAX / 3))


void ADC_init(void);
void Start_ADC_conv();
//u16 Get_ADC_Val(u8);

#endif // _ADC_CONF_H

