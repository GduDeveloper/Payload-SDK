/**
 ********************************************************************
 * @file    test_payload_cam_emu_media.c
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
#include <fcntl.h>
#include <stdlib.h>
#include "gdu_logger.h"
#include "utils/util_misc.h"
#include "utils/util_time.h"
#include "utils/util_file.h"
#include "utils/util_buffer.h"
#include "test_payload_cam_emu_media.h"
#include "test_payload_cam_emu_base.h"
#include "camera_emu/gdu_media_file_manage/gdu_media_file_core.h"
//#include "gdu_high_speed_data_channel.h"

/* Private constants ---------------------------------------------------------*/
#define FFMPEG_CMD_BUF_SIZE                 (256 + 256)
#define SEND_VIDEO_TASK_FREQ                 120
#define VIDEO_FRAME_MAX_COUNT                18000 // max video duration 10 minutes
#define VIDEO_FRAME_AUD_LEN                  6
#define DATA_SEND_FROM_VIDEO_STREAM_MAX_LEN  60000

/* Private types -------------------------------------------------------------*/
typedef enum {
    TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_STOP = 0,
    TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_PAUSE = 1,
    TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_START = 2,
} E_TestPayloadCameraPlaybackCommand;

typedef struct {
    uint8_t isInPlayProcess;
    uint16_t videoIndex;
    char filePath[GDU_FILE_PATH_SIZE_MAX];
    uint32_t videoLengthMs;
    uint64_t startPlayTimestampsUs;
    uint64_t playPosMs;
} T_GduPlaybackInfo;

typedef struct {
    E_TestPayloadCameraPlaybackCommand command;
    uint32_t timeMs;
    char path[GDU_FILE_PATH_SIZE_MAX];
} T_TestPayloadCameraPlaybackCommand;

typedef struct {
    float durationS;
    uint32_t positionInFile;
    uint32_t size;
} T_TestPayloadCameraVideoFrameInfo;

/* Private functions declaration ---------------------------------------------*/
static T_GduReturnCode GduPlayback_StopPlay(T_GduPlaybackInfo *playbackInfo);
static T_GduReturnCode GduPlayback_PausePlay(T_GduPlaybackInfo *playbackInfo);
static T_GduReturnCode GduPlayback_SetPlayFile(T_GduPlaybackInfo *playbackInfo, const char *filePath,
                                               uint16_t index);
static T_GduReturnCode GduPlayback_SeekPlay(T_GduPlaybackInfo *playbackInfo, uint32_t seekPos);
static T_GduReturnCode GduPlayback_StartPlay(T_GduPlaybackInfo *playbackInfo);
static T_GduReturnCode GduPlayback_GetPlaybackStatus(T_GduPlaybackInfo *playbackInfo,
                                                     T_GduCameraPlaybackStatus *playbackStatus);
static T_GduReturnCode GduPlayback_GetVideoLengthMs(const char *filePath, uint32_t *videoLengthMs);
static T_GduReturnCode GduPlayback_StartPlayProcess(const char *filePath, uint32_t playPosMs);
static T_GduReturnCode GduPlayback_StopPlayProcess(void);
static T_GduReturnCode
GduPlayback_VideoFileTranscode(const char *inPath, const char *outFormat, char *outPath, uint16_t outPathBufferSize);
static T_GduReturnCode
GduPlayback_GetFrameInfoOfVideoFile(const char *path, T_TestPayloadCameraVideoFrameInfo *frameInfo,
                                    uint32_t frameInfoBufferCount, uint32_t *frameCount);
static T_GduReturnCode GduPlayback_GetFrameRateOfVideoFile(const char *path, float *frameRate);
static T_GduReturnCode
GduPlayback_GetFrameNumberByTime(T_TestPayloadCameraVideoFrameInfo *frameInfo, uint32_t frameCount,
                                 uint32_t *frameNumber, uint32_t timeMs);
static T_GduReturnCode GetMediaFileDir(char *dirPath);
static T_GduReturnCode GetMediaFileOriginData(const char *filePath, uint32_t offset, uint32_t length,
                                              uint8_t *data);

static T_GduReturnCode CreateMediaFileThumbNail(const char *filePath);
static T_GduReturnCode GetMediaFileThumbNailInfo(const char *filePath, T_GduCameraMediaFileInfo *fileInfo);
static T_GduReturnCode GetMediaFileThumbNailData(const char *filePath, uint32_t offset, uint32_t length,
                                                 uint8_t *data);
static T_GduReturnCode DestroyMediaFileThumbNail(const char *filePath);

static T_GduReturnCode CreateMediaFileScreenNail(const char *filePath);
static T_GduReturnCode GetMediaFileScreenNailInfo(const char *filePath, T_GduCameraMediaFileInfo *fileInfo);
static T_GduReturnCode GetMediaFileScreenNailData(const char *filePath, uint32_t offset, uint32_t length,
                                                  uint8_t *data);
static T_GduReturnCode DestroyMediaFileScreenNail(const char *filePath);

static T_GduReturnCode DeleteMediaFile(char *filePath);
static T_GduReturnCode SetMediaPlaybackFile(const char *filePath);
static T_GduReturnCode StartMediaPlayback(void);
static T_GduReturnCode StopMediaPlayback(void);
static T_GduReturnCode PauseMediaPlayback(void);
static T_GduReturnCode SeekMediaPlayback(uint32_t playbackPosition);
static T_GduReturnCode GetMediaPlaybackStatus(T_GduCameraPlaybackStatus *status);

static T_GduReturnCode StartDownloadNotification(void);
static T_GduReturnCode StopDownloadNotification(void);

_Noreturn static void *UserCameraMedia_SendVideoTask(void *arg);

