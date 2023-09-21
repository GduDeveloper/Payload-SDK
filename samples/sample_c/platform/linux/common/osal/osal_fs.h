/**
 ********************************************************************
 * @file    osal_fs.h
 * @brief   This is the header file for "osal_fs.c", defining the structure and
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
#ifndef OSAL_FS_H
#define OSAL_FS_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode Osal_FileOpen(const char *fileName, const char *fileMode, T_GduFileHandle *fileObj);

T_GduReturnCode Osal_FileClose(T_GduFileHandle fileObj);

T_GduReturnCode Osal_FileWrite(T_GduFileHandle fileObj, const uint8_t *buf, uint32_t len, uint32_t *realLen);

T_GduReturnCode Osal_FileRead(T_GduFileHandle fileObj, uint8_t *buf, uint32_t len, uint32_t *realLen);

T_GduReturnCode Osal_FileSeek(T_GduFileHandle fileObj, uint32_t offset);

T_GduReturnCode Osal_FileSync(T_GduFileHandle fileObj);

T_GduReturnCode Osal_DirOpen(const char *filePath, T_GduDirHandle *dirObj);

T_GduReturnCode Osal_DirClose(T_GduDirHandle dirObj);

T_GduReturnCode Osal_DirRead(T_GduDirHandle dirObj, T_GduFileInfo *fileInfo);

T_GduReturnCode Osal_Mkdir(const char *filePath);

T_GduReturnCode Osal_Unlink(const char *filePath);

T_GduReturnCode Osal_Rename(const char *oldFilePath, const char *newFilePath);

T_GduReturnCode Osal_Stat(const char *filePath, T_GduFileInfo *fileInfo);

#ifdef __cplusplus
}
#endif

#endif // OSAL_FS_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
