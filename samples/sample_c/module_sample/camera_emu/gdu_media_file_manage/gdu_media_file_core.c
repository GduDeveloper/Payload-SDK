/**
 ********************************************************************
 * @file    gdu_media_file_core.c
 * @brief
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <gdu_logger.h>

#include "gdu_media_file_core.h"
#include "gdu_media_file_jpg.h"
#include "gdu_media_file_mp4.h"
#include "gdu_platform.h"

/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/

/* Private values ------------------------------------------------------------*/
//@formatter:off
static const T_GduMediaFileOptItem s_mediaFileOpt[] =
{
    //JPEG File Operation Item
    {
        GDU_CAMERA_FILE_TYPE_JPEG ,
        GduMediaFile_IsSupported_JPG,
        GduMediaFile_GetAttrFunc_JPG,
        GduMediaFile_GetDataOrigin_JPG,
        GduMediaFile_GetFileSizeOrigin_JPG,
        GduMediaFile_CreateThumbNail_JPG,
        GduMediaFile_GetFileSizeThumbNail_JPG,
        GduMediaFile_GetDataThumbNail_JPG,
        GduMediaFile_DestroyThumbNail_JPG,
        GduMediaFile_CreateScreenNail_JPG,
        GduMediaFile_GetFileSizeScreenNail_JPG,
        GduMediaFile_GetDataScreenNail_JPG,
        GduMediaFile_DestroyScreenNail_JPG,
    },
    //MP4 File Operation Item
    {
        GDU_CAMERA_FILE_TYPE_MP4 ,
        GduMediaFile_IsSupported_MP4,
        GduMediaFile_GetAttrFunc_MP4,
        GduMediaFile_GetDataOrigin_MP4,
        GduMediaFile_GetFileSizeOrigin_MP4,
        GduMediaFile_CreateThumbNail_MP4,
        GduMediaFile_GetFileSizeThumbNail_MP4,
        GduMediaFile_GetDataThumbNail_MP4,
        GduMediaFile_DestroyThumbNail_MP4,
        GduMediaFile_CreateScreenNail_MP4,
        GduMediaFile_GetFileSizeScreenNail_MP4,
        GduMediaFile_GetDataScreenNail_MP4,
        GduMediaFile_DestroyScreenNail_MP4,
    },
};
static const uint32_t s_mediaFileOptCount = sizeof (s_mediaFileOpt) / sizeof(T_GduMediaFileOptItem);
//@formatter:on

/* Exported functions definition ---------------------------------------------*/
bool GduMediaFile_IsSupported(const char *filePath)
{
    int i;

    for (i = 0; i < s_mediaFileOptCount; i++) {
        if (s_mediaFileOpt[i].isSupportedFunc(filePath) == true) {
            return true;
        }
    }

    return false;
}

