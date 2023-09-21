/**
 ********************************************************************
 * @file    gdu_media_file_mp4.c
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
#include "gdu_media_file_mp4.h"
#include "gdu_media_file_core.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <gdu_logger.h>
#include <stdlib.h>
#include "gdu_platform.h"
#include "utils/util_time.h"
#include "utils/util_file.h"

/* Private constants ---------------------------------------------------------*/

#define MP4_FILE_SUFFIX                 ".mp4"
#define MP4_TEMP_FILE_TEMPLATE_STR      "MP4_TEMP_XXXXXX.jpg"
#define MP4_TEMP_FILE_PATH_MAX_LEN      50
#define FFMPEG_CMD_BUF_SIZE             (256 + 256)

#define MP4_THM_SCALE_CFG_STR           "scale=100:-1"
#define MP4_SCR_SCALE_CFG_STR           "scale=600:-1"

/* Private types -------------------------------------------------------------*/
typedef struct {
    FILE *tempFile;
    char tempfilePath[MP4_TEMP_FILE_PATH_MAX_LEN];
} T_GduMP4TempPicPriv;

/* Private functions declaration ---------------------------------------------*/
static T_GduReturnCode GduMediaFile_CreateTempPicPriv_MP4(const char *srcFilePath, const char *scaleCfgStr,
                                                          T_GduMP4TempPicPriv **pTempPicPrivHandle);
static T_GduReturnCode GduMediaFile_DestroyTempPicPriv_MP4(T_GduMP4TempPicPriv *tempPicPrivHandle);

/* Private values ------------------------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/
bool GduMediaFile_IsSupported_MP4(const char *filePath)
{
    if (filePath == NULL) {
        USER_LOG_ERROR("input parameter is null error");
        return false;
    }

    if (strcmp(&filePath[strlen(filePath) - 4], MP4_FILE_SUFFIX) == 0) {
        return true;
    }

    return false;
}

T_GduReturnCode GduMediaFile_GetAttrFunc_MP4(struct _GduMediaFile *mediaFileHandle,
                                             T_GduCameraMediaFileAttr *mediaFileAttr)
{
    FILE *fp;
    char ffmpegCmdStr[FFMPEG_CMD_BUF_SIZE];
    float hour, minute, second;
    char tempTailStr[128];
    int ret;
    T_GduReturnCode psdkStat;

    snprintf(ffmpegCmdStr, FFMPEG_CMD_BUF_SIZE, "ffmpeg -i \"%s\" 2>&1 | grep \"Duration\"", mediaFileHandle->filePath);
    fp = popen(ffmpegCmdStr, "r");

    if (fp == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    ret = fscanf(fp, "  Duration: %f:%f:%f,%127s", &hour, &minute, &second, tempTailStr);
    if (ret <= 0) {
        USER_LOG_ERROR("MP4 File Get Duration Error\n");
        psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        goto out;
    }

    mediaFileAttr->attrVideoDuration = (uint32_t) (hour * 3600 + minute * 60 + second + 0.5);

    /*! The user needs to obtain the frame rate and resolution of the video file by ffmpeg tools.
     * Also the frame rate and resolution of video need convert to enum E_GduCameraVideoFrameRate or
     * E_GduCameraVideoResolution.
     */
    mediaFileAttr->attrVideoFrameRate = GDU_CAMERA_VIDEO_FRAME_RATE_30_FPS;
    mediaFileAttr->attrVideoResolution = GDU_CAMERA_VIDEO_RESOLUTION_1920x1080;

    psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;

out:
    pclose(fp);

    return psdkStat;
}

T_GduReturnCode GduMediaFile_GetDataOrigin_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                               uint8_t *data, uint16_t *realLen)
{
    return UtilFile_GetFileDataByPath(mediaFileHandle->filePath, offset, len, data, realLen);
}

T_GduReturnCode GduMediaFile_GetFileSizeOrigin_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t *fileSize)
{
    return UtilFile_GetFileSizeByPath(mediaFileHandle->filePath, fileSize);
}

T_GduReturnCode GduMediaFile_CreateThumbNail_MP4(struct _GduMediaFile *mediaFileHandle)
{

    return GduMediaFile_CreateTempPicPriv_MP4(mediaFileHandle->filePath, MP4_THM_SCALE_CFG_STR,
                                              (T_GduMP4TempPicPriv **) &mediaFileHandle->mediaFileThm.privThm);
}

T_GduReturnCode GduMediaFile_GetFileSizeThumbNail_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t *fileSize)
{
    T_GduMP4TempPicPriv *jpgFileThmPriv = (T_GduMP4TempPicPriv *) mediaFileHandle->mediaFileThm.privThm;

    return UtilFile_GetFileSize(jpgFileThmPriv->tempFile, fileSize);
}

