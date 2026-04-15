/**
 ********************************************************************
 * @file    test_payload_cam_emu_common.c
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
#include "math.h"
#include "test_payload_cam_emu_base.h"
#include "utils/util_misc.h"
#include "gdu_logger.h"
#include "gdu_platform.h"
#include "gdu_payload_camera.h"
#include "gdu_aircraft_info.h"
#include "gdu_gimbal.h"
//#include "gdu_xport.h"
//#include "gimbal_emu/test_payload_gimbal_emu.h"

/* Private constants ---------------------------------------------------------*/
#define PAYLOAD_CAMERA_EMU_TASK_FREQ            (100)
#define PAYLOAD_CAMERA_EMU_TASK_STACK_SIZE      (2048)
#define SDCARD_TOTAL_SPACE_IN_MB                (32 * 1024)
#define SDCARD_PER_PHOTO_SPACE_IN_MB            (4)
#define SDCARD_PER_SECONDS_RECORD_SPACE_IN_MB   (2)
#define ZOOM_OPTICAL_FOCAL_MAX_LENGTH           (300)
#define ZOOM_OPTICAL_FOCAL_MIN_LENGTH           (10)
#define ZOOM_OPTICAL_FOCAL_LENGTH_STEP          (10)
#define ZOOM_OPTICAL_FOCAL_LENGTH_CTRL_STEP     (5)
#define ZOOM_DIGITAL_BASE_FACTOR                (1.0)
#define ZOOM_DIGITAL_STEP_FACTOR                (0.1)
#define ZOOM_DIGITAL_MAX_FACTOR                 (6.0)
#define FOCUS_MAX_RINGVALUE                     (1000)
#define FOCUS_MID_RINGVALUE                     (500)
#define IMAGE_SENSOR_X_SIZE                     (88.0f) // unit: 0.1mm
#define IMAGE_SENSOR_Y_SIZE                     (66.0f) // unit: 0.1mm
#define CENTER_POINT_IN_SCREEN_X_VALUE          (0.5f)
#define CENTER_POINT_IN_SCREEN_Y_VALUE          (0.5f)
#define TAP_ZOOM_DURATION                       (2000) // unit: ms
#define INTERVAL_PHOTOGRAPH_ALWAYS_COUNT        (255)
#define INTERVAL_PHOTOGRAPH_INTERVAL_INIT_VALUE (1)    // unit: s
#define TAKING_PHOTO_SPENT_TIME_MS_EMU          (500)
#define PHOTO_FILE_PATH                         "../../../../../api_sample/camera_media_emu/media_file/GDU_0001_ORG.jpg"

// Attention: you can select which camera function you want to run, default: all on.
#define USER_CAMERA_EMU_METERING_ON                1
#define USER_CAMERA_EMU_FOCUS_ON                   1
#define USER_CAMERA_EMU_HYBRID_ZOOM_ON             1
#define USER_CAMERA_EMU_TAP_ZOOM_ON                1

/* Private types -------------------------------------------------------------*/
typedef struct {
    E_GduGimbalRotationMode rotationMode;
    T_GduGimbalRotationProperty rotationProperty;
    T_GduAttitude3d rotationValue;
} T_TestCameraGimbalRotationArgument;

/* Private variables ---------------------------------------------------------*/
static bool s_isCamInited = false;

static T_GduCameraCommonHandler s_commonHandler;
static T_GduCameraExposureMeteringHandler s_exposureMeteringHandler;
static T_GduCameraFocusHandler s_focusHandler;
static T_GduCameraDigitalZoomHandler s_digitalZoomHandler;
static T_GduCameraOpticalZoomHandler s_opticalZoomHandler;
static T_GduCameraTapZoomHandler s_tapZoomHandler;

static T_GduTaskHandle s_userCameraThread;

static T_GduCameraSystemState s_cameraState = {0};
static E_GduCameraShootPhotoMode s_cameraShootPhotoMode = GDU_CAMERA_SHOOT_PHOTO_MODE_SINGLE;
static E_GduCameraBurstCount s_cameraBurstCount = GDU_CAMERA_BURST_COUNT_2;
static T_GduCameraPhotoTimeIntervalSettings s_cameraPhotoTimeIntervalSettings = {INTERVAL_PHOTOGRAPH_ALWAYS_COUNT,
                                                                                 INTERVAL_PHOTOGRAPH_INTERVAL_INIT_VALUE};
static T_GduCameraSDCardState s_cameraSDCardState = {0};
static T_GduMutexHandle s_commonMutex = {0};

static E_GduCameraMeteringMode s_cameraMeteringMode = GDU_CAMERA_METERING_MODE_CENTER;
static T_GduCameraSpotMeteringTarget s_cameraSpotMeteringTarget = {0};

static E_GduCameraFocusMode s_cameraFocusMode = GDU_CAMERA_FOCUS_MODE_AUTO;
static T_GduCameraPointInScreen s_cameraFocusTarget = {0};
static uint32_t s_cameraFocusRingValue = FOCUS_MID_RINGVALUE;
static T_GduCameraFocusAssistantSettings s_cameraFocusAssistantSettings = {0};

static E_GduCameraZoomDirection s_cameraZoomDirection = GDU_CAMERA_ZOOM_DIRECTION_OUT;
static E_GduCameraZoomSpeed s_cameraZoomSpeed = GDU_CAMERA_ZOOM_SPEED_NORMAL;
static uint32_t s_cameraOpticalZoomFocalLength = ZOOM_OPTICAL_FOCAL_MIN_LENGTH;
static gdu_f32_t s_cameraDigitalZoomFactor = ZOOM_DIGITAL_BASE_FACTOR;
static bool s_isStartContinuousOpticalZoom = false;
static bool s_isOpticalZoomReachLimit = false;
static T_GduMutexHandle s_zoomMutex = {0};

