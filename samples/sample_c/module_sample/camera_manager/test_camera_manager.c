/**
 ********************************************************************
 * @file    test_camera_manager.c
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
//#include <widget_interaction_test/test_widget_interaction.h>
#include <utils/util_misc.h>
#include "test_camera_manager.h"
#include "gdu_camera_manager.h"
#include "gdu_platform.h"
#include "gdu_logger.h"
#include "gdu_liveview.h"
/* Private constants ---------------------------------------------------------*/
#define TEST_CAMERA_MANAGER_MEDIA_FILE_NAME_MAX_SIZE             32
#define TEST_CAMERA_MANAGER_MEDIA_EXTEND_INFO_MAX_SIZE           128

#define CAMERA_MANAGER_TASK_FREQ         (1)
#define CAMERA_MANAGER_TASK_STACK_SIZE   (2048)
/* Private types -------------------------------------------------------------*/
typedef struct {
    E_GduCameraType cameraType;
    char *cameraTypeStr;
} T_GduTestCameraTypeStr;

/* Private values -------------------------------------------------------------*/
static const T_GduTestCameraTypeStr s_cameraTypeStrList[] = {
    {GDU_CAMERA_TYPE_UNKNOWN, "Unknown"},
    {GDU_CAMERA_TYPE_8K,    "8K camera"},
    {GDU_CAMERA_TYPE_8KC,    "8KC camera"},
    {GDU_CAMERA_TYPE_30X,    "30X camera"},
    {GDU_CAMERA_TYPE_PFL_ONE,    "PFL_ONE camera"},
    {GDU_CAMERA_TYPE_PDL_300G,    "PDL_300G camera"},
    {GDU_CAMERA_TYPE_PDL_1K ,    "PDL_1K camera"},
    {GDU_CAMERA_TYPE_PQL_02 ,    "PQL_02 camera"},
    {GDU_CAMERA_TYPE_SX_DL,    "SX Dual-lens  camera"},
    {GDU_CAMERA_TYPE_SX_TL,    "SX Triple-lens camera"},
    {GDU_CAMERA_TYPE_SX_FL,    "SX Quad-lens camera"},
    {GDU_CAMERA_TYPE_GD_FL,    "GD Quad-lens camera"},
    {GDU_CAMERA_TYPE_GD_1K_FL,    "GD_1K Quad-lens camera"},
    {GDU_CAMERA_TYPE_GD_DL,    "GD Dual-lens camera"},
    {GDU_CAMERA_TYPE_GD_TL,    "GD Triple-lens camera"},
    {GDU_CAMERA_TYPE_PSDK,    "psdk camera"},
};

static FILE *s_downloadMediaFile = NULL;
static T_GduCameraManagerFileList s_meidaFileList;
static T_GduTaskHandle s_userCameraManagerThread;
/* Private functions declaration ---------------------------------------------*/
static uint8_t GduTest_CameraManagerGetCameraTypeIndex(E_GduCameraType cameraType);
static T_GduReturnCode GduTest_CameraManagerMediaDownloadAndDeleteMediaFile(E_GduMountPosition position);
static T_GduReturnCode GduTest_CameraManagerDownloadFileDataCallback(T_GduDownloadFilePacketInfo packetInfo,
                                                                     const uint8_t *data, uint16_t len);

