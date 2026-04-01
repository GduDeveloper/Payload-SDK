/**
 ********************************************************************
 * @file    gdu_camera_manager.h
 * @brief   This is the header file for "gdu_camera_manager.c", defining the structure and
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
#ifndef GDU_CAMERA_MANAGER_H
#define GDU_CAMERA_MANAGER_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"
#include "gdu_payload_camera.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/*! @brief CameraModule work modes.
 */
typedef enum {
    /*!
     - Capture mode. In this mode, the user can capture pictures.
     */
    GDU_CAMERA_MANAGER_WORK_MODE_SHOOT_PHOTO = 0,
    /*!
     - Record mode. In this mode, the user can record videos.
     */
    GDU_CAMERA_MANAGER_WORK_MODE_RECORD_VIDEO = 1,
    /*!
     * The camera work mode is unknown.
     */
    GDU_CAMERA_MANAGER_WORK_MODE_WORK_MODE_UNKNOWN = 0xFF,
} E_GduCameraManagerWorkMode;

/*! @brief The ShootPhoto mode itself can have several modes. The default
 * value is SINGLE.
 */
typedef enum {
    /*!
     - Sets the camera to take a single photo.
     */
    GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_SINGLE = 0x01,
//    /*!
//     - Sets the camera to take an HDR photo. X5 camera, X5R camera, XT camera,
//     Z30 camera, Phantom 4 Pro camera, X4S camera and X5S camera do not support
//     HDR mode.
//     */
//    GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_HDR = 0x02,
    /*!
     - Set the camera to take multiple photos at once. XT camera does not
     support Burst mode.
     */
    GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_BURST = 0x04,
   /*!
    - Automatic Exposure Bracketing (AEB) capture. In this mode you can quickly
    take multiple shots (the default is 3) at different exposures without
    having to manually change any settings between frames. XT camera and Z30
    camera does not support AEB mode.
    */
   GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_AEB = 0x05,
    /*!
     - Sets the camera to take a picture (or multiple pictures) continuously at
     a set time interval. The minimum interval for JPEG format of any quality is
     2s. For all cameras except X4S, X5S and Phantom 4 Pro camera: The minimum
     interval for RAW or RAW+JPEG format is 10s. For the X4S, X5S and Phantom 4
     Pro cameras the minimum interval for RAW or RAW+JPEG dformat is 5s.
     */
    GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_INTERVAL = 0x06,
//    /*!
//     - Sets the camera to take a burst of RAW photos. Use getRAWPhotoBurstCount
//     to check how many photos have been shot. Only supported by X5S.
//     */
//    GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_RAW_BURST = 0x09,
//    /*!
//     - 	Sets the camera to take an regional photos. It is supported by H20/H20T.
//     */
//    GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_REGIONAL_SR = 0x16,
    /*!
     - The shoot photo mode is unknown.
     */
    GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_UNKNOWN = 0xFF,
} E_GduCameraManagerShootPhotoMode;

/*! @brief the photo action of INTERVAL shooting photo mode
 */
typedef enum {
    GDU_CAMERA_MANAGER_EXPOSURE_MODE_PROGRAM_AUTO = 1,       /*!< Program mode */
    GDU_CAMERA_MANAGER_EXPOSURE_MODE_SHUTTER_PRIORITY = 2,   /*!< Shutter priority mode */
    GDU_CAMERA_MANAGER_EXPOSURE_MODE_APERTURE_PRIORITY = 3,  /*!< Aperture priority mode */
    GDU_CAMERA_MANAGER_EXPOSURE_MODE_EXPOSURE_MANUAL = 4,    /*!< Manual mode */
    GDU_CAMERA_MANAGER_EXPOSURE_MODE_EXPOSURE_UNKNOWN = 0xFF /*!< The camera exposure mode is unknown. */
} E_GduCameraManagerExposureMode;

/*! @brief the photo action of INTERVAL shooting photo mode
 */
typedef enum {
    /*! The number of pictures to continuously take at one time in AEB mode is 3
     */
    GDU_CAMERA_MANAGER_PHOTO_AEB_COUNT_3 = 3,
    /*! The number of pictures to continuously take at one time in AEB mode is 5
     */
    GDU_CAMERA_MANAGER_PHOTO_AEB_COUNT_5 = 5,
    /*! The number of pictures to continuously take at one time in AEB mode is 7
     */
    GDU_CAMERA_MANAGER_PHOTO_AEB_COUNT_7 = 7,
    /*! The number of pictures to continuously take at one time in AEB mode is
     * unknown.
     */
    GDU_CAMERA_MANAGER_PHOTO_AEB_COUNT_KNOWN = 0xFF,
} E_GduCameraManagerPhotoAEBCount;

/*! @breif CameraModule focus mode. If the physical AF switch on the camera is
 * set to auto.
 */
typedef enum {
    /*!
     - The camera's focus mode is set to manual. In this mode, user sets the
     focus ring value to adjust the focal distance.
     */
    GDU_CAMERA_MANAGER_FOCUS_MODE_MANUAL = 0,
    /*!
     - The camera's focus mode is set to auto. For the Z30 camera, the focus is
     calculated completely automatically. For all other cameras, a focus target
     can be set by the user, which is used to calculate focus automatically.
     */
    GDU_CAMERA_MANAGER_FOCUS_MODE_AUTO = 1,
    /*!
     - The camera's focus mode is unknown.
     */
    GDU_CAMERA_MANAGER_FOCUS_MODE_UNKNOWN = 0xFF,
} E_GduCameraManagerFocusMode;

/*! @breif CameraModule shutter mode.
 */
typedef enum {
    /*! The shutter mode of camera is automatical */
    GDU_CAMERA_MANAGER_SHUTTER_AUTO_MODE = 0x00,
    /*! The shutter mode of camera is manual, the shutter speed setting is
       valid. */
    GDU_CAMERA_MANAGER_SHUTTER_MANUAL_MODE = 0x01,
} E_GduCameraManagerShutterMode;

/*! @brief CameraModule shutter speed values.
 */
typedef enum {
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_AUTO = 0,     /*!< 1/8000 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_8 = 1,     /*!< 1/6400 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_4 = 2,     /*!< 1/6000 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_2 = 3,     /*!< 1/5000 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1 = 4,     /*!< 1/4000 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_4 = 5,     /*!< 1/3200 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_8 = 6,     /*!< 1/3000 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_15 = 7,     /*!< 1/2500 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_30 = 8,     /*!< 1/2000 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_60 = 9,     /*!< 1/1600 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_125 = 10,    /*!< 1/1500 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_250 = 11,    /*!< 1/1250 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_500 = 12,    /*!< 1/1000 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_1000 = 13,     /*!< 1/800 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_2000 = 14,     /*!< 1/725 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_4000 = 15,     /*!< 1/640 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_5000 = 16,     /*!< 1/500 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_6000 = 17,     /*!< 1/400 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_8000 = 18,     /*!< 1/350 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_1_2 = 19,     /*!< 1/320 s */
    GDU_CAMERA_MANAGER_SHUTTER_SPEED_UNKNOWN = 0xFF, /*!< Unknown */
} E_GduCameraManagerShutterSpeed;

/*! @brief CameraModule ISO values.
 */