T_GduReturnCode
GduMediaFile_GetDataThumbNail_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                  uint8_t *data, uint16_t *realLen)
{
    T_GduMP4TempPicPriv *jpgFileThmPriv = (T_GduMP4TempPicPriv *) mediaFileHandle->mediaFileThm.privThm;

    return UtilFile_GetFileData(jpgFileThmPriv->tempFile, offset, len, data, realLen);
}

T_GduReturnCode GduMediaFile_DestroyThumbNail_MP4(struct _GduMediaFile *MediaFileHandle)
{
    return GduMediaFile_DestroyTempPicPriv_MP4(MediaFileHandle->mediaFileThm.privThm);
}

T_GduReturnCode GduMediaFile_CreateScreenNail_MP4(struct _GduMediaFile *mediaFileHandle)
{
    return GduMediaFile_CreateTempPicPriv_MP4(mediaFileHandle->filePath, MP4_SCR_SCALE_CFG_STR,
                                              (T_GduMP4TempPicPriv **) &mediaFileHandle->mediaFileScr.privScr);
}

T_GduReturnCode GduMediaFile_GetFileSizeScreenNail_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t *fileSize)
{
    T_GduMP4TempPicPriv *jpgFileScrPriv = (T_GduMP4TempPicPriv *) mediaFileHandle->mediaFileScr.privScr;

    return UtilFile_GetFileSize(jpgFileScrPriv->tempFile, fileSize);
}

T_GduReturnCode
GduMediaFile_GetDataScreenNail_MP4(struct _GduMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                   uint8_t *data, uint16_t *realLen)
{
    T_GduMP4TempPicPriv *jpgFileScrPriv = (T_GduMP4TempPicPriv *) mediaFileHandle->mediaFileScr.privScr;

    return UtilFile_GetFileData(jpgFileScrPriv->tempFile, offset, len, data, realLen);
}

T_GduReturnCode GduMediaFile_DestroyScreenNail_MP4(struct _GduMediaFile *mediaFileHandle)
{
    return GduMediaFile_DestroyTempPicPriv_MP4(mediaFileHandle->mediaFileScr.privScr);
}

/* Private functions definition-----------------------------------------------*/
static T_GduReturnCode GduMediaFile_CreateTempPicPriv_MP4(const char *srcFilePath, const char *scaleCfgStr,
                                                          T_GduMP4TempPicPriv **pTempPicPrivHandle)
{
    char ffmpeg_cmd[FFMPEG_CMD_BUF_SIZE];
    int cmdRet;
    T_GduRunTimeStamps tiStart, tiEnd;
    T_GduMP4TempPicPriv *mp4TempPicFile;
    T_GduReturnCode psdkStat;
    int tempFd;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    tiStart = GduUtilTime_GetRunTimeStamps();

    *pTempPicPrivHandle = osalHandler->Malloc(sizeof(T_GduMP4TempPicPriv));
    if (*pTempPicPrivHandle == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    mp4TempPicFile = *pTempPicPrivHandle;

#if 1
    //get temp file name
    strcpy(mp4TempPicFile->tempfilePath, MP4_TEMP_FILE_TEMPLATE_STR);
    tempFd = mkstemps(mp4TempPicFile->tempfilePath, strlen(MP4_FILE_SUFFIX));
    if (tempFd < 0) {
        USER_LOG_ERROR("JPG Create Temp File Error, tempFd = %d\n", tempFd);
        psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        goto err_create_temp;
    }
    close(tempFd);
    unlink(mp4TempPicFile->tempfilePath);
#endif

    //ffmpeg cmd send
    snprintf(ffmpeg_cmd, FFMPEG_CMD_BUF_SIZE, "ffmpeg -i \"%s\" -vf %s -ss 00:00:00 -vframes 1 %s 1>/dev/null 2>&1",
             srcFilePath, scaleCfgStr, mp4TempPicFile->tempfilePath);

    cmdRet = system(ffmpeg_cmd);

    tiEnd = GduUtilTime_GetRunTimeStamps();

    USER_LOG_DEBUG("JPG Create TempFile, RealTime = %ld us\n", tiEnd.realUsec - tiStart.realUsec);

    if (cmdRet != 0) {
        USER_LOG_ERROR("JPG ffmpeg cmd call error, ret = %d\n", cmdRet);
        psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        goto err_system_cmd;
    }

    //open temp file
    mp4TempPicFile->tempFile = fopen(mp4TempPicFile->tempfilePath, "rb+");
    if (mp4TempPicFile->tempFile == NULL) {
        psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        goto err_file_open;
    }

    //unlink temp file
    unlink(mp4TempPicFile->tempfilePath);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;

err_file_open:
    unlink(mp4TempPicFile->tempfilePath);
err_system_cmd:
err_create_temp:
    osalHandler->Free(*pTempPicPrivHandle);

    return psdkStat;
}

static T_GduReturnCode GduMediaFile_DestroyTempPicPriv_MP4(T_GduMP4TempPicPriv *tempPicPrivHandle)
{
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    fclose(tempPicPrivHandle->tempFile);
    osalHandler->Free(tempPicPrivHandle);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