/* Exported functions definition ---------------------------------------------*/
/*! @brief Sample to set EV for camera, using async api
 *
 *  @note In this interface, ev will be got then be set.
 *  In order to use this function, the camera exposure mode should be
 *  set to be GDU_CAMERA_MANAGER_EXPOSURE_MODE_PROGRAM_AUTO,
 *  GDU_CAMERA_MANAGER_EXPOSURE_MODE_SHUTTER_PRIORITY or
 *  GDU_CAMERA_MANAGER_EXPOSURE_MODE_APERTURE_PRIORITY first
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param dataTarget the target exposure mode
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetEV(E_GduMountPosition position,
                                           E_GduCameraManagerExposureCompensation exposureCompensation)
{
    T_GduReturnCode returnCode;
    E_GduCameraManagerExposureCompensation exposureCompensationTemp;

    returnCode = GduCameraManager_GetExposureCompensation(position, &exposureCompensationTemp);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Get mounted position %d exposure compensation failed, error code: 0x%08X.",
                       position, returnCode);
    }

    if (exposureCompensationTemp == exposureCompensation) {
        USER_LOG_INFO("The mount position %d camera's exposure compensation is already what you expected.",
                      position);
        return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }

    returnCode = GduCameraManager_SetExposureCompensation(position, exposureCompensation);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Set mounted position %d camera's exposure compensation %d failed,"
                       "error code: 0x%08X.", position, exposureCompensation, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to set exposure mode for camera, using async api
 *
 *  @note In this interface, exposure will be got then be set.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param dataTarget the target exposure mode
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetExposureMode(E_GduMountPosition position,
                                                     E_GduCameraManagerExposureMode exposureMode)
{
    T_GduReturnCode returnCode;
    E_GduCameraManagerExposureMode exposureModeTemp;

    returnCode = GduCameraManager_GetExposureMode(position, &exposureModeTemp);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Get mounted position %d exposure mode failed, error code: 0x%08X",
                       position, returnCode);
        return returnCode;
    }

    if (exposureModeTemp == exposureMode) {
        USER_LOG_INFO("The mounted position %d camera's exposure mode is already what you expected.",
                      position);
        return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }

    returnCode = GduCameraManager_SetExposureMode(position, exposureMode);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Set mounted position %d camera's exposure mode %d failed, current exposure is %d,"
                       " error code: 0x%08X", position, exposureMode, exposureModeTemp, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to set ISO value for camera, using async api
 *
 *  @note In this interface, ISO will be got then be set.
 *  For the X5, X5R, X4S and X5S, the ISO value can be set for all
 *  modes. For the other cameras, the ISO value can only be set when
 *  the camera exposure mode is in Manual mode
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param dataTarget the target ISO value
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetISO(E_GduMountPosition position,
                                            E_GduCameraManagerISO isoData)
{
    T_GduReturnCode returnCode;
    E_GduCameraManagerISO isoDataTemp;

    returnCode = GduCameraManager_GetISO(position, &isoDataTemp);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Get mounted position %d camera's iso failed, error code: 0x%08X.",
                       position, returnCode);
        return returnCode;
    }

    if (isoDataTemp == isoData) {
        USER_LOG_INFO("The mounted position %d camera's iso is already what you expected.",
                      position);
        return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }

    returnCode = GduCameraManager_SetISO(position, isoData);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Set mounted position %d camera's iso %d failed, "
                       "error code: 0x%08X.", position, isoData, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to set shutter speed for camera, using async api
 *
 *  @note In this interface, shutter speed will be got then be set.
 *  The shutter speed can be set only when the camera exposure mode
 *  is Shutter mode or Manual mode. The shutter speed should not be
 *  set slower than the video frame rate when the camera's mode is
 *  RECORD_VIDEO.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param dataTarget the target shutter speed
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetShutterSpeed(E_GduMountPosition position,
                                                     E_GduCameraManagerShutterSpeed shutterSpeed)
{
    T_GduReturnCode returnCode;
    E_GduCameraManagerShutterSpeed shutterSpeedTemp;

    returnCode = GduCameraManager_GetShutterSpeed(position, &shutterSpeedTemp);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Get mounted position %d camera's shutter speed failed, "
                       "error code: 0x%08X.", position, returnCode);
        return returnCode;
    }

    if (shutterSpeedTemp == shutterSpeed) {
        USER_LOG_INFO("The mounted position %d camera's shutter speed is already what you expected.",
                      position);
        return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }

    returnCode = GduCameraManager_SetShutterSpeed(position, shutterSpeed);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Set mounted position %d camera's shutter speed %d failed, "
                       "error code: 0x%08X.", position, shutterSpeed, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to set shutter aperture value for camera, using async api
 *
 *  @note In this interface, aperture value will be got then be set.
 *  In order to use this function, the exposure must be in MANUAL or APERTURE_PRIORITY.
*   Supported only by the X5, X5R, X4S, X5S camera.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param dataTarget the target aperture value
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetAperture(E_GduMountPosition position,
                                                 E_GduCameraManagerAperture aperture)
{
    T_GduReturnCode returnCode;
    E_GduCameraManagerAperture apertureTemp;

    returnCode = GduCameraManager_GetAperture(position, &apertureTemp);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Get mounted position %d camera's aperture failed, "
                       "error code: 0x%08X.", position, returnCode);
        return returnCode;
    }

    if (apertureTemp == aperture) {
        USER_LOG_INFO("The mounted position %d camera's aperture is already what you expected.",
                      position);
        return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }

    returnCode = GduCameraManager_SetAperture(position, aperture);

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Set mounted position %d camera's aperture %d failed, "
                       "error code: 0x%08X.", position, aperture, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to set focus point for camera, using async api
 *
 *  @note In this interface, focus mode will be set to be AUTO. Then the
 * focus point will be set to be (x, y).
 * When the focus mode is auto, the target point is the focal point.
 * When the focus mode is manual, the target point is the zoom out area
 * if the focus assistant is enabled for the manual mode. Supported only
 * by the X5, X5R, Z3 cameras, Mavic Pro camera, Phantom 4 Pro camera,
 * Mavic 2 Pro, Mavic 2 Zoom Camera, Mavic 2 Enterprise Camera, X5S. "
 * It's should be attention that X4S will keep focus point as (0.5,0.5) "
 * all the time, the setting of focus point to X4S will quickly replaced "
 * by (0.5, 0.5).
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param x the x value of target focus point, 0~1
 *  @param y the y value of target focus point, 0~1
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetFocusPoint(E_GduMountPosition position,
                                                   T_GduCameraManagerFocusPosData focusPoint)
{
    T_GduReturnCode returnCode;

    /*!< set camera focus mode to be CameraModule::FocusMode::AUTO */
    USER_LOG_INFO("Set mounted position %d camera's focus mode to auto mode.",
                  position);
    returnCode = GduCameraManager_SetFocusMode(position, GDU_CAMERA_MANAGER_FOCUS_MODE_AUTO);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Set mounted position %d camera's focus mode(%d) failed,"
                       " error code :0x%08X.", position, GDU_CAMERA_MANAGER_FOCUS_MODE_AUTO,
                       returnCode);
        return returnCode;
    }

    USER_LOG_INFO("Set mounted position %d camera's focus point to (%0.1f, %0.1f).",
                  position, focusPoint.focusX, focusPoint.focusY);
    returnCode = GduCameraManager_SetFocusTarget(position, focusPoint);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Set mounted position %d camera's focus point(%0.1f, %0.1f) failed,"
                       " error code :0x%08X.", position, focusPoint.focusX, focusPoint.focusY,
                       returnCode);
    }

    return returnCode;
}

