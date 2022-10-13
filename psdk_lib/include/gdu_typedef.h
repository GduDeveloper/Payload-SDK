/**
 ********************************************************************
 * @file    gdu_typedef.h
 * @brief   This is the header file for "gdu_typedef.c", defining the structure and
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
#ifndef GDU_TYPEDEF_H
#define GDU_TYPEDEF_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gdu_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define GDU_PI                     (3.14159265358979323846f)
#define GDU_FILE_NAME_SIZE_MAX     256
#define GDU_FILE_PATH_SIZE_MAX     (GDU_FILE_NAME_SIZE_MAX + 256)
#define GDU_IP_ADDR_STR_SIZE_MAX   15
#define GDU_MD5_BUFFER_LEN         16

#define GDU_SUBSCRIPTION_MODULE_INDEX_OFFSET    24u
#define GDU_SUBSCRIPTION_MODULE_INDEX_MASK      0xFF000000u
#define GDU_SUBSCRIPTION_TOPIC_CODE_OFFSET      0u
#define GDU_SUBSCRIPTION_TOPIC_CODE_MASK        0x00FFFFFFu

#define GDU_DATA_SUBSCRIPTION_TOPIC(subscriptionModule, topicCode) \
(uint32_t)\
(((((uint32_t)(subscriptionModule)) << (GDU_SUBSCRIPTION_MODULE_INDEX_OFFSET)) & (GDU_SUBSCRIPTION_MODULE_INDEX_MASK)) | \
((((uint32_t)(topicCode)) << (GDU_SUBSCRIPTION_TOPIC_CODE_OFFSET)) & (GDU_SUBSCRIPTION_TOPIC_CODE_MASK)))

/**
 * @brief Type define double as gdu_f64_t.
 */
typedef double gdu_f64_t;
/**
 * @brief Type define float as gdu_f32_t.
 */
typedef float gdu_f32_t;
/**
 * @brief Type define uint64 as T_GduReturnCode.
 * @details The type can be any value of ::GduErrorCode.
 */
typedef uint64_t T_GduReturnCode;

typedef enum {
    GDU_MOUNT_POSITION_TYPE_UNKNOWN = 0,
    GDU_MOUNT_POSITION_TYPE_PAYLOAD_PORT = 1,
    GDU_MOUNT_POSITION_TYPE_EXTENSION_PORT = 2
} E_GduMountPositionType;

typedef enum {
    GDU_MOUNT_POSITION_UNKNOWN = 0,
    GDU_MOUNT_POSITION_PAYLOAD_PORT_NO1 = 1,
    GDU_MOUNT_POSITION_PAYLOAD_PORT_NO2 = 2,
    GDU_MOUNT_POSITION_PAYLOAD_PORT_NO3 = 3,
    GDU_MOUNT_POSITION_EXTENSION_PORT = 4
} E_GduMountPosition;

typedef enum {
    GDU_AIRCRAFT_SERIES_UNKNOWN = 0,
    GDU_AIRCRAFT_SERIES_M200_V2 = 1,
    GDU_AIRCRAFT_SERIES_M300 = 2,
} E_GduAircraftSeries;

typedef enum {
    GDU_AIRCRAFT_TYPE_UNKNOWN = 0, /*!< Aircraft type is unknown. */
    GDU_AIRCRAFT_TYPE_M200_V2 = 44, /*!< Aircraft type is Matrice 200 V2. */
    GDU_AIRCRAFT_TYPE_M210_V2 = 45, /*!< Aircraft type is Matrice 220 V2. */
    GDU_AIRCRAFT_TYPE_M210RTK_V2 = 46, /*!< Aircraft type is Matrice 210 RTK V2. */
    GDU_AIRCRAFT_TYPE_M300_RTK = 60, /*!< Aircraft type is Matrice 300 RTK. */
} E_GduAircraftType;

/**
 * @brief Camera type.
 */
typedef enum {
    GDU_CAMERA_TYPE_UNKNOWN = 0, /*!< Camera type is unknown. */
    GDU_CAMERA_TYPE_Z30 = 20, /*!< Camera type is Z30. */
    GDU_CAMERA_TYPE_XT2 = 26, /*!< Camera type is XT2. */
    GDU_CAMERA_TYPE_PSDK = 31, /*!< Camera type is third party camera based on Payload SDK. */
    GDU_CAMERA_TYPE_XTS = 41, /*!< Camera type is XT S. */
    GDU_CAMERA_TYPE_H20 = 42, /*!< Camera type is H20. */
    GDU_CAMERA_TYPE_H20T = 43, /*!< Camera type is H20T. */
    GDU_CAMERA_TYPE_P1 = 50, /*!< Camera type is P1. */
    GDU_CAMERA_TYPE_L1, /*!< Camera type is L1. */
} E_GduCameraType;

