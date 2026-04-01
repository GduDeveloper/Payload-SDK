/**
 ********************************************************************
 * @file    gdu_gimbal_manager.h
 * @brief   This is the header file for "gdu_gimbal_manager.c", defining the structure and
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
#ifndef GDU_GIMBAL_MANAGER_H
#define GDU_GIMBAL_MANAGER_H

/* Includes ------------------------------------------------------------------*/
#include <gdu_gimbal.h>
#include "gdu_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Gimbal manager rotation command property.
 */
typedef struct {
    E_GduGimbalRotationMode rotationMode; /*!< Rotation gimbal mode. */
    gdu_f32_t pitch; /*!< Pitch angle in degree, unit: deg */
    gdu_f32_t roll; /*!< Roll angle in degree, unit: deg */
    gdu_f32_t yaw; /*!< Yaw angle in degree, unit: deg */
    gdu_f64_t time; /*!< Expected execution time for gimbal rotation, in seconds. */
} T_GduGimbalManagerRotation;


typedef struct {
    uint8_t lock; /*wether head of aircraft and gimbal locked. 0:gimbal rotate only 1: gimbal and aircraft rotate together */
    gdu_f64_t latitude;  /*!< unit: deg */
    gdu_f64_t longitude; /*!< unit: deg */
    gdu_f64_t height; /*!< unit: cm */
} T_GduGimbalManagerLookAt;
/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize the gimbal manager module.
 * @note This interface must be initialized after GduCore_Init.
 * @return Execution result.
 */
T_GduReturnCode GduGimbalManager_Init(void);

/**
 * @brief DeInitialize the gimbal manager module.
 * @return Execution result.
 */
T_GduReturnCode GduGimbalManager_Deinit(void);

/**
 * @brief move the gimbal to the specified coordinates .
 * @param mountPosition: unused
 * @param info: transfer th specified coordinates and move mode
 * @return Execution result.
 */
T_GduReturnCode GduGimbalManager_LookAt(E_GduMountPosition mountPosition, T_GduGimbalManagerLookAt info);
/**
 * @brief Set the work mode of the gimbal.
 * @param mountPosition: gimbal mount position, input limit see enum E_GduMountPosition
 * @param mode: gimbal work mode, input limit see enum E_GduGimbalMode
 * @return Execution result.
 */
T_GduReturnCode GduGimbalManager_SetMode(E_GduMountPosition mountPosition, E_GduGimbalMode mode);

/**
 * @brief Reset the pitch and yaw of the gimbal.
 * @param mountPosition: gimbal mount position, input limit see enum E_GduMountPosition
 * @param mode: reset mode, input limit see enum E_GduGimbalResetMode
 * @return Execution result.
 */
T_GduReturnCode GduGimbalManager_Reset(E_GduMountPosition mountPosition, E_GduGimbalResetMode resetMode);

/**
 * @brief Rotate the angle of the gimbal.
 * @param mountPosition: gimbal mount position, input limit see enum E_GduMountPosition
 * @param rotation: the rotation parameters to be executed on the target gimbal, including the rotation mode, target
 * angle value and executed time, refer to T_GduGimbalManagerRotation； if you use GDU_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE mode, angle unit: 0.01 deg  
 * if use GDU_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE mode, angle unit: 1 deg
 * @return Execution result.
 */
T_GduReturnCode GduGimbalManager_Rotate(E_GduMountPosition mountPosition, T_GduGimbalManagerRotation rotation);

/*!
 * @brief Prototype of callback function used to enable or disable extended pitch axis angle range.
 * @note Switching the gimbal limit euler angle of pitch axis to the extended limit angle or the default limit
 * angle.
 * @param mountPosition: gimbal mount position, input limit see enum E_GduMountPosition
 * @param enabledFlag: flag specifying whether to enable or disable the extended pitch axis angle range..
 * @return Execution result.
 */
T_GduReturnCode GduGimbalManager_SetPitchRangeExtensionEnabled(E_GduMountPosition mountPosition, bool enabledFlag);

/**
 * @brief Set max speed percentage for gimbal controller.
 * @param mountPosition: gimbal mount position, input limit see enum E_GduMountPosition
 * @param axis: axis to be set.
 * @param maxSpeedPercentage: max speed value. Recommended calculation formula is "spd = default_max_spd * x / 100",
 * where 'x' is the default maximum speed value. Range from 1 to 100.
 * @return Execution result.
 */
T_GduReturnCode GduGimbalManager_SetControllerMaxSpeedPercentage(E_GduMountPosition mountPosition, E_GduGimbalAxis axis,
                                                                 uint8_t maxSpeedPercentage);

/**
 * @brief Set smooth factor for gimbal controller, using for smooth control.
 * @param mountPosition: gimbal mount position, input limit see enum E_GduMountPosition
 * @param axis: axis to be set.
 * @param smoothingFactor: smooth factor. A larger value results in slower gimbal acceleration. Recommended
 * calculation formula is "acc = 10000 * (0.8 ^ (1 + x)) deg/s^2", x is smooth factor. Range from 0 to 30.
 * @return Execution result.
 */
T_GduReturnCode GduGimbalManager_SetControllerSmoothFactor(E_GduMountPosition mountPosition, E_GduGimbalAxis axis,
                                                           uint8_t smoothingFactor);

/**
 * @brief Restore factory settings of gimbal, including fine tune angle, pitch angle extension enable flag and max
 * speed etc.
 * @param mountPosition: gimbal mount position, input limit see enum E_GduMountPosition
 * @return Execution result.
 */
T_GduReturnCode GduGimbalManager_RestoreFactorySettings(E_GduMountPosition mountPosition);

#ifdef __cplusplus
}
#endif

#endif // GDU_GIMBAL_MANAGER_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
