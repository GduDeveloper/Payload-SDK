/**
 ********************************************************************
 * @file    test_payload_msg.c
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
#include "test_payload_msg.h"
#include "gdu_logger.h"
#include "gdu_aircraft_info.h"
#include "gdu_payload_msg.h"
/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/
T_GduPayloadNetMsg s_payloadNetMsg[MAX_IP_NUM];

/* Private functions declaration ---------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/

/**
 * @brief Initialise power management module, including apply high power (only RTOS) and register power off notification
 * callback function.
 * @note GDU development board 1.0 can not accept high power, so do not call this function in GDU development board
 * 1.0 project.
 * @return Execution result.
 */
T_GduReturnCode GduTest_PayloadMsgStartService(void)
{
    T_GduReturnCode returnCode;
    T_GduAircraftInfoBaseInfo baseInfo = {0};

    returnCode = GduPayloadMsg_Init();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("paylaod msg init error: 0x%08llX.", returnCode);
        return returnCode;
    }

    returnCode = GduAircraftInfo_GetBaseInfo(&baseInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get aircraft base info error: 0x%08llX.", returnCode);
        return returnCode;
    }

    T_GduPayloadMsgHandler payloadMsgHandler = {
        .getPayloadVersion = GduTest_getPayloadVersion,
        .getPayloadIp = GduTest_getPayloadIp,
        .setPayloadIp = GduTest_setPayloadIp,
    };

    returnCode = GduPayloadMsg_RegPayloadMsgHandler(&payloadMsgHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("regsiter payload msg power handler error");
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_getPayloadVersion(T_GduFirmwareVersion *version)
{
    //attention your device version length need less than 32 bytes
    //strcpy(version, "1.2.3");
    //strcpy(version, "L1V1.0.5-A2(578.926)");
    strcpy(version, "your device version");
    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_getPayloadIp(int index, T_GduPayloadNetMsg *payloadNetMsg)
{
    memcpy(s_payloadNetMsg[index].ip, payloadNetMsg->ip, 4);
    memcpy(s_payloadNetMsg[index].mask, payloadNetMsg->mask, 4);
    memcpy(s_payloadNetMsg[index].gateway, payloadNetMsg->gateway, 4);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_setPayloadIp(T_GduPayloadNetMsg payloadNetMsg)
{
    memcpy(payloadNetMsg.ip, s_payloadNetMsg[payloadNetMsg.index].ip, 4);
    memcpy(payloadNetMsg.mask, s_payloadNetMsg[payloadNetMsg.index].mask, 4);
    memcpy(payloadNetMsg.gateway, s_payloadNetMsg[payloadNetMsg.index].gateway, 4);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