T_GduReturnCode GduMediaFile_CreateHandle(const char *filePath, T_GduMediaFileHandle *pMediaFileHandle)
{
    int optIndex;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    for (optIndex = 0; optIndex < s_mediaFileOptCount; optIndex++) {
        if (s_mediaFileOpt[optIndex].isSupportedFunc(filePath) == true) {
            break;
        }
    }

    if (optIndex == s_mediaFileOptCount) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    *pMediaFileHandle = osalHandler->Malloc(sizeof(T_GduMediaFile));
    if (*pMediaFileHandle == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    (*pMediaFileHandle)->filePath = osalHandler->Malloc(strlen(filePath) + 1);
    if ((*pMediaFileHandle)->filePath == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    (*pMediaFileHandle)->mediaFileOptItem = s_mediaFileOpt[optIndex];
    (*pMediaFileHandle)->mediaFileThm.privThm = NULL;
    (*pMediaFileHandle)->mediaFileScr.privScr = NULL;

    strcpy((*pMediaFileHandle)->filePath, filePath);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduMediaFile_DestroyHandle(T_GduMediaFileHandle mediaFileHandle)
{
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    osalHandler->Free(mediaFileHandle->filePath);
    osalHandler->Free(mediaFileHandle);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduMediaFile_GetMediaFileType(T_GduMediaFileHandle mediaFileHandle,
                                              E_GduCameraMediaFileType *mediaFileType)
{
    *mediaFileType = mediaFileHandle->mediaFileOptItem.mediaFileType;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduMediaFile_GetMediaFileAttr(T_GduMediaFileHandle mediaFileHandle,
                                              T_GduCameraMediaFileAttr *mediaFileAttr)
{
    if (mediaFileHandle->mediaFileOptItem.getAttrFunc == NULL) {
        USER_LOG_ERROR("Media file handle getAttrFunc null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getAttrFunc(mediaFileHandle, mediaFileAttr);
}

T_GduReturnCode GduMediaFile_GetDataOrg(struct _GduMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint16_t *realLen)
{
    if (mediaFileHandle->mediaFileOptItem.getDataOrgFunc == NULL) {
        USER_LOG_ERROR("Media file handle getDataOrgFunc null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getDataOrgFunc(mediaFileHandle, offset, len, data, realLen);
}

T_GduReturnCode GduMediaFile_GetFileSizeOrg(struct _GduMediaFile *mediaFileHandle, uint32_t *fileSize)
{
    if (mediaFileHandle->mediaFileOptItem.getFileSizeOrgFunc == NULL) {
        USER_LOG_ERROR("Media file handle getFileSizeOrgFunc null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getFileSizeOrgFunc(mediaFileHandle, fileSize);
}

T_GduReturnCode GduMediaFile_CreateThm(T_GduMediaFileHandle mediaFileHandle)
{
    if (mediaFileHandle->mediaFileOptItem.createThmFunc == NULL) {
        USER_LOG_ERROR("Media file handle createThmFunc null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.createThmFunc(mediaFileHandle);
}

T_GduReturnCode GduMediaFile_GetFileSizeThm(T_GduMediaFileHandle mediaFileHandle, uint32_t *fileSize)
{
    if (mediaFileHandle->mediaFileOptItem.getFileSizeThmFunc == NULL) {
        USER_LOG_ERROR("Media file handle getFileSizeThmFunc null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getFileSizeThmFunc(mediaFileHandle, fileSize);
}

T_GduReturnCode GduMediaFile_GetDataThm(T_GduMediaFileHandle mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint16_t *realLen)
{
    if (mediaFileHandle->mediaFileOptItem.getDataThmFunc == NULL) {
        USER_LOG_ERROR("Media file handle getDataThmFunc null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getDataThmFunc(mediaFileHandle, offset, len, data, realLen);
}

T_GduReturnCode GduMediaFile_DestoryThm(T_GduMediaFileHandle mediaFileHandle)
{
    if (mediaFileHandle->mediaFileOptItem.destroyThmFunc == NULL) {
        USER_LOG_ERROR("Media file handle destroyThmFunc null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.destroyThmFunc(mediaFileHandle);
}

T_GduReturnCode GduMediaFile_CreateScr(T_GduMediaFileHandle mediaFileHandle)
{
    if (mediaFileHandle->mediaFileOptItem.creatScrFunc == NULL) {
        USER_LOG_ERROR("Media file handle creatScrFunc null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.creatScrFunc(mediaFileHandle);
}

T_GduReturnCode GduMediaFile_GetFileSizeScr(T_GduMediaFileHandle mediaFileHandle, uint32_t *fileSize)
{
    if (mediaFileHandle->mediaFileOptItem.getFileSizeScrFunc == NULL) {
        USER_LOG_ERROR("Media file handle getFileSizeScrFunc null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getFileSizeScrFunc(mediaFileHandle, fileSize);
}

T_GduReturnCode GduMediaFile_GetDataScr(T_GduMediaFileHandle mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint16_t *realLen)
{
    if (mediaFileHandle->mediaFileOptItem.getDataScrFunc == NULL) {
        USER_LOG_ERROR("Media file handle getDataScrFunc null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getDataScrFunc(mediaFileHandle, offset, len, data, realLen);
}

T_GduReturnCode GduMediaFile_DestroyScr(T_GduMediaFileHandle mediaFileHandle)
{
    if (mediaFileHandle->mediaFileOptItem.destroyScrFunc == NULL) {
        USER_LOG_ERROR("Media file handle destroyScrFunc null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.destroyScrFunc(mediaFileHandle);
}

/* Private functions definition-----------------------------------------------*/

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