/*! @brief Sample to set tap-zoom point for camera, using async api
 *
 *  @note In this interface, tap-zoom function will be enable and the
 * multiplier will be set. Then the tap-zoom function will start with the
 * target tap-zoom point (x, y)
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param multiplier the zoom multiplier of each tap zoom
 *  @param x the x value of target tap-zoom point, 0~1
 *  @param y the y value of target tap-zoom point, 0~1
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetTapZoomPoint(E_GduMountPosition position, uint8_t multiplier,
                                                     T_GduCameraManagerTapZoomPosData tapZoomPosData)
{
    T_GduReturnCode returnCode;

    /*!< set camera tap zoom enable parameter to be enable */
    USER_LOG_INFO("Enable mounted position %d camera's tap zoom status.",
                  position);
    returnCode = GduCameraManager_SetTapZoomEnabled(position, true);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Enable mounted position %d camera's tap zoom function failed,"
                       " error code :0x%08X.", position, returnCode);
        return returnCode;
    }

    // /*!< set camera tap zoom multiplier parameter */
    // USER_LOG_INFO("Set mounted position %d camera's tap zoom multiplier to %d x.",
    //               position, multiplier);
    // returnCode = GduCameraManager_SetTapZoomMultiplier(position, multiplier);
    // if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
    //     returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
    //     USER_LOG_ERROR("Set mounted position %d camera's tap zoom multiplier(%d) failed,"
    //                    " error code :0x%08X.", position, multiplier, returnCode);
    //     return returnCode;
    // }

    USER_LOG_INFO("Set mounted position %d camera's tap zoom point to (%f, %f).",
                  position, tapZoomPosData.focusX, tapZoomPosData.focusY);
    returnCode = GduCameraManager_TapZoomAtTarget(position, tapZoomPosData);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Set mounted position %d camera's tap zoom target point(%f ,%f) failed,"
                       " error code :0x%08X.", position, tapZoomPosData.focusX, tapZoomPosData.focusY,
                       returnCode);
    }

    return returnCode;
}

/*! @brief Sample to execute position zoom on camera, using sync api
 *
 *  @note It is only supported by X5, X5R and X5S camera on Osmo with lens
 * Olympus M.Zuiko ED 14-42mm f/3.5-5.6 EZ, Z3 camera, Z30 camera.
 *  @note In this interface, the zoom will set the zoom factor as the your
 * target value.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param factor target zoom factor
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerOpticalZoom(E_GduMountPosition position,
                                                 E_GduCameraZoomDirection zoomDirection,
                                                 gdu_f32_t factor)
{
    T_GduReturnCode returnCode;
    T_GduCameraManagerOpticalZoomParam opticalZoomParam;

    returnCode = GduCameraManager_GetOpticalZoomParam(position, &opticalZoomParam);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Get mounted position %d camera's zoom param failed, error code :0x%08X",
                       position, returnCode);
        return returnCode;
    }

    USER_LOG_INFO("The mounted position %d camera's current optical zoom factor is:%0.1f x, "
                  "max optical zoom factor is :%0.1f x", position, opticalZoomParam.currentOpticalZoomFactor,
                  opticalZoomParam.maxOpticalZoomFactor);

    USER_LOG_INFO("Set mounted position %d camera's zoom factor: %0.1f x.", position, factor);
    returnCode = GduCameraManager_SetOpticalZoomParam(position, zoomDirection, factor);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_INFO("Set mounted position %d camera's zoom factor(%0.1f) failed, error code :0x%08X",
                      position, factor, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to execute continuous zoom on camera, using sync api
 *
 *  @note It is only supported by X5, X5R and X5S camera on Osmo with lens
 * Olympus M.Zuiko ED 14-42mm f/3.5-5.6 EZ, Z3 camera, Z30 camera.
 *  @note In this interface, the zoom will start with the designated direction
 * and speed, and will stop after zoomTimeInSecond second(s).
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param direction the choice of zoom out or zoom in
 *  @param speed zooming speed
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStartContinuousZoom(E_GduMountPosition position,
                                                         E_GduCameraZoomDirection zoomDirection,
                                                         E_GduCameraZoomSpeed zoomSpeed)
{
    T_GduReturnCode returnCode;

    //    USER_LOG_INFO("Mounted position %d camera start continuous optical zoom.\r\n", position);
    returnCode = GduCameraManager_StartContinuousOpticalZoom(position, zoomDirection, zoomSpeed);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Mounted position %d camera start continuous zoom  failed,"
                       " error code :0x%08X.", position, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to stop continuous zoom on camera, using async api
 *
 *  @note It is only supported by X5, X5R and X5S camera on Osmo with lens
 * Olympus M.Zuiko ED 14-42mm f/3.5-5.6 EZ, Z3 camera, Z30 camera.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStopContinuousZoom(E_GduMountPosition position)
{
    T_GduReturnCode returnCode;

//    USER_LOG_INFO("Mounted position %d camera stop continuous optical zoom.\r\n", position);
    returnCode = GduCameraManager_StopContinuousOpticalZoom(position);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Mounted position %d camera stop continuous zoom failed,"
                       " error code :0x%08X", position, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to shoot single photo, using async api
 *
 *  @note In this interface, camera will be set to be the SHOOT_PHOTO mode
 * then start to shoot a single photo.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStartShootSinglePhoto(E_GduMountPosition position)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    /*!< set camera work mode as shoot photo */
    USER_LOG_INFO("Set mounted position %d camera's work mode as shoot-photo mode", position);
    returnCode = GduCameraManager_SetMode(position, GDU_CAMERA_MANAGER_WORK_MODE_SHOOT_PHOTO);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("set mounted position %d camera's work mode as shoot-photo mode failed,"
                       " error code :0x%08X", position, returnCode);
        return returnCode;
    }

    /*!< set shoot-photo mode */
    USER_LOG_INFO("Set mounted position %d camera's shoot photo mode as single-photo mode", position);
    returnCode = GduCameraManager_SetShootPhotoMode(position, GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_AEB);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("set mounted position %d camera's shoot photo mode as single-photo mode failed,"
                       " error code :0x%08X", position, returnCode);
        return returnCode;
    }

    /*! wait the APP change the shoot-photo mode display */
    USER_LOG_INFO("Sleep 0.5s...");
    osalHandler->TaskSleepMs(500);

    /*!< start to shoot single photo */
    USER_LOG_INFO("Mounted position %d camera start to shoot photo", position);
    returnCode = GduCameraManager_StartShootPhoto(position, GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_AEB);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Mounted position %d camera shoot photo failed, "
                       "error code :0x%08X", position, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to shoot burst photo, using async api
 *
 *  @note In this interface, camera will be set to be the SHOOT_PHOTO mode
 * then start to shoot a burst photo.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param count The number of pictures in each burst shooting
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStartShootBurstPhoto(E_GduMountPosition position,
                                                          E_GduCameraBurstCount burstCount)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    /*!< set camera work mode as shoot photo */
    USER_LOG_INFO("set mounted position %d camera's work mode as shoot photo mode.", position);
    returnCode = GduCameraManager_SetMode(position, GDU_CAMERA_MANAGER_WORK_MODE_SHOOT_PHOTO);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("set mounted position %d camera's work mode as shoot photo mode failed,"
                       " error code :0x%08X.", position, returnCode);
        return returnCode;
    }

    /*!< set shoot-photo mode */
    USER_LOG_INFO("Set mounted position %d camera's shoot photo mode as burst-photo mode", position);
    returnCode = GduCameraManager_SetShootPhotoMode(position, GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_BURST);
    if (returnCode == GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        return returnCode;
    }

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("set mounted position %d camera's shoot photo mode as burst-photo mode failed,"
                       " error code :0x%08X", position, returnCode);
        return returnCode;
    }

    /*! wait the APP change the shoot-photo mode display */
    USER_LOG_INFO("Sleep 0.5s...");
    osalHandler->TaskSleepMs(500);

    /*!< set shoot-photo mode parameter */
    USER_LOG_INFO("Set mounted position %d camera's burst count to %d", position, burstCount);
    returnCode = GduCameraManager_SetPhotoBurstCount(position, burstCount);

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("set mounted position %d camera's burst count(%d) failed,"
                       " error code :0x%08X.", position, burstCount, returnCode);
        return returnCode;
    }
    /*!< start to shoot single photo */
    USER_LOG_INFO("Mounted position %d camera start to shoot photo.", position);
    returnCode = GduCameraManager_StartShootPhoto(position, GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_BURST);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Mounted position %d camera shoot photo failed, "
                       "error code :0x%08X.", position, returnCode);
    }
    USER_LOG_INFO("Sleep 0.5s...");
    osalHandler->TaskSleepMs(500);
    returnCode = GduCameraManager_StartShootPhoto(position, GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_BURST);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Mounted position %d camera shoot photo failed, "
                       "error code :0x%08X.", position, returnCode);
    }
    USER_LOG_INFO("Sleep 0.5s...");
    osalHandler->TaskSleepMs(500);
    returnCode = GduCameraManager_StartShootPhoto(position, GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_BURST);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Mounted position %d camera shoot photo failed, "
                       "error code :0x%08X.", position, returnCode);
    }
    return returnCode;
}

