/**
 ********************************************************************
 * @file    test_gimbal_manager.c
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
#include <utils/util_misc.h>
#include "test_gimbal_manager.h"
#include "gdu_platform.h"
#include "gdu_logger.h"
#include "gdu_gimbal_manager.h"
#include "gdu_fc_subscription.h"
#include "gdu_aircraft_info.h"

/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/
typedef enum {
    GDU_TEST_GIMBAL_ROTATION,
    GDU_TEST_GIMBAL_RESET,
} E_GduTestGimbalAction;

typedef struct {
    E_GduTestGimbalAction action;
    T_GduGimbalManagerRotation rotation;
} T_GduTestGimbalActionList;

/* Private values -------------------------------------------------------------*/
static const T_GduTestGimbalActionList s_rotationActionList[] =
{
    //{.action = GDU_TEST_GIMBAL_RESET},
      {.action = GDU_TEST_GIMBAL_ROTATION, .rotation.rotationMode = GDU_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE, 0,  0,   1000,  0.2},
      {.action = GDU_TEST_GIMBAL_ROTATION, .rotation.rotationMode = GDU_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE, 0,  0,   1000,  0.2},
          {.action = GDU_TEST_GIMBAL_ROTATION, .rotation.rotationMode = GDU_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE, 0,   30,  0,  0.2},
          {.action = GDU_TEST_GIMBAL_ROTATION, .rotation.rotationMode = GDU_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE, 0,   -30, 0,  0.2},
    //   {.action = GDU_TEST_GIMBAL_ROTATION, .rotation.rotationMode = GDU_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE, 0,   0,   1000,  0.2},
      {.action = GDU_TEST_GIMBAL_ROTATION, .rotation.rotationMode = GDU_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE, 0,   0,   1000,  0.2},
     //{.action = GDU_TEST_GIMBAL_RESET},
        // {.action = GDU_TEST_GIMBAL_ROTATION, .rotation.rotationMode = GDU_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE, -90, 0,   0,  0.5},
        // {.action = GDU_TEST_GIMBAL_ROTATION, .rotation.rotationMode = GDU_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE, -60, 0,   0,  0.5},
        // {.action = GDU_TEST_GIMBAL_ROTATION, .rotation.rotationMode = GDU_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE, -30, 0,   0,  0.5},
        // {.action = GDU_TEST_GIMBAL_RESET},
};

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode GduTest_GimbalManagerRunSample(E_GduMountPosition mountPosition, E_GduGimbalMode gimbalMode)
{
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    T_GduReturnCode returnCode;
    T_GduGimbalManagerRotation rotation;
    T_GduAircraftInfoBaseInfo baseInfo;
    E_GduAircraftSeries aircraftSeries;
    T_GduReturnCode gduStat;
	T_GduDataTimestamp  timestamp;
    T_GduAttitude3d eulerInfo;

    returnCode = GduAircraftInfo_GetBaseInfo(&baseInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Failed to get aircraft base info, return code 0x%08X", returnCode);
        goto out;
    }

    USER_LOG_INFO("Gimbal manager sample start");
    GduTest_WidgetLogAppend("Gimbal manager sample start");

    returnCode = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_EULER_ANGLE_INFO, GDU_DATA_SUBSCRIPTION_TOPIC_10_HZ, NULL);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Failed to subscribe topic %d, 0x%08X", GDU_FC_SUBSCRIPTION_TOPIC_GIMBAL_ANGLES, returnCode);
        goto out;
    }

    USER_LOG_INFO("--> Step 1: Init gimbal manager module");
    GduTest_WidgetLogAppend("--> Step 1: Init gimbal manager module");
    returnCode = GduGimbalManager_Init();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Init gimbal manager failed, error code: 0x%08X", returnCode);
        goto out;
    }

    if (gimbalMode == GDU_GIMBAL_MODE_FREE) {
        USER_LOG_INFO("--> Step 2: Set gimbal to free mode");
        GduTest_WidgetLogAppend("--> Step 2: Set gimbal to free mode");
    } else if (gimbalMode == GDU_GIMBAL_MODE_YAW_FOLLOW) {
        USER_LOG_INFO("--> Step 2: Set gimbal to yaw follow mode");
        GduTest_WidgetLogAppend("--> Step 2: Set gimbal to yaw follow mode");
    }
    returnCode = GduGimbalManager_SetMode(mountPosition, gimbalMode);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set gimbal mode failed, error code: 0x%08X", returnCode);
        goto out;
    }

    USER_LOG_INFO("--> Step 3: Reset gimbal angles.\r\n");
    returnCode = GduGimbalManager_Reset(mountPosition, GDU_GIMBAL_RESET_MODE_PITCH_AND_YAW);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Reset gimbal failed, error code: 0x%08X", returnCode);
    }
    osalHandler->TaskSleepMs(5000);
    USER_LOG_INFO("--> Step 4: Rotate gimbal to target angle by action list\r\n");
    for (int i = 0; i < sizeof(s_rotationActionList) / sizeof(T_GduTestGimbalActionList); ++i) {
        if (s_rotationActionList[i].action == GDU_TEST_GIMBAL_RESET) {
            USER_LOG_INFO("Target gimbal reset.\r\n");
            returnCode = GduGimbalManager_Reset(mountPosition, GDU_GIMBAL_RESET_MODE_PITCH_AND_YAW);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Reset gimbal failed, error code: 0x%08X", returnCode);
            }
            osalHandler->TaskSleepMs(5000);
        } else if (s_rotationActionList[i].action == GDU_TEST_GIMBAL_ROTATION) {

            // if (gimbalMode == GDU_GIMBAL_MODE_FREE &&
            //     s_rotationActionList[i].rotation.rotationMode == GDU_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE) {
            //     continue;
            // }

            rotation = s_rotationActionList[i].rotation;
            USER_LOG_INFO("Target gimbal pry = (%.1f, %.1f, %.1f), mode:%d", rotation.pitch, rotation.roll, rotation.yaw, rotation.rotationMode);

            returnCode = GduGimbalManager_Rotate(mountPosition, rotation);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Target gimbal pry = (%.1f, %.1f, %.1f) failed, error code: 0x%08X",
                               s_rotationActionList[i].rotation.pitch, s_rotationActionList[i].rotation.roll,
                               s_rotationActionList[i].rotation.yaw,
                               returnCode);
            }
            osalHandler->TaskSleepMs(5000);
        }
    }

    USER_LOG_INFO("--> Step 5: Deinit gimbal manager module");
    GduTest_WidgetLogAppend("--> Step 5: Deinit gimbal manager module");
    returnCode = GduGimbalManager_Deinit();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Deinit gimbal manager failed, error code: 0x%08X", returnCode);
        goto out;
    }

out:
    USER_LOG_INFO("Gimbal manager sample end");

    return returnCode;
}

/* Private functions definition-----------------------------------------------*/
/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
