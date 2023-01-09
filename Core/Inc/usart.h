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

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

/**
 * @brief receive CRLF end string with uart
 * @property receiveCR CR = 0x0d <br> receiveCRLF LF = 0x0a <br> itBuff interrupt handler function to use <br> data callback function to use
 *
 */
typedef struct {
    uint8_t receiveCR; // CR = 0x0d
    uint8_t receiveCRLF; // LF = 0x0a
    uint8_t itBuff;
    uint8_t dataMaxLen;
    uint8_t dataLen;
    uint8_t *data;
    UART_HandleTypeDef *handle;
} Uart_Receive_CRLF;

typedef struct {
    uint8_t receiveSuccess;
    uint8_t itBuff;
    uint8_t dataMaxLen;
    uint8_t dataLen;
    uint8_t *data;
    UART_HandleTypeDef *handle;
} Uart_Receive_M4255;


extern Uart_Receive_M4255 uart1ReceiveData;
extern Uart_Receive_M4255 uart2ReceiveData;
extern Uart_Receive_CRLF uart3ReceiveData;

void Uart_Receive_CRLF_Clean(Uart_Receive_CRLF *uart_rec);

void Uart_Receive_CRLF_Callback(Uart_Receive_CRLF *uart_rec);

void Uart_Receive_M4255_Clean(Uart_Receive_M4255 *uart_rec);

void Uart_Receive_M4255_Callback(Uart_Receive_M4255 *uart_rec);

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

