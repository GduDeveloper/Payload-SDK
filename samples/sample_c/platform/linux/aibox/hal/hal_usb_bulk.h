/**
 ********************************************************************
 * @file    hal_usb_bulk.h
 * @brief   This is the header file for "hal_usb_bulk.c", defining the structure and
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
#ifndef HAL_USB_BULK_H
#define HAL_USB_BULK_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libusb-1.0/libusb.h>

#include "gdu_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode HalUsbBulk_Init(T_GduHalUsbBulkInfo usbBulkInfo, T_GduUsbBulkHandle *usbBulkHandle);

T_GduReturnCode HalUsbBulk_DeInit(T_GduUsbBulkHandle usbBulkHandle);

T_GduReturnCode HalUsbBulk_WriteData(T_GduUsbBulkHandle usbBulkHandle, const uint8_t *buf, uint32_t len,
                                     uint32_t *realLen);

T_GduReturnCode HalUsbBulk_ReadData(T_GduUsbBulkHandle usbBulkHandle, uint8_t *buf, uint32_t len, uint32_t *realLen);

T_GduReturnCode HalUsbBulk_GetDeviceInfo(T_GduHalUsbBulkDeviceInfo *deviceInfo);

#ifdef __cplusplus
}
#endif

#endif // HAL_USB_BULK_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
