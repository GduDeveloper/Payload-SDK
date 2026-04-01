/**
 ********************************************************************
 * @file    hal_uart.h
 * @brief   This is the header file for "hal_uart.c", defining the structure and
 * (exported) function prototypes.
 *
 * @copyright (c) 2021 GDU. All rights reserved.
 *
 * All information contained herein is, and remains, the property of GDU.
 * The intellectual and technical concepts contained herein are proprietary
 * to GDU and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of GDU.
 *
 * If you receive this source code without GDU’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify GDU of its removal. GDU reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HAL_UART_H
#define HAL_UART_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "stdlib.h"

#include "gdu_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
//User can config dev based on there environmental conditions
//#define LINUX_UART_DEV1    "/dev/serial_tool"//"/dev/ttyUSB0"	//uart to aircraft
//#define LINUX_UART_DEV1    "/dev/ttyUSB0"	//uart to aircraft
#define LINUX_UART_DEV1    "/dev/ttyUSB0"	//uart to aircraft
//#define LINUX_UART_DEV1    "/dev/ttyS1"	//uart to aircraft
//#define LINUX_UART_DEV2    "/dev/ttyACM0"
#define LINUX_UART_DEV2    "/dev/tty3"		//uart to print log

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode HalUart_Init(E_GduHalUartNum uartNum, uint32_t baudRate, T_GduUartHandle *uartHandle);

T_GduReturnCode HalUart_DeInit(T_GduUartHandle uartHandle);

T_GduReturnCode HalUart_WriteData(T_GduUartHandle uartHandle, const uint8_t *buf, uint32_t len, uint32_t *realLen);

T_GduReturnCode HalUart_ReadData(T_GduUartHandle uartHandle, uint8_t *buf, uint32_t len, uint32_t *realLen);

T_GduReturnCode HalUart_GetStatus(E_GduHalUartNum uartNum, T_GduUartStatus *status);

#ifdef __cplusplus
}
#endif

#endif // HAL_UART_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
