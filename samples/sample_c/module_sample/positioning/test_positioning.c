/**
 ********************************************************************
 * @file    test_positioning.c
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
#include <fc_subscription/test_fc_subscription.h>
#include "test_positioning.h"
#include "gdu_positioning.h"
#include "gdu_logger.h"
#include "utils/util_misc.h"
#include "gdu_platform.h"
#include "time_sync/test_time_sync.h"

/* Private constants ---------------------------------------------------------*/
#define POSITIONING_TASK_FREQ                     (1)
#define POSITIONING_TASK_STACK_SIZE               (2048)

#define GDU_TEST_POSITIONING_EVENT_COUNT          (5)
#define GDU_TEST_TIME_INTERVAL_AMONG_EVENTS_US    (1000)

/* Private types -------------------------------------------------------------*/


/* Private functions declaration ---------------------------------------------*/
static void *GduTest_PositioningTask(void *arg);

/* Private variables ---------------------------------------------------------*/
static T_GduTaskHandle s_userPositioningThread;
static int32_t s_eventIndex = 0;

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode GduTest_PositioningStartService(void)
{
    T_GduReturnCode gduStat;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    gduStat = GduPositioning_Init();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("positioning module init error.");
        return gduStat;
    }

    USER_LOG_DEBUG("positioning start service");
    GduPositioning_SetTaskIndex(0);

    if (osalHandler->TaskCreate("user_positioning_task", GduTest_PositioningTask,
                                POSITIONING_TASK_STACK_SIZE, NULL, &s_userPositioningThread) !=
        GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("user positioning task create error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/
#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *GduTest_PositioningTask(void *arg)
{
    int32_t i = 0;
    T_GduReturnCode gduStat;
    uint64_t ppsNewestTriggerTimeUs = 0;
	uint64_t posTime = 0;
    uint32_t currentTimeMs = 0;
	uint64_t currentTimeUs = 0;
    T_GduPositioningEventInfo eventInfo[GDU_TEST_POSITIONING_EVENT_COUNT] = {0};
    T_GduPositioningPositionInfo positionInfo[GDU_TEST_POSITIONING_EVENT_COUNT] = {0};
    T_GduTimeSyncAircraftTime aircraftTime = {0};
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    uint8_t totalSatelliteNumber = 0;
	uint32_t beforeTime = 0, afterTime = 0;

    USER_UTIL_UNUSED(arg);

    while (1) {
        osalHandler->TaskSleepMs(1000 / POSITIONING_TASK_FREQ);

        gduStat = GduTest_FcSubscriptionGetTotalSatelliteNumber(&totalSatelliteNumber);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get total satellite number error: 0x%08llX.", gduStat);
            continue;
        }

        gduStat = GduTest_TimeSyncGetNewestPpsTriggerLocalTimeUs(&ppsNewestTriggerTimeUs);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get newest pps trigger time error: 0x%08llX.", gduStat);
            continue;
        }

        gduStat = osalHandler->GetTimeMs(&currentTimeMs);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get current time error: 0x%08llX.", gduStat);
            continue;
        }

        for (i = 0; i < GDU_TEST_POSITIONING_EVENT_COUNT; ++i) {
            eventInfo[i].eventSetIndex = s_eventIndex;
            eventInfo[i].targetPointIndex = i;

			currentTimeUs = ((uint64_t)currentTimeMs - (i+1) * 300) * 1000;
            gduStat = GduTimeSync_TransferToAircraftTime(currentTimeUs, &aircraftTime);
            if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("transfer to aircraft time error: 0x%08llX.", gduStat);
                continue;
            }

            eventInfo[i].eventTime = aircraftTime;
        }

        gduStat = GduPositioning_GetPositionInformationSync(GDU_TEST_POSITIONING_EVENT_COUNT, eventInfo, positionInfo);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get position information error. error code is 0x%08llX", gduStat);
            continue;
        }

        USER_LOG_DEBUG("request position of target points success.");
        USER_LOG_DEBUG("detail position information:*************");
        for (i = 0; i < GDU_TEST_POSITIONING_EVENT_COUNT; ++i) {
            USER_LOG_DEBUG("position solution property: %d. eventTime:(%d-%d-%d %d:%d:%d:%d)", positionInfo[i].positionSolutionProperty, eventInfo[i].eventTime.year, eventInfo[i].eventTime.month, eventInfo[i].eventTime.day, \
																								eventInfo[i].eventTime.hour, eventInfo[i].eventTime.minute , eventInfo[i].eventTime.second, eventInfo[i].eventTime.microsecond);
//            USER_LOG_DEBUG("pitchAttitudeAngle: %d\trollAttitudeAngle: %d\tyawAttitudeAngle: %d",
//                           positionInfo[i].uavAttitude.pitch, positionInfo[i].uavAttitude.roll,
//                           positionInfo[i].uavAttitude.yaw);
//            USER_LOG_DEBUG("northPositionOffset: %d\tearthPositionOffset: %d\tdownPositionOffset: %d",
//                           positionInfo[i].offsetBetweenMainAntennaAndTargetPoint.x,
//                           positionInfo[i].offsetBetweenMainAntennaAndTargetPoint.y,
//                           positionInfo[i].offsetBetweenMainAntennaAndTargetPoint.z);
           USER_LOG_DEBUG("longitude: %.8f\tlatitude: %.8f\theight: %.8f",
                          positionInfo[i].targetPointPosition.longitude,
                          positionInfo[i].targetPointPosition.latitude,
                           positionInfo[i].targetPointPosition.height);
//            USER_LOG_DEBUG(
//                "longStandardDeviation: %.8f\tlatStandardDeviation: %.8f\thgtStandardDeviation: %.8f",
//                positionInfo[i].targetPointPositionStandardDeviation.longitude,
//                positionInfo[i].targetPointPositionStandardDeviation.latitude,
//                positionInfo[i].targetPointPositionStandardDeviation.height);
        }
		
        s_eventIndex++;
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
