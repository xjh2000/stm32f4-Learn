/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "log.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim5;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void) {
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void) {
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void) {
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void) {
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void) {
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void) {
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void) {
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void) {
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void) {
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void) {
    /* USER CODE BEGIN USART1_IRQn 0 */
    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
    /* USER CODE BEGIN USART1_IRQn 1 */
    /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void) {
    /* USER CODE BEGIN TIM5_IRQn 0 */

    /* USER CODE END TIM5_IRQn 0 */
    HAL_TIM_IRQHandler(&htim5);
    /* USER CODE BEGIN TIM5_IRQn 1 */

    /* USER CODE END TIM5_IRQn 1 */
}

/* USER CODE BEGIN 1 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // when this is called , the reception interrupt have been closed,
        // so we need to open again, for next data receive
        HAL_UART_Receive_IT(&huart1, (uint8_t *) aRxBuffer, RXBUFFERSIZE);

        if ((USART_RX_STA & 0x8000) == 0) {
            if (USART_RX_STA & 0x4000) {
                if (aRxBuffer[0] != 0x0a)USART_RX_STA = 0;
                else USART_RX_STA |= 0x8000;
            } else {
                if (aRxBuffer[0] == 0x0d)USART_RX_STA |= 0x4000;
                else {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = aRxBuffer[0];
                    USART_RX_STA++;
                    if (USART_RX_STA > (USART_REC_LEN - 1))USART_RX_STA = 0;
                }
            }
        }

    }
}


//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于 32 位定时器来说,1us 计数器加 1,溢出时间:4294 秒)
uint8_t TIM5CH1_CAPTURE_STA = 0; //输入捕获状态
uint32_t TIM5CH1_CAPTURE_VAL;//输入捕获值(TIM2/TIM5 是 32 位)

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//更新中断发生时执行
{
    if (htim->Instance == TIM5) {
        if ((TIM5CH1_CAPTURE_STA & 0X80) == 0)//还未成功捕获
        {
            if (TIM5CH1_CAPTURE_STA & 0X40)//已经捕获到高电平了
            {
                if ((TIM5CH1_CAPTURE_STA & 0X3F) == 0X3F)//高电平太长了
                {
                    TIM5CH1_CAPTURE_STA |= 0X80; //标记成功捕获了一次
                    TIM5CH1_CAPTURE_VAL = 0XFFFFFFFF;
                } else TIM5CH1_CAPTURE_STA++;
            }
        }
    }

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
    if (htim->Instance == TIM5) {
        if ((TIM5CH1_CAPTURE_STA & 0X80) == 0)//还未成功捕获
        {
            if (TIM5CH1_CAPTURE_STA & 0X40) //捕获到一个下降沿
            {
                TIM5CH1_CAPTURE_STA |= 0X80; //标记成功捕获到一次高电平脉宽
                TIM5CH1_CAPTURE_VAL =
                        HAL_TIM_ReadCapturedValue(&htim5, TIM_CHANNEL_1);
//获取当前的捕获值.
                TIM_RESET_CAPTUREPOLARITY(&htim5,
                                          TIM_CHANNEL_1); //清除设置
                TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1,
                                        TIM_ICPOLARITY_RISING);//上升沿捕获
            } else //还未开始,第一次捕获上升沿
            {
                TIM5CH1_CAPTURE_STA = 0; //清空
                TIM5CH1_CAPTURE_VAL = 0;
                TIM5CH1_CAPTURE_STA |= 0X40; //标记捕获到了上升沿
                __HAL_TIM_DISABLE(&htim5); //关闭定时器 5
                __HAL_TIM_SET_COUNTER(&htim5, 0);
                TIM_RESET_CAPTUREPOLARITY(&htim5,
                                          TIM_CHANNEL_1); //清除原来设置
                TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1,
                                        TIM_ICPOLARITY_FALLING);//下降沿捕获
                __HAL_TIM_ENABLE(&htim5);//使能定时器 5
            }
        }
    }

}

/* USER CODE END 1 */
