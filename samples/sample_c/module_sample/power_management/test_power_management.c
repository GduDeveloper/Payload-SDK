/**
 ********************************************************************
 * @file    test_power_management.c
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
#include "test_power_management.h"
#include "gdu_logger.h"
#include "gdu_aircraft_info.h"

/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static T_GduReturnCode GduTest_PowerOffNotificationCallback(bool *powerOffPreparationFlag);

/* Private variables ---------------------------------------------------------*/
static T_GduTestApplyHighPowerHandler s_applyHighPowerHandler;

/* Exported functions definition ---------------------------------------------*/
/**
 * @brief Register handler function for applying high power. This function have to be called before calling
 * GduTest_PowerManagementInit(), except for in Linux, because GduTest_PowerManagementInit() do not apply high power
 * in Linux OS.
 * @param applyHighPowerHandler: pointer to handler function for applying high power.
 * @return Execution result.
 */
T_GduReturnCode GduTest_RegApplyHighPowerHandler(T_GduTestApplyHighPowerHandler *applyHighPowerHandler)
{
    if (applyHighPowerHandler->pinInit == NULL) {
        USER_LOG_ERROR("reg apply high power handler pinInit error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    if (applyHighPowerHandler->pinWrite == NULL) {
        USER_LOG_ERROR("reg apply high power handler pinWrite error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    memcpy(&s_applyHighPowerHandler, applyHighPowerHandler, sizeof(T_GduTestApplyHighPowerHandler));

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Initialise power management module, including apply high power (only RTOS) and register power off notification
 * callback function.
 * @note GDU development board 1.0 can not accept high power, so do not call this function in GDU development board
 * 1.0 project.
 * @return Execution result.
 */
T_GduReturnCode GduTest_PowerManagementStartService(void)
{
    T_GduReturnCode returnCode;
    T_GduAircraftInfoBaseInfo baseInfo = {0};

    returnCode = GduPowerManagement_Init();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("power management init error: 0x%08llX.", returnCode);
        return returnCode;
    }

    returnCode = GduAircraftInfo_GetBaseInfo(&baseInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get aircraft base info error: 0x%08llX.", returnCode);
        return returnCode;
    }

    // apply high power
    if (s_applyHighPowerHandler.pinInit == NULL) {
        USER_LOG_ERROR("apply high power pin init interface is NULL error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (s_applyHighPowerHandler.pinWrite == NULL) {
        USER_LOG_ERROR("apply high power pin write interface is NULL error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    returnCode = s_applyHighPowerHandler.pinInit();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("apply high power pin init error");
        return returnCode;
    }

    returnCode = GduPowerManagement_RegWriteHighPowerApplyPinCallback(s_applyHighPowerHandler.pinWrite);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("register WriteHighPowerApplyPinCallback error.");
        return returnCode;
    }

    returnCode = GduPowerManagement_ApplyHighPowerSync();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("apply high power error");
        return returnCode;
    }
	else
	{
		USER_LOG_DEBUG("apply high power success");
	}

    // register power off notification callback function
    returnCode = GduPowerManagement_RegPowerOffNotificationCallback(GduTest_PowerOffNotificationCallback);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("register power off notification callback function error");
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/
static T_GduReturnCode GduTest_PowerOffNotificationCallback(bool *powerOffPreparationFlag)
{
    USER_LOG_INFO("----------------------------------------aircraft will power off soon.");

    *powerOffPreparationFlag = true;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
