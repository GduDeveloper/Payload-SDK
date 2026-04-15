/**
 ********************************************************************
 * @file    test_widget_speaker.c
 * @brief
 *
 * @copyright (c) 2018 GDU. All rights reserved.
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
#include "test_widget_speaker.h"
#include "gdu_logger.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "utils/util_misc.h"
#include "utils/util_md5.h"
#include <gdu_aircraft_info.h>

#ifdef OPUS_INSTALLED

#include <opus/opus.h>

#endif

/* Private constants ---------------------------------------------------------*/
#define WIDGET_SPEAKER_TASK_STACK_SIZE          (2048)

/*! Attention: replace your audio device name here. */
#define WIDGET_SPEAKER_USB_AUDIO_DEVICE_NAME    "alsa_output.usb-C-Media_Electronics_Inc._USB_Audio_Device-00.analog-stereo"

#define WIDGET_SPEAKER_AUDIO_OPUS_FILE_NAME     "test_audio.opus"
#define WIDGET_SPEAKER_AUDIO_PCM_FILE_NAME      "test_audio.pcm"

#define WIDGET_SPEAKER_AUDIO_MP3_FILE_NAME     "test_audio.mp3"

#define WIDGET_SPEAKER_TTS_FILE_NAME            "test_tts.txt"
#define WIDGET_SPEAKER_TTS_OUTPUT_FILE_NAME     "tts_audio.wav"
#define WIDGET_SPEAKER_TTS_FILE_MAX_SIZE        (3000)

/* The frame size is hardcoded for this sample code but it doesn't have to be */
#define WIDGET_SPEAKER_AUDIO_OPUS_MAX_PACKET_SIZE          (3 * 1276)
#define WIDGET_SPEAKER_AUDIO_OPUS_MAX_FRAME_SIZE           (6 * 960)
#define WIDGET_SPEAKER_AUDIO_OPUS_SAMPLE_RATE              (16000)
#define WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS                 (1)

#define WIDGET_SPEAKER_AUDIO_OPUS_DECODE_FRAME_SIZE_8KBPS  (40)
#define WIDGET_SPEAKER_AUDIO_OPUS_DECODE_BITRATE_8KBPS     (8000)

/* The speaker initialization parameters */
#define WIDGET_SPEAKER_DEFAULT_VOLUME                (30)
#define EKHO_INSTALLED                               (1)

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/
static T_GduWidgetSpeakerHandler s_speakerHandler = {0};
static T_GduMutexHandle s_speakerMutex = {0};
static T_GduWidgetSpeakerState s_speakerState = {0};
static T_GduTaskHandle s_widgetSpeakerTestThread;

typedef enum 
{
	OPUS = 0,
	MP3 = 1,
}E_AudioType;

static FILE *s_audioFile = NULL;
static FILE *s_ttsFile = NULL;
static bool s_isDecodeFinished = true;

static E_AudioType s_isAudioType = OPUS;

static uint16_t s_decodeBitrate = 0;

/* Private functions declaration ---------------------------------------------*/
static void SetSpeakerState(E_GduWidgetSpeakerState speakerState);
static T_GduReturnCode GetSpeakerState(T_GduWidgetSpeakerState *speakerState);
static T_GduReturnCode GetSpeakerParam(T_GduWidgetSpeakerParam *speakerParam);
static T_GduReturnCode SetWorkMode(E_GduWidgetSpeakerWorkMode workMode);
static T_GduReturnCode SetPlayMode(E_GduWidgetSpeakerPlayMode playMode);
static T_GduReturnCode StartPlay(void);
static T_GduReturnCode StopPlay(void);
static T_GduReturnCode SetVolume(uint8_t volume);
static T_GduReturnCode ReceiveTtsData(E_GduWidgetTransmitDataEvent event,
                                      uint32_t offset, uint8_t *buf, uint16_t size);
static T_GduReturnCode ReceiveAudioData(E_GduWidgetTransmitDataEvent event,
                                        uint32_t offset, uint8_t *buf, uint16_t size);

static T_GduReturnCode ReceiveMp3AudioData(E_GduWidgetTransmitDataEvent event,
                                        uint32_t offset, uint8_t *buf, uint16_t size);
static T_GduReturnCode ReceiveRealTimeAudioData(E_GduWidgetTransmitDataEvent event,
                                        uint32_t offset, uint8_t *buf, uint16_t size);

