/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "usart.h"
#include "at_log.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId dataUpdateTaskHandle;
osThreadId dataPushTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void mqtt_sample(void);
void data_template_sample(void);
/* USER CODE END FunctionPrototypes */

void DataUpdateTask(void const *argument);

void DataPushTask(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName) {
    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
    called if a stack overflow is detected. */
    Log_e("%s have been stack over flow", pcTaskName);
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void) {
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created. It is also called by various parts of the
    demo application. If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    Log_e("malloc failed");
}
/* USER CODE END 5 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* definition and creation of dataUpdateTask */
//    osThreadDef(dataUpdateTask, DataUpdateTask, osPriorityNormal, 0, 1024);
//    dataUpdateTaskHandle = osThreadCreate(osThread(dataUpdateTask), NULL);

    /* definition and creation of dataPushTask */
//    osThreadDef(dataPushTask, DataPushTask, osPriorityIdle, 0, 1024);
//    dataPushTaskHandle = osThreadCreate(osThread(dataPushTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */

    data_template_sample();
    /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_DataUpdateTask */
/**
  * @brief  Function implementing the dataUpdateTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_DataUpdateTask */
void DataUpdateTask(void const *argument) {
    /* USER CODE BEGIN DataUpdateTask */
//    char *tempStr = "AT \r\n";
//    HAL_UART_Transmit(&huart3, tempStr, sizeof(tempStr), 100);
    /* Infinite loop */
    for (;;) {
//        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//        HAL_UART_Transmit(&huart1, uart3ReceiveData.data, uart3ReceiveData.dataLen, 100);
//        Uart_Receive_CRLF_Clean(&uart3ReceiveData);
    }
    /* USER CODE END DataUpdateTask */
}

/* USER CODE BEGIN Header_DataPushTask */
/**
* @brief Function implementing the dataPushTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DataPushTask */
void DataPushTask(void const *argument) {
    /* USER CODE BEGIN DataPushTask */
    /* Infinite loop */
    for (;;) {
        osDelay(1);
    }
    /* USER CODE END DataPushTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