typedef enum {
    /*! The ISO value is automatically set. This cannot be used for all cameras
       when in Manual mode. */
    GDU_CAMERA_MANAGER_ISO_AUTO = 0x00,
    /*!  The ISO value is set to 50. */
    GDU_CAMERA_MANAGER_ISO_50 = 0x01,
    /*! The ISO value is set to 100. */
    GDU_CAMERA_MANAGER_ISO_100 = 0x02,
    /*! The ISO value is set to 100.*/
    GDU_CAMERA_MANAGER_ISO_200 = 0x03,
    /*! The ISO value is set to 100.*/
    GDU_CAMERA_MANAGER_ISO_400 = 0x04,
    /*! The ISO value is set to 100.*/
    GDU_CAMERA_MANAGER_ISO_800 = 0x05,
    /*! The ISO value is set to 100.*/
    GDU_CAMERA_MANAGER_ISO_1600 = 0x06,
    /*! The ISO value is set to 100.*/
    GDU_CAMERA_MANAGER_ISO_3200 = 0x07,
    /*! The ISO value is set to 100.*/
    GDU_CAMERA_MANAGER_ISO_6400 = 0x08,
    /*! The ISO value is set to 100.*/
    GDU_CAMERA_MANAGER_ISO_12800 = 0x09,
    /*! The ISO value is set to 100.*/
    GDU_CAMERA_MANAGER_ISO_25600 = 0x0A,
    /*! ISO value is fixed by the camera firmware. When the camera color is set
     to D_LOG, camera will fix the ISO to a specific value in order to optimize
     the performance.
     */
    GDU_CAMERA_MANAGER_ISO_FIXED = 0xFF,
} E_GduCameraManagerISO;

/*! @brief CameraModule exposure compensation.
 */
typedef enum {
	GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_AUTO = 0,
    /*! The camera's exposure compensation is -5.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_12_0 = 1,
    /*! The camera's exposure compensation is -4.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_8_0 = 2,
    /*! The camera's exposure compensation is -4.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_4_0 = 3,
    /*! The camera's exposure compensation is -4.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_3_0 = 4,
    /*! The camera's exposure compensation is -3.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_2_75 = 5,
    /*! The camera's exposure compensation is -3.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_2_7 = 6,
    /*! The camera's exposure compensation is -3.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_2_5 = 7,
    /*! The camera's exposure compensation is -3.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_2_3 = 8,
    /*! The camera's exposure compensation is -2.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_2_25 = 9,
    /*! The camera's exposure compensation is -2.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_2 = 10,
    /*! The camera's exposure compensation is -2.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_1_75 = 11,
    /*! The camera's exposure compensation is -1.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_1_7 = 12,
    /*! The camera's exposure compensation is -1.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_1_5 = 13,
    /*! The camera's exposure compensation is -1.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_1_3 = 14,
    /*! The camera's exposure compensation is -0.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_1_25 = 15,
    /*! The camera's exposure compensation is -0.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_1 = 16,
    /*! The camera's exposure compensation is +0.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_0_75 = 17,
    /*! The camera's exposure compensation is +0.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_0_7 = 18,
    /*! The camera's exposure compensation is +1.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_0_5 = 19,
    /*! The camera's exposure compensation is +1.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_0_3 = 20,
    /*! The camera's exposure compensation is +1.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_0_25 = 21,
    /*! The camera's exposure compensation is +2.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_0_0 = 22,
    /*! The camera's exposure compensation is +2.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_0_25 = 23,
    /*! The camera's exposure compensation is +2.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_0_3 = 24,
    /*! The camera's exposure compensation is +3.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_0_5 = 25,
    /*! The camera's exposure compensation is +3.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_0_7 = 26,
    /*! The camera's exposure compensation is +3.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_0_75 = 27,
    /*! The camera's exposure compensation is +4.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_1_0 = 28,
    /*! The camera's exposure compensation is +4.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_1_25 = 29,
    /*! The camera's exposure compensation is +4.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_1_3 = 30,
    /*! The camera's exposure compensation is +5.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_1_5 = 31,
    /*! The camera's exposure compensation is +2.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_1_7 = 32,
    /*! The camera's exposure compensation is +2.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_1_75 = 33,
    /*! The camera's exposure compensation is +3.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_2_0 = 34,
    /*! The camera's exposure compensation is +3.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_2_25 = 35,
    /*! The camera's exposure compensation is +3.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_2_3 = 36,
    /*! The camera's exposure compensation is +4.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_2_5 = 37,
    /*! The camera's exposure compensation is +4.3ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_2_7 = 38,
    /*! The camera's exposure compensation is +4.7ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_2_75 = 39,
    /*! The camera's exposure compensation is +5.0ev.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_3_0 = 40,
	/*! The camera's exposure compensation is +4.3ev.*/
	GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_4_0 = 41,
	/*! The camera's exposure compensation is +4.7ev.*/
	GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_8_0 = 42,
	/*! The camera's exposure compensation is +5.0ev.*/
	GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_12_0 = 43,

    /*! The camera's exposure compensation is fixed by the camera.*/
    GDU_CAMERA_MANAGER_EXPOSURE_COMPENSATION_FIXED = 0xFF,
} E_GduCameraManagerExposureCompensation;

/*! @brief CameraModule aperture values.
 *  @note X5, X5R, Z30, Phantom 4 Pro camera, X4S and X5S support this
 * setting.
 */
typedef enum {
    /*! 	The Aperture value is f/1.6. It is only supported by Z30
       camera.*/
    GDU_CAMERA_MANAGER_APERTURE_F_1_DOT_6 = 160,
    /*! The Aperture value is f/1.7.*/
    GDU_CAMERA_MANAGER_APERTURE_F_1_DOT_7 = 170,
    /*! The Aperture value is f/1.8.*/
    GDU_CAMERA_MANAGER_APERTURE_F_1_DOT_8 = 180,
    /*! The Aperture value is f/2.*/
    GDU_CAMERA_MANAGER_APERTURE_F_2 = 200,
    /*! The Aperture value is f/2.2.*/
    GDU_CAMERA_MANAGER_APERTURE_F_2_DOT_2 = 220,
    /*! The Aperture value is f/2.4. It is only supported by Z30 camera.*/
    GDU_CAMERA_MANAGER_APERTURE_F_2_DOT_4 = 240,
    /*! The Aperture value is f/2.5.*/
    GDU_CAMERA_MANAGER_APERTURE_F_2_DOT_5 = 250,
    /*! The Aperture value is f/2.6.*/
    GDU_CAMERA_MANAGER_APERTURE_F_2_DOT_6 = 260,
    /*! The Aperture value is f/2.8.*/
    GDU_CAMERA_MANAGER_APERTURE_F_2_DOT_8 = 280,
    /*! The Aperture value is f/3.2.*/
    GDU_CAMERA_MANAGER_APERTURE_F_3_DOT_2 = 320,
    /*! The Aperture value is f/3.4.*/
    GDU_CAMERA_MANAGER_APERTURE_F_3_DOT_4 = 340,
    /*! The Aperture value is f/3.5.*/
    GDU_CAMERA_MANAGER_APERTURE_F_3_DOT_5 = 350,
    /*! The Aperture value is f/4.*/
    GDU_CAMERA_MANAGER_APERTURE_F_4 = 400,
    /*! The Aperture value is f/4.5.*/
    GDU_CAMERA_MANAGER_APERTURE_F_4_DOT_5 = 450,
    /*! The Aperture value is f/4.8.*/
    GDU_CAMERA_MANAGER_APERTURE_F_4_DOT_8 = 480,
    /*! The Aperture value is f/5.*/
    GDU_CAMERA_MANAGER_APERTURE_F_5 = 500,
    /*! The Aperture value is f/5.6.*/
    GDU_CAMERA_MANAGER_APERTURE_F_5_DOT_6 = 560,
    /*! The Aperture value is f/6.3.*/
    GDU_CAMERA_MANAGER_APERTURE_F_6_DOT_3 = 630,
    /*! The Aperture value is f/6.8.*/
    GDU_CAMERA_MANAGER_APERTURE_F_6_DOT_8 = 680,
    /*! The Aperture value is f/7.1.*/
    GDU_CAMERA_MANAGER_APERTURE_F_7_DOT_1 = 710,
    /*! The Aperture value is f/8.*/
    GDU_CAMERA_MANAGER_APERTURE_F_8 = 800,
    /*! The Aperture value is f/9.*/
    GDU_CAMERA_MANAGER_APERTURE_F_9 = 900,
    /*! The Aperture value is f/9.6.*/
    GDU_CAMERA_MANAGER_APERTURE_F_9_DOT_6 = 960,
    /*! The Aperture value is f/10.*/
    GDU_CAMERA_MANAGER_APERTURE_F_10 = 1000,
    /*! The Aperture value is f/11.*/
    GDU_CAMERA_MANAGER_APERTURE_F_11 = 1100,
    /*! The Aperture value is f/13.*/
    GDU_CAMERA_MANAGER_APERTURE_F_13 = 1300,
    /*! The Aperture value is f/14.*/
    GDU_CAMERA_MANAGER_APERTURE_F_14 = 1400,
    /*! The Aperture value is f/16.*/
    GDU_CAMERA_MANAGER_APERTURE_F_16 = 1600,
    /*! The Aperture value is f/18.*/
    GDU_CAMERA_MANAGER_APERTURE_F_18 = 1800,
    /*! The Aperture value is f/19.*/
    GDU_CAMERA_MANAGER_APERTURE_F_19 = 1900,
    /*! The Aperture value is f/20.*/
    GDU_CAMERA_MANAGER_APERTURE_F_20 = 2000,
    /*! The Aperture value is f/22.*/
    GDU_CAMERA_MANAGER_APERTURE_F_22 = 2200,
    /*! The Aperture value is Unknown. */
    GDU_CAMERA_MANAGER_APERTURE_F_UNKNOWN = 0xFFFF,
} E_GduCameraManagerAperture;