/*! @brief Sample to shoot AEB photo, using async api
 *
 *  @note In this interface, camera will be set to be the SHOOT_PHOTO mode
 * then start to shoot a AEB photo.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param photoNum The number of pictures in each AEB shooting
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStartShootAEBPhoto(E_GduMountPosition position,
                                                        E_GduCameraManagerPhotoAEBCount aebCount)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    /*!< set camera work mode as shoot photo */
    USER_LOG_INFO("set mounted position %d camera's work mode as shoot photo mode.", position);
    returnCode = GduCameraManager_SetMode(position, GDU_CAMERA_MANAGER_WORK_MODE_SHOOT_PHOTO);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("set mounted position %d camera's work mode as shoot photo mode failed,"
                       " error code :0x%08X.", position, returnCode);
        return returnCode;
    }

    /*!< set shoot-photo mode */
    USER_LOG_INFO("Set mounted position %d camera's shoot photo mode as AEB-photo mode", position);
    returnCode = GduCameraManager_SetShootPhotoMode(position, GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_AEB);
    if (returnCode == GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        return returnCode;
    }

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("set mounted position %d camera's shoot photo mode as AEB-photo mode failed,"
                       " error code :0x%08X.", position, returnCode);
        return returnCode;
    }

    /*! wait the APP change the shoot-photo mode display */
    USER_LOG_INFO("Sleep 0.5s...");
    osalHandler->TaskSleepMs(500);

    /*!< set shoot-photo mode parameter */
    USER_LOG_INFO("Set mounted position %d camera's AEB count to %d", position, aebCount);
    returnCode = GduCameraManager_SetPhotoAEBCount(position, aebCount);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("set mounted position %d camera's AEB count(%d) failed,"
                       " error code :0x%08X.", position, aebCount, returnCode);
        return returnCode;
    }
    /*!< start to shoot single photo */
    USER_LOG_INFO("Mounted position %d camera start to shoot photo.", position);
    returnCode = GduCameraManager_StartShootPhoto(position, GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_AEB);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Mounted position %d camera shoot photo failed, "
                       "error code :0x%08X.", position, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to start shooting interval photo, using async api
 *
 *  @note In this interface, camera will be set to be the SHOOT_PHOTO mode
 * then start to shoot a interval photo.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param intervalData the parameter of interval shooting
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStartShootIntervalPhoto(E_GduMountPosition position,
                                                             T_GduCameraPhotoTimeIntervalSettings intervalData)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    /*!< set camera work mode as shoot photo */
    USER_LOG_INFO("set mounted position %d camera's work mode as shoot photo mode.", position);
    returnCode = GduCameraManager_SetMode(position, GDU_CAMERA_MANAGER_WORK_MODE_SHOOT_PHOTO);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("set mounted position %d camera's work mode as shoot photo mode failed,"
                       " error code :0x%08X.", position, returnCode);
        return returnCode;
    }

    /*!< set shoot-photo mode */
    USER_LOG_INFO("Set mounted position %d camera's shoot photo mode as interval-photo mode", position);
    returnCode = GduCameraManager_SetShootPhotoMode(position, GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_INTERVAL);
    if (returnCode == GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        return returnCode;
    }

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("set mounted position %d camera's shoot photo mode as interval-photo mode failed,"
                       " error code :0x%08X", position, returnCode);
        return returnCode;
    }

    /*! wait the APP change the shoot-photo mode display */
    USER_LOG_INFO("Sleep 0.5s...");
    osalHandler->TaskSleepMs(500);

    /*!< set shoot-photo mode parameter */
    USER_LOG_INFO("Set mounted position %d camera's interval time to %d s", position, intervalData.timeIntervalSeconds);
    returnCode = GduCameraManager_SetPhotoTimeIntervalSettings(position, intervalData);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("set mounted position %d camera's time interval parameter"
                       "(photo number:%d, time interval:%d) failed, error code :0x%08X.",
                       position, intervalData.captureCount, intervalData.timeIntervalSeconds, returnCode);
        return returnCode;
    }

    /*! wait the APP change the shoot-photo mode display */
    USER_LOG_INFO("Sleep 0.5s...");
    osalHandler->TaskSleepMs(500);

    /*!< start to shoot single photo */
    USER_LOG_INFO("Mounted position %d camera start to shoot photo.", position);
    returnCode = GduCameraManager_StartShootPhoto(position, GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_INTERVAL);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Mounted position %d camera shoot photo failed, "
                       "error code :0x%08X.", position, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to stop shooting, using async api
 *
 *  @note In this interface, camera will stop all the shooting action
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStopShootPhoto(E_GduMountPosition position)
{
    T_GduReturnCode returnCode;

    USER_LOG_INFO("Mounted position %d camera stop to shoot photo.", position);
    returnCode = GduCameraManager_StopShootPhoto(position);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Mounted position %d camera stop to shoot photo failed,"
                       " error code:0x%08X.", position, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to start record video, using async api
 *
 *  @note In this interface, camera will be set to be the RECORD_VIDEO mode
 * then start to record video.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStartRecordVideo(E_GduMountPosition position)
{
    T_GduReturnCode returnCode;
    /*!< set camera work mode as record video */
    USER_LOG_INFO("set mounted position %d camera's work mode as record-video mode", position);
    returnCode = GduCameraManager_SetMode(position, GDU_CAMERA_MANAGER_WORK_MODE_RECORD_VIDEO);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("set mounted position %d camera's work mode as record-video mode failed,"
                       " error code :0x%08X", position, returnCode);
        return returnCode;
    }

    /*!< start to take video */
    USER_LOG_INFO("Mounted position %d camera start to record video.", position);
    returnCode = GduCameraManager_StartRecordVideo(position);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Mounted position %d camera start to record video failed,"
                       " error code:0x%08X.", position, returnCode);
    }

    return returnCode;
}

