#include "adc_conf.h"

void ADC_init(void)		//Initializes ADC collection
{
	ADC_InitTypeDef		ADC_InitStructure	= {0};
	NVIC_InitTypeDef	NVIC_InitStructure	= {0};

	//RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOA, ENABLE);
	//RCC_PB2PeriphClockCmd(RCC_PB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	/*
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	*/

	ADC_InitStructure.ADC_Mode					= ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode			= DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode	= DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv		= ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign				= ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel			= 1;
	ADC_Init(ADC1, &ADC_InitStructure);

    ADC_InjectedSequencerLengthConfig(ADC1, 1);
    //ADC_InjectedChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5);
    ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);

 	// Настройка последовательности каналов
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5); // Канал 0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_7Cycles5); // Канал 1

	NVIC_InitStructure.NVIC_IRQChannel						= ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	//ADC_ExternalTrigConvCmd Trig_DLY(ADC1, ADC_ExternalTrigInjec_DLY, 0x10);	/* external trigger sources delay time */
	//ADC_Calibration_Vol(ADC1, ADC_CALVOL_50PERCENT);
	ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

	//ADC_FIFO_Cmd(ADC1, DISABLE);
	//ADC_BufferCmd(ADC1, DISABLE); // disable buffer

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));
}

void ADC_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	// Сброс настроек АЦП
	ADC_DeInit(ADC1);

	// Конфигурация АЦП
	ADC_InitStructure.ADC_Mode					= ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode			= DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode	= DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv		= ADC_ExternalTrigInjecConv_T2_TRGO;
	ADC_InitStructure.ADC_DataAlign				= ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel			= 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	// Настройка канала ADC1 Channel 0 (PA0)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);

	// Включение АЦП
	//ADC_Cmd(ADC1, ENABLE);

    // Enable ADC1 interrupt on conversion complete
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);


	// Калибровка АЦП
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1))
		;
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1))
		;
    // Enable ADC1 interrupt in NVIC
    NVIC_EnableIRQ(ADC_IRQn);
}

u16 Get_ADC_Val(u8 ch)
{
	u16 val;

	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_7Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

	val = ADC_GetConversionValue(ADC1);

	return val;
}