/* Private variables -------------------------------------------------------------*/
static T_GduCameraMediaDownloadPlaybackHandler s_psdkCameraMedia = {0};
static T_GduPlaybackInfo s_playbackInfo = {0};
static T_GduTaskHandle s_userSendVideoThread;
static T_UtilBuffer s_mediaPlayCommandBufferHandler = {0};
static T_GduMutexHandle s_mediaPlayCommandBufferMutex = {0};
static uint8_t s_mediaPlayCommandBuffer[sizeof(T_TestPayloadCameraPlaybackCommand) * 32] = {0};
static const char *s_frameKeyChar = "[PACKET]";
static const char *s_frameDurationTimeKeyChar = "duration_time";
static const char *s_framePositionKeyChar = "pos";
static const char *s_frameSizeKeyChar = "size";
static T_GduMediaFileHandle s_mediaFileThumbNailHandle;
static T_GduMediaFileHandle s_mediaFileScreenNailHandle;
static const uint8_t s_frameAudInfo[VIDEO_FRAME_AUD_LEN] = {0x00, 0x00, 0x00, 0x01, 0x09, 0x10};

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode GduTest_CameraEmuMediaStartService(void)
{
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    T_GduReturnCode returnCode;
    // const T_GduDataChannelBandwidthProportionOfHighspeedChannel bandwidthProportionOfHighspeedChannel =
    //     {10, 60, 30};

    s_psdkCameraMedia.GetMediaFileDir = GetMediaFileDir;
    s_psdkCameraMedia.GetMediaFileOriginInfo = GduTest_CameraMediaGetFileInfo;
    s_psdkCameraMedia.GetMediaFileOriginData = GetMediaFileOriginData;

    s_psdkCameraMedia.CreateMediaFileThumbNail = CreateMediaFileThumbNail;
    s_psdkCameraMedia.GetMediaFileThumbNailInfo = GetMediaFileThumbNailInfo;
    s_psdkCameraMedia.GetMediaFileThumbNailData = GetMediaFileThumbNailData;
    s_psdkCameraMedia.DestroyMediaFileThumbNail = DestroyMediaFileThumbNail;

    s_psdkCameraMedia.CreateMediaFileScreenNail = CreateMediaFileScreenNail;
    s_psdkCameraMedia.GetMediaFileScreenNailInfo = GetMediaFileScreenNailInfo;
    s_psdkCameraMedia.GetMediaFileScreenNailData = GetMediaFileScreenNailData;
    s_psdkCameraMedia.DestroyMediaFileScreenNail = DestroyMediaFileScreenNail;

    s_psdkCameraMedia.DeleteMediaFile = DeleteMediaFile;

    s_psdkCameraMedia.SetMediaPlaybackFile = SetMediaPlaybackFile;

    s_psdkCameraMedia.StartMediaPlayback = StartMediaPlayback;
    s_psdkCameraMedia.StopMediaPlayback = StopMediaPlayback; 
    s_psdkCameraMedia.PauseMediaPlayback = PauseMediaPlayback;
    s_psdkCameraMedia.SeekMediaPlayback = SeekMediaPlayback;
    s_psdkCameraMedia.GetMediaPlaybackStatus = GetMediaPlaybackStatus;

    s_psdkCameraMedia.StartDownloadNotification = StartDownloadNotification;
    s_psdkCameraMedia.StopDownloadNotification = StopDownloadNotification;

    if (osalHandler->MutexCreate(&s_mediaPlayCommandBufferMutex) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("mutex create error"); 
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN; 
    }

    UtilBuffer_Init(&s_mediaPlayCommandBufferHandler, s_mediaPlayCommandBuffer, sizeof(s_mediaPlayCommandBuffer));

    returnCode = GduPayloadCamera_RegMediaDownloadPlaybackHandler(&s_psdkCameraMedia);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("psdk camera media function init error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    // returnCode = GduHighSpeedDataChannel_SetBandwidthProportion(bandwidthProportionOfHighspeedChannel);
    // if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    //     USER_LOG_ERROR("Set data channel bandwidth width proportion error.");
    //     return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    // }

    returnCode = osalHandler->TaskCreate("user_camera_media_task", UserCameraMedia_SendVideoTask, 2048,
                                         NULL, &s_userSendVideoThread);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("user send video task create error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_CameraMediaGetFileInfo(const char *filePath, T_GduCameraMediaFileInfo *fileInfo)
{
    T_GduReturnCode returnCode;
    T_GduMediaFileHandle mediaFileHandle;

    returnCode = GduMediaFile_CreateHandle(filePath, &mediaFileHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduMediaFile_GetMediaFileType(mediaFileHandle, &fileInfo->type);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get type error stat:0x%08llX", returnCode);
        goto out;
    }

    returnCode = GduMediaFile_GetMediaFileAttr(mediaFileHandle, &fileInfo->mediaFileAttr);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get attr error stat:0x%08llX", returnCode);
        goto out;
    }

    returnCode = GduMediaFile_GetFileSizeOrg(mediaFileHandle, &fileInfo->fileSize);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get size error stat:0x%08llX", returnCode);
        goto out;
    }

out:
    returnCode = GduMediaFile_DestroyHandle(mediaFileHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return returnCode;
}

/* Private functions definition-----------------------------------------------*/
static T_GduReturnCode GduPlayback_StopPlay(T_GduPlaybackInfo *playbackInfo)
{
    T_GduReturnCode returnCode;

    returnCode = GduPlayback_StopPlayProcess();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("stop play error ");
    }

    playbackInfo->isInPlayProcess = 0;
    playbackInfo->playPosMs = 0;

    return returnCode;
}

static T_GduReturnCode GduPlayback_PausePlay(T_GduPlaybackInfo *playbackInfo)
{
    T_GduReturnCode returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    T_TestPayloadCameraPlaybackCommand playbackCommand = {0};
    if (playbackInfo->isInPlayProcess) {
        playbackCommand.command = TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_PAUSE;

        if (osalHandler->MutexLock(s_mediaPlayCommandBufferMutex) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("mutex lock error");
            return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        }

        if (UtilBuffer_GetUnusedSize(&s_mediaPlayCommandBufferHandler) >= sizeof(T_TestPayloadCameraPlaybackCommand)) {
            UtilBuffer_Put(&s_mediaPlayCommandBufferHandler, (const uint8_t *) &playbackCommand,
                           sizeof(T_TestPayloadCameraPlaybackCommand));
        } else {
            USER_LOG_ERROR("Media playback command buffer is full.");
            returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
        }

        if (osalHandler->MutexUnlock(s_mediaPlayCommandBufferMutex) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("mutex unlock error");
            return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        }
    }

    playbackInfo->isInPlayProcess = 0;

    return returnCode;
}

static T_GduReturnCode GduPlayback_SetPlayFile(T_GduPlaybackInfo *playbackInfo, const char *filePath, uint16_t index)
{
    T_GduReturnCode returnCode;

    if (strlen(filePath) > GDU_FILE_PATH_SIZE_MAX) {
        USER_LOG_ERROR("Gdu playback file path out of length range error\n");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    strcpy(playbackInfo->filePath, filePath);
    playbackInfo->videoIndex = index;

    returnCode = GduPlayback_GetVideoLengthMs(filePath, &playbackInfo->videoLengthMs);

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduPlayback_SeekPlay(T_GduPlaybackInfo *playbackInfo, uint32_t seekPos)
{
    T_GduRunTimeStamps ti;
    T_GduReturnCode returnCode;

    returnCode = GduPlayback_PausePlay(playbackInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("pause play error \n");
        return returnCode;
    }

    playbackInfo->playPosMs = seekPos;
    returnCode = GduPlayback_StartPlayProcess(playbackInfo->filePath, playbackInfo->playPosMs);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("start playback process error \n");
        return returnCode;
    }

    playbackInfo->isInPlayProcess = 1;
    ti = GduUtilTime_GetRunTimeStamps();
    playbackInfo->startPlayTimestampsUs = ti.realUsec - playbackInfo->playPosMs * 1000;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduPlayback_StartPlay(T_GduPlaybackInfo *playbackInfo)
{
    T_GduRunTimeStamps ti;
    T_GduReturnCode returnCode;

    if (playbackInfo->isInPlayProcess == 1) {
        //already in playing, return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }

    returnCode = GduPlayback_StartPlayProcess(playbackInfo->filePath, playbackInfo->playPosMs);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("start play process error \n");
        return returnCode;
    }

    playbackInfo->isInPlayProcess = 1;

    ti = GduUtilTime_GetRunTimeStamps();
    playbackInfo->startPlayTimestampsUs = ti.realUsec - playbackInfo->playPosMs * 1000;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduPlayback_GetPlaybackStatus(T_GduPlaybackInfo *playbackInfo,
                                                     T_GduCameraPlaybackStatus *playbackStatus)
{
    T_GduRunTimeStamps timeStamps;

    memset(playbackStatus, 0, sizeof(T_GduCameraPlaybackStatus));

    //update playback pos info
    if (playbackInfo->isInPlayProcess) {
        timeStamps = GduUtilTime_GetRunTimeStamps();
        playbackInfo->playPosMs = (timeStamps.realUsec - playbackInfo->startPlayTimestampsUs) / 1000;

        if (playbackInfo->playPosMs >= playbackInfo->videoLengthMs) {
            if (GduPlayback_PausePlay(playbackInfo) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
            }
        }
    }

    //set playback status
    if (playbackInfo->isInPlayProcess == 0 && playbackInfo->playPosMs != 0) {
        playbackStatus->playbackMode = GDU_CAMERA_PLAYBACK_MODE_PAUSE;
    } else if (playbackInfo->isInPlayProcess) {
        playbackStatus->playbackMode = GDU_CAMERA_PLAYBACK_MODE_PLAY;
    } else {
        playbackStatus->playbackMode = GDU_CAMERA_PLAYBACK_MODE_STOP;
    }

    playbackStatus->playPosMs = playbackInfo->playPosMs;
    playbackStatus->videoLengthMs = playbackInfo->videoLengthMs;

    if (playbackInfo->videoLengthMs != 0) {
        playbackStatus->videoPlayProcess =
            (playbackInfo->videoLengthMs - playbackInfo->playPosMs) / playbackInfo->videoLengthMs;
    } else {
        playbackStatus->videoPlayProcess = 0;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduPlayback_GetVideoLengthMs(const char *filePath, uint32_t *videoLengthMs)
{
    FILE *fp;
    T_GduReturnCode returnCode;
    char ffmpegCmdStr[FFMPEG_CMD_BUF_SIZE];
    float hour, minute, second;
    char tempTailStr[128];
    int ret;

    snprintf(ffmpegCmdStr, FFMPEG_CMD_BUF_SIZE, "ffmpeg -i \"%s\" 2>&1 | grep \"Duration\"", filePath);
    fp = popen(ffmpegCmdStr, "r");

    if (fp == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    ret = fscanf(fp, "  Duration: %f:%f:%f,%127s", &hour, &minute, &second, tempTailStr);
    if (ret <= 0) {
        USER_LOG_ERROR("MP4 File Get Duration Error\n");
        returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        goto out;
    }

    *videoLengthMs = (uint32_t) ((hour * 3600 + minute * 60 + second) * 1000);
    returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;

out:
    pclose(fp);

    return returnCode;
}

static T_GduReturnCode GduPlayback_StartPlayProcess(const char *filePath, uint32_t playPosMs)
{
    T_GduReturnCode returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_TestPayloadCameraPlaybackCommand mediaPlayCommand = {0};
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    mediaPlayCommand.command = TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_START;
    mediaPlayCommand.timeMs = playPosMs;

    if (strlen(filePath) >= sizeof(mediaPlayCommand.path)) {
        USER_LOG_ERROR("File path is too long.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
    }  
    memcpy(mediaPlayCommand.path, filePath, strlen(filePath));

    if (osalHandler->MutexLock(s_mediaPlayCommandBufferMutex) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("mutex lock error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (UtilBuffer_GetUnusedSize(&s_mediaPlayCommandBufferHandler) >= sizeof(T_TestPayloadCameraPlaybackCommand)) {
        UtilBuffer_Put(&s_mediaPlayCommandBufferHandler, (const uint8_t *) &mediaPlayCommand,
                       sizeof(T_TestPayloadCameraPlaybackCommand));
        USER_LOG_DEBUG("start process-----------, command:%d, path:%s, timeMs:%d", mediaPlayCommand.command, mediaPlayCommand.path, mediaPlayCommand.timeMs);
    } else {
        USER_LOG_ERROR("Media playback command buffer is full.");
        returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
    }

    if (osalHandler->MutexUnlock(s_mediaPlayCommandBufferMutex) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("mutex unlock error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    
    return returnCode;
}

static T_GduReturnCode GduPlayback_StopPlayProcess(void)
{
    T_GduReturnCode returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_TestPayloadCameraPlaybackCommand playbackCommand = {0};
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    playbackCommand.command = TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_STOP;

    if (osalHandler->MutexLock(s_mediaPlayCommandBufferMutex) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("mutex lock error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (UtilBuffer_GetUnusedSize(&s_mediaPlayCommandBufferHandler) >= sizeof(T_TestPayloadCameraPlaybackCommand)) {
        UtilBuffer_Put(&s_mediaPlayCommandBufferHandler, (const uint8_t *) &playbackCommand,
                       sizeof(T_TestPayloadCameraPlaybackCommand));
    } else {
        USER_LOG_ERROR("Media playback command buffer is full.");
        returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
    }

    if (osalHandler->MutexUnlock(s_mediaPlayCommandBufferMutex) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("mutex unlock error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return returnCode;
}

static T_GduReturnCode GduPlayback_VideoFileTranscode(const char *inPath, const char *outFormat, char *outPath,
                                                      uint16_t outPathBufferSize)
{
    T_GduReturnCode returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_GduReturnCode gduStatus = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    FILE *fpCommand = NULL;
    char ffmpegCmdStr[FFMPEG_CMD_BUF_SIZE];
    char *directory = NULL;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    directory = osalHandler->Malloc(GDU_FILE_PATH_SIZE_MAX);
    if (directory == NULL) {
        USER_LOG_ERROR("malloc memory for directory fail.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    gduStatus = GduUserUtil_GetCurrentFileDirPath(inPath, GDU_FILE_PATH_SIZE_MAX, directory);
    if (gduStatus != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get directory of file error: 0x%08llX.", gduStatus);
        returnCode = gduStatus;
        goto out;
    }

    snprintf(outPath, outPathBufferSize, "%sout.%s", directory, outFormat);
    snprintf(ffmpegCmdStr, FFMPEG_CMD_BUF_SIZE,
             "echo \"y\" | ffmpeg -i %s -codec copy -bsf: h264_mp4toannexb -f %s %s 1>/dev/null 2>&1", inPath,
             outFormat, outPath);

	USER_LOG_DEBUG("............ffmpegCmdStr:%s", ffmpegCmdStr);
    fpCommand = popen(ffmpegCmdStr, "r");
    if (fpCommand == NULL) {
        USER_LOG_ERROR("execute transcode command fail.");
        returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        goto out;
    }

    pclose(fpCommand);

out:
    osalHandler->Free(directory);

    return returnCode;
}

static T_GduReturnCode
GduPlayback_GetFrameInfoOfVideoFile(const char *path, T_TestPayloadCameraVideoFrameInfo *frameInfo,
                                    uint32_t frameInfoBufferCount, uint32_t *frameCount)
{
    long ret;
    T_GduReturnCode returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    FILE *fpCommand = NULL;
    char ffmpegCmdStr[FFMPEG_CMD_BUF_SIZE];
    char *frameInfoString = NULL;
    char *frameLocation = NULL;
    char frameParameterFormat[50] = {0};
    char *frameDurationTimeLocation = NULL;
    float frameDurationTimeS = 0;
    uint32_t frameNumber = 0;
    char *framePositionLocation = NULL;
    uint32_t framePosition = 0;
    char *frameSizeLocation = NULL;
    uint32_t frameSize = 0;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    frameInfoString = osalHandler->Malloc(VIDEO_FRAME_MAX_COUNT * 1024);
    if (frameInfoString == NULL) {
        USER_LOG_ERROR("malloc memory for frame info fail.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }
    memset(frameInfoString, 0, VIDEO_FRAME_MAX_COUNT * 1024);

    snprintf(ffmpegCmdStr, FFMPEG_CMD_BUF_SIZE, "ffprobe -show_packets \"%s\" 2>/dev/null", path);
    fpCommand = popen(ffmpegCmdStr, "r");
    if (fpCommand == NULL) {
        USER_LOG_ERROR("execute show frames commands fail.");
        returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        goto out2;
    }

    ret = (long) fread(frameInfoString, 1, VIDEO_FRAME_MAX_COUNT * 1024, fpCommand);
    if (ret < 0) {
        USER_LOG_ERROR("read show frames commands result error.");
        goto out1;
    }
    frameInfoString[ret] = '\0';

    frameLocation = frameInfoString;
    *frameCount = 0;
    while (1) {
        // find frame
        frameLocation = strstr(frameLocation, s_frameKeyChar);
        if (frameLocation == NULL) {
            USER_LOG_DEBUG("reach file tail.");
            break;
        }

        // find frame duration
        frameDurationTimeLocation = strstr(frameLocation, s_frameDurationTimeKeyChar);
        if (frameDurationTimeLocation == NULL) {
            USER_LOG_ERROR("can not find pkt_duration_time.");
            returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }

        ret = snprintf(frameParameterFormat, sizeof(frameParameterFormat), "%s=%%f", s_frameDurationTimeKeyChar);
        if (ret < 0) {
            USER_LOG_ERROR("snprintf frameParameterFormat fail.");
            returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
            goto out1;
        }

        ret = sscanf(frameDurationTimeLocation, frameParameterFormat, &frameDurationTimeS);
        if (ret <= 0) {
            USER_LOG_ERROR("can not find pkt_duration_time.");
            returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }
        frameInfo[frameNumber].durationS = frameDurationTimeS;

        // find frame position
        framePositionLocation = strstr(frameLocation, s_framePositionKeyChar);
        if (framePositionLocation == NULL) {
            USER_LOG_ERROR("can not found pkt_pos.");
            returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }

        ret = snprintf(frameParameterFormat, sizeof(frameParameterFormat), "%s=%%d", s_framePositionKeyChar);
        if (ret < 0) {
            USER_LOG_ERROR("snprintf frameParameterFormat fail.");
            returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
            goto out1;
        }

        ret = sscanf(framePositionLocation, frameParameterFormat, &framePosition);
        if (ret <= 0) {
            USER_LOG_ERROR("can not found pkt_pos.");
            returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }
        frameInfo[frameNumber].positionInFile = framePosition;

        // find frame size
        frameSizeLocation = strstr(frameLocation, s_frameSizeKeyChar);
        if (frameSizeLocation == NULL) {
            USER_LOG_ERROR("can not found pkt_size.");
            returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }

        ret = snprintf(frameParameterFormat, sizeof(frameParameterFormat), "%s=%%d", s_frameSizeKeyChar);
        if (ret < 0) {
            USER_LOG_ERROR("snprintf frameParameterFormat fail.");
            returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
            goto out1;
        }

        ret = sscanf(frameSizeLocation, frameParameterFormat, &frameSize);
        if (ret <= 0) {
            USER_LOG_ERROR("can not find pkt_size.");
            returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }
        frameInfo[frameNumber].size = frameSize;

        frameLocation += strlen(s_frameKeyChar);
        frameNumber++;
        (*frameCount)++;

        if (frameNumber >= frameInfoBufferCount) {
            USER_LOG_ERROR("frame buffer is full.");
            returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
            goto out1;
        }
    }

out1:
    pclose(fpCommand);

out2:
    osalHandler->Free(frameInfoString);

    return returnCode;
}

static T_GduReturnCode GduPlayback_GetFrameRateOfVideoFile(const char *path, float *frameRate)
{
    int ret;
    T_GduReturnCode returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    FILE *fpCommand = NULL;
    char ffmpegCmdStr[FFMPEG_CMD_BUF_SIZE] = {0};
    int frameRateMolecule = 0;
    int frameRateDenominator = 0;
    unsigned char result[256] = {0};
    snprintf(ffmpegCmdStr, FFMPEG_CMD_BUF_SIZE, "ffprobe -show_streams \"%s\" 2>/dev/null | grep r_frame_rate", path);
    fpCommand = popen(ffmpegCmdStr, "r");
    if (fpCommand == NULL) {
        USER_LOG_ERROR("execute show frame rate command fail.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    fscanf(fpCommand,"%s",result);
    USER_LOG_DEBUG("result: %s", result);
    ret = sscanf(result, "r_frame_rate=%d/%d", &frameRateMolecule, &frameRateDenominator);
    if (ret <= 0) {
        USER_LOG_ERROR("can not find frame rate.  path:%s", path);
        returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
        goto out;
    }
    *frameRate = (float) frameRateMolecule / (float) frameRateDenominator;

out:
    pclose(fpCommand);

    return returnCode;
}

static T_GduReturnCode GduPlayback_GetFrameNumberByTime(T_TestPayloadCameraVideoFrameInfo *frameInfo,
                                                        uint32_t frameCount, uint32_t *frameNumber, uint32_t timeMs)
{
    uint32_t i = 0;
    double camulativeTimeS = 0;
    double timeS = (double) timeMs / 1000.0;

    for (i = 0; i < frameCount; ++i) {
        camulativeTimeS += frameInfo[i].durationS;

        if (camulativeTimeS >= timeS) {
            *frameNumber = i;
            return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
        }
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
}

static T_GduReturnCode GetMediaFileDir(char *dirPath)
{
    T_GduReturnCode returnCode;
    char curFileDirPath[GDU_FILE_PATH_SIZE_MAX];
    char tempPath[GDU_FILE_PATH_SIZE_MAX];

    returnCode = GduUserUtil_GetCurrentFileDirPath(__FILE__, GDU_FILE_PATH_SIZE_MAX, curFileDirPath);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file current path error, stat = 0x%08llX", returnCode);
        return returnCode;
    }

    snprintf(dirPath, GDU_FILE_PATH_SIZE_MAX, "%smedia_file", curFileDirPath);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetMediaFileOriginData(const char *filePath, uint32_t offset, uint32_t length, uint8_t *data)
{
    T_GduReturnCode returnCode;
    uint16_t realLen = 0;
    T_GduMediaFileHandle mediaFileHandle;

    returnCode = GduMediaFile_CreateHandle(filePath, &mediaFileHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduMediaFile_GetDataOrg(mediaFileHandle, offset, length, data, &realLen);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get data error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduMediaFile_DestroyHandle(mediaFileHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode CreateMediaFileThumbNail(const char *filePath)
{
    T_GduReturnCode returnCode;

    returnCode = GduMediaFile_CreateHandle(filePath, &s_mediaFileThumbNailHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduMediaFile_CreateThm(s_mediaFileThumbNailHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create thumb nail error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetMediaFileThumbNailInfo(const char *filePath, T_GduCameraMediaFileInfo *fileInfo)
{
    T_GduReturnCode returnCode;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileThumbNailHandle == NULL) {
        USER_LOG_ERROR("Media file thumb nail handle null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = GduMediaFile_GetMediaFileType(s_mediaFileThumbNailHandle, &fileInfo->type);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get type error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduMediaFile_GetMediaFileAttr(s_mediaFileThumbNailHandle, &fileInfo->mediaFileAttr);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get attr error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduMediaFile_GetFileSizeThm(s_mediaFileThumbNailHandle, &fileInfo->fileSize);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get size error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetMediaFileThumbNailData(const char *filePath, uint32_t offset, uint32_t length, uint8_t *data)
{
    T_GduReturnCode returnCode;
    uint16_t realLen = 0;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileThumbNailHandle == NULL) {
        USER_LOG_ERROR("Media file thumb nail handle null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = GduMediaFile_GetDataThm(s_mediaFileThumbNailHandle, offset, length, data, &realLen);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get data error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode DestroyMediaFileThumbNail(const char *filePath)
{
    T_GduReturnCode returnCode;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileThumbNailHandle == NULL) {
        USER_LOG_ERROR("Media file thumb nail handle null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = GduMediaFile_DestoryThm(s_mediaFileThumbNailHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy thumb nail error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduMediaFile_DestroyHandle(s_mediaFileThumbNailHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode CreateMediaFileScreenNail(const char *filePath)
{
    T_GduReturnCode returnCode;

    returnCode = GduMediaFile_CreateHandle(filePath, &s_mediaFileScreenNailHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduMediaFile_CreateScr(s_mediaFileScreenNailHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create screen nail error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetMediaFileScreenNailInfo(const char *filePath, T_GduCameraMediaFileInfo *fileInfo)
{
    T_GduReturnCode returnCode;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileScreenNailHandle == NULL) {
        USER_LOG_ERROR("Media file screen nail handle null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = GduMediaFile_GetMediaFileType(s_mediaFileScreenNailHandle, &fileInfo->type);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get type error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduMediaFile_GetMediaFileAttr(s_mediaFileScreenNailHandle, &fileInfo->mediaFileAttr);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get attr error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduMediaFile_GetFileSizeScr(s_mediaFileScreenNailHandle, &fileInfo->fileSize);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get size error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetMediaFileScreenNailData(const char *filePath, uint32_t offset, uint32_t length,
                                                  uint8_t *data)
{
    T_GduReturnCode returnCode;
    uint16_t realLen = 0;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileScreenNailHandle == NULL) {
        USER_LOG_ERROR("Media file screen nail handle null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = GduMediaFile_GetDataScr(s_mediaFileScreenNailHandle, offset, length, data, &realLen);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get size error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode DestroyMediaFileScreenNail(const char *filePath)
{
    T_GduReturnCode returnCode;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileScreenNailHandle == NULL) {
        USER_LOG_ERROR("Media file screen nail handle null error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = GduMediaFile_DestroyScr(s_mediaFileScreenNailHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy screen nail error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduMediaFile_DestroyHandle(s_mediaFileScreenNailHandle);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode DeleteMediaFile(char *filePath)
{
    T_GduReturnCode returnCode;

    USER_LOG_INFO("delete media file:%s", filePath);
    returnCode = UtilFile_Delete(filePath);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file delete error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetMediaPlaybackFile(const char *filePath)
{
    USER_LOG_INFO("set media playback file:%s", filePath);
    T_GduReturnCode returnCode;

    returnCode = GduPlayback_StopPlay(&s_playbackInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        return returnCode;
    }

    returnCode = GduPlayback_SetPlayFile(&s_playbackInfo, filePath, 0);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        return returnCode;
    }

    returnCode = GduPlayback_StartPlay(&s_playbackInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode StartMediaPlayback(void)
{
    T_GduReturnCode returnCode;

    USER_LOG_INFO("start media playback");
    returnCode = GduPlayback_StartPlay(&s_playbackInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("start media playback status error, stat:0x%08llX", returnCode);
        return returnCode;
    }

    return returnCode;
}

static T_GduReturnCode StopMediaPlayback(void)
{
    T_GduReturnCode returnCode;

    USER_LOG_INFO("stop media playback");
    returnCode = GduPlayback_StopPlay(&s_playbackInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("stop media playback error, stat:0x%08llX", returnCode);
        return returnCode;
    }

    return returnCode;
}

static T_GduReturnCode PauseMediaPlayback(void)
{
    T_GduReturnCode returnCode;

    USER_LOG_INFO("pause media playback");
    returnCode = GduPlayback_PausePlay(&s_playbackInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("pause media playback error, stat:0x%08llX", returnCode);
        return returnCode;
    }

    return returnCode;
}

static T_GduReturnCode SeekMediaPlayback(uint32_t playbackPosition)
{
    T_GduReturnCode returnCode;

    USER_LOG_INFO("seek media playback:%d", playbackPosition);
    returnCode = GduPlayback_SeekPlay(&s_playbackInfo, playbackPosition);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("seek media playback error, stat:0x%08llX", returnCode);
        return returnCode;
    }

    return returnCode;
}

static T_GduReturnCode GetMediaPlaybackStatus(T_GduCameraPlaybackStatus *status)
{
    T_GduReturnCode returnCode;

    returnCode = GduPlayback_GetPlaybackStatus(&s_playbackInfo, status);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get playback status error, stat:0x%08llX", returnCode);
        return returnCode;
    }

    status->videoPlayProcess = (uint8_t) (((float) s_playbackInfo.playPosMs / (float) s_playbackInfo.videoLengthMs) *
                                          100);

    USER_LOG_DEBUG("get media playback status %d %d %d %d", status->videoPlayProcess, status->playPosMs,
                   status->videoLengthMs, status->playbackMode);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode StartDownloadNotification(void)
{
    T_GduReturnCode returnCode;
    // T_GduDataChannelBandwidthProportionOfHighspeedChannel bandwidthProportion = {0};

    USER_LOG_DEBUG("media download start notification.");

    // bandwidthProportion.dataStream = 0;
    // bandwidthProportion.videoStream = 0;
    // bandwidthProportion.downloadStream = 100;

    // returnCode = GduHighSpeedDataChannel_SetBandwidthProportion(bandwidthProportion);
    // if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    //     USER_LOG_ERROR("Set bandwidth proportion for high speed channel error, stat:0x%08llX.", returnCode);
    //     return returnCode;
    // }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode StopDownloadNotification(void)
{
    T_GduReturnCode returnCode;
    // T_GduDataChannelBandwidthProportionOfHighspeedChannel bandwidthProportion = {0};

    USER_LOG_DEBUG("media download stop notification.");

    // bandwidthProportion.dataStream = 10;
    // bandwidthProportion.videoStream = 60;
    // bandwidthProportion.downloadStream = 30;

    // returnCode = GduHighSpeedDataChannel_SetBandwidthProportion(bandwidthProportion);
    // if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    //     USER_LOG_ERROR("Set bandwidth proportion for high speed channel error, stat:0x%08llX.", returnCode);
    //     return returnCode;
    // }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *UserCameraMedia_SendVideoTask(void *arg)
{
    int ret;
    T_GduReturnCode returnCode;
    static uint32_t sendVideoStep = 0;
    FILE *fpFile = NULL;
    unsigned long dataLength = 0;
    uint16_t lengthOfDataToBeSent = 0;
    int lengthOfDataHaveBeenSent = 0;
    char *dataBuffer = NULL;
    T_TestPayloadCameraPlaybackCommand playbackCommand = {0};
    uint16_t bufferReadSize = 0;
    char *videoFilePath = NULL;
    char *transcodedFilePath = NULL;
    float frameRate = 1.0f;
    T_TestPayloadCameraVideoFrameInfo *frameInfo = NULL;
    uint32_t frameNumber = 0;
    uint32_t frameCount = 0;
    uint32_t startTimeMs = 0;
    bool sendVideoFlag = true;
    bool sendOneTimeFlag = false;
    T_GduDataChannelState videoStreamState = {0};
    E_GduCameraMode mode = GDU_CAMERA_MODE_SHOOT_PHOTO;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    uint32_t frameBufSize = 0;
    E_GduCameraVideoStreamType videoStreamType;
    char curFileDirPath[GDU_FILE_PATH_SIZE_MAX];
    char tempPath[GDU_FILE_PATH_SIZE_MAX];

    USER_UTIL_UNUSED(arg);

    returnCode = GduUserUtil_GetCurrentFileDirPath(__FILE__, GDU_FILE_PATH_SIZE_MAX, curFileDirPath);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file current path error, stat = 0x%08llX", returnCode);
        exit(1);
    }
    snprintf(tempPath, GDU_FILE_PATH_SIZE_MAX, "%smedia_file/PSDK_0004_ORG.mp4", curFileDirPath);

    videoFilePath = osalHandler->Malloc(GDU_FILE_PATH_SIZE_MAX);
    if (videoFilePath == NULL) {
        USER_LOG_ERROR("malloc memory for video file path fail.");
        exit(1);
    }

    transcodedFilePath = osalHandler->Malloc(GDU_FILE_PATH_SIZE_MAX);
    if (transcodedFilePath == NULL) {
        USER_LOG_ERROR("malloc memory for transcoded file path fail.");
        exit(1);
    }

    frameInfo = osalHandler->Malloc(VIDEO_FRAME_MAX_COUNT * sizeof(T_TestPayloadCameraVideoFrameInfo));
    if (frameInfo == NULL) {
        USER_LOG_ERROR("malloc memory for frame info fail.");
        exit(1);
    }
    memset(frameInfo, 0, VIDEO_FRAME_MAX_COUNT * sizeof(T_TestPayloadCameraVideoFrameInfo));

    returnCode = GduPlayback_StopPlayProcess();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("stop playback and start liveview error: 0x%08llX.", returnCode);
        exit(1);
    }

    while (1) {
        osalHandler->TaskSleepMs(1000 / SEND_VIDEO_TASK_FREQ);
        // response playback command
        if (osalHandler->MutexLock(s_mediaPlayCommandBufferMutex) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("mutex lock error");
            continue;
        }

        bufferReadSize = UtilBuffer_Get(&s_mediaPlayCommandBufferHandler, (uint8_t *) &playbackCommand,
                                        sizeof(T_TestPayloadCameraPlaybackCommand));

        if (osalHandler->MutexUnlock(s_mediaPlayCommandBufferMutex) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("mutex unlock error");
            continue;
        }

        if (bufferReadSize != sizeof(T_TestPayloadCameraPlaybackCommand))
            goto send;

        switch (playbackCommand.command) {
            case TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_STOP:
                snprintf(videoFilePath, GDU_FILE_PATH_SIZE_MAX, "%s", tempPath);
                startTimeMs = 0;
                // sendVideoFlag = false;
                // sendOneTimeFlag = true;
                sendVideoFlag = false;
                sendOneTimeFlag = true;
                break;
            case TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_PAUSE:
                sendVideoFlag = false;
                goto send;
            case TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_START:
                snprintf(videoFilePath, GDU_FILE_PATH_SIZE_MAX, "%s", playbackCommand.path);
                startTimeMs = playbackCommand.timeMs;
                sendVideoFlag = true;
                sendOneTimeFlag = true;
                break;
            default:
                USER_LOG_ERROR("playback command invalid: %d.", playbackCommand.command);
                sendVideoFlag = false;
                goto send;
        }

		USER_LOG_DEBUG("............playbackCommand.command:%d, videoFilePath:%s", playbackCommand.command, videoFilePath);

        // video send preprocess
        returnCode = GduPlayback_VideoFileTranscode(videoFilePath, "h264", transcodedFilePath,
                                                    GDU_FILE_PATH_SIZE_MAX);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("transcode video file error: 0x%08llX.", returnCode);
            continue;
        }

        returnCode = GduPlayback_GetFrameRateOfVideoFile(transcodedFilePath, &frameRate);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get frame rate of video error: 0x%08llX.", returnCode);
            continue;
        }
        else
        {
            USER_LOG_DEBUG("frameRate:%f", frameRate);
        }

        returnCode = GduPlayback_GetFrameInfoOfVideoFile(transcodedFilePath, frameInfo, VIDEO_FRAME_MAX_COUNT,
                                                         &frameCount);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get frame info of video error: 0x%08llX.", returnCode);
            continue;
        }
        // else
        // {
        //     USER_LOG_DEBUG("frameRate:%f", frameRate);
        // }

        returnCode = GduPlayback_GetFrameNumberByTime(frameInfo, frameCount, &frameNumber,
                                                      startTimeMs);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get start frame number error: 0x%08llX.", returnCode);
            continue;
        }

        if (fpFile != NULL)
            fclose(fpFile);

        fpFile = fopen(transcodedFilePath, "rb+");
        if (fpFile == NULL) {
            USER_LOG_ERROR("open video file fail.");
            continue;
        }

send:
        if (fpFile == NULL) {
            USER_LOG_ERROR("open video file fail.");
            continue;
        }

        if (sendVideoFlag != true)
            continue;

        returnCode = GduTest_CameraGetMode(&mode);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            continue;
        }

        returnCode = GduTest_CameraGetVideoStreamType(&videoStreamType);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            continue;
        }

        if (mode == GDU_CAMERA_MODE_PLAYBACK && s_playbackInfo.isInPlayProcess == false) {
            continue;
        }

        if (!USER_UTIL_IS_WORK_TURN(sendVideoStep++, frameRate, SEND_VIDEO_TASK_FREQ))
            continue;

        frameBufSize = frameInfo[frameNumber].size;
        if (videoStreamType == GDU_CAMERA_VIDEO_STREAM_TYPE_H264_GDU_FORMAT) {
            frameBufSize = frameBufSize + VIDEO_FRAME_AUD_LEN;
        }

        dataBuffer = calloc(frameBufSize, 1);
        if (dataBuffer == NULL) {
            USER_LOG_ERROR("malloc fail.");
            goto free;
        }

        ret = fseek(fpFile, frameInfo[frameNumber].positionInFile, SEEK_SET);
        if (ret != 0) {
            USER_LOG_ERROR("fseek fail.");
            goto free;
        }

        dataLength = fread(dataBuffer, 1, frameInfo[frameNumber].size, fpFile);
        if (dataLength != frameInfo[frameNumber].size) {
            USER_LOG_ERROR("read data from video file error.");
        } else {
            static int count = 0;
            if(count++ % 20 == 0)
                USER_LOG_DEBUG("read data from video file success, len = %d B\r\n", dataLength);
        }

        if (videoStreamType == GDU_CAMERA_VIDEO_STREAM_TYPE_H264_GDU_FORMAT) {
            memcpy(&dataBuffer[frameInfo[frameNumber].size], s_frameAudInfo, VIDEO_FRAME_AUD_LEN);
            dataLength = dataLength + VIDEO_FRAME_AUD_LEN;
        }

        lengthOfDataHaveBeenSent = 0;
        while (dataLength - lengthOfDataHaveBeenSent) {
            lengthOfDataToBeSent = USER_UTIL_MIN(DATA_SEND_FROM_VIDEO_STREAM_MAX_LEN,
                                                 dataLength - lengthOfDataHaveBeenSent);
            returnCode = GduPayloadCamera_SendVideoStream((const uint8_t *) dataBuffer + lengthOfDataHaveBeenSent,
                                                          lengthOfDataToBeSent);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("send video stream error: 0x%08llX.", returnCode);
            }
            else
            {
                //USER_LOG_DEBUG("send video stream success len: %d.", lengthOfDataToBeSent);
            }
            lengthOfDataHaveBeenSent += lengthOfDataToBeSent;
        }

        if ((frameNumber++) >= frameCount) {
            USER_LOG_DEBUG("reach file tail.");
            frameNumber = 0;

            if (sendOneTimeFlag == true)
                sendVideoFlag = false;
        }

        // returnCode = GduPayloadCamera_GetVideoStreamState(&videoStreamState);
        // if (returnCode == GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        //     USER_LOG_DEBUG(
        //         "video stream state: realtimeBandwidthLimit: %d, realtimeBandwidthBeforeFlowController: %d, realtimeBandwidthAfterFlowController:%d busyState: %d.",
        //         videoStreamState.realtimeBandwidthLimit, videoStreamState.realtimeBandwidthBeforeFlowController,
        //         videoStreamState.realtimeBandwidthAfterFlowController,
        //         videoStreamState.busyState);
        // } else {
        //     USER_LOG_ERROR("get video stream state error.");
        // }

free:
        free(dataBuffer);
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

//#endif

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