typedef enum {
    GDU_CAMERA_MANAGER_RECORDING_CONTROL_STOP = 0,
    GDU_CAMERA_MANAGER_RECORDING_CONTROL_BEGIN = 1,
    GDU_CAMERA_MANAGER_RECORDING_CONTROL_PAUSE = 2,
    GDU_CAMERA_MANAGER_RECORDING_CONTROL_RESUME = 3,
} E_GduCameraManagerRecordingControl;

typedef enum {
    GDU_CAMERA_MANAGER_FILE_LIST_COUNT_60_PER_SLICE = 60,
    GDU_CAMERA_MANAGER_FILE_LIST_COUNT_120_PER_SLICE = 120,
    GDU_CAMERA_MANAGER_FILE_LIST_COUNT_ALL_PER_SLICE = 0xFFFF,
} E_GduCameraManagerFileListCountPerSlice;

typedef enum {
    GDU_CAMERA_MANAGER_SOURCE_DEFAULT_CAM = 0x0,
    GDU_CAMERA_MANAGER_SOURCE_WIDE_CAM = 0x1,
    GDU_CAMERA_MANAGER_SOURCE_ZOOM_CAM = 0x2,
    GDU_CAMERA_MANAGER_SOURCE_IR_CAM = 0x3,
    GDU_CAMERA_MANAGER_SOURCE_VISIBLE_CAM = 0x7,
} E_GduCameraManagerStreamSource;

typedef enum {
    GDU_CAMERA_MANAGER_NIGHT_SCENE_MODE_DISABLE = 0,
    GDU_CAMERA_MANAGER_NIGHT_SCENE_MODE_ENABLE = 1,
    GDU_CAMERA_MANAGER_NIGHT_SCENE_MODE_AUTO = 2,
} E_GduCameraManagerNightSceneMode;

typedef enum {
    GDU_CAMERA_MANAGER_LASER_RANGING_DISABLE = 0,
    GDU_CAMERA_MANAGER_LASER_RANGING_ENABLE = 1,
} E_GduCameraManagerLaserRanging;

typedef enum {
    GDU_CAMERA_MANAGER_CAPTURE_OR_RECORDING_CAPTURE = 0,
    GDU_CAMERA_MANAGER_CAPTURE_OR_RECORDING_RECORDING = 1,
} E_GduCameraManagerCaptureOrRecording;

typedef enum {
    GDU_CAMERA_MANAGER_EXPAND_NAME_TYPE_FILE = 1,
    GDU_CAMERA_MANAGER_EXPAND_NAME_TYPE_DIR = 2,
} E_GduCameraManagerExpandNameType;

typedef enum {
    GDU_CAMERA_MANAGER_PHOTO_RATIO_4X3 = 0,
    GDU_CAMERA_MANAGER_PHOTO_RATIO_16X9 = 1,
    GDU_CAMERA_MANAGER_PHOTO_RATIO_3X2 = 2,
    GDU_CAMERA_MANAGER_PHOTO_RATIO_1X1 = 3,
    GDU_CAMERA_MANAGER_PHOTO_RATIO_18X3 = 4,
    GDU_CAMERA_MANAGER_PHOTO_RATIO_5X4 = 5,
} E_GduCameraManagerPhotoRatio;

typedef struct {
    uint8_t firmware_version[4];
} T_GduCameraManagerFirmwareVersion;

typedef struct {
    uint16_t  ack; /*< 数据,赋值操作,ACK帧:0x00:成功 0x01:失败*/
    uint16_t  distance; /*< 数据,赋值操作,激光测距距离,低字节在前，单位0.1米*/
    int32_t  longitude; /*< 数据,赋值操作,目标位置经度*/
    int32_t  latitude; /*< 数据,赋值操作,目标位置纬度*/
    int32_t  relative_H; /*< 数据,赋值操作,目标位置相对高度*/
    int32_t  sea_H; /*< 数据,赋值操作,目标位置海拔高度*/
    uint16_t  Horizontal_distance; /*< 数据,赋值操作,目标水平距离*/
} T_GduCameraManagerLaserDistanceInfo;

/*! @brief Tap zoom target point data struct, used by user.
 */
typedef T_GduCameraPointInScreen T_GduCameraManagerTapZoomPosData;

/*! @brief Tap focus target point data struct, used by user.
 */
typedef T_GduCameraPointInScreen T_GduCameraManagerFocusPosData;

typedef struct {
    gdu_f32_t currentOpticalZoomFactor;
    gdu_f32_t maxOpticalZoomFactor;
} T_GduCameraManagerOpticalZoomParam;

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
} T_GduCameraManagerFileCreateTime;

typedef struct {
    union {
        struct {
            uint32_t attributePhotoReserved: 22;
            uint32_t attributePhotoRatio: 8;
            uint32_t attributePhotoRotation: 2;
            uint8_t reserved[12];
        } photoAttribute;
        struct {
            uint32_t attributeVideoDuration: 16;
            uint32_t attributeVideoFramerate: 6;
            uint32_t attributeVideoRotation: 2;
            uint32_t attributeVideoResolution: 8;
            uint8_t reserved[12];
        } videoAttribute;
    };
} T_GduCameraManagerFileAttributeData;

typedef struct {
    char fileName[256];
    uint32_t fileSize;
    uint32_t fileIndex;
    T_GduCameraManagerFileCreateTime createTime;
    E_GduCameraMediaFileType type;
    T_GduCameraManagerFileAttributeData attributeData;
} T_GduCameraManagerFileListInfo;

typedef struct {
    uint16_t totalCount;
    T_GduCameraManagerFileListInfo *fileListInfo;
} T_GduCameraManagerFileList;

typedef struct {
    uint16_t sliceStartIndex;
    E_GduCameraManagerFileListCountPerSlice countPerSlice;
} T_GduCameraManagerSliceConfig;

typedef enum {
    GDU_DOWNLOAD_FILE_EVENT_START,
    GDU_DOWNLOAD_FILE_EVENT_TRANSFER,
    GDU_DOWNLOAD_FILE_EVENT_END,
} E_GduDownloadFileEvent;

typedef enum {
    GDU_CAMERA_MANAGER_VIDEO_RESOLUTION_640X480P = 0, // 640X480P
    GDU_CAMERA_MANAGER_VIDEO_RESOLUTION_1280X640P = 2, // 1280X640P
    GDU_CAMERA_MANAGER_VIDEO_RESOLUTION_1280X720P = 4, // 1280X720P
    GDU_CAMERA_MANAGER_VIDEO_RESOLUTION_1920X1080P = 10, // 1920X1080P
    GDU_CAMERA_MANAGER_VIDEO_RESOLUTION_3840X2160P = 16, // 3840X2160P
} E_GduCameraManagerVideoResolution;