static bool s_isTapZoomEnabled = false;
static T_GduCameraTapZoomState s_cameraTapZoomState = {0};
static uint8_t s_tapZoomMultiplier = 1;
static uint32_t s_tapZoomStartTime = 0;
static bool s_isStartTapZoom = false;
static bool s_isTapZooming = false;
static T_TestCameraGimbalRotationArgument s_tapZoomNewestGimbalRotationArgument = {0};
static uint32_t s_tapZoomNewestTargetHybridFocalLength = 0; // unit: 0.1mm
static T_GduMutexHandle s_tapZoomMutex = NULL;
static E_GduCameraVideoStreamType s_cameraVideoStreamType;

/* Private functions declaration ---------------------------------------------*/
static T_GduReturnCode GetSystemState(T_GduCameraSystemState *systemState);
static T_GduReturnCode SetMode(E_GduCameraMode mode);
static T_GduReturnCode StartRecordVideo(void);
static T_GduReturnCode StopRecordVideo(void);
static T_GduReturnCode StartShootPhoto(void);
static T_GduReturnCode StopShootPhoto(void);
static T_GduReturnCode SetShootPhotoMode(E_GduCameraShootPhotoMode mode);
static T_GduReturnCode GetShootPhotoMode(E_GduCameraShootPhotoMode *mode);
static T_GduReturnCode SetPhotoBurstCount(E_GduCameraBurstCount burstCount);
static T_GduReturnCode GetPhotoBurstCount(E_GduCameraBurstCount *burstCount);
static T_GduReturnCode SetPhotoTimeIntervalSettings(T_GduCameraPhotoTimeIntervalSettings settings);
static T_GduReturnCode GetPhotoTimeIntervalSettings(T_GduCameraPhotoTimeIntervalSettings *settings);
static T_GduReturnCode GetSDCardState(T_GduCameraSDCardState *sdCardState);
static T_GduReturnCode FormatSDCard(void);
static T_GduReturnCode setPhotoStorageParam(T_GduCameraStoreParam param);

static T_GduReturnCode SetMeteringMode(E_GduCameraMeteringMode mode);
static T_GduReturnCode GetMeteringMode(E_GduCameraMeteringMode *mode);
static T_GduReturnCode SetSpotMeteringTarget(T_GduCameraSpotMeteringTarget target);
static T_GduReturnCode GetSpotMeteringTarget(T_GduCameraSpotMeteringTarget *target);

static T_GduReturnCode SetFocusMode(E_GduCameraFocusMode mode);
static T_GduReturnCode GetFocusMode(E_GduCameraFocusMode *mode);
static T_GduReturnCode SetFocusTarget(T_GduCameraPointInScreen target);
static T_GduReturnCode GetFocusTarget(T_GduCameraPointInScreen *target);
static T_GduReturnCode SetFocusAssistantSettings(T_GduCameraFocusAssistantSettings settings);
static T_GduReturnCode GetFocusAssistantSettings(T_GduCameraFocusAssistantSettings *settings);
static T_GduReturnCode SetFocusRingValue(uint32_t value);
static T_GduReturnCode GetFocusRingValue(uint32_t *value);
static T_GduReturnCode GetFocusRingValueUpperBound(uint32_t *value);

static T_GduReturnCode SetDigitalZoomFactor(gdu_f32_t factor);
static T_GduReturnCode SetOpticalZoomFocalLength(uint32_t focalLength);
static T_GduReturnCode GetOpticalZoomFocalLength(uint32_t *focalLength);
static T_GduReturnCode GetOpticalZoomSpec(T_GduCameraOpticalZoomSpec *spec);
static T_GduReturnCode StartContinuousOpticalZoom(E_GduCameraZoomDirection direction, E_GduCameraZoomSpeed speed);
static T_GduReturnCode StopContinuousOpticalZoom(void);
static T_GduReturnCode GetTapZoomState(T_GduCameraTapZoomState *state);
static T_GduReturnCode SetTapZoomEnabled(bool enabledFlag);
static T_GduReturnCode GetTapZoomEnabled(bool *enabledFlag);
static T_GduReturnCode SetTapZoomMultiplier(uint8_t multiplier);
static T_GduReturnCode GetTapZoomMultiplier(uint8_t *multiplier);
static T_GduReturnCode TapZoomAtTarget(T_GduCameraPointInScreen target);
static T_GduReturnCode GduTest_CameraHybridZoom(uint32_t focalLength);
//static T_GduReturnCode GduTest_CameraRotationGimbal(T_TestCameraGimbalRotationArgument gimbalRotationArgument);

static void *UserCamera_Task(void *arg);

/* Exported functions definition ---------------------------------------------*/

/* Private functions definition-----------------------------------------------*/
static T_GduReturnCode GetSystemState(T_GduCameraSystemState *systemState)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *systemState = s_cameraState;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetMode(E_GduCameraMode mode)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_cameraState.cameraMode = mode;
    USER_LOG_INFO("set camera mode:%d", mode);

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode StartRecordVideo(void)
{
    T_GduReturnCode gduStat;
    T_GduReturnCode returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    gduStat = osalHandler->MutexLock(s_commonMutex);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", gduStat);
        return gduStat;
    }

    if (s_cameraState.isRecording != false) {
        USER_LOG_ERROR("camera is already in recording state");
        returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE;
        goto out;
    }

    s_cameraState.isRecording = true;
    USER_LOG_INFO("start record video");

out:
    gduStat = osalHandler->MutexUnlock(s_commonMutex);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", gduStat);
        return gduStat;
    }

    return returnCode;
}

static T_GduReturnCode StopRecordVideo(void)
{
    T_GduReturnCode gduStat;
    T_GduReturnCode returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    gduStat = osalHandler->MutexLock(s_commonMutex);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", gduStat);
        return gduStat;
    }

    if (s_cameraState.isRecording != true) {
        USER_LOG_ERROR("camera is not in recording state");
        returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE;
        goto out;
    }

    s_cameraState.isRecording = false;
    s_cameraState.currentVideoRecordingTimeInSeconds = 0;
    USER_LOG_INFO("stop record video");

out:
    gduStat = osalHandler->MutexUnlock(s_commonMutex);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", gduStat);
        return gduStat;
    }

    return returnCode;
}