#ifdef SYSTEM_ARCH_LINUX
static void *GduTest_WidgetSpeakerTask(void *arg);
static uint32_t GduTest_GetVoicePlayProcessId(void);
static uint32_t GduTest_KillVoicePlayProcess(uint32_t pid);
static T_GduReturnCode GduTest_DecodeAudioData(void);
static T_GduReturnCode GduTest_PlayAudioData(void);

static T_GduReturnCode GduTest_PlayMp3AudioData(void);

static T_GduReturnCode GduTest_PlayTtsData(void);
static T_GduReturnCode GduTest_CheckFileMd5Sum(const char *path, uint8_t *buf, uint16_t size);
#endif

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode GduTest_WidgetSpeakerStartService(void)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    s_speakerHandler.GetSpeakerState = GetSpeakerState;
    s_speakerHandler.SetWorkMode = SetWorkMode;
    s_speakerHandler.StartPlay = StartPlay;
    s_speakerHandler.StopPlay = StopPlay;
    s_speakerHandler.SetPlayMode = SetPlayMode;
    s_speakerHandler.SetVolume = SetVolume;
    s_speakerHandler.ReceiveTtsData = ReceiveTtsData;
    s_speakerHandler.ReceiveVoiceData = ReceiveAudioData;

    s_speakerHandler.ReceiveMp3VoiceData = ReceiveMp3AudioData;
    s_speakerHandler.ReceiveRealTimeVoiceData = ReceiveRealTimeAudioData;
    s_speakerHandler.GetSpeakerParam = GetSpeakerParam;

    returnCode = osalHandler->MutexCreate(&s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Create speaker mutex error: 0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduWidget_RegSpeakerHandler(&s_speakerHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Register speaker handler error: 0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_speakerState.state = GDU_WIDGET_SPEAKER_STATE_IDEL;
    s_speakerState.workMode = GDU_WIDGET_SPEAKER_WORK_MODE_VOICE;
    //s_speakerState.workMode = GDU_WIDGET_SPEAKER_WORK_MODE_TTS;
    s_speakerState.playMode = GDU_WIDGET_SPEAKER_PLAY_MODE_SINGLE_PLAY;

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    returnCode = SetVolume(WIDGET_SPEAKER_DEFAULT_VOLUME);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set speaker volume error: 0x%08llX", returnCode);
        return returnCode;
    }

#ifdef SYSTEM_ARCH_LINUX
    if (osalHandler->TaskCreate("user_widget_speaker_task", GduTest_WidgetSpeakerTask, WIDGET_SPEAKER_TASK_STACK_SIZE,
                                NULL,
                                &s_widgetSpeakerTestThread) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Gdu widget speaker test task create error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
#endif

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/
#ifdef SYSTEM_ARCH_LINUX
static uint32_t GduTest_GetVoicePlayProcessId(void)
{
    FILE *fp;
    char cmdStr[128];
    uint32_t pid;
    int ret;

    snprintf(cmdStr, 128, "pgrep ffplay");
    fp = popen(cmdStr, "r");
    if (fp == NULL) {
        USER_LOG_ERROR("fp is null.");
        return 0;
    }

    ret = fscanf(fp, "%u", &pid);
    if (ret <= 0) {
        pid = 0;
        goto out;
    }

out:
    pclose(fp);

    return pid;
}

static uint32_t GduTest_KillVoicePlayProcess(uint32_t pid)
{
    FILE *fp;
    char cmdStr[128];

    snprintf(cmdStr, 128, "kill %d", pid);
    fp = popen(cmdStr, "r");
    if (fp == NULL) {
        USER_LOG_ERROR("fp is null.");
        return 0;
    }

    pclose(fp);

    return pid;
}

static T_GduReturnCode GduTest_DecodeAudioData(void)
{
#ifdef OPUS_INSTALLED
    FILE *fin;
    FILE *fout;
    OpusDecoder *decoder;
    opus_int16 out[WIDGET_SPEAKER_AUDIO_OPUS_MAX_FRAME_SIZE * WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS];
    uint8_t cbits[WIDGET_SPEAKER_AUDIO_OPUS_MAX_PACKET_SIZE];
    int32_t nbBytes;
    int32_t err;

    /*! Attention: you can use "ffmpeg -i xxx.mp3 -ar 16000 -ac 1 out.wav" and use opus-tools to generate opus file for test */
    fin = fopen(WIDGET_SPEAKER_AUDIO_OPUS_FILE_NAME, "r");
    if (fin == NULL) {
        USER_LOG_ERROR("failed to open input file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    fout = fopen(WIDGET_SPEAKER_AUDIO_PCM_FILE_NAME, "w");
    if (fout == NULL) {
        USER_LOG_ERROR("failed to open output file: %s\n", strerror(errno));
        goto open_pcm_audio_failed;
    }

    /* Create a new decoder state. */
    decoder = opus_decoder_create(WIDGET_SPEAKER_AUDIO_OPUS_SAMPLE_RATE, WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS, &err);
    if (err < 0) {
        USER_LOG_ERROR("failed to create decoder: %s\n", opus_strerror(err));
        goto create_decoder_failed;
    }

    while (1) {
        int i;
        unsigned char pcm_bytes[WIDGET_SPEAKER_AUDIO_OPUS_MAX_FRAME_SIZE * WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS * 2];
        int frame_size;

        /* Read a 16 bits/sample audio frame. */
        nbBytes = fread(cbits, 1, s_decodeBitrate / WIDGET_SPEAKER_AUDIO_OPUS_DECODE_BITRATE_8KBPS *
                                  WIDGET_SPEAKER_AUDIO_OPUS_DECODE_FRAME_SIZE_8KBPS, fin);
        if (feof(fin))
            break;

        /* Decode the data. In this example, frame_size will be constant because
           the encoder is using a constant frame size. However, that may not
           be the case for all encoders, so the decoder must always check
           the frame size returned. */
        frame_size = opus_decode(decoder, cbits, nbBytes, out, WIDGET_SPEAKER_AUDIO_OPUS_MAX_FRAME_SIZE, 0);
        if (frame_size < 0) {
            USER_LOG_ERROR("decoder failed: %s\n", opus_strerror(frame_size));
            goto decode_data_failed;
        }

        USER_LOG_DEBUG("decode data to file: %d\r\n", frame_size * WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS);
        /* Convert to little-endian ordering. */
        for (i = 0; i < WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS * frame_size; i++) {
            pcm_bytes[2 * i] = out[i] & 0xFF;
            pcm_bytes[2 * i + 1] = (out[i] >> 8) & 0xFF;
        }
        /* Write the decoded audio to file. */
        fwrite(pcm_bytes, sizeof(short), frame_size * WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS, fout);
    }

    USER_LOG_INFO("Decode Finished...");
    s_isDecodeFinished = true;

decode_data_failed:
    opus_decoder_destroy(decoder);
create_decoder_failed:
    fclose(fout);
open_pcm_audio_failed:
    fclose(fin);
#endif

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduTest_PlayAudioData(void)
{
    char cmdStr[128];

    memset(cmdStr, 0, sizeof(cmdStr));
    USER_LOG_INFO("Start Playing...");

    snprintf(cmdStr, sizeof(cmdStr), "ffplay -nodisp -autoexit -ar 16000 -ac 1 -f s16le -i %s 2>/dev/null",
             WIDGET_SPEAKER_AUDIO_PCM_FILE_NAME);

    return GduUserUtil_RunSystemCmd(cmdStr);
}


static T_GduReturnCode GduTest_PlayMp3AudioData(void)
{
    char cmdStr[128];

    memset(cmdStr, 0, sizeof(cmdStr));
    USER_LOG_INFO("Start Playing...");

    //snprintf(cmdStr, sizeof(cmdStr), "ffplay -nodisp -autoexit -ar 16000 -ac 1 -f s16le -i %s 2>/dev/null",WIDGET_SPEAKER_AUDIO_MP3_FILE_NAME);
    snprintf(cmdStr, sizeof(cmdStr), "ffplay -nodisp -autoexit -ar 16000 -ac 1 %s 2>/dev/null",WIDGET_SPEAKER_AUDIO_MP3_FILE_NAME);

	printf("cmdStr: %s\n",cmdStr);

    return GduUserUtil_RunSystemCmd(cmdStr);
}


static T_GduReturnCode GduTest_PlayTtsData(void)
{
    FILE *txtFile;
    uint8_t data[WIDGET_SPEAKER_TTS_FILE_MAX_SIZE] = {0};
    int32_t readLen;
    char cmdStr[WIDGET_SPEAKER_TTS_FILE_MAX_SIZE + 128];
    T_GduAircraftInfoBaseInfo aircraftInfoBaseInfo;
    T_GduReturnCode returnCode;

    returnCode = GduAircraftInfo_GetBaseInfo(&aircraftInfoBaseInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get aircraft base info error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (aircraftInfoBaseInfo.aircraftType == GDU_AIRCRAFT_TYPE_S400) {
        return GduTest_PlayAudioData();
    } else {
        txtFile = fopen(WIDGET_SPEAKER_TTS_FILE_NAME, "r");
        if (txtFile == NULL) {
            USER_LOG_ERROR("failed to open input file: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }

        readLen = fread(data, 1, WIDGET_SPEAKER_TTS_FILE_MAX_SIZE - 1, txtFile);
        if (readLen <= 0) {
            USER_LOG_ERROR("Read tts file failed, error code: %d", readLen);
            fclose(txtFile);
            return GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
        }

        data[readLen] = '\0';

        fclose(txtFile);

        USER_LOG_INFO("Read tts file success, len: %d", readLen);
        USER_LOG_INFO("Content: %s", data);

        memset(cmdStr, 0, sizeof(cmdStr));

        SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_IN_TTS_CONVERSION);

#if EKHO_INSTALLED
        /*! Attention: you can use other tts opensource function to convert txt to speech, example used ekho v7.5 */
        snprintf(cmdStr, sizeof(cmdStr), " ekho %s -s 20 -p 20 -a 100 -o %s", data,
                 WIDGET_SPEAKER_TTS_OUTPUT_FILE_NAME);
#else
        USER_LOG_WARN(
        "Ekho is not installed, please visit https://www.eguidedog.net/ekho.php to install it or use other TTS tools to convert audio");
#endif
        GduUserUtil_RunSystemCmd(cmdStr);

        SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_PLAYING);
        USER_LOG_INFO("Start TTS Playing...");
        memset(cmdStr, 0, sizeof(cmdStr));
        snprintf(cmdStr, sizeof(cmdStr), "ffplay -nodisp -autoexit -ar 16000 -ac 1 -f s16le -i %s 2>/dev/null",
                 WIDGET_SPEAKER_TTS_OUTPUT_FILE_NAME);

        return GduUserUtil_RunSystemCmd(cmdStr);
    }
}

static T_GduReturnCode GduTest_CheckFileMd5Sum(const char *path, uint8_t *buf, uint16_t size)
{
    MD5_CTX md5Ctx;
    uint32_t readFileTotalSize = 0;
    uint16_t readLen;
    T_GduReturnCode returnCode;
    uint8_t readBuf[1024];
    uint8_t md5Sum[16] = {0};
    FILE *file = NULL;;

    UtilMd5_Init(&md5Ctx);

    file = fopen(path, "rb");
    if (file == NULL) {
        USER_LOG_ERROR("Open tts file error.");
    }

    while (1) {
        returnCode = fseek(file, readFileTotalSize, SEEK_SET);
        if (returnCode != 0) {
            USER_LOG_INFO("fseek file error");
        }

        readLen = fread(readBuf, 1, sizeof(readBuf), file);
        if (readLen > 0) {
            readFileTotalSize += readLen;
            UtilMd5_Update(&md5Ctx, readBuf, readLen);
        }

        if (feof(file))
            break;
    }

    UtilMd5_Final(&md5Ctx, md5Sum);
    fclose(file);

    if (size == sizeof(md5Sum)) {
        if (memcmp(md5Sum, buf, sizeof(md5Sum)) == 0) {
            USER_LOG_INFO("MD5 sum check success");
            return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
        } else {
            return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }
    } else {
        USER_LOG_ERROR("MD5 sum length error");
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
#endif

static void SetSpeakerState(E_GduWidgetSpeakerState speakerState)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
    }

    s_speakerState.state = speakerState;

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
    }
}

static T_GduReturnCode GetSpeakerState(T_GduWidgetSpeakerState *speakerState)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *speakerState = s_speakerState;

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetSpeakerParam(T_GduWidgetSpeakerParam *speakerParam)
{
	T_GduReturnCode returnCode;
	T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();


	//TODO: put your speaker param to speakerParam

	return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_GduReturnCode SetWorkMode(E_GduWidgetSpeakerWorkMode workMode)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("Set widget speaker work mode: %d", workMode);
    s_speakerState.workMode = workMode;

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetPlayMode(E_GduWidgetSpeakerPlayMode playMode)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("Set widget speaker play mode: %d", playMode);
    s_speakerState.playMode = playMode;

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode StartPlay(void)
{
    uint32_t pid;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

#ifdef SYSTEM_ARCH_LINUX
    pid = GduTest_GetVoicePlayProcessId();
    if (pid != 0) {
        GduTest_KillVoicePlayProcess(pid);
    }
#endif

    osalHandler->TaskSleepMs(5);
    USER_LOG_INFO("Start widget speaker play");
    SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_PLAYING);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode StopPlay(void)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    uint32_t pid;

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("Stop widget speaker play");
    s_speakerState.state = GDU_WIDGET_SPEAKER_STATE_IDEL;

#ifdef SYSTEM_ARCH_LINUX
    pid = GduTest_GetVoicePlayProcessId();
    if (pid != 0) {
        GduTest_KillVoicePlayProcess(pid);
    }
#endif

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetVolume(uint8_t volume)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    char cmdStr[128];
    int32_t ret = 0;
    float realVolume;

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    realVolume = 1.5f * (float) volume;
    s_speakerState.volume = volume;

    USER_LOG_INFO("Set widget speaker volume: %d", volume);

#ifdef PLATFORM_ARCH_x86_64
    snprintf(cmdStr, sizeof(cmdStr), "pactl list | grep %s -q", WIDGET_SPEAKER_USB_AUDIO_DEVICE_NAME);
    ret = system(cmdStr);
    if (ret == GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        memset(cmdStr, 0, sizeof(cmdStr));
        snprintf(cmdStr, sizeof(cmdStr), "pactl set-sink-volume %s %d%%", WIDGET_SPEAKER_USB_AUDIO_DEVICE_NAME,
                 (int32_t) realVolume);

        returnCode = GduUserUtil_RunSystemCmd(cmdStr);
        if (returnCode < 0) {
            USER_LOG_ERROR("Set widget speaker volume error: %d", ret);
        }
    } else {
        USER_LOG_WARN("No audio device found, please add audio device and init speaker volume here.");
    }
#else
    USER_LOG_WARN("No audio device found, please add audio device and init speaker volume here!!!");
#endif

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode ReceiveTtsData(E_GduWidgetTransmitDataEvent event,
                                      uint32_t offset, uint8_t *buf, uint16_t size)
{
    uint16_t writeLen;
    T_GduReturnCode returnCode;

    if (event == GDU_WIDGET_TRANSMIT_DATA_EVENT_START) {
        USER_LOG_INFO("Create tts file.");
#ifdef SYSTEM_ARCH_LINUX
        s_ttsFile = fopen(WIDGET_SPEAKER_TTS_FILE_NAME, "wb");
        if (s_ttsFile == NULL) {
            USER_LOG_ERROR("Open tts file error.");
        }
#endif
        if (s_speakerState.state != GDU_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_TRANSMITTING);
        }
    } else if (event == GDU_WIDGET_TRANSMIT_DATA_EVENT_TRANSMIT) {
        USER_LOG_INFO("Transmit tts file, offset: %d, size: %d", offset, size);
#ifdef SYSTEM_ARCH_LINUX
        if (s_ttsFile != NULL) {
            fseek(s_ttsFile, offset, SEEK_SET);
            writeLen = fwrite(buf, 1, size, s_ttsFile);
            if (writeLen != size) {
                USER_LOG_ERROR("Write tts file error %d", writeLen);
            }
        }
#endif
        if (s_speakerState.state != GDU_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_TRANSMITTING);
        }
    } else if (event == GDU_WIDGET_TRANSMIT_DATA_EVENT_FINISH) {
        USER_LOG_INFO("Close tts file.");
#ifdef SYSTEM_ARCH_LINUX
        if (s_ttsFile != NULL) {
            fclose(s_ttsFile);
        }

        returnCode = GduTest_CheckFileMd5Sum(WIDGET_SPEAKER_TTS_FILE_NAME, buf, size);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("File md5 sum check failed");
        }
#endif
        if (s_speakerState.state != GDU_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_IDEL);
        }
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode ReceiveAudioData(E_GduWidgetTransmitDataEvent event,
                                        uint32_t offset, uint8_t *buf, uint16_t size)
{
    uint16_t writeLen;
    T_GduReturnCode returnCode;

    if (event == GDU_WIDGET_TRANSMIT_DATA_EVENT_START) {
        s_isDecodeFinished = false;
#ifdef SYSTEM_ARCH_LINUX
        USER_LOG_INFO("Create voice file.");
        s_audioFile = fopen(WIDGET_SPEAKER_AUDIO_OPUS_FILE_NAME, "wb");
        if (s_audioFile == NULL) {
            USER_LOG_ERROR("Create tts file error.");
        }
        if (s_speakerState.state != GDU_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_TRANSMITTING);
        }
#endif
    } else if (event == GDU_WIDGET_TRANSMIT_DATA_EVENT_TRANSMIT) {
        USER_LOG_INFO("Transmit voice file, offset: %d, size: %d", offset, size);
#ifdef SYSTEM_ARCH_LINUX
        if (s_audioFile != NULL) {
            fseek(s_audioFile, offset, SEEK_SET);
            writeLen = fwrite(buf, 1, size, s_audioFile);
            if (writeLen != size) {
                USER_LOG_ERROR("Write tts file error %d", writeLen);
            }
        }
#endif
        if (s_speakerState.state != GDU_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_TRANSMITTING);
        }
    } else if (event == GDU_WIDGET_TRANSMIT_DATA_EVENT_FINISH) {
        int ret = 0;
        if (s_audioFile != NULL) {
            USER_LOG_INFO("Close voice file");
            if (fclose(s_audioFile) == EOF) {  // 关闭文件
                USER_LOG_ERROR("Failed to close file");
            }
            else
            {
                s_audioFile = NULL;
            }
        }
#ifdef SYSTEM_ARCH_LINUX
        returnCode = GduTest_CheckFileMd5Sum(WIDGET_SPEAKER_AUDIO_OPUS_FILE_NAME, buf, size);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("File md5 sum check failed");
            return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }
#endif
        if (s_speakerState.state != GDU_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_IDEL);
        }
#ifdef SYSTEM_ARCH_LINUX
        GduTest_DecodeAudioData();
#endif
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode ReceiveRealTimeAudioData(E_GduWidgetTransmitDataEvent event,
                                        uint32_t offset, uint8_t *buf, uint16_t size)
{
    //TODO rev real time voce
    USER_LOG_DEBUG("rev stream data %d bytes. event:%d", size, event);
    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode ReceiveMp3AudioData(E_GduWidgetTransmitDataEvent event,
                                        uint32_t offset, uint8_t *buf, uint16_t size)
{
    uint16_t writeLen;
    T_GduReturnCode returnCode;
    T_GduWidgetTransDataContent transDataContent = {0};

    if (event == GDU_WIDGET_TRANSMIT_DATA_EVENT_START) {
		s_isAudioType = MP3;
        //s_isDecodeFinished = false;
#ifdef SYSTEM_ARCH_LINUX
        USER_LOG_INFO("Create voice file.");
        s_audioFile = fopen(WIDGET_SPEAKER_AUDIO_MP3_FILE_NAME, "wb");
        if (s_audioFile == NULL) {
            USER_LOG_ERROR("Create tts file error.");
        }
        if (s_speakerState.state != GDU_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_TRANSMITTING);
        }
#endif

        memcpy(&transDataContent, buf, size);
        s_decodeBitrate = transDataContent.transDataStartContent.fileDecodeBitrate;
        USER_LOG_INFO("Create mp3 voice file: %s, decoder bitrate: %d.", transDataContent.transDataStartContent.fileName,
                      transDataContent.transDataStartContent.fileDecodeBitrate);
    } else if (event == GDU_WIDGET_TRANSMIT_DATA_EVENT_TRANSMIT) {
        USER_LOG_INFO("Transmit voice file, offset: %d, size: %d", offset, size);
#ifdef SYSTEM_ARCH_LINUX
        if (s_audioFile != NULL) {
            fseek(s_audioFile, offset, SEEK_SET);
            writeLen = fwrite(buf, 1, size, s_audioFile);
            if (writeLen != size) {
                USER_LOG_ERROR("Write tts file error %d", writeLen);
            }
        }
#endif
        if (s_speakerState.state != GDU_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_TRANSMITTING);
        }
    } else if (event == GDU_WIDGET_TRANSMIT_DATA_EVENT_FINISH) {
        USER_LOG_INFO("Close voice file.");
        if (s_audioFile != NULL) {
            fclose(s_audioFile);
        }

#ifdef SYSTEM_ARCH_LINUX

#if 1
        returnCode = GduTest_CheckFileMd5Sum(WIDGET_SPEAKER_AUDIO_MP3_FILE_NAME, buf, size);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("File md5 sum check failed");
            return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }
#endif

#endif
        if (s_speakerState.state != GDU_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(GDU_WIDGET_SPEAKER_STATE_IDEL);
        }

#ifdef SYSTEM_ARCH_LINUX
        //GduTest_DecodeAudioData();
#endif
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


#ifdef SYSTEM_ARCH_LINUX
#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *GduTest_WidgetSpeakerTask(void *arg)
{
    T_GduReturnCode gduReturnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    USER_UTIL_UNUSED(arg);

    while (1) {
        osalHandler->TaskSleepMs(10);

        if (s_speakerState.state == GDU_WIDGET_SPEAKER_STATE_PLAYING) {
            if (s_speakerState.playMode == GDU_WIDGET_SPEAKER_PLAY_MODE_LOOP_PLAYBACK) {
                if (s_speakerState.workMode == GDU_WIDGET_SPEAKER_WORK_MODE_VOICE) {
					if(OPUS == s_isAudioType )
					{
                    	USER_LOG_DEBUG("Waiting opus decoder finished...");
                    	while (s_isDecodeFinished == false) {
                    	    osalHandler->TaskSleepMs(1);
                    	}
                    	gduReturnCode = GduTest_PlayAudioData();
                    	if (gduReturnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    	    USER_LOG_ERROR("Play audio data failed, error: 0x%08llX.", gduReturnCode);
                    	}
					}
					else if(MP3 == s_isAudioType )
					{
                    	gduReturnCode = GduTest_PlayMp3AudioData();
                    	if (gduReturnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    	    USER_LOG_ERROR("Play mp3 audio data failed, error: 0x%08llX.", gduReturnCode);
                    	}
					}
                } else {
                    gduReturnCode = GduTest_PlayTtsData();
                    if (gduReturnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                        USER_LOG_ERROR("Play tts data failed, error: 0x%08llX.", gduReturnCode);
                    }
                }
                osalHandler->TaskSleepMs(1000);
            } else {
                if (s_speakerState.workMode == GDU_WIDGET_SPEAKER_WORK_MODE_VOICE) {
					if(OPUS == s_isAudioType )
					{
						USER_LOG_DEBUG("Waiting opus decoder finished...");
						while (s_isDecodeFinished == false) {
							osalHandler->TaskSleepMs(1);
						}
						gduReturnCode = GduTest_PlayAudioData();
						if (gduReturnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
							USER_LOG_ERROR("Play audio data failed, error: 0x%08llX.", gduReturnCode);
						}
					}
					else if(MP3 == s_isAudioType )
					{
                    	gduReturnCode = GduTest_PlayMp3AudioData();
                    	if (gduReturnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    	    USER_LOG_ERROR("Play mp3 audio data failed, error: 0x%08llX.", gduReturnCode);
                    	}
					}
				} else {
					gduReturnCode = GduTest_PlayTtsData();
					if (gduReturnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
						USER_LOG_ERROR("Play tts data failed, error: 0x%08llX.", gduReturnCode);
					}
				}

				gduReturnCode = osalHandler->MutexLock(s_speakerMutex);
				if (gduReturnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
					USER_LOG_ERROR("lock mutex error: 0x%08llX.", gduReturnCode);
				}

				if (s_speakerState.playMode == GDU_WIDGET_SPEAKER_PLAY_MODE_SINGLE_PLAY) {
					s_speakerState.state = GDU_WIDGET_SPEAKER_STATE_IDEL;
				}

				gduReturnCode = osalHandler->MutexUnlock(s_speakerMutex);
				if (gduReturnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
					USER_LOG_ERROR("unlock mutex error: 0x%08llX.", gduReturnCode);
				}
			}
		}
	}
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif
#endif
/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/