/**
 * @brief Camera optical zoom specifies.
 */
typedef struct {
    uint16_t maxFocalLength; /*!< The maximum focal length of the lens, unit: 0.1mm. */
    uint16_t minFocalLength; /*!< The minimum focal length of the lens, unit: 0.1mm. */
    uint16_t focalLengthStep; /*!< The minimum interval of focal length change, unit: 0.1mm. */
} T_GduCameraOpticalZoomSpec;

/**
 * @brief Gimbal work mode, specifies how gimbal follow aircraft movement.
 */
typedef enum {
    GDU_GIMBAL_MODE_FREE = 0, /*!< Free mode, fix gimbal attitude in the ground coordinate, ignoring movement of aircraft. */
    GDU_GIMBAL_MODE_FPV = 1, /*!< FPV (First Person View) mode, only control roll and yaw angle of gimbal in the ground coordinate to follow aircraft. */
    GDU_GIMBAL_MODE_YAW_FOLLOW = 2, /*!< Yaw follow mode, only control yaw angle of gimbal in the ground coordinate to follow aircraft. */
} E_GduGimbalMode;

/**
 * @brief Gimbal rotation mode, specifies control style.
 */
typedef enum {
    GDU_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE = 0, /*!< Relative angle rotation mode, represents rotating gimbal specified angles based on current angles. */
    GDU_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE = 1, /*!< Absolute angle rotation mode, represents rotating gimbal to specified angles in the ground coordinate. */
    GDU_GIMBAL_ROTATION_MODE_SPEED = 2, /*!< Speed rotation mode, specifies rotation speed of gimbal in the ground coordinate. */
} E_GduGimbalRotationMode;

/**
 * @brief Mobile APP system language.
 */
typedef enum {
    GDU_MOBILE_APP_LANGUAGE_UNKNOWN = 255, /*!< The system language of the mobile app is unknown */
    GDU_MOBILE_APP_LANGUAGE_ENGLISH = 0, /*!< The system language of the mobile app is English */
    GDU_MOBILE_APP_LANGUAGE_CHINESE = 1, /*!< The system language of the mobile app is Chinese */
    GDU_MOBILE_APP_LANGUAGE_JAPANESE = 2, /*!< The system language of the mobile app is Japanese */
    GDU_MOBILE_APP_LANGUAGE_FRENCH = 3, /*!< The system language of the mobile app is French */
} E_GduMobileAppLanguage;

/**
 * @brief Mobile APP screen size type.
 */
typedef enum {
    GDU_MOBILE_APP_SCREEN_TYPE_UNKNOWN = 255, /*!< Mobile APP screen type is unknown. */
    GDU_MOBILE_APP_SCREEN_TYPE_BIG_SCREEN = 0, /*!< The big screen of mobile device refers to a screen
                                                                * size greater than or equal to 6 inches. */
    GDU_MOBILE_APP_SCREEN_TYPE_LITTLE_SCREEN = 1, /*!< The little screen of mobile device refers to a
                                                                    * screen size less than 6 inches. */
} E_GduMobileAppScreenType;

/**
 * @brief Subscription frequency type.
 */
typedef enum {
    GDU_DATA_SUBSCRIPTION_TOPIC_1_HZ = 1,
    GDU_DATA_SUBSCRIPTION_TOPIC_5_HZ = 5,
    GDU_DATA_SUBSCRIPTION_TOPIC_10_HZ = 10,
//    GDU_DATA_SUBSCRIPTION_TOPIC_50_HZ = 50,
//    GDU_DATA_SUBSCRIPTION_TOPIC_100_HZ = 100,
//    GDU_DATA_SUBSCRIPTION_TOPIC_200_HZ = 200,
//    GDU_DATA_SUBSCRIPTION_TOPIC_400_HZ = 400,
} E_GduDataSubscriptionTopicFreq;

/**
 * @brief GDU module enum for defining data subscription module.
 */
