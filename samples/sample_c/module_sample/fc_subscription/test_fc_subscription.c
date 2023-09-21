/**
 ********************************************************************
 * @file    test_fc_subscription.c
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
#include <math.h>
#include "test_fc_subscription.h"
#include "gdu_logger.h"
#include "gdu_platform.h"

/* Private constants ---------------------------------------------------------*/
#define FC_SUBSCRIPTION_TASK_FREQ         (1)
#define FC_SUBSCRIPTION_TASK_STACK_SIZE   (2048)

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static void *UserFcSubscription_Task(void *arg);
static T_GduReturnCode GduTest_FcSubscriptionReceiveQuaternionCallback(const uint8_t *data, uint16_t dataSize,
                                                                const T_GduDataTimestamp *timestamp);
/* Private variables ---------------------------------------------------------*/
static T_GduTaskHandle s_userFcSubscriptionThread;
static bool s_userFcSubscriptionDataShow = false;
static uint8_t s_totalSatelliteNumberUsed = 0;

/* Exported functions definition ---------------------------------------------*/

T_GduReturnCode GduReceiveGpsPositionCallback(const uint8_t *data, uint16_t dataSize, const T_GduDataTimestamp *timestamp)
{
	T_GduFcSubscriptionGpsPosition *pos = (T_GduFcSubscriptionGpsPosition *)data;

	//USER_LOG_DEBUG("gps position is :%d, %d, %d", pos->x, pos->y, pos->z);

	return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_FcSubscriptionStartService(void)
{
    T_GduReturnCode gduStat;
    T_GduOsalHandler *osalHandler = NULL;

    osalHandler = GduPlatform_GetOsalHandler();

    gduStat = GduFcSubscription_Init();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("init data subscription module error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

//    gduStat = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_QUATERNION, GDU_DATA_SUBSCRIPTION_TOPIC_10_HZ,
//                                               GduTest_FcSubscriptionReceiveQuaternionCallback);
//    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//        USER_LOG_ERROR("Subscribe topic quaternion error.");
//        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
//    } else {
//        USER_LOG_DEBUG("Subscribe topic quaternion success.");
//    }
//
//    gduStat = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_VELOCITY, GDU_DATA_SUBSCRIPTION_TOPIC_1_HZ,
//                                               NULL);
//    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//        USER_LOG_ERROR("Subscribe topic velocity error.");
//        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
//    } else {
//        USER_LOG_DEBUG("Subscribe topic velocity success.");
//    }

    gduStat = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_GPS_POSITION, GDU_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                               GduReceiveGpsPositionCallback);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic gps position error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    } else {
        USER_LOG_DEBUG("Subscribe topic gps position success.");
    }

//    gduStat = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_GPS_DETAILS, GDU_DATA_SUBSCRIPTION_TOPIC_1_HZ,
//                                               NULL);
//    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//        USER_LOG_ERROR("Subscribe topic gps details error.");
//        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
//    } else {
//        USER_LOG_DEBUG("Subscribe topic gps details success.");
//    }

     if (osalHandler->TaskCreate("user_subscription_task", UserFcSubscription_Task,
                                 FC_SUBSCRIPTION_TASK_STACK_SIZE, NULL, &s_userFcSubscriptionThread) !=
         GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
         USER_LOG_ERROR("user data subscription task create error.");
         return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
     }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_FcSubscriptionRunSample(void)
{
    T_GduReturnCode gduStat;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    T_GduFcSubscriptionVelocity velocity = {0};
    T_GduDataTimestamp timestamp = {0};
    T_GduFcSubscriptionGpsPosition gpsPosition = {0};
    T_GduFcSubscriptionSingleBatteryInfo singleBatteryInfo = {0};

    USER_LOG_INFO("Fc subscription sample start");

    USER_LOG_INFO("--> Step 1: Init fc subscription module");
    gduStat = GduFcSubscription_Init();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("init data subscription module error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    USER_LOG_INFO("--> Step 2: Subscribe the topics of quaternion, velocity and gps position");
    gduStat = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_QUATERNION, GDU_DATA_SUBSCRIPTION_TOPIC_10_HZ,
                                               GduTest_FcSubscriptionReceiveQuaternionCallback);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic quaternion error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    gduStat = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_VELOCITY, GDU_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                               NULL);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic velocity error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    gduStat = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_GPS_POSITION, GDU_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                               NULL);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic gps position error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    USER_LOG_INFO("--> Step 3: Get latest value of the subscribed topics in the next 20s\r\n");

    for (int i = 0; i < 10; ++i) {
        osalHandler->TaskSleepMs(1000 / FC_SUBSCRIPTION_TASK_FREQ);
        gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_VELOCITY,
                                                          (uint8_t *) &velocity,
                                                          sizeof(T_GduFcSubscriptionVelocity),
                                                          &timestamp);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic velocity error.");
        } else {
            USER_LOG_INFO("velocity: x = %f y = %f z = %f healthFlag = %d.", velocity.data.x, velocity.data.y,
                          velocity.data.z, velocity.health);
        }

        gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_GPS_POSITION,
                                                          (uint8_t *) &gpsPosition,
                                                          sizeof(T_GduFcSubscriptionGpsPosition),
                                                          &timestamp);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps position error.");
        } else {
            USER_LOG_INFO("gps position: x = %d y = %d z = %d.", gpsPosition.x, gpsPosition.y, gpsPosition.z);
        }

        //Attention: if you want to subscribe the single battery info on M300 RTK, you need connect USB cable to
        //OSDK device or use topic GDU_FC_SUBSCRIPTION_TOPIC_BATTERY_INFO instead.
        gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_BATTERY_SINGLE_INFO_INDEX1,
                                                          (uint8_t *) &singleBatteryInfo,
                                                          sizeof(T_GduFcSubscriptionSingleBatteryInfo),
                                                          &timestamp);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic battery single info index1 error.");
        } else {
            USER_LOG_INFO(
                "battery single info index1: capacity percent = %ld% voltage = %ldV temperature = %.2f degree.",
                singleBatteryInfo.batteryCapacityPercent,
                singleBatteryInfo.currentVoltage / 1000,
                (gdu_f32_t) singleBatteryInfo.batteryTemperature / 10);
        }

        gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_BATTERY_SINGLE_INFO_INDEX2,
                                                          (uint8_t *) &singleBatteryInfo,
                                                          sizeof(T_GduFcSubscriptionSingleBatteryInfo),
                                                          &timestamp);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic battery single info index2 error.");
        } else {
            USER_LOG_INFO(
                "battery single info index2: capacity percent = %ld% voltage = %ldV temperature = %.2f degree.\r\n",
                singleBatteryInfo.batteryCapacityPercent,
                singleBatteryInfo.currentVoltage / 1000,
                (gdu_f32_t) singleBatteryInfo.batteryTemperature / 10);
        }
    }

    USER_LOG_INFO("--> Step 4: Deinit fc subscription module");

    gduStat = GduFcSubscription_DeInit();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Deinit fc subscription error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    USER_LOG_INFO("Fc subscription sample end");

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_FcSubscriptionDataShowTrigger(void)
{
    s_userFcSubscriptionDataShow = !s_userFcSubscriptionDataShow;
    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_FcSubscriptionGetTotalSatelliteNumber(uint8_t *number)
{
    *number = s_totalSatelliteNumberUsed;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/
#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *UserFcSubscription_Task(void *arg)
{
    T_GduReturnCode gduStat;
    T_GduFcSubscriptionVelocity velocity = {0};
    T_GduDataTimestamp timestamp = {0};
    T_GduFcSubscriptionGpsPosition gpsPosition = {0};
    T_GduFcSubscriptionGpsDetails gpsDetails = {0};
    T_GduOsalHandler *osalHandler = NULL;
	T_GduFcSubscriptionGpsTime gpsTime = 0;

    USER_UTIL_UNUSED(arg);
    osalHandler = GduPlatform_GetOsalHandler();

    while (1) {
        osalHandler->TaskSleepMs(1000 / FC_SUBSCRIPTION_TASK_FREQ);

        gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_VELOCITY,
                                                          (uint8_t *) &velocity,
                                                          sizeof(T_GduFcSubscriptionVelocity),
                                                          &timestamp);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic velocity error.");
        }

        if (s_userFcSubscriptionDataShow == true) {
            USER_LOG_INFO("velocity: x %f y %f z %f, healthFlag %d.", velocity.data.x, velocity.data.y,
                          velocity.data.z, velocity.health);
        }

        gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_GPS_POSITION,
                                                          (uint8_t *) &gpsPosition,
                                                          sizeof(T_GduFcSubscriptionGpsPosition),
                                                          &timestamp);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps position error.");
        }

        if (s_userFcSubscriptionDataShow == true) {
            USER_LOG_INFO("gps position: x %d y %d z %d.", gpsPosition.x, gpsPosition.y, gpsPosition.z);
        }

        gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_GPS_DETAILS,
                                                          (uint8_t *) &gpsDetails,
                                                          sizeof(T_GduFcSubscriptionGpsDetails),
                                                          &timestamp);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps details error.");
        }

        gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_GPS_TIME,
                                                          (uint8_t *) &gpsTime,
                                                          sizeof(T_GduFcSubscriptionGpsTime),
                                                          &timestamp);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps time error.");
        }

        gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_GPS_DATE,
                                                          (uint8_t *) &gpsTime,
                                                          sizeof(T_GduFcSubscriptionGpsTime),
                                                          &timestamp);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps time error.");
        }

        if (s_userFcSubscriptionDataShow == true) {
            USER_LOG_INFO("gps total satellite number used: %d %d %d.",
                          gpsDetails.gpsSatelliteNumberUsed,
                          gpsDetails.glonassSatelliteNumberUsed,
                          gpsDetails.totalSatelliteNumberUsed);
            s_totalSatelliteNumberUsed = gpsDetails.totalSatelliteNumberUsed;
        }

    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

