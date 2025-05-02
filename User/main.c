
#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gpio_conf.h"
#include "timers_conf.h"
#include "adc_conf.h"
#include "limits.h"

/* Global define */
#define TASK1_TASK_PRIO     5
#define TASK1_STK_SIZE      256
#define TASK2_TASK_PRIO     5
#define TASK2_STK_SIZE      256

/* Global Variable */
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;

extern u_int16_t RxData;

#define STP_SW	10

void task1_task(void *pvParameters)     // task1 handler
{
	u8 cnt = 0;
	while(1)
	{
		Start_ADC_conv();

		if (RxData == 1)		// автоматический алгоритм переключения
		{
			if (cnt > 3*STP_SW) cnt = 0;
			if (cnt < STP_SW+1)
			{
				TIM_SetCompare1(TIM9, STP_PWM);		// (PA2) R
				TIM_SetCompare2(TIM9, 0);			// (PA3) G
				TIM_SetCompare3(TIM9, 0);			// (PA4) B

			}
			else if (cnt < 2*STP_SW+1)
			{
				TIM_SetCompare1(TIM9, 0);			// (PA2) R
				TIM_SetCompare2(TIM9, STP_PWM);		// (PA3) G
				TIM_SetCompare3(TIM9, 0);			// (PA4) B
			}
			else
			{
				TIM_SetCompare1(TIM9, 0);			// (PA2) R
				TIM_SetCompare2(TIM9, 0);			// (PA3) G
				TIM_SetCompare3(TIM9, STP_PWM);		// (PA4) B
			}
			cnt++;
		}
		//printf("task1 entry - \%d\r\n", cnt++);
		GPIO_TOGGLE_PIN(OUT_LED12_PORT, OUT_LED1_PIN);

		vTaskDelay(20);
	}
}

void task2_task(void *pvParameters)     // task2 handler
{
	/*
	while(1)
	{
		printf("task2 entry\r\n");
		GPIO_TOGGLE_PIN(OUT_LED12_PORT, OUT_LED2_PIN);
		vTaskDelay(500);
	}*/

	uint32_t ADC_Value32;
	BaseType_t xResult;
	
	for(;;)
	{
		// Ожидаем уведомления (блокирующий вызов)
		xResult = xTaskNotifyWait(0x00,				// Не очищаем биты при входе
								 ULONG_MAX,			// Очищаем все биты при выходе
								 &ADC_Value32,		// Получаем значение
								 portMAX_DELAY);
		
		if(xResult == pdTRUE)
		{
			// Извлекаем 16-битное значение (первые 2 байта)
			uint16_t ADC_Value = (uint16_t)(ADC_Value32 & 0xFFFF);
			
			// Обрабатываем данные
			printf("Получено значение: %u\n", ADC_Value);
			if (RxData == 0)
			{
				if (ADC_Value < TRANS_RG)
				{
					ADC_Value = STP_PWM * ADC_Value / (ADC_MAX / 3);
					TIM_SetCompare1(TIM9, STP_PWM - ADC_Value);		// (PA2) R
					TIM_SetCompare2(TIM9, ADC_Value);				// (PA3) G
					TIM_SetCompare3(TIM9, 0);	
				}
				else if (ADC_Value < TRANS_GB)
				{
					ADC_Value = STP_PWM * (ADC_Value - TRANS_RG) / (ADC_MAX / 3);
					TIM_SetCompare1(TIM9, 0);
					TIM_SetCompare2(TIM9, STP_PWM - ADC_Value);		// G
					TIM_SetCompare3(TIM9, ADC_Value);				// B
				}
				else
				{
					ADC_Value = STP_PWM * (ADC_Value - TRANS_GB) / (ADC_MAX / 3);
					TIM_SetCompare3(TIM9, STP_PWM - ADC_Value);		// B
					TIM_SetCompare1(TIM9, ADC_Value);				// R
					TIM_SetCompare2(TIM9, 0);
				}
			}

		}
	}
}

/************************** MAIN *************************************/
int main(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();

	Delay_Init();
	USART_Printf_Init(115200);
		
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID());
	printf("FreeRTOS Kernel Version:%s\r\n",tskKERNEL_VERSION_NUMBER);

	GPIO_init();
	TIM9_PWMOut_init(STP_PWM-1, 960-1, STP_PWM/2);  // 100kHz / 80 = 1,25 kHz, 96000000 / 960 = 100kHz, 50% 
	ADC_init();

	TIM_SetCompare1(TIM9, 0);  // 30% (PA2) R
	TIM_SetCompare2(TIM9, 0);  // 70% (PA3) G
	TIM_SetCompare3(TIM9, 0);  // 20% (PA4) B

	/* create two task */

	xTaskCreate(	(TaskFunction_t)	task1_task,
					(const char*)		"task1",
					(uint16_t)			TASK1_STK_SIZE,
					(void*)				NULL,
					(UBaseType_t)		TASK1_TASK_PRIO,
					(TaskHandle_t*)		&Task1Task_Handler);

	xTaskCreate(	(TaskFunction_t)	task2_task,
					(const char*)		"task2",
					(uint16_t)			TASK2_STK_SIZE,
					(void*)				NULL,
					(UBaseType_t)		TASK2_TASK_PRIO,
					(TaskHandle_t*)		&Task2Task_Handler);

	vTaskStartScheduler();

	while(1)
	{
		printf("shouldn't run at here!!\n");
	}
}