typedef enum {
    GDU_DATA_SUBSCRIPTION_MODULE_FC = 0,
    GDU_DATA_SUBSCRIPTION_MODULE_CAMERA,
    GDU_DATA_SUBSCRIPTION_MODULE_ERROR,
} E_GduDataSubscriptionModule;

/**
 * @brief SDK adapter type.
 */
typedef enum {
    GDU_SDK_ADAPTER_TYPE_UNKNOWN = 0, /*!< SDK adapter type is unknown. */
    GDU_SDK_ADAPTER_TYPE_SKYPORT_V2 = 1, /*!< SDK adapter type is Skyport V2. */
    GDU_SDK_ADAPTER_TYPE_XPORT = 2, /*!< SDK adapter type is X-Port. */
    GDU_SDK_ADAPTER_TYPE_NONE = 3, /*!< don't have any adapter outside */
} E_GduSdkAdapterType;

typedef enum {
    GDU_CHANNEL_ADDRESS_UNKNOWN = 0,
    GDU_CHANNEL_ADDRESS_PAYLOAD_PORT_NO1,
    GDU_CHANNEL_ADDRESS_PAYLOAD_PORT_NO2,
    GDU_CHANNEL_ADDRESS_PAYLOAD_PORT_NO3,
    GDU_CHANNEL_ADDRESS_EXTENSION_PORT,
    GDU_CHANNEL_ADDRESS_MASTER_RC_APP,
    GDU_CHANNEL_ADDRESS_SLAVE_RC_APP,
} E_GduChannelAddress;

/**
 * @brief Camera focus target point when in focus mode.
 */
typedef struct {
    gdu_f32_t focusX; /*!< Specifies horizontal zone coordinate. This parameter is between 0 and 1.
                            The point [0.0, 0.0] represents the top-left angle of the screen.*/
    gdu_f32_t focusY; /*!< Specifies vertical zone coordinate. This parameter is between 0 and 1. */
} T_GduCameraPointInScreen;

/**
 * @brief Camera time interval settings when in interval shootPhoto mode.
 */
typedef struct {
    uint8_t captureCount; /*!< Specifies the total capture count of interval settings.
 *                             0:reserve 1~254:number 255:keep capturing till stop */
    uint16_t timeIntervalSeconds; /*!< Specifies the interval time between two captures, unit: s*/
} T_GduCameraPhotoTimeIntervalSettings;

/**
 * @brief Camera zoom speed.
 */
typedef enum {
    GDU_CAMERA_ZOOM_SPEED_SLOWEST = 72, /*!< Lens zooms in slowest speed. */
    GDU_CAMERA_ZOOM_SPEED_SLOW = 73, /*!< Lens zooms in slow speed. */
    GDU_CAMERA_ZOOM_SPEED_MODERATELY_SLOW = 74, /*!< Lens zooms in speed slightly slower than normal speed. */
    GDU_CAMERA_ZOOM_SPEED_NORMAL = 75, /*!< Lens zooms in normal speed. */
    GDU_CAMERA_ZOOM_SPEED_MODERATELY_FAST = 76, /*!< Lens zooms very in speed slightly faster than normal speed. */
    GDU_CAMERA_ZOOM_SPEED_FAST = 77, /*!< Lens zooms very in fast speed. */
    GDU_CAMERA_ZOOM_SPEED_FASTEST = 78, /*!< Lens zooms very in fastest speed. */
} E_GduCameraZoomSpeed;

typedef enum {
    /*! The number of pictures to continuously take each time in BURST mode is 2
     */
    GDU_CAMERA_BURST_COUNT_2  = 2,
    /*! The number of pictures to continuously take each time in BURST mode is 3
     */
    GDU_CAMERA_BURST_COUNT_3 = 3,
    /*! The number of pictures to continuously take each time in BURST mode is 5
     */
    GDU_CAMERA_BURST_COUNT_5 = 5,
    /*! The number of pictures to continuously take each time in BURST mode is 7
     */
    GDU_CAMERA_BURST_COUNT_7 = 7,
    /*! The number of pictures to continuously take at one time in BURST mode is
     * 10, Only supported by X4S camera, X5S camera and Phantom 4 Pro camera.
     */
    GDU_CAMERA_BURST_COUNT_10 = 10,
    /*! The number of pictures to continuously take at one time in BURST mode is
     * 14, Only supported by X4S camera, X5S camera and Phantom 4 Pro camera.
     */
    GDU_CAMERA_BURST_COUNT_14 = 14,
    /*!	The camera burst shoot count value is unknown.
     */
    GDU_CAMERA_BURST_COUNT_KNOWN = 0xFF,
} E_GduCameraBurstCount;

