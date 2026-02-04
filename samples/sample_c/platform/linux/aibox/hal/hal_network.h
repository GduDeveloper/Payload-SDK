/**
 ********************************************************************
 * @file    hal_network.h
 * @brief   This is the header file for "hal_network.c", defining the structure and
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
#ifndef HAL_NETWORK_H
#define HAL_NETWORK_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/** @attention  User can config network card name here, if your device is not MF2C/G, please comment below and add your
 * NIC name micro define as #define 'LINUX_NETWORK_DEV   "your NIC name"'.
 */
#ifdef PLATFORM_ARCH_x86_64
//#define LINUX_NETWORK_DEV           "enp0s31f6"
#define LINUX_NETWORK_DEV           "ens33"
#else
#define LINUX_NETWORK_DEV           "eth0"
#endif
/**
 * @attention
 */


#define LINUX_CMD_STR_MAX_SIZE      (128)

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode HalNetWork_Init(const char *ipAddr, const char *netMask, T_GduNetworkHandle *halObj);

T_GduReturnCode HalNetWork_DeInit(T_GduNetworkHandle halObj);

#ifdef __cplusplus
}
#endif

#endif // HAL_NETWORK_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
