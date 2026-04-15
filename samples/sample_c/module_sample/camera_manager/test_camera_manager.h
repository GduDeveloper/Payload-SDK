/**
 ********************************************************************
 * @file    test_camera_manager.h
 * @brief   This is the header file for "test_camera_manager.c", defining the structure and
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
#ifndef TEST_CAMERA_MANAGER_H
#define TEST_CAMERA_MANAGER_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"
#include "gdu_camera_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum {
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_SHUTTER_SPEED,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_APERTURE,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_EV,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_ISO,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_FOCUS_POINT,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_TAP_ZOOM_POINT,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_ZOOM_PARAM,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_SINGLE_PHOTO,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_AEB_PHOTO,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_BURST_PHOTO,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_INTERVAL_PHOTO,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_RECORD_VIDEO,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_DOWNLOAD_AND_DELETE_MEDIA_FILE,
    E_GDU_TEST_CAMERA_MANAGER_SAMPLE_SELECT_RADER_RANGING,
} E_GduTestCameraManagerSampleSelect;
/* Exported functions --------------------------------------------------------*/

/*! @brief Sample to set exposure compensation value for camera, using async
 * api
 *
 *  @note In this interface, exposure compensation value will be got then be
 * set.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param dataTarget the target exposure compensation value
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetEV(E_GduMountPosition position,
                                           E_GduCameraManagerExposureCompensation exposureCompensation);

/*! @brief Sample to set exposure mode for camera, using async api
 *
 *  @note In this interface, exposure will be got then be set.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param dataTarget the target exposure mode
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetExposureMode(E_GduMountPosition position,
                                                     E_GduCameraManagerExposureMode exposureMode);

/*! @brief Sample to set ISO value for camera, using async api
 *
 *  @note In this interface, ISO will be got then be set.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param dataTarget the target ISO value
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetISO(E_GduMountPosition position,
                                            E_GduCameraManagerISO isoData);
/*! @brief Sample to set shutter speed for camera, using async api
 *
 *  @note In this interface, shutter speed will be got then be set.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param dataTarget the target shutter speed
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetShutterSpeed(E_GduMountPosition position,
                                                     E_GduCameraManagerShutterSpeed shutterSpeed);

/*! @brief Sample to set shutter aperture value for camera, using async api
 *
 *  @note In this interface, aperture value will be got then be set.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param dataTarget the target aperture value
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetAperture(E_GduMountPosition position,
                                                 E_GduCameraManagerAperture aperture);

/*! @brief Sample to set focus point for camera, using async api
 *
 *  @note In this interface, focus mode will be set to be AUTO. Then the
 * focus point will be set to be (x, y)
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @param x the x value of target focus point, 0~1
 *  @param y the y value of target focus point, 0~1
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerSetFocusPoint(E_GduMountPosition position,
                                                   T_GduCameraManagerFocusPosData tapFocusPos);

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
                                                     T_GduCameraManagerTapZoomPosData tapZoomPosData);
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
                                                         E_GduCameraZoomSpeed zoomSpeed);

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
                                                 gdu_f32_t factor);

/*! @brief Sample to stop continuous zoom on camera, using async api
 *
 *  @note It is only supported by X5, X5R and X5S camera on Osmo with lens
 * Olympus M.Zuiko ED 14-42mm f/3.5-5.6 EZ, Z3 camera, Z30 camera.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStopContinuousZoom(E_GduMountPosition position);

/*! @brief Sample to shoot single photo, using async api
 *
 *  @note In this interface, camera will be set to be the SHOOT_PHOTO mode
 * then start to shoot a single photo.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStartShootSinglePhoto(E_GduMountPosition position);

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
                                                          E_GduCameraBurstCount burstCount);

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
                                                        E_GduCameraManagerPhotoAEBCount aebCount);

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
                                                             T_GduCameraPhotoTimeIntervalSettings intervalData);

/*! @brief Sample to stop shooting, using async api
 *
 *  @note In this interface, camera will stop all the shooting action
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStopShootPhoto(E_GduMountPosition position);

/*! @brief Sample to start record video, using async api
 *
 *  @note In this interface, camera will be set to be the RECORD_VIDEO mode
 * then start to record video.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStartRecordVideo(E_GduMountPosition position);

/*! @brief Sample to stop record video, using async api
 *
 *  @note In this interface, camera will be set to be the RECORD_VIDEO mode
 * then stop recording video.
 *  @param index payload node index, input limit see enum
 * GDU::OSDK::PayloadIndexType
 *  @return T_GduReturnCode error code
 */
T_GduReturnCode GduTest_CameraManagerStopRecordVideo(E_GduMountPosition position);

T_GduReturnCode GduTest_CameraManagerRunSample(E_GduMountPosition mountPosition,
                                               E_GduTestCameraManagerSampleSelect cameraManagerSampleSelect);

#ifdef __cplusplus
}
#endif

#endif // TEST_CAMERA_MANAGER_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
