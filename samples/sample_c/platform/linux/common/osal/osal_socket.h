/**
 ********************************************************************
 * @file    osal_socket.h
 * @brief   This is the header file for "osal_socket.c", defining the structure and
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
#ifndef OSAL_SOCKET_H
#define OSAL_SOCKET_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode Osal_Socket(E_GduSocketMode mode, T_GduSocketHandle *socketHandle);

T_GduReturnCode Osal_Close(T_GduSocketHandle socketHandle);

T_GduReturnCode Osal_Bind(T_GduSocketHandle socketHandle, const char *ipAddr, uint32_t port);

T_GduReturnCode Osal_UdpSendData(T_GduSocketHandle socketHandle, const char *ipAddr, uint32_t port,
                                 const uint8_t *buf, uint32_t len, uint32_t *realLen);

T_GduReturnCode Osal_UdpRecvData(T_GduSocketHandle socketHandle, char *ipAddr, uint32_t *port,
                                 uint8_t *buf, uint32_t len, uint32_t *realLen);

T_GduReturnCode Osal_TcpListen(T_GduSocketHandle socketHandle);

T_GduReturnCode Osal_TcpAccept(T_GduSocketHandle socketHandle, char *ipAddr, uint32_t *port,
                               T_GduSocketHandle *outSocketHandle);

T_GduReturnCode Osal_TcpConnect(T_GduSocketHandle socketHandle, const char *ipAddr, uint32_t port);

T_GduReturnCode Osal_TcpSendData(T_GduSocketHandle socketHandle,
                                 const uint8_t *buf, uint32_t len, uint32_t *realLen);

T_GduReturnCode Osal_TcpRecvData(T_GduSocketHandle socketHandle,
                                 uint8_t *buf, uint32_t len, uint32_t *realLen);

#ifdef __cplusplus
}
#endif

#endif // OSAL_SOCKET_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