/*! @brief Sample to stop record video, using async api
 *
 *  @note In this interface, camera will be set to be the RECORD_VIDEO mode
 * then stop recording video.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStopRecordVideo(E_GduMountPosition position)
{
    T_GduReturnCode returnCode;
    USER_LOG_INFO("Mounted position %d camera stop to record video.", position);
    returnCode = GduCameraManager_StopRecordVideo(position);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
        returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
        USER_LOG_ERROR("Mounted position %d camera stop to record video failed,"
                       " error code:0x%08X.", position, returnCode);
    }

    return returnCode;
}

static void *camera_manager_Task(void *arg)
{
    T_GduReturnCode gduStat;
    T_GduCameraManagerLaserDistanceInfo *status = NULL;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    USER_UTIL_UNUSED(arg);
    GduCameraManager_GetLaserPeriodicRangingStatus(&status);
    while (1) {
        osalHandler->TaskSleepMs(1000 / CAMERA_MANAGER_TASK_FREQ);

        USER_LOG_DEBUG("get laser periodic ranging ack:%d distance:%d, longitude:%d, latitude:%d, relative_H:%d, sea_H:%d, Horizontal_distance:%d", \
		status->ack, status->distance, status->longitude, status->latitude, status->relative_H, status->sea_H, status->Horizontal_distance);

    }
}

T_GduReturnCode GduTest_CameraManagerRunSample(E_GduMountPosition mountPosition,
                                               E_GduTestCameraManagerSampleSelect cameraManagerSampleSelect)
{
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    T_GduReturnCode returnCode;
    E_GduCameraType cameraType;
    T_GduCameraManagerFirmwareVersion firmwareVersion;
    T_GduCameraManagerFocusPosData focusPosData;
    T_GduCameraManagerTapZoomPosData tapZoomPosData;

    USER_LOG_INFO("Camera manager sample start");
    GduTest_WidgetLogAppend("Camera manager sample start");

    USER_LOG_INFO("--> Step 2: Get camera type and version");
    GduTest_WidgetLogAppend("--> Step 2: Get camera type and version");
    returnCode = GduCameraManager_GetCameraType(mountPosition, &cameraType);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get mounted position %d camera's type failed, error code: 0x%08X\r\n",
                       mountPosition, returnCode);
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    USER_LOG_INFO("Mounted position %d camera's type is %s",
                  mountPosition,
                  s_cameraTypeStrList[GduTest_CameraManagerGetCameraTypeIndex(cameraType)].cameraTypeStr);

    returnCode = GduCameraManager_GetFirmwareVersion(mountPosition, &firmwareVersion);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get mounted position %d camera's firmware version failed, error code: 0x%08X\r\n",
                       mountPosition, returnCode);
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    USER_LOG_INFO("Mounted position %d camera's firmware is V%d.%d.%d.%d\r\n", mountPosition,
                  firmwareVersion.firmware_version[0], firmwareVersion.firmware_version[1],
                  firmwareVersion.firmware_version[2], firmwareVersion.firmware_version[3]);

    switch (cameraManagerSampleSelect) {
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_SHUTTER_SPEED: {
            USER_LOG_INFO("--> Function a: Set camera shutter speed to 1/100 s");
            GduTest_WidgetLogAppend("--> Function a: Set camera shutter speed to 1/100 s");

            USER_LOG_INFO("Set mounted position %d camera's exposure mode to shutter priority mode.",
                            mountPosition);
            returnCode = GduTest_CameraManagerSetExposureMode(mountPosition,
                                                                GDU_CAMERA_MANAGER_EXPOSURE_MODE_SHUTTER_PRIORITY);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's exposure mode failed,"
                                "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            

            USER_LOG_INFO("Set mounted position %d camera's shutter speed to 1/125 s.",
                          mountPosition);
            returnCode = GduTest_CameraManagerSetShutterSpeed(mountPosition,
                                                              GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_125);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's shutter speed failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_APERTURE: {
            USER_LOG_INFO("--> Function b: Set camera aperture to 400(F/4)");
            GduTest_WidgetLogAppend("--> Function b: Set camera aperture to 400(F/4)");

            USER_LOG_INFO("Set mounted position %d camera's exposure mode to aperture priority mode.",
                            mountPosition);
            returnCode = GduTest_CameraManagerSetExposureMode(mountPosition,
                                                                GDU_CAMERA_MANAGER_EXPOSURE_MODE_APERTURE_PRIORITY);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's exposure mode failed,"
                                "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            
            USER_LOG_INFO("Set mounted position %d camera's aperture to 400(F/4).",
                          mountPosition);
            returnCode = GduTest_CameraManagerSetAperture(mountPosition, GDU_CAMERA_MANAGER_APERTURE_F_4);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's aperture failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_EV: {
            USER_LOG_INFO("--> Function c: Set camera ev value to +0.3ev");
            GduTest_WidgetLogAppend("--> Function c: Set camera ev value to +0.3ev");
            USER_LOG_INFO("Set mounted position %d camera's exposure mode to auto mode.",
                          mountPosition);
            returnCode = GduTest_CameraManagerSetExposureMode(mountPosition,
                                                              GDU_CAMERA_MANAGER_EXPOSURE_MODE_PROGRAM_AUTO);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's exposure mode failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }

            USER_LOG_INFO("Set mounted position %d camera's ev value to +0.3ev.",
                          mountPosition);
            returnCode = GduTest_CameraManagerSetEV(mountPosition, GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_0_3);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's EV failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_ISO: {
            USER_LOG_INFO("--> Function d: Set camera ISO value to 1600");
            GduTest_WidgetLogAppend("--> Function d: Set camera ISO value to 1600");
            USER_LOG_INFO("Set mounted position %d camera's exposure mode to manual mode.",
                          mountPosition);
            returnCode = GduTest_CameraManagerSetExposureMode(mountPosition,
                                                              GDU_CAMERA_MANAGER_EXPOSURE_MODE_EXPOSURE_MANUAL);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's exposure mode failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }

            USER_LOG_INFO("Set mounted position %d camera's ISO value to 1600.",
                          mountPosition);
            returnCode = GduTest_CameraManagerSetISO(mountPosition, GDU_CAMERA_MANAGER_ISO_1600);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's iso failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_FOCUS_POINT: {
            USER_LOG_INFO("--> Function e: Set camera focus point to (0.3,0.4)");
            GduTest_WidgetLogAppend("--> Function e: Set camera focus point to (0.3,0.4)");
            focusPosData.focusX = 0.3f;
            focusPosData.focusY = 0.4f;
            returnCode = GduTest_CameraManagerSetFocusPoint(mountPosition, focusPosData);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's focus point(0.8,0.8) failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_TAP_ZOOM_POINT: {
            USER_LOG_INFO("--> Function f: Set camera tap zoom point from (5x, 0.3m, 0.3m) to (4x, 0.8m, 0.7m)");
            GduTest_WidgetLogAppend(
                "--> Function f: Set camera tap zoom point from (5x, 0.3m, 0.3m) to (4x, 0.8m, 0.7m)");
            tapZoomPosData.focusX = 0.1f;
            tapZoomPosData.focusY = 0.5f;
            returnCode = GduTest_CameraManagerSetTapZoomPoint(mountPosition, 1, tapZoomPosData);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's tap zoom point(5, 0.3m,0.3m) failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            USER_LOG_INFO("Sleep 5s...");
            {
                int i = 0;
                for(i = 0; i < 5; i++)
                {
                    osalHandler->TaskSleepMs(1000);
                    uint8_t tapZoomMultiplier = 0;
                    GduCameraManager_GetTapZoomMultiplier(mountPosition, &tapZoomMultiplier);
                    USER_LOG_DEBUG("tapZoomMultiplier:%d", tapZoomMultiplier);
                }
            }
            

            tapZoomPosData.focusX = 0.9f;
            tapZoomPosData.focusY = 0.5f;
            returnCode = GduTest_CameraManagerSetTapZoomPoint(mountPosition, 4, tapZoomPosData);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's tap zoom point(4, 0.8m,0.7m) failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_ZOOM_PARAM: {
            USER_LOG_INFO("--> Function g: Camera zoom from 10x to -5x");
            GduTest_WidgetLogAppend("--> Function g: Camera zoom from 10x to -5x");
            returnCode = GduTest_CameraManagerOpticalZoom(mountPosition, GDU_CAMERA_ZOOM_DIRECTION_IN,
                                                          10);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's optical zoom factor 10x failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            USER_LOG_INFO("Sleep 4s...");
            osalHandler->TaskSleepMs(4000);

            returnCode = GduTest_CameraManagerOpticalZoom(mountPosition, GDU_CAMERA_ZOOM_DIRECTION_OUT, 5);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set mounted position %d camera's optical zoom factor -5x failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            USER_LOG_INFO("Sleep 4s...");
            osalHandler->TaskSleepMs(4000);

            USER_LOG_INFO(
                "Mounted position %d camera start continuous zoom with zoom-out direction and normal zoom speed.",
                mountPosition);
            returnCode = GduTest_CameraManagerStartContinuousZoom(mountPosition,
                                                                  GDU_CAMERA_ZOOM_DIRECTION_OUT,
                                                                  GDU_CAMERA_ZOOM_SPEED_NORMAL);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Mounted position %d camera start continuous zoom failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }

            USER_LOG_INFO("Sleep 8s...");
            osalHandler->TaskSleepMs(8000);

            USER_LOG_INFO("Mounted position %d camera stop continuous zoom.", mountPosition);
            returnCode = GduTest_CameraManagerStopContinuousZoom(mountPosition);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Mounted position %d camera stop continuous zoom failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_SINGLE_PHOTO: {
            USER_LOG_INFO("--> Function h: Shoot Single photo");
            GduTest_WidgetLogAppend("--> Function h: Shoot Single photo");
            returnCode = GduTest_CameraManagerStartShootSinglePhoto(mountPosition);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Mounted position %d camera shoot single photo failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_AEB_PHOTO: {
            USER_LOG_INFO("--> Function i: Shoot AEB photo with 3 pictures in one time");
            GduTest_WidgetLogAppend("--> Function i: Shoot AEB photo with 3 pictures in one time");
            returnCode = GduTest_CameraManagerStartShootAEBPhoto(mountPosition, GDU_CAMERA_MANAGER_PHOTO_AEB_COUNT_3);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Mounted position %d camera shoot AEB photo failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_BURST_PHOTO: {
            USER_LOG_INFO("--> Function j: Shoot Burst photo with 5 pictures in one time");
            GduTest_WidgetLogAppend("--> Function j: Shoot Burst photo with 5pictures in one time");
            returnCode = GduTest_CameraManagerStartShootBurstPhoto(mountPosition,
                                                                   GDU_CAMERA_BURST_COUNT_5);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Mounted position %d camera shoot burst photo failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_INTERVAL_PHOTO: {
            USER_LOG_INFO("--> Function k: Shoot Interval photo with 3s intervals in 15s");
            GduTest_WidgetLogAppend("--> Function k: Shoot Interval photo with 3s intervals in 15s");
            T_GduCameraPhotoTimeIntervalSettings intervalData;
            intervalData.captureCount = 255;
            intervalData.timeIntervalSeconds = 3;

            returnCode = GduTest_CameraManagerStartShootIntervalPhoto(mountPosition, intervalData);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Mounted position %d camera shoot internal photo failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }

            USER_LOG_INFO("Sleep 15s...");
            osalHandler->TaskSleepMs(15000);

            returnCode = GduTest_CameraManagerStopShootPhoto(mountPosition);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Mounted position %d camera stop shoot photo failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_RECORD_VIDEO: {
            USER_LOG_INFO("--> Function l: Record video in next 10s");
            GduTest_WidgetLogAppend("--> Function l: Record video in next 10s");
            returnCode = GduTest_CameraManagerStartRecordVideo(mountPosition);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Mounted position %d camera start record video failed, error code: 0x%08X\r\n",
                               mountPosition, returnCode);
            }

            USER_LOG_INFO("Sleep 10s...");
            {
                int i = 0;
                for(i = 0; i < 10; i++)
                {
                    E_GduCameraManagerRecordingState state;
                    GduCameraManager_GetRecordingState(mountPosition, &state);
                    osalHandler->TaskSleepMs(1000);
                    USER_LOG_DEBUG("----------------------------record state:%d", state);
                }
            }
            

            returnCode = GduTest_CameraManagerStopRecordVideo(mountPosition);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Mounted position %d camera stop record video failed, error code: 0x%08X\r\n",
                               mountPosition, returnCode);
            }

            {
                E_GduCameraManagerRecordingState state;
                GduCameraManager_GetRecordingState(mountPosition, &state);
                USER_LOG_DEBUG("---------------------------------record state:%d", state);
            }
            break;
        }
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_DOWNLOAD_AND_DELETE_MEDIA_FILE:
#ifdef SYSTEM_ARCH_LINUX
           GduTest_CameraManagerMediaDownloadAndDeleteMediaFile(mountPosition);
#else
            USER_LOG_WARN("This feature does not support RTOS platform.");
#endif
            break;
        case E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_RADER_RANGING: {
            USER_LOG_INFO("--> Function K: laser ranging");
            GduTest_WidgetLogAppend("--> Function K: laser ranging");

            returnCode = GduCameraManager_laserPeriodicRanging(mountPosition,
                                                                   GDU_CAMERA_MANAGER_LASER_RANGING_ENABLE);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Mounted position %d camera enable laser ranging failed,"
                               "error code: 0x%08X\r\n", mountPosition, returnCode);
            }
            break;
        }
        default: {
            USER_LOG_ERROR("There is no valid command input!");
            break;
        }
    }

    if (osalHandler->TaskCreate("camera_manager_task", camera_manager_Task,
                                CAMERA_MANAGER_TASK_STACK_SIZE, NULL, &s_userCameraManagerThread) !=
        GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("user camera manager task create error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    USER_LOG_INFO("Camera manager sample end");
    GduTest_WidgetLogAppend("Camera manager sample end");
    return returnCode;
}

/* Private functions definition-----------------------------------------------*/
static uint8_t GduTest_CameraManagerGetCameraTypeIndex(E_GduCameraType cameraType)
{
    uint8_t i;

    for (i = 0; i < sizeof(s_cameraTypeStrList) / sizeof(s_cameraTypeStrList[0]); i++) {
        if (s_cameraTypeStrList[i].cameraType == cameraType) {
            return i;
        }
    }

    return 0;
}