typedef enum {
    GDU_CAMERA_MANAGER_VIDEO_FRAME_RATE_15FPS = 0, // 14.985
    GDU_CAMERA_MANAGER_VIDEO_FRAME_RATE_25FPS = 2, // 25.000
    GDU_CAMERA_MANAGER_VIDEO_FRAME_RATE_30FPS = 3, // 29.970
    GDU_CAMERA_MANAGER_VIDEO_FRAME_RATE_60FPS = 6, // 59.940
} E_GduCameraManagerVideoFrameRate;

typedef enum {
    GDU_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_RAW = 0,
    GDU_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_JPEG = 1,
    GDU_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_RAW_JPEG = 2,
    GDU_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_YUV = 3, // 保存为YUV格式的图片
    GDU_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_RJPEG = 7, // Radiometric JPEG
} E_GduCameraManagerPhotoStorageFormat;

typedef enum {
    GDU_CAMERA_MANAGER_VIDEO_STORAGE_FORMAT_MOV = 0,
    GDU_CAMERA_MANAGER_VIDEO_STORAGE_FORMAT_MP4 = 1,
} E_GduCameraManagerVideoStorageFormat;

typedef enum {
    GDU_CAMERA_MANAGER_METERING_MODE_CENTRAL = 0,
    GDU_CAMERA_MANAGER_METERING_MODE_AVERAGE = 1,
    GDU_CAMERA_MANAGER_METERING_MODE_SPOT = 2,
} E_GduCameraManagerMeteringMode;

typedef enum {
    GDU_CAMERA_MANAGER_FFC_MODE_MANUAL = 0,
    GDU_CAMERA_MANAGER_FFC_MODE_AUTO = 1,
} E_GduCameraManagerFfcMode;

typedef enum {
    GDU_CAMERA_MANAGER_IR_GAIN_MODE_AUTO = 0,
    GDU_CAMERA_MANAGER_IR_GAIN_MODE_LOW = 1,
    GDU_CAMERA_MANAGER_IR_GAIN_MODE_HIGH = 2,
} E_GduCameraManagerIrGainMode;

typedef enum {
    /* not capturing*/
    GDU_CAMERA_MANAGER_CAPTURING_STATE_IDLE = 0,

    /* doing single capture */
    GDU_CAMERA_MANAGER_CAPTURING_STATE_SINGLE = 1,

    /* doing multi capture */
    GDU_CAMERA_MANAGER_CAPTURING_STATE_MULTI = 2,
} E_GduCameraManagerCapturingState;

typedef enum {
    GDU_CAMERA_MANAGER_RECORDING_STATE_IDLE = 0,
    GDU_CAMERA_MANAGER_RECORDING_STATE_STARTING = 1,
    GDU_CAMERA_MANAGER_RECORDING_STATE_RECORDING = 2,
    GDU_CAMERA_MANAGER_RECORDING_STATE_STOPPING = 3,
} E_GduCameraManagerRecordingState;

/*!< Attention: when the remote control is in split-screen mode, the coordinate range of the x-axis is 0 ~ 0.5
* */
typedef struct {
    gdu_f32_t pointX;               /*! x-coordinate of point thermometry, range: 0 ~ 1 */
    gdu_f32_t pointY;               /*! y-coordinate of point thermometry, range: 0 ~ 1 */
} T_GduCameraManagerPointThermometryCoordinate;

typedef struct {
    gdu_f32_t areaTempLtX;          /*! x-coordinate of the upper left corner of the area thermometry, range: 0 ~ 1 */
    gdu_f32_t areaTempLtY;          /*! y-coordinate of the upper left corner of the area thermometry, range: 0 ~ 1 */
    gdu_f32_t areaTempRbX;          /*! x-coordinate of the lower right corner of the area thermometry, range: 0 ~ 1 */
    gdu_f32_t areaTempRbY;          /*! y-coordinate of the lower right corner of the area thermometry, range: 0 ~ 1 */
} T_GduCameraManagerAreaThermometryCoordinate;

//result of point thermometry
typedef struct {
    gdu_f32_t pointX;              /*! x-coordinate of point thermometry, range: 0 ~ 1 */
    gdu_f32_t pointY;              /*! y-coordinate of point thermometry, range: 0 ~ 1 */
    gdu_f32_t pointTemperature;    /*! The temperature of the current point */
} T_GduCameraManagerPointThermometryData;

//result of area thermometry
typedef struct {
    gdu_f32_t areaTempLtX;           /*! x_coordinate of the upper left corner of the current thermometry area */
    gdu_f32_t areaTempLtY;           /*! y_coordinate of the upper left corner of the current thermometry area */
    gdu_f32_t areaTempRbX;           /*! x_coordinate of the lower right corner of the current thermometry area */
    gdu_f32_t areaTempRbY;           /*! y_coordinate of the lower right corner of the current thermometry area */
    gdu_f32_t areaAveTemp;           /*! The average temperature of the current thermometry area */
    gdu_f32_t areaMinTemp;           /*! The minimum temperature of the current thermometry area */
    gdu_f32_t areaMaxTemp;           /*! The maximum temperature of the current thermometry area */
    gdu_f32_t areaMinTempPointX;     /*! x_coordinate of the minimum temperature in the thermometry area */
    gdu_f32_t areaMinTempPointY;     /*! y_coordinate of the minimum temperature in the thermometry area */
    gdu_f32_t areaMaxTempPointX;     /*! x_coordinate of the maximum temperature in the thermometry area */
    gdu_f32_t areaMaxTempPointY;     /*! y_coordinate of the maximum temperature in the thermometry area */
} T_GduCameraManagerAreaThermometryData;

typedef struct {
    E_GduDownloadFileEvent downloadFileEvent;
    uint32_t fileIndex;
    uint32_t fileSize;
    gdu_f32_t progressInPercent;
} T_GduDownloadFilePacketInfo;

typedef struct {
    gdu_f64_t longitude; /*! Range: [-180,180] */
    gdu_f64_t latitude; /*! Range: [-90,90] */
    int32_t altitude; /*! Unit: 0.1m */
    int32_t distance; /*! Unit: 0.1m */
    int16_t screenX; /*! Unit: 0.1% */
    int16_t screenY; /*! Unit: 0.1% */
    bool enable_lidar;
    uint8_t exception;
} T_GduCameraManagerLaserRangingInfo;

typedef struct {
    uint32_t size;
    E_GduCameraManagerStreamSource streamSource[4];
    E_GduCameraManagerStreamSource streamStorage[4];
} T_GduCameraManagerStreamList;

typedef struct {
    E_GduCameraManagerVideoResolution videoResolution;
    E_GduCameraManagerVideoFrameRate videoFrameRate;
} T_GduCameraManagerVideoFormat;

typedef struct {
    uint8_t size;
    union {
        E_GduCameraManagerPhotoStorageFormat photoStorageFormat[16];
        E_GduCameraManagerVideoStorageFormat videoStorageFormat[16];
        E_GduCameraManagerPhotoRatio photoRatioFormat[16];
        E_GduCameraManagerStreamSource streamSource[16];
        E_GduCameraManagerStreamSource streamStorage[16];
        E_GduCameraManagerNightSceneMode nightSceneMode[16];
    };
    uint32_t minValue;
    uint32_t maxValue;
} T_GduCameraManagerRangeList;

typedef struct {
    double lowGainTempMin;
    double lowGainTempMax;
    double highGainTempMin;
    double highGainTempMax;
} T_GduCameraManagerIrTempMeterRange;

typedef struct {
    uint32_t totalCapacity;     /* MByte */
    uint32_t remainCapacity;    /* MByte */
} T_GduCameraManagerStorageInfo;

typedef T_GduReturnCode (*GduCameraManagerDownloadFileDataCallback)(T_GduDownloadFilePacketInfo packetInfo,
                                                                    const uint8_t *data,
                                                                    uint16_t dataLen);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise camera manager module, and user should call this function
 * before using camera manager features.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_Init(void);

