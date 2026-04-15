/**
 ********************************************************************
 * @file    test_data_transmission.c
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
#include "test_data_transmission.h"
#include "gdu_logger.h"
#include "gdu_platform.h"
#include "utils/util_misc.h"
#include "gdu_data_transmission.h"
#include "gdu_aircraft_info.h"

/* Private constants ---------------------------------------------------------*/
#define DATA_TRANSMISSION_TASK_FREQ         (1)
#define DATA_TRANSMISSION_TASK_STACK_SIZE   (2048)

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static void *UserDataTransmission_Task(void *arg);
static T_GduReturnCode ReceiveDataFromMobile(const uint8_t *data, uint16_t len);
static T_GduReturnCode ReceiveDataFromCloud(const uint8_t *data, uint16_t len);
static T_GduReturnCode ReceiveDataFromExtensionPort(const uint8_t *data, uint16_t len);
static T_GduReturnCode ReceiveDataFromPayload(const uint8_t *data, uint16_t len);

/* Private variables ---------------------------------------------------------*/
static T_GduTaskHandle s_userDataTransmissionThread;
static T_GduAircraftInfoBaseInfo s_aircraftInfoBaseInfo;

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode GduTest_DataTransmissionStartService(void)
{
    T_GduReturnCode gduStat;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    E_GduChannelAddress channelAddress;
    char ipAddr[GDU_IP_ADDR_STR_SIZE_MAX];
    uint16_t port;

    gduStat = GduLowSpeedDataChannel_Init();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("init data transmission module error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    gduStat = GduAircraftInfo_GetBaseInfo(&s_aircraftInfoBaseInfo);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get aircraft base info error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    channelAddress = GDU_CHANNEL_ADDRESS_MASTER_RC_APP;
    gduStat = GduLowSpeedDataChannel_RegRecvDataCallback(channelAddress, ReceiveDataFromMobile);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("register receive data from mobile error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (osalHandler->TaskCreate("user_transmission_task", UserDataTransmission_Task,
                                DATA_TRANSMISSION_TASK_STACK_SIZE, NULL, &s_userDataTransmissionThread) !=
        GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("user data transmission task create error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_DataTransmissionStopService(void)
{
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    T_GduReturnCode returnCode;

    if (osalHandler->TaskDestroy(s_userDataTransmissionThread) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("user data transmission task destroy error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    returnCode = GduLowSpeedDataChannel_DeInit();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("deinit data transmission module error.");
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

static void *UserDataTransmission_Task(void *arg)
{
    T_GduReturnCode gduStat;
    const uint8_t dataToBeSent[10] = {0x00, 0x11, 0x22, 0x33, 0x44,0x55,0x66,0x77,0x88,0x99};
    T_GduDataChannelState state = {0};
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    E_GduChannelAddress channelAddress;

    USER_UTIL_UNUSED(arg);

    while (1) {
        osalHandler->TaskSleepMs(1000 / DATA_TRANSMISSION_TASK_FREQ);

        channelAddress = GDU_CHANNEL_ADDRESS_MASTER_RC_APP;
        gduStat = GduLowSpeedDataChannel_SendData(channelAddress, dataToBeSent, sizeof(dataToBeSent));
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
            USER_LOG_ERROR("send data to mobile error.");

    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

static T_GduReturnCode ReceiveDataFromMobile(const uint8_t *data, uint16_t len)
{
    char *printData = NULL;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    printData = osalHandler->Malloc(len + 1);
    if (printData == NULL) {
        USER_LOG_ERROR("malloc memory for printData fail.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    strncpy(printData, (const char *) data, len);
    printData[len] = '\0';
    USER_LOG_INFO("receive data from mobile: %s, len:%d.", printData, len);
    GduTest_WidgetLogAppend("receive data: %s, len:%d.", printData, len);

    osalHandler->Free(printData);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode ReceiveDataFromCloud(const uint8_t *data, uint16_t len)
{
    char *printData = NULL;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    printData = osalHandler->Malloc(len + 1);
    if (printData == NULL) {
        USER_LOG_ERROR("malloc memory for printData fail.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    strncpy(printData, (const char *) data, len);
    printData[len] = '\0';
    USER_LOG_INFO("receive data from cloud: %s, len:%d.", printData, len);
    GduTest_WidgetLogAppend("receive data: %s, len:%d.", printData, len);

    osalHandler->Free(printData);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode ReceiveDataFromExtensionPort(const uint8_t *data, uint16_t len)
{
    char *printData = NULL;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    printData = osalHandler->Malloc(len + 1);
    if (printData == NULL) {
        USER_LOG_ERROR("malloc memory for printData fail.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    strncpy(printData, (const char *) data, len);
    printData[len] = '\0';
    USER_LOG_INFO("receive data from extension port: %s, len:%d.", printData, len);
    GduTest_WidgetLogAppend("receive data: %s, len:%d.", printData, len);

    osalHandler->Free(printData);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode ReceiveDataFromPayload(const uint8_t *data, uint16_t len)
{
    char *printData = NULL;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    printData = osalHandler->Malloc(len + 1);
    if (printData == NULL) {
        USER_LOG_ERROR("malloc memory for printData fail.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    strncpy(printData, (const char *) data, len);
    printData[len] = '\0';
    USER_LOG_INFO("receive data from payload port: %s, len:%d.", printData, len);
    GduTest_WidgetLogAppend("receive data: %s, len:%d.", printData, len);

    osalHandler->Free(printData);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
