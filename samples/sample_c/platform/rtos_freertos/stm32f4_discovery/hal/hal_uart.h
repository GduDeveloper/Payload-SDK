/**
 ********************************************************************
 * @file    hal.h
 * @version V2.0.0
 * @date    2019/8/30
 * @brief   This is the header file for "hal.c", defining the structure and
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
#ifndef HAL_H
#define HAL_H

/* Includes ------------------------------------------------------------------*/
//#include "gdu_platform.h"
#include "stdint.h"
#include "gdu_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

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

#endif // HAL_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
