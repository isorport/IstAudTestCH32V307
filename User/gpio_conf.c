#include "gpio_conf.h"

void GPIO_init()
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	// === GPIO A
	GPIO_InitStructure.GPIO_Pin		= OUT_R_LED_PIN | OUT_G_LED_PIN | OUT_B_LED_PIN;	// RGB светодиод
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_OD;									// ОК output
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;
	GPIO_Init(OUT_RGB_LED_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= IN_VAR_PIN;			// VAR analog input
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AIN;
	GPIO_Init(IN_VAR_PORT, &GPIO_InitStructure);

	// === GPIO B
	GPIO_InitStructure.GPIO_Pin		= OUT_LED1_PIN | OUT_LED2_PIN;				// LED for tasks
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;
	GPIO_Init(OUT_LED12_PORT, &GPIO_InitStructure);
}
