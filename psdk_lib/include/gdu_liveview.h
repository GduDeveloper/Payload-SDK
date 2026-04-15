/**
 ********************************************************************
 * @file    gdu_liveview.h
 * @brief   This is the header file for "gdu_liveview.c", defining the structure and
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
#ifndef GDU_LIVEVIEW_H
#define GDU_LIVEVIEW_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Liveview camera mount position.
 */
typedef enum {
    GDU_LIVEVIEW_CAMERA_POSITION_NO_1 = GDU_MOUNT_POSITION_PAYLOAD_PORT_NO1,
    GDU_LIVEVIEW_CAMERA_POSITION_NO_2 = GDU_MOUNT_POSITION_PAYLOAD_PORT_NO2,
    GDU_LIVEVIEW_CAMERA_POSITION_NO_3 = GDU_MOUNT_POSITION_PAYLOAD_PORT_NO3,
    GDU_LIVEVIEW_CAMERA_POSITION_FPV = 7
} E_GduLiveViewCameraPosition;

/**
 * @brief Liveview camera stream source.
 */
typedef enum {
    GDU_LIVEVIEW_CAMERA_SOURCE_DEFAULT = 0,
    GDU_LIVEVIEW_CAMERA_SOURCE_H20_WIDE = 1,
    GDU_LIVEVIEW_CAMERA_SOURCE_H20_ZOOM = 2,
    GDU_LIVEVIEW_CAMERA_SOURCE_H20T_WIDE = 1,
    GDU_LIVEVIEW_CAMERA_SOURCE_H20T_ZOOM = 2,
    GDU_LIVEVIEW_CAMERA_SOURCE_H20T_IR = 3,
    GDU_LIVEVIEW_CAMERA_SOURCE_M30_ZOOM = 1,
    GDU_LIVEVIEW_CAMERA_SOURCE_M30_WIDE = 2,
    GDU_LIVEVIEW_CAMERA_SOURCE_M30T_ZOOM = 1,
    GDU_LIVEVIEW_CAMERA_SOURCE_M30T_WIDE = 2,
    GDU_LIVEVIEW_CAMERA_SOURCE_M30T_IR = 3
} E_GduLiveViewCameraSource;

/**
 * @brief Liveview camera h264 stream callback.
 */
typedef void (*GduLiveview_H264Callback)(E_GduLiveViewCameraPosition position, const uint8_t *buf, uint32_t len);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize the liveview module.
 * @note The interface initialization needs to be after GduCore_Init.
 * @return Execution result.
 */
T_GduReturnCode GduLiveview_Init(void);

/**
 * @brief Deinitialize the liveview module.
 * @return Execution result.
 */
T_GduReturnCode GduLiveview_Deinit(void);

/**
 * @brief Start the FPV or Camera H264 Stream by selected position.
 * @param position: point out which camera to output the H264 stream
 * @param source: point out which sub camera to output the H264 stream
 * @param callback: callback function that is called in a callback thread when a new h264 frame is received
 * @return Execution result.
 */
T_GduReturnCode GduLiveview_StartH264Stream(E_GduLiveViewCameraPosition position, E_GduLiveViewCameraSource source,
                                            GduLiveview_H264Callback callback);

/**
 * @brief Stop the FPV or Camera H264 Stream by selected position.
 * @param position: point out which camera to output the H264 stream
 * @return Execution result.
 */
T_GduReturnCode GduLiveview_StopH264Stream(E_GduLiveViewCameraPosition position);

#ifdef __cplusplus
}
#endif

#endif // GDU_LIVEVIEW_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
