/**
 ********************************************************************
 * @file    gdu_media_file_mp4.h
 * @brief   This is the header file for "gdu_media_file_mp4.c", defining the structure and
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
#ifndef PSDK_MEDIA_FILE_MP4_H
#define PSDK_MEDIA_FILE_MP4_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gdu_payload_camera.h>
#include <gdu_typedef.h>
#include "gdu_media_file_core.h"

/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/
bool GduMediaFile_IsSupported_MP4(const char *filePath);

T_GduReturnCode GduMediaFile_GetAttrFunc_MP4(struct _GduMediaFile *mediaFileHandle,
                                             T_GduCameraMediaFileAttr *mediaFileAttr);

T_GduReturnCode GduMediaFile_GetDataOrigin_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                               uint8_t *data, uint16_t *realLen);
T_GduReturnCode GduMediaFile_GetFileSizeOrigin_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t *fileSize);

T_GduReturnCode GduMediaFile_CreateThumbNail_MP4(struct _GduMediaFile *mediaFileHandle);
T_GduReturnCode GduMediaFile_GetFileSizeThumbNail_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t *fileSize);
T_GduReturnCode
GduMediaFile_GetDataThumbNail_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                  uint8_t *data, uint16_t *realLen);
T_GduReturnCode GduMediaFile_DestroyThumbNail_MP4(struct _GduMediaFile *MediaFileHandle);

T_GduReturnCode GduMediaFile_CreateScreenNail_MP4(struct _GduMediaFile *mediaFileHandle);
T_GduReturnCode GduMediaFile_GetFileSizeScreenNail_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t *fileSize);
T_GduReturnCode
GduMediaFile_GetDataScreenNail_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                   uint8_t *data, uint16_t *realLen);
T_GduReturnCode GduMediaFile_DestroyScreenNail_MP4(struct _GduMediaFile *mediaFileHandle);

#ifdef __cplusplus
}
#endif

#endif // PSDK_MEDIA_FILE_MP4_H

/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