/**
 * @brief Camera zoom direction.
 */
typedef enum {
    GDU_CAMERA_ZOOM_DIRECTION_OUT = 0, /*!< The lens moves in the far direction, the zoom factor becomes smaller. */
    GDU_CAMERA_ZOOM_DIRECTION_IN = 1, /*!< The lens moves in the near direction, the zoom factor becomes larger. */
} E_GduCameraZoomDirection;
#pragma pack (1)

/**
 * @brief Data channel state.
 */
typedef struct {
    /*! Realtime bandwidth limitation, varying with link status of aircraft system for some channels, such as data
     * stream, video stream and download stream. Must ensure actual bandwidth of data transmission is less than
     * realtime bandwidth limitation, unit: byte/s. */
    int32_t realtimeBandwidthLimit;

    /*! Realtime actual transmission bandwidth of data transmission channel calculated before flow controller, unit: byte/s. */
    int32_t realtimeBandwidthBeforeFlowController;

    /*! Realtime actual transmission bandwidth of data transmission channel calculated after flow controller, unit:
     * byte/s. If specified channel without flow controller. the value is equal to
     * ::realtimeBandwidthBeforeFlowController. */
    int32_t realtimeBandwidthAfterFlowController;

    /*! State specified whether the channel is busy or not. When data can not be sent to the endpoint directly, instead be
     * sent to buffer of flow controller or discarded, the busy state will be set. At this time, the user should stop
     * transmitting data or decrease the amount of data to be transmitted via this channel. When data bandwidth restore to
     * normal, this state will be clear again. */
    bool busyState;
} T_GduDataChannelState;

typedef struct Vector3d {
    int32_t x; /*!< Specifies int32 value of x for vector. */
    int32_t y; /*!< Specifies int32 value of y for vector. */
    int32_t z; /*!< Specifies int32 value of z for vector. */
} T_GduVector3d;

typedef struct Vector3f {
    gdu_f32_t x; /*!< Specifies float value of x for vector. */
    gdu_f32_t y; /*!< Specifies float value of y for vector. */
    gdu_f32_t z; /*!< Specifies float value of z for vector. */
} T_GduVector3f;

typedef struct {
    int32_t pitch; /*!< Specifies int32 value of pitch for attitude. */
    int32_t roll; /*!< Specifies int32 value of roll for attitude */
    int32_t yaw; /*!< Specifies int32 value of yaw for attitude */
} T_GduAttitude3d;

typedef struct {
    gdu_f32_t pitch; /*!< Specifies float value of pitch for attitude. */
    gdu_f32_t roll; /*!< Specifies float value of roll for attitude */
    gdu_f32_t yaw; /*!< Specifies float value of yaw for attitude */
} T_GduAttitude3f;

typedef struct {
    gdu_f32_t q0; /*!< w, when converted to a rotation matrix or Euler angles. */
    gdu_f32_t q1; /*!< x, when converted to a rotation matrix or Euler angles. */
    gdu_f32_t q2; /*!< y, when converted to a rotation matrix or Euler angles. */
    gdu_f32_t q3; /*!< z, when converted to a rotation matrix or Euler angles. */
} T_GduQuaternion4f;

/**
 * @brief Timestamp data structure.
 */
typedef struct {
    uint32_t millisecond; /*!< Millisecond. */
    uint32_t microsecond; /*!< Microsecond. */
} T_GduDataTimestamp;

#pragma pack ()
/**
 * @brief Prototype of callback function used to receive data of topic.
 * @warning User can not execute blocking style operations or functions in the callback function, because that will block PSDK
 * root thread, causing problems such as slow system response, payload disconnection or infinite loop.
 * @param data: pointer to data of the topic, user need transfer type of this pointer to the corresponding data structure
 * pointer for getting every item of the topic conveniently.
 * @param dataSize: the size of memory space pointed by data argument, equal to data structure size corresponding to the topic.
 * @param timestamp: pointer to timestamp corresponding this data.
 * @return Execution result.
 */
typedef T_GduReturnCode (*GduReceiveDataOfTopicCallback)(const uint8_t *data, uint16_t dataSize,
                                                         const T_GduDataTimestamp *timestamp);

/* Exported functions --------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif // GDU_TYPEDEF_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
