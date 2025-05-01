//#include "stdio.h"
#include "timers_conf.h"


volatile uint16_t lptim_interrupt_count = 0; // Счетчик прерываний LPTIM

void TIM4_PWMOut_init()
{
	// pwm на выход PB10 TIM4_CH1
	//GPIO_InitTypeDef			GPIO_InitStructure			= {0};
	TIM_OCInitTypeDef			TIM_OCInitStructure			= {0};
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure	= {0};

	//RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOA | RCC_PB2Periph_TIM1, ENABLE );

	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);	// переводим tim4 на альтернативу

	/*
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	*/

	TIM_TimeBaseInitStructure.TIM_Period			= 10;	// ATRLR длина 
	TIM_TimeBaseInitStructure.TIM_Prescaler			= 0;	// PSC предделитель
	TIM_TimeBaseInitStructure.TIM_ClockDivision		= TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OutputState		= TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse			= 2;
	TIM_OCInitStructure.TIM_OCPolarity		= TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCMode			= TIM_OCMode_PWM1;			//TIM_OCMode_PWM2
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);

	TIM_CtrlPWMOutputs(TIM4, ENABLE);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}


/* PWM Output Mode Definition */
#define PWM_MODE1   0
#define PWM_MODE2   1

/* PWM Output Mode Selection */
//#define PWM_MODE PWM_MODE1
#define PWM_MODE PWM_MODE2

/*********************************************************************
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 */
void TIM9_PWMOut_init( u16 arr, u16 psc, u16 ccp )
{
	TIM_OCInitTypeDef TIM_OCInitStructure={0};
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

	RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM9, ENABLE);

	TIM_TimeBaseInitStructure.TIM_Period			= arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler			= psc;
	TIM_TimeBaseInitStructure.TIM_ClockDivision		= TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM_TimeBaseInit	(TIM9, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode					= TIM_OCMode_PWM1;		//TIM_OCMode_PWM2
	TIM_OCInitStructure.TIM_OutputState				= TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse					= ccp;
	TIM_OCInitStructure.TIM_OCPolarity				= TIM_OCPolarity_Low;

    // Применяем настройки к каналам
    TIM_OC1Init(TIM9, &TIM_OCInitStructure);  // CH1 (PA2)
    TIM_OC2Init(TIM9, &TIM_OCInitStructure);  // CH2 (PA3)
    TIM_OC3Init(TIM9, &TIM_OCInitStructure);  // CH3 (PA4)

	// Включаем предделитель (если нужен)
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM9, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs		(TIM9, ENABLE);
	TIM_ARRPreloadConfig	(TIM9, ENABLE);
	TIM_Cmd					(TIM9, ENABLE);		// Запускаем таймер
}