/**
 * @brief Deinitialise camera manager module.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_DeInit(void);

/**
 * @brief Get camera type of the selected camera mounted position.
 * @param position: camera mounted position
 * @param cameraType: see references of E_GduCameraType.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetCameraType(E_GduMountPosition position, E_GduCameraType *cameraType);

/**
 * @brief Get camera firmware version of the selected camera mounted position.
 * @param position: camera mounted position
 * @param firmwareVersion: see references of T_GduCameraManagerFirmwareVersion.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetFirmwareVersion(E_GduMountPosition position,
                                                    T_GduCameraManagerFirmwareVersion *firmwareVersion);

/**
 * @brief enable or disable laser period ranging switch
 * @param position: camera mounted position
 * @param laserSwitch: switch value
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_laserPeriodicRanging(E_GduMountPosition position, 
                                                    E_GduCameraManagerLaserRanging laserSwitch);

/**
 * @brief get laser period ranging data
 * @param position: camera mounted position
 * @param status: periodic ranging status
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetLaserPeriodicRangingStatus(T_GduCameraManagerLaserDistanceInfo **status);

/**
 * @brief Get camera connect status.
 * @param position: camera mounted position
 * @param connectStatus: returned value of connect status
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetCameraConnectStatus(E_GduMountPosition position,
                                                        bool *connectStatus);

/**
 * @brief Set camera working mode of the selected camera mounted position.
 * @note Set the camera's work mode to taking pictures, video, playback or
 * download and so on. Please note that you cannot change the mode when a certain task
 * is executing.This action will cost about 1~2s.
 * @param position: camera mounted position
 * @param workMode: see reference of E_GduCameraManagerWorkMode.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetMode(E_GduMountPosition position,
                                         E_GduCameraManagerWorkMode workMode);

/**
 * @brief Get camera working mode of the selected camera mounted position.
 * @param position: camera mounted position
 * @param workMode: see reference of E_GduCameraManagerWorkMode.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetMode(E_GduMountPosition position,
                                         E_GduCameraManagerWorkMode *workMode);

/**
* @brief Set camera shoot mode of the selected camera mounted position.
* @param position: camera mounted position
* @param mode: see reference of E_GduCameraManagerShootPhotoMode.
* @return Execution result.
*/
T_GduReturnCode GduCameraManager_SetShootPhotoMode(E_GduMountPosition position,
                                                   E_GduCameraManagerShootPhotoMode mode);

/**
* @brief Get camera shoot mode of the selected camera mounted position.
* @param position: camera mounted position
* @param mode: see reference of E_GduCameraManagerShootPhotoMode.
* @return Execution result.
*/
T_GduReturnCode GduCameraManager_GetShootPhotoMode(E_GduMountPosition position,
                                                   E_GduCameraManagerShootPhotoMode *takePhotoMode);

