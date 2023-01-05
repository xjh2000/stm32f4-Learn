/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define RXBUFFERSIZE 1 //串口1中断接受缓冲区的大小
#define USART_REC_LEN 200 //串口接受缓冲区大小

extern uint32_t USART_RX_STA; //串口接受的一个标识位 ,[15]接收到/r/n,[14]接收到/r,[13,0]接收到的数据数量
extern uint8_t aRxBuffer[RXBUFFERSIZE];//串口1接受中断用的数据缓冲区
extern uint8_t USART_RX_BUF[USART_REC_LEN];//串口数据接受的缓冲存储区

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