static T_GduReturnCode GduTest_CameraManagerMediaDownloadAndDeleteMediaFile(E_GduMountPosition position)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    returnCode = GduCameraManager_DownloadFileList(position, &s_meidaFileList);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Download file list failed, error code: 0x%08X.", returnCode);
        return returnCode;
    }

    returnCode = GduCameraManager_RegDownloadFileDataCallback(position, GduTest_CameraManagerDownloadFileDataCallback);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Register download file data callback failed, error code: 0x%08X.", returnCode);
        return returnCode;
    }

    if (s_meidaFileList.totalCount > 0) {
        for (int i = 0; i < s_meidaFileList.totalCount; ++i) {
            if (s_meidaFileList.fileListInfo[i].fileSize < 1 * 1024 * 1024) {
                USER_LOG_INFO(
                    "Media file_%03d name: %s, index: %d, time:%04d-%02d-%02d_%02d:%02d:%02d, size: %.2f KB, type: %d",
                    i, s_meidaFileList.fileListInfo[i].fileName,
                    s_meidaFileList.fileListInfo[i].fileIndex,
                    s_meidaFileList.fileListInfo[i].createTime.year,
                    s_meidaFileList.fileListInfo[i].createTime.month,
                    s_meidaFileList.fileListInfo[i].createTime.day,
                    s_meidaFileList.fileListInfo[i].createTime.hour,
                    s_meidaFileList.fileListInfo[i].createTime.minute,
                    s_meidaFileList.fileListInfo[i].createTime.second,
                    (gdu_f32_t) s_meidaFileList.fileListInfo[i].fileSize / 1024,
                    s_meidaFileList.fileListInfo[i].type);
            } else {
                USER_LOG_INFO(
                    "Media file_%03d name: %s, index: %d,  time:%04d-%02d-%02d_%02d:%02d:%02d, size: %.2f MB, type: %d",
                    i, s_meidaFileList.fileListInfo[i].fileName,
                    s_meidaFileList.fileListInfo[i].fileIndex,
                    s_meidaFileList.fileListInfo[i].createTime.year,
                    s_meidaFileList.fileListInfo[i].createTime.month,
                    s_meidaFileList.fileListInfo[i].createTime.day,
                    s_meidaFileList.fileListInfo[i].createTime.hour,
                    s_meidaFileList.fileListInfo[i].createTime.minute,
                    s_meidaFileList.fileListInfo[i].createTime.second,
                    (gdu_f32_t) s_meidaFileList.fileListInfo[i].fileSize / (1024 * 1024),
                    s_meidaFileList.fileListInfo[i].type);
            }
        }

        osalHandler->TaskSleepMs(1000);

        returnCode = GduCameraManager_DeleteFileByPath(position, s_meidaFileList.fileListInfo[0].fileName);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("Download media file by index failed, error code: 0x%08X.", returnCode);
            return returnCode;
        }

        osalHandler->TaskSleepMs(1000);

        returnCode = GduCameraManager_DeleteFileByPath(position, s_meidaFileList.fileListInfo[0].fileName);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("Delete media file by index failed, error code: 0x%08X.", returnCode);
            return returnCode;
        }

        osalHandler->TaskSleepMs(1000);
    } else {
        USER_LOG_WARN("Media file is not existed in sdcard.");
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduTest_CameraManagerDownloadFileDataCallback(T_GduDownloadFilePacketInfo packetInfo,
                                                                     const uint8_t *data, uint16_t len)
{
    char fileName[TEST_CAMERA_MANAGER_MEDIA_FILE_NAME_MAX_SIZE];
    char extendInfo[TEST_CAMERA_MANAGER_MEDIA_EXTEND_INFO_MAX_SIZE];
    int32_t i;

    sprintf(extendInfo, " FileIndex: %d", packetInfo.fileIndex);

    if (packetInfo.downloadFileEvent == GDU_DOWNLOAD_FILE_EVENT_START) {
        for (i = 0; i < s_meidaFileList.totalCount; ++i) {
            if (s_meidaFileList.fileListInfo[i].fileIndex == packetInfo.fileIndex) {
                break;
            }
        }
        sprintf(fileName, "%s", s_meidaFileList.fileListInfo[i].fileName);
        USER_LOG_INFO("Start download media file %s", s_meidaFileList.fileListInfo[i].fileName);
        s_downloadMediaFile = fopen(fileName, "wb+");
        if (s_downloadMediaFile == NULL) {
            return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }
        fwrite(data, 1, len, s_downloadMediaFile);
    } else if (packetInfo.downloadFileEvent == GDU_DOWNLOAD_FILE_EVENT_TRANSFER) {
        if (s_downloadMediaFile != NULL) {
            fwrite(data, 1, len, s_downloadMediaFile);
        }
    } else if (packetInfo.downloadFileEvent == GDU_DOWNLOAD_FILE_EVENT_END) {
        if (s_downloadMediaFile != NULL) {
            fwrite(data, 1, len, s_downloadMediaFile);
        }

        USER_LOG_INFO("End download media file");
        fclose(s_downloadMediaFile);
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
