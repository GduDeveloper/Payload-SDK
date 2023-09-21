/**
 ********************************************************************
 * @file    test_time_sync.c
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
#include "test_time_sync.h"
#include "gdu_time_sync.h"
#include "gdu_logger.h"
#include "utils/util_misc.h"
#include "gdu_platform.h"

/* Private constants ---------------------------------------------------------*/
#define GDU_TEST_TIME_SYNC_TASK_FREQ            (1)
#define GDU_TEST_TIME_SYNC_TASK_STACK_SIZE      (2048)

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static void *GduTest_TimeSyncTask(void *arg);

/* Private variables ---------------------------------------------------------*/
static T_GduTestTimeSyncHandler s_timeSyncHandler;
static T_GduTaskHandle s_timeSyncThread;

/* Exported functions definition ---------------------------------------------*/
/**
 * @brief Register handler function for initialising PPS pin configure and reporting the latest local time when PPS is
 * triggered. This function have to be called before calling GduTest_TimeSyncInit().
 * @param timeSyncHandler: pointer to handler function for time synchronization.
 * @return Execution result.
 */
T_GduReturnCode GduTest_TimeSyncRegHandler(T_GduTestTimeSyncHandler *timeSyncHandler)
{
    if (timeSyncHandler->PpsSignalResponseInit == NULL) {
        USER_LOG_ERROR("reg time sync handler PpsSignalResponseInit error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    if (timeSyncHandler->GetNewestPpsTriggerLocalTimeUs == NULL) {
        USER_LOG_ERROR("reg time sync handler GetNewestPpsTriggerLocalTimeUs error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    memcpy(&s_timeSyncHandler, timeSyncHandler, sizeof(T_GduTestTimeSyncHandler));

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_TimeSyncStartService(void)
{
    T_GduReturnCode gduStat;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    gduStat = GduTimeSync_Init();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("time synchronization module init error.");
        return gduStat;
    }

    if (s_timeSyncHandler.PpsSignalResponseInit == NULL) {
        USER_LOG_ERROR("time sync handler PpsSignalResponseInit interface is NULL error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (s_timeSyncHandler.GetNewestPpsTriggerLocalTimeUs == NULL) {
        USER_LOG_ERROR("time sync handler GetNewestPpsTriggerLocalTimeUs interface is NULL error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    // users must register getNewestPpsTriggerTime callback function
    gduStat = GduTimeSync_RegGetNewestPpsTriggerTimeCallback(s_timeSyncHandler.GetNewestPpsTriggerLocalTimeUs);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("register GetNewestPpsTriggerLocalTimeUsCallback error.");
        return gduStat;
    }

    if (osalHandler->TaskCreate("user_time_sync_task", GduTest_TimeSyncTask,
                                GDU_TEST_TIME_SYNC_TASK_STACK_SIZE, NULL, &s_timeSyncThread) !=
        GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("user time sync task create error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    gduStat = s_timeSyncHandler.PpsSignalResponseInit();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("pps signal response init error");
        return gduStat;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_TimeSyncGetNewestPpsTriggerLocalTimeUs(uint64_t *localTimeUs)
{
    if (s_timeSyncHandler.GetNewestPpsTriggerLocalTimeUs == NULL) {
        USER_LOG_ERROR("GetNewestPpsTriggerLocalTimeUs null error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return s_timeSyncHandler.GetNewestPpsTriggerLocalTimeUs(localTimeUs);
}

/* Private functions definition-----------------------------------------------*/
#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *GduTest_TimeSyncTask(void *arg)
{
    T_GduReturnCode gduStat;
    uint32_t currentTimeMs = 0;
	uint64_t currentTimeUs = 0;
    T_GduTimeSyncAircraftTime aircraftTime = {0};
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    uint8_t totalSatelliteNumber = 0;

    USER_UTIL_UNUSED(arg);

    while (1) {
        osalHandler->TaskSleepMs(1000 / GDU_TEST_TIME_SYNC_TASK_FREQ);

        gduStat = GduTest_FcSubscriptionGetTotalSatelliteNumber(&totalSatelliteNumber);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get total satellite number error: 0x%08llX.", gduStat);
            continue;
        }

        gduStat = osalHandler->GetTimeMs(&currentTimeMs);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get current time error: 0x%08llX.", gduStat);
            continue;
        }

		currentTimeUs = (uint64_t)currentTimeMs * 1000;

        gduStat = GduTimeSync_TransferToAircraftTime(currentTimeUs, &aircraftTime);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("transfer to aircraft time error: 0x%08llX.", gduStat);
            continue;
        }

        USER_LOG_DEBUG("currentTimeMs: %d    current aircraft time is %d.%d.%d %d:%d:%d_%d.", currentTimeMs, aircraftTime.year, aircraftTime.month,
                       aircraftTime.day, aircraftTime.hour, aircraftTime.minute, aircraftTime.second,
                       aircraftTime.microsecond);
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