/**
 * @brief Start to shoot photo.
 * @note Camera must be in ShootPhoto mode. For thermal imaging camera,
 * Single photo can be taken while recording video. The SD card state should
 * be checked before this method is used to ensure sufficient space exists.
 * @param position: camera mounted position
 * @param mode: see reference of E_GduCameraManagerShootPhotoMode.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_StartShootPhoto(E_GduMountPosition position,
                                                 E_GduCameraManagerShootPhotoMode mode);

/**
 * @brief Stop to shoot photo when you are in taking photo.
 * @note StartShootPhoto has been invoked and the shoot mode is either
 * Interval or Time-lapse. If the shoot mode is set to single, the camera
 * will automatically stop taking the photo once the individual photo is
 * taken.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_StopShootPhoto(E_GduMountPosition position);

/**
 * @brief Get camera capturing state.
 * @note L1/P1 do not support this API.
 * @param position: camera mounted position
 * @param capturingState: result of getting, see E_GduCameraManagerCapturingState.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetCapturingState(E_GduMountPosition position,
                                                   E_GduCameraManagerCapturingState *capturingState);

/**
 * @brief Set the burst count in the burst take-photo mode.
 * @param position: camera mounted position
 * @param count: see reference of E_GduCameraBurstCount.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetPhotoBurstCount(E_GduMountPosition position,
                                                    E_GduCameraBurstCount count);

/**
 * @brief Get the burst count in the burst take-photo mode.
 * @param position: camera mounted position
 * @param count: see reference of E_GduCameraBurstCount.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetPhotoBurstCount(E_GduMountPosition position,
                                                    E_GduCameraBurstCount *count);

/**
 * @brief Set the burst count in the AEB(Automatic Exposure Bracketing) take-photo mode.
 * @param position: camera mounted position
 * @param count: see reference of E_GduCameraManagerPhotoAEBCount.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetPhotoAEBCount(E_GduMountPosition position,
                                                  E_GduCameraManagerPhotoAEBCount count);

/**
 * @brief Get the burst count in the AEB(Automatic Exposure Bracketing) take-photo mode.
 * @param position: camera mounted position
 * @param count: see reference of E_GduCameraManagerPhotoAEBCount.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetPhotoAEBCount(E_GduMountPosition position,
                                                  E_GduCameraManagerPhotoAEBCount *count);

/**
 * @brief Set the parameters in the INTERVAL take-photo mode.
 * @note When in this shoot-photo mode, The camera will capture a photo, wait
 * a specified interval of time, take another photo, and continue in this
 * manner until it has taken the required number of photos. Also supported by
 * thermal imaging camera.
 * @param position: camera mounted position
 * @param intervalSetting: see reference of T_GduCameraPhotoTimeIntervalSettings.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetPhotoTimeIntervalSettings(E_GduMountPosition position,
                                                              T_GduCameraPhotoTimeIntervalSettings intervalSetting);

/**
 * @brief Get the parameters in the INTERVAL take-photo mode.
 * @param position: camera mounted position
 * @param intervalSetting: see reference of T_GduCameraPhotoTimeIntervalSettings.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetPhotoTimeIntervalSettings(E_GduMountPosition position,
                                                              T_GduCameraPhotoTimeIntervalSettings *intervalSetting);

/**
 * @brief Get remain time of interval shooting.
 * @note L1/P1 do not support this API.
 * @param position: camera mounted position
 * @param remainTime: time in seconds.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetIntervalShootingRemainTime(E_GduMountPosition position,
                                                               uint8_t *remainTime);

/**
 * @brief Set camera focus mode of the selected camera mounted position.
 * @note Set the lens focus mode. When the focus mode is auto, the target
 * point is the focal point. When the focus mode is manual, the target point
 * is the zoom out area if the focus assistant is enabled for the manual
 * mode.
 * @param position: camera mounted position
 * @param focusMode: see reference of E_GduCameraManagerFocusMode.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetFocusMode(E_GduMountPosition position,
                                              E_GduCameraManagerFocusMode focusMode);
/**
 * @brief Get camera focus mode of the selected camera mounted position.
 * @param position: camera mounted position
 * @param focusMode: see reference of E_GduCameraManagerFocusMode.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetFocusMode(E_GduMountPosition position,
                                              E_GduCameraManagerFocusMode *focusMode);

/**
 * @brief Set amera focus point of the selected camera mounted position.
 * @note  Sets the lens focus target point. When the focus mode is auto, the
 * target point is the focal point. When the focus mode is manual, the target
 * point is the zoom out area if the focus assistant is enabled for the manual
 * mode.
 * @param position: camera mounted position
 * @param focusPosData: see reference of T_GduCameraManagerFocusPosData.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetFocusTarget(E_GduMountPosition position,
                                                T_GduCameraManagerFocusPosData focusPosData);

/**
 * @brief Get amera focus point of the selected camera mounted position.
 * @param position: camera mounted position
 * @param focusPosData: see reference of T_GduCameraManagerFocusPosData.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetFocusTarget(E_GduMountPosition position,
                                                T_GduCameraManagerFocusPosData *tapFocusPos);

/**
 * @brief Start camera optical zooming of the selected camera mounted position.
 * @note Start changing the focal length of the lens in specified direction
 * with specified speed. Focal length change (zooming) will halt when maximum
 * or minimum focal lengths are reached, or GduCameraManager_StopContinuousOpticalZoom*
 * is called.
 * @param position: camera mounted position
 * @param zoomDirection: optical zoom direction, see reference of E_GduCameraZoomDirection.
 * @param zoomSpeed: optical zoom direction, see reference of E_GduCameraZoomSpeed.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_StartContinuousOpticalZoom(E_GduMountPosition position,
                                                            E_GduCameraZoomDirection zoomDirection,
                                                            E_GduCameraZoomSpeed zoomSpeed);

/**
 * @brief Stop camera optical zooming of the selected camera mounted position.
 * @note Called to stop focal length changing, when it currently is from
 * calling GduCameraManager_StartContinuousOpticalZoom*.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_StopContinuousOpticalZoom(E_GduMountPosition position);

/**
 * @brief Set parameters for camera optical zooming of the selected camera mounted position.
 * @note In this interface, the zoom will set the zoom factor as the your
 * target value.
 * @param position: camera mounted position
 * @param zoomDirection: optical zoom direction, see reference of E_GduCameraZoomDirection.
 * @param factor: target zoom factor.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetOpticalZoomParam(E_GduMountPosition position,
                                                     E_GduCameraZoomDirection zoomDirection,
                                                     gdu_f32_t factor);

/**
 * @brief Get parameters for camera optical zooming of the selected camera mounted position.
 * @param position: camera mounted position
 * @param opticalZoomParam: see reference of T_GduCameraManagerOpticalZoomParam.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetOpticalZoomParam(E_GduMountPosition position,
                                                     T_GduCameraManagerOpticalZoomParam *opticalZoomParam);

/**
 * @brief Set parameters for camera infrared zooming of the selected camera mounted position.
 * @param position: camera mounted position
 * @param factor: target zoom factor.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetInfraredZoomParam(E_GduMountPosition position,
                                                      gdu_f32_t factor);

/**
 * @brief Stop camera optical zooming of the selected camera mounted position.
 * @note Called to stop focal length changing, when it currently is from
 * calling GduCameraManager_StartContinuousOpticalZoom*.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_StopContinuousOpticalZoom(E_GduMountPosition position);

/**
 * @brief Enable/Disable camera's tap-zoom function of the selected camera mounted position.
 * @note TapZoomAtTarget can only be called when tap-zoom is enabled.
 * @param position: camera mounted position
 * @param param: enable/disable
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetTapZoomEnabled(E_GduMountPosition position,
                                                   bool param);

/**
 * @brief Get status of camera's tap-zoom function of the selected camera mounted position.
 * @param position: camera mounted position
 * @param param: enable/disable
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetTapZoomEnabled(E_GduMountPosition position, bool *param);

/**
 * @brief Set camera's tap-zoom multiplier of the selected camera mounted position.
 * @note Tap-zoom uses a multiplier to change the zoom scale when called. The
 * inal zoom scale for a TapZoom will be: Current Zoom Scale x Multiplier.
 * @param position: camera mounted position
 * @param tapZoomMultiplier: The multiplier range is [1,5]. A multiplier of 1 will not change the zoom.
 * hen the multiplier is 1, the zoom scale will not change during TapZoom.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetTapZoomMultiplier(E_GduMountPosition position, uint8_t tapZoomMultiplier);

/**
 * @brief Get camera's tap-zoom multiplier of the selected camera mounted position.
 * @param position: camera mounted position
 * @param tapZoomMultiplier: The multiplier range is [1,5]. A multiplier of 1 will not change the zoom.
 * When the multiplier is 1, the zoom scale will not change during TapZoom.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetTapZoomMultiplier(E_GduMountPosition position, uint8_t *tapZoomMultiplier);

/**
 * @brief Set camera's tap-zoom point of the selected camera mounted position.
 * @note Tap-zoom at the target. It can be called only when TapZoom is
 * enabled. When a new target is set, the gimbal will rotate and locate the
 * target in the center of the screen. At the same time, the camera will zoom
 * by multiplying the TapZoom multiplier
 * @param position: camera mounted position
 * @param tapZoomPos: see reference of T_GduCameraManagerTapZoomPosData.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_TapZoomAtTarget(E_GduMountPosition position,
                                                 T_GduCameraManagerTapZoomPosData tapZoomPos);

/**
 * @brief Get camera focus ring value range.
 * @param position: camera mounted position
 * @param rangeList: returned value of range.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetFocusRingRange(E_GduMountPosition position,
                                                   T_GduCameraManagerRangeList *rangeList);

/**
 * @brief Set camera focus ring value.
 * @param position: camera mounted position
 * @param value: focus ring value.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetFocusRingValue(E_GduMountPosition position,
                                                   uint16_t value);

/**
 * @brief Get camera focus ring value.
 * @param position: camera mounted position
 * @param value: focus ring value to be returned.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetFocusRingValue(E_GduMountPosition position,
                                                   uint16_t *value);

/**
 * @brief Set camera's exposure mode of the selected camera mounted position.
 * @note  The different exposure modes define whether aperture, shutter speed,
 * ISO can be set automatically or manually. Exposure compensation can be
 * changed in all modes except manual mode where it is not settable.
 * @param position: camera mounted position
 * @param mode: see reference of E_GduCameraManagerExposureMode.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetExposureMode(E_GduMountPosition position,
                                                 E_GduCameraManagerExposureMode mode);

/**
 * @brief Get camera's exposure mode of the selected camera mounted position.
 * @note  The different exposure modes define whether aperture, shutter speed,
 * ISO can be set automatically or manually. Exposure compensation can be
 * changed in all modes except manual mode where it is not settable.
 * @param position: camera mounted position
 * @param mode: see reference of E_GduCameraManagerExposureMode.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetExposureMode(E_GduMountPosition position,
                                                 E_GduCameraManagerExposureMode *mode);

/**
 * @brief Set camera's iso value of the selected camera mounted position.
 * @note  ISO value can only be set when the camera exposure mode is in
 * manual mode.
 * @param position: camera mounted position
 * @param iso: see reference of E_GduCameraManagerISO.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetISO(E_GduMountPosition position,
                                        E_GduCameraManagerISO iso);

/**
 * @brief Get camera's iso value of the selected camera mounted position.
 * @param position: camera mounted position
 * @param iso: see reference of E_GduCameraManagerISO.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetISO(E_GduMountPosition position,
                                        E_GduCameraManagerISO *iso);

/**
 * @brief Set camera's aperture size value of the selected camera mounted position.
 * @note The exposure mode must be in GDU_CAMERA_MANAGER_EXPOSURE_MODE_EXPOSURE_MANUAL or
 * GDU_CAMERA_MANAGER_EXPOSURE_MODE_APERTURE_PRIORITY.
 * @param position: camera mounted position
 * @param aperture: see reference of E_GduCameraManagerAperture.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetAperture(E_GduMountPosition position,
                                             E_GduCameraManagerAperture aperture);

/**
 * @brief Get camera's aperture size value of the selected camera mounted position.
 * @param position: camera mounted position
 * @param aperture: see reference of E_GduCameraManagerAperture.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetAperture(E_GduMountPosition position,
                                             E_GduCameraManagerAperture *aperture);

/**
 * @brief Set camera's shutter value of the selected camera mounted position.
 * @note Set the camera shutter speed. The shutter speed should not be set
 * slower than the video frame rate when the camera's mode is RECORD_VIDEO.
 * For example, if the video frame rate is 30fps, the shutterSpeed must be <=
 * 1/30. Precondition: The shutter speed can be set only when the camera
 * exposure mode is GDU_CAMERA_MANAGER_EXPOSURE_MODE_EXPOSURE_MANUAL mode or
 * GDU_CAMERA_MANAGER_EXPOSURE_MODE_SHUTTER_PRIORITY
 * @param position: camera mounted position
 * @param shutterSpeed: see reference of E_GduCameraManagerShutterSpeed.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetShutterSpeed(E_GduMountPosition position,
                                                 E_GduCameraManagerShutterSpeed shutterSpeed);

/**
 * @brief Get camera's shutter value of the selected camera mounted position.
 * @param position: camera mounted position
 * @param shutterSpeed: see reference of E_GduCameraManagerShutterSpeed.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetShutterSpeed(E_GduMountPosition position,
                                                 E_GduCameraManagerShutterSpeed *shutterSpeed);

/**
 * @brief Set camera's EV value of the selected camera mounted position.
 * @note Set the camera's exposure compensation. In order to use this
 * function, set the camera exposure mode to shutter, program or aperture.
 * exposure mode is GDU_CAMERA_MANAGER_EXPOSURE_MODE_EXPOSURE_MANUAL mode or
 * GDU_CAMERA_MANAGER_EXPOSURE_MODE_SHUTTER_PRIORITY or
 * GDU_CAMERA_MANAGER_EXPOSURE_APERTURE_PRIORITY
 * @param position: camera mounted position
 * @param ev: see reference of E_GduCameraManagerExposureCompensation.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetExposureCompensation(E_GduMountPosition position,
                                                         E_GduCameraManagerExposureCompensation ev);

/**
 * @brief Get camera's EV value of the selected camera mounted position.
 * @param position: camera mounted position
 * @param ev: see reference of E_GduCameraManagerExposureCompensation.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetExposureCompensation(E_GduMountPosition position,
                                                         E_GduCameraManagerExposureCompensation *ev);

/**
 * @brief Set AE lock mode.
 * @param position: camera mounted position
 * @param enable: ture to enable, false to diasble.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetAELockEnabled(E_GduMountPosition position,
                                                  bool enable);

/**
 * @brief Get AE lock mode.
 * @note Camera L1/P1 don't support this API.
 * @param position: camera mounted position
 * @param enable: result of AE lock mode.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetAELockEnabled(E_GduMountPosition position,
                                                  bool *enable);

/**
 * @brief Reset camera settings.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_ResetCameraSettings(E_GduMountPosition position);

/**
 * @brief Start to take video of the selected camera mounted position.
 * @note Camera must be in RECORD_VIDEO mode. For thermal imaging camera,
 * user can take Single photo when recording video.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_StartRecordVideo(E_GduMountPosition position);

/**
 * @brief Stop to take video of the selected camera mounted position.
 * @note Precondition: The camera is recording currently.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_StopRecordVideo(E_GduMountPosition position);

/**
 * @brief Get camera recording state.
 * @param position: camera mounted position
 * @param recordingState: result of getting, see E_GduCameraManagerRecordingState.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetRecordingState(E_GduMountPosition position,
                                                   E_GduCameraManagerRecordingState *recordingState);

/**
 * @brief Get camera recording time.
 * @note L1/P1 don not support this API.
 * @param position: camera mounted position
 * @param recordingTime: result of getting, unit is seconds.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetRecordingTime(E_GduMountPosition position,
                                                   uint16_t *recordingTime);
/**
 * @brief Get camera stream source range.
 * @param position: camera mounted position
 * @param rangeList: pointer to the result.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetStreamSourceRange(E_GduMountPosition position,
                                                      T_GduCameraManagerRangeList *rangeList);

/**
 * @brief Choose camera stream source.
 * @param position: camera mounted position
 * @param streamSource: stream source to be chose.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetStreamSource(E_GduMountPosition position,
                                                 E_GduCameraManagerStreamSource streamSource);

/**
 * @brief Get photo storage format range.
 * @param position: camera mounted position
 * @param rangeList: range list returned value
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetPhotoFormatStorageRange(E_GduMountPosition position,
                                                     T_GduCameraManagerRangeList *rangeList);

/**
 * @brief Set photo storage format.
 * @param position: camera mounted position
 * @param format: storage format.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetPhotoFormat(E_GduMountPosition position,
                                                E_GduCameraManagerPhotoStorageFormat format);

/**
 * @brief Get photo storage format.
 * @param position: camera mounted position
 * @param format: returned value of storage format.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetPhotoFormat(E_GduMountPosition position,
                                                E_GduCameraManagerPhotoStorageFormat *format);

/**
 * @brief Get video storage format range.
 * @param position: camera mounted position
 * @param rangeList: range list returned value
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetVideoFormatRange(E_GduMountPosition position,
                                                     T_GduCameraManagerRangeList *rangeList);

/**
 * @brief Set video storage format.
 * @param position: camera mounted position
 * @param format: storage format.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetVideoStorageFormat(E_GduMountPosition position,
                                                E_GduCameraManagerVideoStorageFormat format);

/**
 * @brief Get video storage format.
 * @param position: camera mounted position
 * @param format: returned value of storage format.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetVideoFormat(E_GduMountPosition position,
                                                E_GduCameraManagerVideoStorageFormat *format);

/**
 * @brief Get photo ratio range
 * @param position: camera mounted position
 * @param rangeList: range list returned value
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetPhotoRatioRange(E_GduMountPosition position,
                                                     T_GduCameraManagerRangeList *rangeList);

/**
 * @brief Set camera photo ratio
 * @param position: camera mounted position
 * @param photoRatio: ratio to be set
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetPhotoRatio(E_GduMountPosition position,
                                               E_GduCameraManagerPhotoRatio photoRatio);

/**
 * @brief Get camera photo ratio
 * @param position: camera mounted position
 * @param photoRatio: returned value of photo ratio
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetPhotoRatio(E_GduMountPosition position,
                                               E_GduCameraManagerPhotoRatio *photoRatio);

/**
 * @brief Get camera video resolution and frame rate
 * @param position: camera mounted position
 * @param photoRatio: returned value of video resolution and frame rate
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetVideoResolutionFrameRate(E_GduMountPosition position,
                                                             T_GduCameraManagerVideoFormat *videoParam);

/**
 * @brief Get night scene mode range.
 * @param position: camera mounted position.
 * @param tempRange: returned valued of night scene mode range.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetNightSceneModeRange(E_GduMountPosition position,
                                                        T_GduCameraManagerRangeList *rangeList);

/**
 * @brief Set night scene mode.
 * @note Make sure that stream source is zoom or wide camera.
 * @param position: camera mounted position
 * @param nightSceneMode: night scene mode.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetNightSceneMode(E_GduMountPosition position,
                                                  E_GduCameraManagerNightSceneMode nightSceneMode);

/**
 * @brief Get night scene mode.
 * @param position: camera mounted position
 * @param nightSceneMode: pointer to night scene mode.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetNightSceneMode(E_GduMountPosition position,
                                                  E_GduCameraManagerNightSceneMode *nightSceneMode);

/**
 * @brief Get range of stream source(s) can be storaged when capturing or recording.
 * @param position: camera mounted position.
 * @param rangeList: returned value of range, in member streamStorage.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetStreamStorageRange(E_GduMountPosition position,
                                                      T_GduCameraManagerRangeList *rangeList);

/**
 * @brief Select capture or recording stream(s) to store.
 * @note Precondition: set camera's work corresponding to streamType
 * @param position: camera mounted position.
 * @param streamType: capture mode or recording mode.
 * @param streamStorageList: Pointer to the struct that contains stream list.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetCaptureRecordingStreams(E_GduMountPosition position,
                                                            E_GduCameraManagerCaptureOrRecording streamType,
                                                            T_GduCameraManagerStreamList *streamStorageList);

/**
 * @brief Get the stream(s) of capture or recording mode to be storaged.
 * @param position: camera mounted position.
 * @param streamType: capture mode or recording mode.
 * @param streamSourceList: the real return value.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetCaptureRecordingStreams(E_GduMountPosition position,
                                                            E_GduCameraManagerCaptureOrRecording streamType,
                                                            T_GduCameraManagerStreamList *streamStorageList);

/**
 * @brief Turn on/off synchronized split screen zoom function.
 * @param position: camera mounted position.
 * @param enable: set true to turn on, false to turn off.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetSynchronizedSplitScreenZoomEnabled(E_GduMountPosition position, bool enable);


/**
 * @brief Set suffix name of directory or file.
 * @note For file name, the setting is only valid once.
 * @param position: camera mounted position.
 * @param nameType: see E_GduCameraManagerExpandNameType, select to set name of directory or file.
 * @param nameSize: size of name string, must be in rang of 1 ~ 239.
 * @param nameStr: Content of custom suffix name.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetCustomExpandName(E_GduMountPosition position,
                                                     E_GduCameraManagerExpandNameType nameType,
                                                     const uint8_t *nameStr,
                                                     uint32_t nameSize);

/**
 * @brief Get custom past of lastest directory or file name
 * @param position: camera mounted position
 * @param nameType: to choose directory or file to get custom name
 * @param nameStr: name string buffer
 * @param nameSize: its tell the max size of nameStr and changed to to the actually size of
 * name string when function finished.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetCustomExpandName(E_GduMountPosition position,
                                                     E_GduCameraManagerExpandNameType nameType,
                                                     uint8_t *nameStr,
                                                     uint32_t *nameSize);


/**
 * @brief Download selected camera media file list.
 * @note The interface is a synchronous interface, which occupies more CPU resources when using it.
 * If the download file fails, the timeout time is 3S.
 * @param position: the mount position of the camera
 * @param fileList: the pointer to the downloaded camera file list
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_DownloadFileList(E_GduMountPosition position, T_GduCameraManagerFileList *fileList);

/**
 * @brief Download selected camera media file list by slices.
 * @note The interface is a synchronous interface, which occupies more CPU resources when using it.
 * If the download file fails, the timeout time is 3S.
 * @param position: the mount position of the camera
 * @param sliceConfig: the slices config for downloading file list
 * @param fileList: the pointer to the downloaded camera file list
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_DownloadFileListBySlices(E_GduMountPosition position,
                                                          T_GduCameraManagerSliceConfig sliceConfig,
                                                          T_GduCameraManagerFileList *fileList);

/**
* @brief Download selected camera media file list by path.
* @note The interface is a synchronous interface, which occupies more CPU resources when using it.
* If the download file fails, the timeout time is 3S.
* @param position: the mount position of the camera
* @param sliceConfig: the slices config for downloading file list
* @param fileList: the pointer to the downloaded camera file list
* @return Execution result.
*/
T_GduReturnCode GduCameraManager_DownloadFileByPath(E_GduMountPosition position, uint8_t *path);