static T_GduReturnCode GduTest_FcSubscriptionReceiveQuaternionCallback(const uint8_t *data, uint16_t dataSize,
                                                                       const T_GduDataTimestamp *timestamp)
{
    T_GduFcSubscriptionQuaternion *quaternion = (T_GduFcSubscriptionQuaternion *) data;
    gdu_f64_t pitch, yaw, roll;

    USER_UTIL_UNUSED(dataSize);

    pitch = (gdu_f64_t) asinf(-2 * quaternion->q1 * quaternion->q3 + 2 * quaternion->q0 * quaternion->q2) * 57.3;
    roll = (gdu_f64_t) atan2f(2 * quaternion->q1 * quaternion->q2 + 2 * quaternion->q0 * quaternion->q3,
                              -2 * quaternion->q2 * quaternion->q2 - 2 * quaternion->q3 * quaternion->q3 + 1) *
           57.3;
    yaw = (gdu_f64_t) atan2f(2 * quaternion->q2 * quaternion->q3 + 2 * quaternion->q0 * quaternion->q1,
                             -2 * quaternion->q1 * quaternion->q1 - 2 * quaternion->q2 * quaternion->q2 + 1) * 57.3;

    if (s_userFcSubscriptionDataShow == true) {
        USER_LOG_INFO("receive quaternion data.");

        USER_LOG_INFO("timestamp: millisecond %u microsecond %u.", timestamp->millisecond,
                      timestamp->microsecond);
        USER_LOG_INFO("quaternion: %f %f %f %f.\r\n", quaternion->q0, quaternion->q1, quaternion->q2, quaternion->q3);
        USER_LOG_INFO("euler angles: pitch = %.2f roll = %.2f yaw = %.2f.", pitch, yaw, roll);
        //GduTest_WidgetLogAppend("pitch = %.2f roll = %.2f yaw = %.2f.", pitch, yaw, roll);
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