static T_GduReturnCode StartShootPhoto(void)
{
	static uint16_t ShootPhoto = 0;
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

	ShootPhoto ++;

    USER_LOG_INFO("start shoot photo *** %d ",ShootPhoto);
    s_cameraState.isStoring = true;

    if (s_cameraShootPhotoMode == GDU_CAMERA_SHOOT_PHOTO_MODE_SINGLE) {
        s_cameraState.shootingState = GDU_CAMERA_SHOOTING_SINGLE_PHOTO;
    } else if (s_cameraShootPhotoMode == GDU_CAMERA_SHOOT_PHOTO_MODE_BURST) {
        s_cameraState.shootingState = GDU_CAMERA_SHOOTING_BURST_PHOTO;
    } else if (s_cameraShootPhotoMode == GDU_CAMERA_SHOOT_PHOTO_MODE_INTERVAL) {
        s_cameraState.shootingState = GDU_CAMERA_SHOOTING_INTERVAL_PHOTO;
        s_cameraState.isShootingIntervalStart = true;
        s_cameraState.currentPhotoShootingIntervalTimeInSeconds = s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds;
    }

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode StopShootPhoto(void)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("stop shoot photo");
    s_cameraState.shootingState = GDU_CAMERA_SHOOTING_PHOTO_IDLE;
    s_cameraState.isStoring = false;
    s_cameraState.isShootingIntervalStart = false;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetShootPhotoMode(E_GduCameraShootPhotoMode mode)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_cameraShootPhotoMode = mode;
    USER_LOG_INFO("set shoot photo mode:%d", mode);

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetShootPhotoMode(E_GduCameraShootPhotoMode *mode)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *mode = s_cameraShootPhotoMode;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);\
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetPhotoBurstCount(E_GduCameraBurstCount burstCount)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_cameraBurstCount = burstCount;
    USER_LOG_INFO("set photo burst count:%d", burstCount);
    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetPhotoBurstCount(E_GduCameraBurstCount *burstCount)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *burstCount = s_cameraBurstCount;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetPhotoTimeIntervalSettings(T_GduCameraPhotoTimeIntervalSettings settings)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_cameraPhotoTimeIntervalSettings.captureCount = settings.captureCount;
    s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds = settings.timeIntervalSeconds;
    USER_LOG_INFO("set photo interval settings count:%d seconds:%d", settings.captureCount,
                  settings.timeIntervalSeconds);
    s_cameraState.currentPhotoShootingIntervalCount = settings.captureCount;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetPhotoTimeIntervalSettings(T_GduCameraPhotoTimeIntervalSettings *settings)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    memcpy(settings, &s_cameraPhotoTimeIntervalSettings, sizeof(T_GduCameraPhotoTimeIntervalSettings));

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetSDCardState(T_GduCameraSDCardState *sdCardState)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    memcpy(sdCardState, &s_cameraSDCardState, sizeof(T_GduCameraSDCardState));

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode FormatSDCard(void)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("format sdcard");

    memset(&s_cameraSDCardState, 0, sizeof(T_GduCameraSDCardState));
    s_cameraSDCardState.isInserted = true;
    s_cameraSDCardState.totalSpaceInMB = SDCARD_TOTAL_SPACE_IN_MB;
    s_cameraSDCardState.remainSpaceInMB = SDCARD_TOTAL_SPACE_IN_MB;
    s_cameraSDCardState.availableCaptureCount = SDCARD_TOTAL_SPACE_IN_MB / SDCARD_PER_PHOTO_SPACE_IN_MB;
    s_cameraSDCardState.availableRecordingTimeInSeconds =
        SDCARD_TOTAL_SPACE_IN_MB / SDCARD_PER_SECONDS_RECORD_SPACE_IN_MB;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode setPhotoStorageParam(T_GduCameraStoreParam param)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

	//TODO: set photo store directory

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetMeteringMode(E_GduCameraMeteringMode mode)
{
    USER_LOG_INFO("set metering mode:%d", mode);
    s_cameraMeteringMode = mode;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetMeteringMode(E_GduCameraMeteringMode *mode)
{
    *mode = s_cameraMeteringMode;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetSpotMeteringTarget(T_GduCameraSpotMeteringTarget target)
{
    USER_LOG_INFO("set spot metering area col:%d row:%d", target.col, target.row);
    memcpy(&s_cameraSpotMeteringTarget, &target, sizeof(T_GduCameraSpotMeteringTarget));

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetSpotMeteringTarget(T_GduCameraSpotMeteringTarget *target)
{
    memcpy(target, &s_cameraSpotMeteringTarget, sizeof(T_GduCameraSpotMeteringTarget));

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetFocusMode(E_GduCameraFocusMode mode)
{
    USER_LOG_INFO("set focus mode:%d", mode);
    s_cameraFocusMode = mode;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetFocusMode(E_GduCameraFocusMode *mode)
{
    *mode = s_cameraFocusMode;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetFocusTarget(T_GduCameraPointInScreen target)
{
    USER_LOG_INFO("set focus target x:%.2f y:%.2f", target.focusX, target.focusY);
    memcpy(&s_cameraFocusTarget, &target, sizeof(T_GduCameraPointInScreen));

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetFocusTarget(T_GduCameraPointInScreen *target)
{
    memcpy(target, &s_cameraFocusTarget, sizeof(T_GduCameraPointInScreen));

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetFocusAssistantSettings(T_GduCameraFocusAssistantSettings settings)
{
    USER_LOG_INFO("set focus assistant setting MF:%d AF:%d", settings.isEnabledMF, settings.isEnabledAF);
    memcpy(&s_cameraFocusAssistantSettings, &settings, sizeof(T_GduCameraFocusAssistantSettings));

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetFocusAssistantSettings(T_GduCameraFocusAssistantSettings *settings)
{
    memcpy(settings, &s_cameraFocusAssistantSettings, sizeof(T_GduCameraFocusAssistantSettings));

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetFocusRingValue(uint32_t value)
{
    USER_LOG_INFO("set focus ring value:%d", value);
    s_cameraFocusRingValue = value;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetFocusRingValue(uint32_t *value)
{
    *value = s_cameraFocusRingValue;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetFocusRingValueUpperBound(uint32_t *value)
{
    *value = FOCUS_MAX_RINGVALUE;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetDigitalZoomFactor(gdu_f32_t factor)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("set digital zoom factor:%.2f", factor);
    s_cameraDigitalZoomFactor = factor;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetOpticalZoomFocalLength(uint32_t focalLength)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("set optical zoom focal length:%d", focalLength);
    s_isOpticalZoomReachLimit = false;
    s_cameraDigitalZoomFactor = ZOOM_DIGITAL_BASE_FACTOR;
    s_cameraOpticalZoomFocalLength = ZOOM_OPTICAL_FOCAL_MIN_LENGTH;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetOpticalZoomFocalLength(uint32_t *focalLength)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *focalLength = s_cameraOpticalZoomFocalLength;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetOpticalZoomSpec(T_GduCameraOpticalZoomSpec *spec)
{
    spec->maxFocalLength = ZOOM_OPTICAL_FOCAL_MAX_LENGTH;
    spec->minFocalLength = ZOOM_OPTICAL_FOCAL_MIN_LENGTH;
    spec->focalLengthStep = ZOOM_OPTICAL_FOCAL_LENGTH_STEP;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode StartContinuousOpticalZoom(E_GduCameraZoomDirection direction, E_GduCameraZoomSpeed speed)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    //USER_LOG_INFO("start continuous optical zoom direction:%d speed:%d", direction, speed);
    USER_LOG_INFO("start continuous optical zoom direction:%d speed:%d length:%d", direction, speed,s_cameraOpticalZoomFocalLength );
    s_isStartContinuousOpticalZoom = true;
    s_cameraZoomDirection = direction;
    s_cameraZoomSpeed = speed;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode StopContinuousOpticalZoom(void)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("stop continuous optical zoom");
    s_isStartContinuousOpticalZoom = false;
    s_cameraZoomDirection = GDU_CAMERA_ZOOM_DIRECTION_OUT;
    s_cameraZoomSpeed = GDU_CAMERA_ZOOM_SPEED_NORMAL;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetTapZoomState(T_GduCameraTapZoomState *state)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_tapZoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    memcpy(state, &s_cameraTapZoomState, sizeof(T_GduCameraTapZoomState));

    returnCode = osalHandler->MutexUnlock(s_tapZoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetTapZoomEnabled(bool enabledFlag)
{
    USER_LOG_INFO("set tap zoom enabled flag: %d.", enabledFlag);
    s_isTapZoomEnabled = enabledFlag;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetTapZoomEnabled(bool *enabledFlag)
{
    *enabledFlag = s_isTapZoomEnabled;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode SetTapZoomMultiplier(uint8_t multiplier)
{
    USER_LOG_INFO("set tap zoom multiplier: %d.", multiplier);
    s_tapZoomMultiplier = multiplier;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GetTapZoomMultiplier(uint8_t *multiplier)
{
    *multiplier = s_tapZoomMultiplier;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode TapZoomAtTarget(T_GduCameraPointInScreen target)
{
    T_GduReturnCode returnCode;
    E_GduGimbalRotationMode rotationMode;
    T_GduGimbalRotationProperty rotationProperty = {0};
    T_GduAttitude3d rotationValue = {0};
    float hybridFocalLength = 0; // unit: 0.1mm
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    USER_LOG_INFO("tap zoom at target: x %f, y %f.", target.focusX, target.focusY);

    if (s_isTapZoomEnabled != true) {
        USER_LOG_WARN("tap zoom is not enabled.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (s_isTapZooming || s_isStartTapZoom) {
        USER_LOG_WARN("The last tap zoom process is not over.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE;
    }

    rotationMode = GDU_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE;
    rotationProperty.relativeAngleRotation.actionTime = TAP_ZOOM_DURATION / 10;

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    /* Calculation formula: rotation angle = arctan((coordinate of target in sensor - coordinate of center point in
     * sensor) / hybrid focal length). Here, suppose that images of all pixels of sensor are displayed in screen,
     * and that center of the image sensor coincides with center of rotation of the gimbal, and that optical axis of
     * camera coincides with x-axis of gimbal. */
    hybridFocalLength = (gdu_f32_t) s_cameraOpticalZoomFocalLength * s_cameraDigitalZoomFactor;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    rotationValue.pitch = (int32_t) (
        atan2f((target.focusY - CENTER_POINT_IN_SCREEN_Y_VALUE) * IMAGE_SENSOR_Y_SIZE, hybridFocalLength) * 1800 /
        GDU_PI);
    rotationValue.yaw = (int32_t) (
        atan2f((target.focusX - CENTER_POINT_IN_SCREEN_X_VALUE) * IMAGE_SENSOR_X_SIZE, hybridFocalLength) * 1800 /
        GDU_PI);

    returnCode = osalHandler->MutexLock(s_tapZoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_tapZoomNewestGimbalRotationArgument.rotationMode = rotationMode;
    s_tapZoomNewestGimbalRotationArgument.rotationProperty = rotationProperty;
    s_tapZoomNewestGimbalRotationArgument.rotationValue = rotationValue;
    s_tapZoomNewestTargetHybridFocalLength = (uint32_t) (hybridFocalLength * (float) s_tapZoomMultiplier);

    returnCode = osalHandler->MutexUnlock(s_tapZoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_isStartTapZoom = true;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduTest_CameraHybridZoom(uint32_t focalLength)
{
    if (focalLength >= ZOOM_OPTICAL_FOCAL_MIN_LENGTH && focalLength <= ZOOM_OPTICAL_FOCAL_MAX_LENGTH) {
        s_cameraOpticalZoomFocalLength = focalLength;
        s_cameraDigitalZoomFactor = ZOOM_DIGITAL_BASE_FACTOR;
    } else if (focalLength < ZOOM_OPTICAL_FOCAL_MIN_LENGTH) {
        s_cameraOpticalZoomFocalLength = ZOOM_OPTICAL_FOCAL_MIN_LENGTH;
        s_cameraDigitalZoomFactor = ZOOM_DIGITAL_BASE_FACTOR;

        USER_LOG_ERROR("focal length %d is smaller than min optical focal length %d.", focalLength,
                       ZOOM_OPTICAL_FOCAL_MIN_LENGTH);
        return GDU_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
    } else {
        s_cameraOpticalZoomFocalLength = ZOOM_OPTICAL_FOCAL_MAX_LENGTH;
        s_cameraDigitalZoomFactor = (float) focalLength / ZOOM_OPTICAL_FOCAL_MAX_LENGTH;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

#if 0
static T_GduReturnCode GduTest_CameraRotationGimbal(T_TestCameraGimbalRotationArgument gimbalRotationArgument)
{
    T_GduReturnCode returnCode;
    T_GduAircraftInfoBaseInfo aircraftBaseInfo = {0};

    returnCode = GduAircraftInfo_GetBaseInfo(&aircraftBaseInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get aircraft information error: 0x%08llX.", returnCode);
        return returnCode;
    }

    if (aircraftBaseInfo.gduAdapterType == GDU_SDK_ADAPTER_TYPE_SKYPORT_V2) {
        #if 0
        returnCode = GduTest_GimbalRotate(gimbalRotationArgument.rotationMode, gimbalRotationArgument.rotationProperty,
                                          gimbalRotationArgument.rotationValue);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("rotate gimbal error: 0x%08llX.", returnCode);
            return returnCode;
        }
        #endif
            return returnCode;
    } else if (aircraftBaseInfo.gduAdapterType == GDU_SDK_ADAPTER_TYPE_XPORT) {
        returnCode = GduXPort_RotateSync(gimbalRotationArgument.rotationMode, gimbalRotationArgument.rotationProperty,
                                         gimbalRotationArgument.rotationValue);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("rotate XPort error: 0x%08llX.", returnCode);
            return returnCode;
        }
    } else {
        USER_LOG_ERROR("PSDK adapter type is invalid: %d.", aircraftBaseInfo.gduAdapterType);
        return GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
#endif

#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *UserCamera_Task(void *arg)
{
    static uint32_t photoCnt = 0;
    static uint32_t step = 0;
    T_GduReturnCode returnCode;
    int32_t tempFocalLength = 0;
    gdu_f32_t tempDigitalFactor = 0.0f;
    uint32_t currentTime = 0;
    bool isStartIntervalPhotoAction = false;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    USER_UTIL_UNUSED(arg);

    while (1) {
        osalHandler->TaskSleepMs(1000 / PAYLOAD_CAMERA_EMU_TASK_FREQ);
        step++;

        returnCode = osalHandler->MutexLock(s_commonMutex);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
            continue;
        }

        s_cameraSDCardState.availableRecordingTimeInSeconds =
            s_cameraSDCardState.remainSpaceInMB / SDCARD_PER_SECONDS_RECORD_SPACE_IN_MB;
        s_cameraSDCardState.availableCaptureCount = s_cameraSDCardState.remainSpaceInMB / SDCARD_PER_PHOTO_SPACE_IN_MB;

        //realize your photo shoot and storage logic here
        if (s_cameraState.shootingState != GDU_CAMERA_SHOOTING_PHOTO_IDLE &&
            photoCnt++ > TAKING_PHOTO_SPENT_TIME_MS_EMU / (1000 / PAYLOAD_CAMERA_EMU_TASK_FREQ)) {
            photoCnt = 0;

            //store the photo after shooting finished
            if (s_cameraShootPhotoMode == GDU_CAMERA_SHOOT_PHOTO_MODE_SINGLE) {
                s_cameraSDCardState.remainSpaceInMB =
                    s_cameraSDCardState.remainSpaceInMB - SDCARD_PER_PHOTO_SPACE_IN_MB;
                s_cameraState.isStoring = false;
                s_cameraState.shootingState = GDU_CAMERA_SHOOTING_PHOTO_IDLE;
            } else if (s_cameraShootPhotoMode == GDU_CAMERA_SHOOT_PHOTO_MODE_BURST) {
                s_cameraSDCardState.remainSpaceInMB =
                    s_cameraSDCardState.remainSpaceInMB - SDCARD_PER_PHOTO_SPACE_IN_MB * s_cameraBurstCount;
                s_cameraState.isStoring = false;
                s_cameraState.shootingState = GDU_CAMERA_SHOOTING_PHOTO_IDLE;
            } else if (s_cameraShootPhotoMode == GDU_CAMERA_SHOOT_PHOTO_MODE_INTERVAL) {
                if (isStartIntervalPhotoAction == true) {
                    s_cameraState.isStoring = false;
                    s_cameraState.shootingState = GDU_CAMERA_SHOOTING_PHOTO_IDLE;
                    s_cameraSDCardState.remainSpaceInMB =
                        s_cameraSDCardState.remainSpaceInMB - SDCARD_PER_PHOTO_SPACE_IN_MB;
                }
            }

            //check the remain space of sdcard
            if (s_cameraSDCardState.remainSpaceInMB > SDCARD_TOTAL_SPACE_IN_MB) {
                s_cameraSDCardState.remainSpaceInMB = 0;
                s_cameraSDCardState.isFull = true;
            }

#if gdu_ARCH_SYS_LINUX
            T_GduCameraMediaFileInfo mediaFileInfo = {0};

            // push added media file information
            if (s_cameraShootPhotoMode == gdu_CAMERA_SHOOT_PHOTO_MODE_SINGLE) {
                if (UtilFile_IsFileExist(PHOTO_FILE_PATH) == true) {
                    psdkStat = PsdkTest_CameraMediaGetFileInfo(PHOTO_FILE_PATH, &mediaFileInfo);
                    if (psdkStat != gdu_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                        PsdkLogger_UserLogError("Get media file info error 0x%08llX", psdkStat);
                    }

                    psdkStat = PsdkPayloadCamera_PushAddedMediaFileInfo(PHOTO_FILE_PATH, mediaFileInfo);
                    if (psdkStat != gdu_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                        PsdkLogger_UserLogError("Push added media file info error 0x%08llX", psdkStat);
                    }
                } else {
                    PsdkLogger_UserLogWarn("Can't found the media file by path. "
                                           "Probably because media file has not existed. "
                                           "Please check media file if exists. ");
                }
            }
#endif
        }

        returnCode = osalHandler->MutexUnlock(s_commonMutex);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
            continue;
        }

        // 10Hz
        if (USER_UTIL_IS_WORK_TURN(step, 10, PAYLOAD_CAMERA_EMU_TASK_FREQ)) {
            returnCode = osalHandler->MutexLock(s_zoomMutex);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
                continue;
            }

#ifdef USER_CAMERA_EMU_HYBRID_ZOOM_ON
            //Add logic here for zoom camera
            if (s_isStartContinuousOpticalZoom == true) {
                tempDigitalFactor = s_cameraDigitalZoomFactor;
                tempFocalLength = (int32_t) s_cameraOpticalZoomFocalLength;
                if (s_isOpticalZoomReachLimit == false) {
                    if (s_cameraZoomDirection == GDU_CAMERA_ZOOM_DIRECTION_IN) {
                        tempFocalLength += ((int) s_cameraZoomSpeed - GDU_CAMERA_ZOOM_SPEED_SLOWEST + 1) *
                                           ZOOM_OPTICAL_FOCAL_LENGTH_CTRL_STEP;
                    } else if (s_cameraZoomDirection == GDU_CAMERA_ZOOM_DIRECTION_OUT) {
                        tempFocalLength -= ((int) s_cameraZoomSpeed - GDU_CAMERA_ZOOM_SPEED_SLOWEST + 1) *
                                           ZOOM_OPTICAL_FOCAL_LENGTH_CTRL_STEP;
                    }

                    if (tempFocalLength > ZOOM_OPTICAL_FOCAL_MAX_LENGTH) {
                        s_isOpticalZoomReachLimit = true;
                        tempFocalLength = ZOOM_OPTICAL_FOCAL_MAX_LENGTH;
                    }

                    if (tempFocalLength < ZOOM_OPTICAL_FOCAL_MIN_LENGTH) {
                        tempFocalLength = ZOOM_OPTICAL_FOCAL_MIN_LENGTH;
                    }
                } else {
                    if (s_cameraZoomDirection == GDU_CAMERA_ZOOM_DIRECTION_IN) {
                        tempDigitalFactor += (gdu_f32_t) ZOOM_DIGITAL_STEP_FACTOR;
                    } else if (s_cameraZoomDirection == GDU_CAMERA_ZOOM_DIRECTION_OUT) {
                        tempDigitalFactor -= (gdu_f32_t) ZOOM_DIGITAL_STEP_FACTOR;
                    }

                    if (tempDigitalFactor > (gdu_f32_t) ZOOM_DIGITAL_MAX_FACTOR) {
                        tempDigitalFactor = (gdu_f32_t) ZOOM_DIGITAL_MAX_FACTOR;
                    }

                    if (tempDigitalFactor < (gdu_f32_t) ZOOM_DIGITAL_BASE_FACTOR) {
                        s_isOpticalZoomReachLimit = false;
                        tempDigitalFactor = ZOOM_DIGITAL_BASE_FACTOR;
                    }
                }
                s_cameraOpticalZoomFocalLength = (uint16_t) tempFocalLength;
                s_cameraDigitalZoomFactor = tempDigitalFactor;
            }
#endif

#if USER_CAMERA_EMU_TAP_ZOOM_ON
            returnCode = osalHandler->MutexLock(s_tapZoomMutex);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
                goto out;
            }

            if (s_isStartTapZoom) {
                s_isStartTapZoom = false;
                s_isTapZooming = true;

                returnCode = osalHandler->GetTimeMs(&s_tapZoomStartTime);
                if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    USER_LOG_ERROR("get start time error: 0x%08llX.", returnCode);
                }

#if 0
                returnCode = GduTest_CameraRotationGimbal(s_tapZoomNewestGimbalRotationArgument);
                if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
                    USER_LOG_ERROR("rotate gimbal error: 0x%08llX.", returnCode);
                else
                    s_cameraTapZoomState.isGimbalMoving = true;
                    #endif

                returnCode = GduTest_CameraHybridZoom(s_tapZoomNewestTargetHybridFocalLength);
                if (returnCode == GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    s_cameraTapZoomState.zoomState = (gdu_f32_t) s_tapZoomNewestTargetHybridFocalLength >
                                                     ((gdu_f32_t) s_cameraOpticalZoomFocalLength *
                                                      s_cameraDigitalZoomFactor)
                                                     ? GDU_CAMERA_TAP_ZOOM_STATE_ZOOM_IN
                                                     : GDU_CAMERA_TAP_ZOOM_STATE_ZOOM_OUT;
                } else if (returnCode == GDU_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE) {
                    USER_LOG_ERROR("hybrid zoom focal length beyond limit.");
                    s_cameraTapZoomState.zoomState = GDU_CAMERA_TAP_ZOOM_STATE_ZOOM_LIMITED;
                } else {
                    USER_LOG_ERROR("hybrid zoom error: 0x%08llX.", returnCode);
                }
            } else if (s_isTapZooming) {
                returnCode = osalHandler->GetTimeMs(&currentTime);
                if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    USER_LOG_ERROR("get start time error: 0x%08llX.", returnCode);
                }

                if ((currentTime - s_tapZoomStartTime) >= TAP_ZOOM_DURATION) {
                    s_cameraTapZoomState.zoomState = GDU_CAMERA_TAP_ZOOM_STATE_IDLE;
                    s_cameraTapZoomState.isGimbalMoving = false;
                    s_isTapZooming = false;
                }
            }

            returnCode = osalHandler->MutexUnlock(s_tapZoomMutex);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
                goto out;
            }
#endif

out:
            returnCode = osalHandler->MutexUnlock(s_zoomMutex);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
                continue;
            }
        }

        // 1Hz
        if (USER_UTIL_IS_WORK_TURN(step, 1, PAYLOAD_CAMERA_EMU_TASK_FREQ)) {
            returnCode = osalHandler->MutexLock(s_commonMutex);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
                continue;
            }

            if (s_cameraState.isRecording) {
                s_cameraState.currentVideoRecordingTimeInSeconds++;
                s_cameraSDCardState.remainSpaceInMB =
                    s_cameraSDCardState.remainSpaceInMB - SDCARD_PER_SECONDS_RECORD_SPACE_IN_MB;
                if (s_cameraSDCardState.remainSpaceInMB > SDCARD_TOTAL_SPACE_IN_MB) {
                    s_cameraSDCardState.remainSpaceInMB = 0;
                    s_cameraSDCardState.isFull = true;
                }
            }

            if (s_cameraState.isShootingIntervalStart == false) {
                isStartIntervalPhotoAction = false;
            }

            if (s_cameraShootPhotoMode == GDU_CAMERA_SHOOT_PHOTO_MODE_INTERVAL
                && s_cameraState.isShootingIntervalStart == true && s_cameraPhotoTimeIntervalSettings.captureCount > 0
                && s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds > 0) {
                s_cameraState.currentPhotoShootingIntervalTimeInSeconds--;

                if ((s_cameraState.currentPhotoShootingIntervalTimeInSeconds == 0 &&
                     s_cameraState.currentPhotoShootingIntervalCount > 0) ||
                    (s_cameraState.isShootingIntervalStart == true && isStartIntervalPhotoAction == false)) {

                    isStartIntervalPhotoAction = true;

                    s_cameraState.shootingState = GDU_CAMERA_SHOOTING_INTERVAL_PHOTO;
                    s_cameraState.isStoring = true;
                    s_cameraState.currentPhotoShootingIntervalTimeInSeconds
                        = s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds;
                    if (s_cameraState.currentPhotoShootingIntervalCount < INTERVAL_PHOTOGRAPH_ALWAYS_COUNT) {
                        USER_LOG_INFO("interval taking photograph count:%d interval_time:%ds",
                                      (s_cameraPhotoTimeIntervalSettings.captureCount -
                                       s_cameraState.currentPhotoShootingIntervalCount + 1),
                                      s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds);
                        s_cameraState.currentPhotoShootingIntervalCount--;
                        if (s_cameraState.currentPhotoShootingIntervalCount == 0) {
                            s_cameraState.shootingState = GDU_CAMERA_SHOOTING_PHOTO_IDLE;
                            s_cameraState.isStoring = false;
                            s_cameraState.isShootingIntervalStart = false;
                        }
                    } else {
                        USER_LOG_INFO("interval taking photograph always, interval_time:%ds",
                                      s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds);
                        s_cameraState.currentPhotoShootingIntervalCount--;
                    }
                }
            }

            returnCode = osalHandler->MutexUnlock(s_commonMutex);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
                continue;
            }
        }
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

/* Private functions definition-----------------------------------------------*/
T_GduReturnCode GduTest_CameraEmuBaseStartService(void)
{
    T_GduReturnCode returnCode;
    char ipAddr[GDU_IP_ADDR_STR_SIZE_MAX] = {0};
    uint16_t port = 0;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    T_GduAircraftInfoBaseInfo aircraftInfoBaseInfo = {0};

    returnCode = osalHandler->MutexCreate(&s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("create mutex used to lock tap zoom arguments error: 0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = osalHandler->MutexCreate(&s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("create mutex used to lock tap zoom arguments error: 0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = osalHandler->MutexCreate(&s_tapZoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("create mutex used to lock tap zoom arguments error: 0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduPayloadCamera_Init();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("payload camera init error:0x%08llX", returnCode);
    }

    /* Init the SDcard parameters */
    s_cameraSDCardState.isInserted = true;
    s_cameraSDCardState.totalSpaceInMB = SDCARD_TOTAL_SPACE_IN_MB;
    s_cameraSDCardState.remainSpaceInMB = SDCARD_TOTAL_SPACE_IN_MB;
    s_cameraSDCardState.availableCaptureCount = SDCARD_TOTAL_SPACE_IN_MB / SDCARD_PER_PHOTO_SPACE_IN_MB;
    s_cameraSDCardState.availableRecordingTimeInSeconds =
        SDCARD_TOTAL_SPACE_IN_MB / SDCARD_PER_SECONDS_RECORD_SPACE_IN_MB;

    /* Register the camera common handler */
    s_commonHandler.GetSystemState = GetSystemState;
    s_commonHandler.SetMode = SetMode;
    s_commonHandler.GetMode = GduTest_CameraGetMode;
    s_commonHandler.StartRecordVideo = StartRecordVideo;
    s_commonHandler.StopRecordVideo = StopRecordVideo;
    s_commonHandler.StartShootPhoto = StartShootPhoto;
    s_commonHandler.StopShootPhoto = StopShootPhoto;
    s_commonHandler.SetShootPhotoMode = SetShootPhotoMode;
    s_commonHandler.GetShootPhotoMode = GetShootPhotoMode;
    s_commonHandler.SetPhotoBurstCount = SetPhotoBurstCount;
    s_commonHandler.GetPhotoBurstCount = GetPhotoBurstCount;
    s_commonHandler.SetPhotoTimeIntervalSettings = SetPhotoTimeIntervalSettings;
    s_commonHandler.GetPhotoTimeIntervalSettings = GetPhotoTimeIntervalSettings;
    s_commonHandler.GetSDCardState = GetSDCardState;
    s_commonHandler.FormatSDCard = FormatSDCard;
    s_commonHandler.setPhotoStorageParam = setPhotoStorageParam;

    returnCode = GduPayloadCamera_RegCommonHandler(&s_commonHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register common handler error:0x%08llX", returnCode);
    }

    /* Register the camera metering handler */
#if USER_CAMERA_EMU_METERING_ON //相机测光
    s_exposureMeteringHandler.SetMeteringMode = SetMeteringMode;
    s_exposureMeteringHandler.GetMeteringMode = GetMeteringMode;
    s_exposureMeteringHandler.SetSpotMeteringTarget = SetSpotMeteringTarget;
    s_exposureMeteringHandler.GetSpotMeteringTarget = GetSpotMeteringTarget;

    returnCode = GduPayloadCamera_RegExposureMeteringHandler(&s_exposureMeteringHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register exposure metering handler error:0x%08llX", returnCode);
        return returnCode;
    }
#endif

    /* Register the camera focus handler */
#if USER_CAMERA_EMU_FOCUS_ON //相机对焦
    s_focusHandler.SetFocusMode = SetFocusMode;
    s_focusHandler.GetFocusMode = GetFocusMode;
    s_focusHandler.SetFocusTarget = SetFocusTarget;
    s_focusHandler.GetFocusTarget = GetFocusTarget;
    s_focusHandler.SetFocusAssistantSettings = SetFocusAssistantSettings;
    s_focusHandler.GetFocusAssistantSettings = GetFocusAssistantSettings;
    s_focusHandler.SetFocusRingValue = SetFocusRingValue;
    s_focusHandler.GetFocusRingValue = GetFocusRingValue;
    s_focusHandler.GetFocusRingValueUpperBound = GetFocusRingValueUpperBound;

    returnCode = GduPayloadCamera_RegFocusHandler(&s_focusHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register adjustable focal point handler error:0x%08llX", returnCode);
        return returnCode;
    }
#endif

    /* Register the camera digital zoom handler */
#if USER_CAMERA_EMU_HYBRID_ZOOM_ON //摄像机数码变焦处理
    s_digitalZoomHandler.SetDigitalZoomFactor = SetDigitalZoomFactor;
    s_digitalZoomHandler.GetDigitalZoomFactor = GduTest_CameraGetDigitalZoomFactor;

    returnCode = GduPayloadCamera_RegDigitalZoomHandler(&s_digitalZoomHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register digital zoom handler error:0x%08llX", returnCode);
        return returnCode;
    }

    /* Register the camera optical zoom handler */
    s_opticalZoomHandler.SetOpticalZoomFocalLength = SetOpticalZoomFocalLength;//恢复默认值
    s_opticalZoomHandler.GetOpticalZoomFocalLength = GetOpticalZoomFocalLength;//设置光学变焦焦距
    s_opticalZoomHandler.GetOpticalZoomFactor = GduTest_CameraGetOpticalZoomFactor;//获取变倍因数
    s_opticalZoomHandler.GetOpticalZoomSpec = GetOpticalZoomSpec;//获取变焦范围和步长参数
    s_opticalZoomHandler.StartContinuousOpticalZoom = StartContinuousOpticalZoom;//启动变焦
    s_opticalZoomHandler.StopContinuousOpticalZoom = StopContinuousOpticalZoom;//停止变焦

    returnCode = GduPayloadCamera_RegOpticalZoomHandler(&s_opticalZoomHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register optical zoom handler error:0x%08llX", returnCode);
        return returnCode;
    }
#endif

    /* Register the camera tap zoom handler */
#if USER_CAMERA_EMU_TAP_ZOOM_ON //指点变焦
    s_tapZoomHandler.GetTapZoomState = GetTapZoomState;
    s_tapZoomHandler.SetTapZoomEnabled = SetTapZoomEnabled;
    s_tapZoomHandler.GetTapZoomEnabled = GetTapZoomEnabled;
    s_tapZoomHandler.SetTapZoomMultiplier = SetTapZoomMultiplier;
    s_tapZoomHandler.GetTapZoomMultiplier = GetTapZoomMultiplier;
    s_tapZoomHandler.TapZoomAtTarget = TapZoomAtTarget;

    returnCode = GduPayloadCamera_RegTapZoomHandler(&s_tapZoomHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register tap zoom handler error:0x%08llX", returnCode);
        return returnCode;
    }
#endif

    /* Create the camera emu taskHandle */
    if (osalHandler->TaskCreate("user_camera_task", UserCamera_Task,
                                PAYLOAD_CAMERA_EMU_TASK_STACK_SIZE, NULL, &s_userCameraThread)
        != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("user camera taskHandle create error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (GduAircraftInfo_GetBaseInfo(&aircraftInfoBaseInfo) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get aircraft information error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    returnCode = GduPayloadCamera_SetVideoStreamType(GDU_CAMERA_VIDEO_STREAM_TYPE_H264_GDU_FORMAT);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("GDU camera set video stream error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    s_cameraVideoStreamType = GDU_CAMERA_VIDEO_STREAM_TYPE_H264_GDU_FORMAT;

    returnCode = GduPayloadCamera_GetVideoStreamRemoteAddress(ipAddr, &port);
    if (returnCode == GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_DEBUG("Get video stream remote address: %s, port: %d", ipAddr, port);
    } else {
        USER_LOG_ERROR("get video stream remote address error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    s_isCamInited = true;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_CameraGetDigitalZoomFactor(gdu_f32_t *factor)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *factor = s_cameraDigitalZoomFactor;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_CameraGetOpticalZoomFactor(gdu_f32_t *factor)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    //Formula:factor = currentFocalLength / minFocalLength
    *factor = (gdu_f32_t) s_cameraOpticalZoomFocalLength / ZOOM_OPTICAL_FOCAL_MIN_LENGTH;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_CameraGetMode(E_GduCameraMode *mode)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *mode = s_cameraState.cameraMode;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_CameraGetVideoStreamType(E_GduCameraVideoStreamType *type)
{
    if (s_isCamInited == false) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE;
    }

    *type = s_cameraVideoStreamType;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

bool GduTest_CameraIsInited(void)
{
    return s_isCamInited;
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
