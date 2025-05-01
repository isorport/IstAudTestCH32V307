#ifndef _GPIO_CONF_H
#define _GPIO_CONF_H

#include "ch32v30x.h"

#define GPIO_TOGGLE_PIN(port, pin) (port->OUTDR ^= (pin))        // Переключить бит выхода


// === GPIO A
#define OUT_R_LED_PIN		GPIO_Pin_2
#define OUT_G_LED_PIN		GPIO_Pin_3
#define OUT_B_LED_PIN		GPIO_Pin_4
#define OUT_RGB_LED_PORT	GPIOA

#define IN_VAR_PIN			GPIO_Pin_5
#define IN_VAR_CHAN			ADC_Channel_5
#define IN_VAR_PORT			GPIOA

// === GPIO B
#define OUT_LED1_PIN		GPIO_Pin_0
#define OUT_LED2_PIN		GPIO_Pin_1
#define OUT_LED12_PORT		GPIOB

void GPIO_init();

#endif	//_GPIO_CONF_H
