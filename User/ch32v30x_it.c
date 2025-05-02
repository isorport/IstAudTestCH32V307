/********************************** (C) COPYRIGHT *******************************
* File Name          : ch32v30x_it.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2024/03/05
* Description        : Main Interrupt Service Routines.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "ch32v30x_it.h"
#include "FreeRTOS.h"
#include "task.h"
#include "limits.h"

extern TaskHandle_t Task2Task_Handler;

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void ADC1_2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
	while (1)
	{
	}
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
	NVIC_SystemReset();
	while (1)
	{
	}
}

/*********************************************************************
 * @fn      ADC1_2_IRQHandler
 *
 * @brief   This function handles analog wathdog exception.
 *
 * @return  none
 */
void ADC1_2_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (ADC_GetITStatus(ADC1, ADC_IT_EOC)) {

	// printf( "ADC IUpt = \%d\r\n", ADC_GetConversionValue(ADC1) );
	//  Отправляем уведомление задаче с передачей значения
	xTaskNotifyFromISR(	Task2Task_Handler,							// Целевая задача
						(uint32_t)ADC_GetConversionValue(ADC1),		// Передаем 2 байта в младших битах
						eSetValueWithOverwrite, 					// Перезаписываем предыдущее значение
						&xHigherPriorityTaskWoken);
  }

  ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
}