/**
 * @brief Regsiter selected camera download file data callback,
 * @param position: the mount position of the camera
 * @param callback: the download file data callback
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_RegDownloadFileDataCallback(E_GduMountPosition position,
                                                             GduCameraManagerDownloadFileDataCallback callback);

/**
 * @brief Download selected camera media file by file index.
 * @note Only support download one file at the same time, the new file download need wait for the previous file
 * download finished.The interface is a synchronous interface, which occupies more CPU resources when using it.
 * If the download file fails, the timeout time is 3S.
 * @param position: the mount position of the camera
 * @param fileIndex: the index of the camera media file
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_DownloadFileByIndex(E_GduMountPosition position, uint32_t fileIndex);

/**
 * @brief Format SD card.
 * @param position: the mount position of the camera
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_FormatStorage(E_GduMountPosition position);

/**
 * @brief Get storage info of SD card.
 * @note Camera L1/P1 don't support this API.
 * @param position: the mount position of the camera
 * @param storageInfo: result of getting, storage info of SD card.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetStorageInfo(E_GduMountPosition position,
                                                T_GduCameraManagerStorageInfo *storageInfo);

/**
 * @brief Delete selected camera media file by file path.
 * @param position: the mount position of the camera
 * @param filePath: the filePath of the camera media file
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_DeleteFileByPath(E_GduMountPosition position, char * filePath);

/**
 * @brief Get the camera laser ranging info of the selected camera mounted position.
 * @note Maximum data update frequency: 5Hz.
 * @param position: the mount position of the camera
 * @param laserRangingInfo: the pointer to the camera laser ranging info
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetLaserRangingInfo(E_GduMountPosition position,
                                                     T_GduCameraManagerLaserRangingInfo *laserRangingInfo);

/**
 * @brief Set point thermometry coordinates of the selected camera mounted position.
 * @param position: camera mounted position
 * @param pointCoordinate: point thermometry coordinates
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetPointThermometryCoordinate(E_GduMountPosition position,
                                                               T_GduCameraManagerPointThermometryCoordinate pointCoordinate);

/**
 * @brief Get point thermometry result.
 * @note Before get point thermometry data from camera, GduCameraManager_SetPointThermometryCoordinate()
 * function has to be called.
 * @param position: camera mounted position
 * @param pointThermometryData: point thermometry result
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetPointThermometryData(E_GduMountPosition position,
														 T_GduCameraManagerPointThermometryCoordinate pointCoordinate,
                                                         T_GduCameraManagerPointThermometryData *pointThermometryData);

/**
 * @brief Set area thermometry coordinates of the selected camera mounted position.
 * @param position: camera mounted position
 * @param areaCoordinate: area thermometry coordinates
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetAreaThermometryCoordinate(E_GduMountPosition position,
                                                              T_GduCameraManagerAreaThermometryCoordinate areaCoordinate);

/**
 * @brief Get area thermometry result.
 * @note Before get area thermometry data from camera, GduCameraManager_SetAreaThermometryCoordinate()
 * function has to be called.
 * @param position: camera mounted position
 * @param areaThermometryData: area thermometry result
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetAreaThermometryData(E_GduMountPosition position,
                                                        T_GduCameraManagerAreaThermometryData *areaThermometryData);

/**
 * @brief Set FFC mode.
 * @param position: camera mounted position.
 * @param ffcMode: mode to be set.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetFfcMode(E_GduMountPosition position, E_GduCameraManagerFfcMode ffcMode);

/**
 * @brief Trigger FFC one time.
 * @param position: camera mounted position.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_TriggerFfc(E_GduMountPosition position);

/**
 * @brief Set infrared camera gaim mode.
 * @param position: camera mounted position.
 * @param gainMode: gain mode to set.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetInfraredCameraGainMode(E_GduMountPosition position,
                                               E_GduCameraManagerIrGainMode gainMode);

/**
 * @brief Get temprature range of infrared camera.
 * @param position: camera mounted position.
 * @param tempRange: returned valued of temperature range.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetInfraredCameraGainModeTemperatureRange(E_GduMountPosition position,
                                                               T_GduCameraManagerIrTempMeterRange *tempRange);

/**
 * @brief Set camera metering mode.
 * @param position: camera mounted position
 * @param meteringMode: metering mode
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetMeteringMode(E_GduMountPosition position,
                                                 E_GduCameraManagerMeteringMode meteringMode);

/**
 * @brief Get camera metering mode.
 * @param position: camera mounted position
 * @param meteringMode: pointer to returned value of metering mode
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetMeteringMode(E_GduMountPosition position,
                                                 E_GduCameraManagerMeteringMode *meteringMode);

/**
 * @brief Get range of metering point.
 * @param position: camera mounted position
 * @param hrzNum: returned value, horizontal range.
 * @param vtcNum: returned value, vertical range.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetMeteringPointRegionRange(E_GduMountPosition position,
                                                             uint8_t *hrzNum, uint8_t *vtcNum);
/**
 * @brief Set metrting point.
 * @param position: camera mounted position
 * @param x: Horizontal coordinate value, should be no greater than hrzNum - 1.
 * @param y: Horizontal coordinate value, should be no greater than vtcNum - 1.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_SetMeteringPoint(E_GduMountPosition position,
                                                  uint8_t x, uint8_t y);

/**
 * @brief Get camera metering mode.
 * @param position: camera mounted position
 * @param x: returned valued, current metering point in horizontal coordinate.
 * @param y: returned valued, current metering point in vertical coordinate.
 * @return Execution result.
 */
T_GduReturnCode GduCameraManager_GetMeteringPoint(E_GduMountPosition position,
                                                  uint8_t *x, uint8_t *y);

#ifdef __cplusplus
}
#endif

#endif // GDU_CAMERA_MANAGER_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
