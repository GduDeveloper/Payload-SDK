/**
 ********************************************************************
 * @file    gdu_media_file_core.h
 * @brief   This is the header file for "gdu_media_file_core.c", defining the structure and
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
#ifndef PSDK_MEDIA_FILE_CORE_H
#define PSDK_MEDIA_FILE_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gdu_typedef.h>
#include <gdu_payload_camera.h>

/* Exported constants --------------------------------------------------------*/
#define PSDK_MEDIA_FILE_PATH_LEN_MAX           512             /*max file path len */
#define PSDK_MEDIA_DIR_PATH_LEN_MAX            256             /*max dir path len */

/* Exported types ------------------------------------------------------------*/
typedef struct {
    void *privThm;
} T_GduMediaFileThm;

typedef struct {
    void *privScr;
} T_GduMediaFileScr;

struct _GduMediaFile;

typedef struct {
    E_GduCameraMediaFileType mediaFileType;
    bool (*isSupportedFunc)(const char *filePath);

    T_GduReturnCode (*getAttrFunc)(struct _GduMediaFile *mediaFileHandle, T_GduCameraMediaFileAttr *mediaFileAttr);

    T_GduReturnCode (*getDataOrgFunc)(struct _GduMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                      uint8_t *data, uint16_t *realLen);
    T_GduReturnCode (*getFileSizeOrgFunc)(struct _GduMediaFile *mediaFileHandle, uint32_t *fileSize);

    T_GduReturnCode (*createThmFunc)(struct _GduMediaFile *mediaFileHandle);
    T_GduReturnCode (*getFileSizeThmFunc)(struct _GduMediaFile *mediaFileHandle, uint32_t *fileSize);
    T_GduReturnCode (*getDataThmFunc)(struct _GduMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                      uint8_t *data, uint16_t *realLen);
    T_GduReturnCode (*destroyThmFunc)(struct _GduMediaFile *mediaFileHandle);

    T_GduReturnCode (*creatScrFunc)(struct _GduMediaFile *mediaFileHandle);
    T_GduReturnCode (*getFileSizeScrFunc)(struct _GduMediaFile *mediaFileHandle, uint32_t *fileSize);
    T_GduReturnCode (*getDataScrFunc)(struct _GduMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                      uint8_t *data, uint16_t *realLen);
    T_GduReturnCode (*destroyScrFunc)(struct _GduMediaFile *mediaFileHandle);
} T_GduMediaFileOptItem;

typedef struct _GduMediaFile {
    char *filePath;
    T_GduMediaFileOptItem mediaFileOptItem;
    T_GduMediaFileThm mediaFileThm;
    T_GduMediaFileScr mediaFileScr;
} T_GduMediaFile, *T_GduMediaFileHandle;

/* Exported functions --------------------------------------------------------*/
bool GduMediaFile_IsSupported(const char *filePath);
T_GduReturnCode GduMediaFile_CreateHandle(const char *filePath, T_GduMediaFileHandle *pMediaFileHandle);
T_GduReturnCode GduMediaFile_DestroyHandle(T_GduMediaFileHandle mediaFileHandle);

T_GduReturnCode GduMediaFile_GetMediaFileType(T_GduMediaFileHandle mediaFileHandle,
                                              E_GduCameraMediaFileType *mediaFileType);
T_GduReturnCode GduMediaFile_GetMediaFileAttr(T_GduMediaFileHandle mediaFileHandle,
                                              T_GduCameraMediaFileAttr *mediaFileAttr);

T_GduReturnCode GduMediaFile_GetDataOrg(struct _GduMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint16_t *realLen);
T_GduReturnCode GduMediaFile_GetFileSizeOrg(struct _GduMediaFile *mediaFileHandle, uint32_t *fileSize);

T_GduReturnCode GduMediaFile_CreateThm(T_GduMediaFileHandle mediaFileHandle);
T_GduReturnCode GduMediaFile_GetFileSizeThm(T_GduMediaFileHandle mediaFileHandle, uint32_t *fileSize);
T_GduReturnCode GduMediaFile_GetDataThm(T_GduMediaFileHandle mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint16_t *realLen);
T_GduReturnCode GduMediaFile_DestoryThm(T_GduMediaFileHandle mediaFileHandle);

T_GduReturnCode GduMediaFile_CreateScr(T_GduMediaFileHandle mediaFileHandle);
T_GduReturnCode GduMediaFile_GetFileSizeScr(T_GduMediaFileHandle mediaFileHandle, uint32_t *fileSize);
T_GduReturnCode GduMediaFile_GetDataScr(T_GduMediaFileHandle mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint16_t *realLen);
T_GduReturnCode GduMediaFile_DestroyScr(T_GduMediaFileHandle mediaFileHandle);

#ifdef __cplusplus
}
#endif

#endif // PSDK_MEDIA_FILE_CORE_H

/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
