/**
 ********************************************************************
 * @file    application.c
 * @date    2019/8/23
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
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "usb_host.h"
#include "led.h"
#include "pps.h"
#include "apply_high_power.h"
#include "uart.h"
#include "flash_if.h"
#include "upgrade_platform_opt_stm32.h"

#include "application.h"
#include "hal_uart.h"
#include "osal.h"
#include "gdu_sdk_app_info.h"
#include "gdu_sdk_config.h"
#include "gdu_core.h"
#include "gdu_aircraft_info.h"
#include "gdu_logger.h"

#include "utils/util_misc.h"
#include "camera_emu/test_payload_cam_emu_base.h"
#include "fc_subscription/test_fc_subscription.h"
#include "gimbal_emu/test_payload_gimbal_emu.h"
 #include "time_sync/test_time_sync.h"
//#include "xport/test_payload_xport.h"
//#include "payload_collaboration/test_payload_collaboration.h"
#include "widget/test_widget.h"
//#include "widget_interaction_test/test_widget_interaction.h"
//#include "data_transmission/test_data_transmission.h"

//#include "positioning/test_positioning.h"
#include "upgrade/test_upgrade.h"
#include "power_management/test_power_management.h"
#include "hms/test_hms.h"
#include "payload_msg/test_payload_msg.h"
/* Private constants ---------------------------------------------------------*/
#define RUN_INDICATE_TASK_FREQ_1HZ        1
#define RUN_INDICATE_TASK_FREQ_0D1HZ      0.1f

#define GDU_USE_WIDGET_INTERACTION        0
#define GDU_EXTENSION_PORT_SUPPORT        0

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static T_GduReturnCode GduUser_PrintConsole(const uint8_t *data, uint16_t dataLen);
static T_GduReturnCode GduUser_FillInUserInfo(T_GduUserInfo *userInfo);

/* Exported functions definition ---------------------------------------------*/
void GduUser_StartTask(void const *argument)
{
    T_GduReturnCode returnCode;
    T_GduUserInfo userInfo;
    T_GduAircraftInfoBaseInfo aircraftInfoBaseInfo;
    T_GduOsalHandler osalHandler = {
        .TaskCreate = Osal_TaskCreate,
        .TaskDestroy = Osal_TaskDestroy,
        .TaskSleepMs = Osal_TaskSleepMs,
        .MutexCreate= Osal_MutexCreate,
        .MutexDestroy = Osal_MutexDestroy,
        .MutexLock = Osal_MutexLock,
        .MutexUnlock = Osal_MutexUnlock,
        .SemaphoreCreate = Osal_SemaphoreCreate,
        .SemaphoreDestroy = Osal_SemaphoreDestroy,
        .SemaphoreWait = Osal_SemaphoreWait,
        .SemaphoreTimedWait = Osal_SemaphoreTimedWait,
        .SemaphorePost = Osal_SemaphorePost,
        .Malloc = Osal_Malloc,
        .Free = Osal_Free,
        .GetTimeMs = Osal_GetTimeMs,
        .GetTimeUs = Osal_GetTimeUs,
    };
    T_GduLoggerConsole printConsole = {
        .func = GduUser_PrintConsole,
        .consoleLevel = GDU_LOGGER_CONSOLE_LOG_LEVEL_DEBUG,
        .isSupportColor = true,
    };

    T_GduHalUartHandler uartHandler = {
        .UartInit = HalUart_Init,
        .UartDeInit = HalUart_DeInit,
        .UartWriteData = HalUart_WriteData,
        .UartReadData = HalUart_ReadData,
        .UartGetStatus = HalUart_GetStatus,
    };

    UART_Init(GDU_CONSOLE_UART_NUM, GDU_CONSOLE_UART_BAUD);
    Led_Init(LED1);

//Attention: if you want to run payload sdk on extension port, please define the macro GDU_EXTENSION_PORT_SUPPORT.
#if GDU_EXTENSION_PORT_SUPPORT
    MX_USB_HOST_Init();
    Osal_TaskSleepMs(2000);
#endif

    returnCode = GduPlatform_RegOsalHandler(&osalHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register osal handler error");
        goto out;
    }

    returnCode = GduPlatform_RegHalUartHandler(&uartHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register hal uart handler error");
        goto out;
    }

    returnCode = GduLogger_AddConsole(&printConsole);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("add printf console error");
        goto out;
    }

    returnCode = GduUser_FillInUserInfo(&userInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("fill user info error, please check user info config");
        goto out;
    }

    returnCode = GduCore_Init(&userInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("core init error");
        goto out;
    }

    returnCode = GduAircraftInfo_GetBaseInfo(&aircraftInfoBaseInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get aircraft base info error");
        goto out;
    }

    //returnCode = GduCore_SetAlias("PSDK_APPALIAS");
    returnCode = GduCore_SetAlias("PSDK_APPALIAS");
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("set alias error");
        goto out;
    }

	returnCode = GduCore_ApplicationStart();
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		USER_LOG_ERROR("start sdk application error");
	}
    
    USER_LOG_DEBUG("%s", GduCode_GetProductAlias());
#ifdef CONFIG_MODULE_PAYLOAD_MSG_ON
		returnCode = GduTest_PayloadMsgStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("payload msg error");
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_POWER_MANAGEMENT_ON
        T_GduTestApplyHighPowerHandler applyHighPowerHandler = {
            .pinInit = GduTest_HighPowerApplyPinInit,
            .pinWrite = GduTest_WriteHighPowerApplyPin,
        };

        returnCode = GduTest_RegApplyHighPowerHandler(&applyHighPowerHandler);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("regsiter apply high power handler error");
        }

        returnCode = GduTest_PowerManagementStartService();
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("power management init error");
        }
#endif

#ifdef CONFIG_MODULE_SAMPLE_CAMERA_ON
        returnCode = GduTest_CameraEmuBaseStartService();
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("camera emu common init error");
            goto out;
        }
#endif

	#ifdef CONFIG_MODULE_SAMPLE_WIDGET_ON
	#if GDU_USE_WIDGET_INTERACTION
	        returnCode = GduTest_WidgetInteractionStartService();
	        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
	            USER_LOG_ERROR("widget sample init error");
	        }
	#else
	        returnCode = GduTest_WidgetStartService();
	        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
	            USER_LOG_ERROR("widget sample init error");
	        }
	#endif
	#endif

#ifdef CONFIG_MODULE_SAMPLE_DATA_TRANSMISSION_ON
        returnCode = GduTest_DataTransmissionStartService();
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("widget sample init error");
        }
#endif

#ifdef CONFIG_MODULE_SAMPLE_FC_SUBSCRIPTION_ON
        returnCode = GduTest_FcSubscriptionStartService();
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("data subscription sample init error\n");
        }

		//GduTest_HmsRunSample();
#endif

#ifdef CONFIG_MODULE_SAMPLE_GIMBAL_ON
#if 0
        if (aircraftInfoBaseInfo.gduAdapterType == GDU_SDK_ADAPTER_TYPE_SKYPORT_V2) {
            if (GduTest_GimbalStartService() != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("psdk gimbal init error");
            }
        }
#endif
		if (GduTest_GimbalStartService() != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("psdk gimbal init error");
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_XPORT_ON
		if (aircraftInfoBaseInfo.gduAdapterType == GDU_SDK_ADAPTER_TYPE_XPORT) {
			if (GduTest_XPortStartService() != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
				USER_LOG_ERROR("psdk xport init error");
			}
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_TIME_SYNC_ON
    T_GduTestTimeSyncHandler testTimeSyncHandler = {
        .PpsSignalResponseInit = GduTest_PpsSignalResponseInit,
        .GetNewestPpsTriggerLocalTimeUs = GduTest_GetNewestPpsTriggerLocalTimeUs,
    };

    if (GduTest_TimeSyncRegHandler(&testTimeSyncHandler) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("regsiter time sync handler error");
        goto out;
    }

    if (GduTest_TimeSyncStartService() != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("psdk time sync init error");
    }
#endif

#ifdef CONFIG_MODULE_SAMPLE_PAYLOAD_COLLABORATION_ON
		if (aircraftInfoBaseInfo.mountPosition != GDU_MOUNT_POSITION_EXTENSION_PORT) {
			returnCode = GduTest_PayloadCollaborationStartService();
			if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
				USER_LOG_ERROR("Payload collaboration sample init error\n");
			}
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_POSITIONING_ON
		if (GduTest_PositioningStartService() != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("psdk positioning init error");
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_FLIGHT_CTRL_ON
		if (GduFlightController_Init() != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("psdk flight controller init error");
		}
#endif

#ifdef CONFIG_MODULE_PPK_ON
		if (GduTest_PPKStartService() != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("psdk ppk init error");
		}
#endif


#ifdef CONFIG_MODULE_SAMPLE_UPGRADE_ON
		T_GduTestUpgradePlatformOpt stm32UpgradePlatformOpt = {
			.rebootSystem = GduUpgradePlatformStm32_RebootSystem,
			.cleanUpgradeProgramFileStoreArea = GduUpgradePlatformStm32_CleanUpgradeProgramFileStoreArea,
			.createUpgradeProgramFile = GduUpgradePlatformStm32_CreateUpgradeProgramFile,
			.writeUpgradeProgramFile = GduUpgradePlatformStm32_WriteUpgradeProgramFile,
			.readUpgradeProgramFile = GduUpgradePlatformStm32_ReadUpgradeProgramFile,
			.closeUpgradeProgramFile = GduUpgradePlatformStm32_CloseUpgradeProgramFile,
			.replaceOldProgram = GduUpgradePlatformStm32_ReplaceOldProgram,
			.setUpgradeRebootState = GduUpgradePlatformStm32_SetUpgradeRebootState,
			.getUpgradeRebootState = GduUpgradePlatformStm32_GetUpgradeRebootState,
			.cleanUpgradeRebootState = GduUpgradePlatformStm32_CleanUpgradeRebootState,
		};
		T_GduTestUpgradeConfig testUpgradeConfig = {
			.firmwareVersion = {1, 0, 0, 0},
			.transferType = GDU_FIRMWARE_TRANSFER_TYPE_UART,
			.needReplaceProgramBeforeReboot = false
		};
		if (GduTest_UpgradeStartService(&stm32UpgradePlatformOpt, testUpgradeConfig) !=
				GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			printf("psdk upgrade init error");
		}
#endif

	while (1) {
		Osal_TaskSleepMs(1000);
		Led_Trigger(LED1);
	}

out:
	vTaskDelete(xTaskGetCurrentTaskHandle());
}

#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

void GduUser_MonitorTask(void const *argument)
{
	static uint32_t runIndicateTaskStep = 0;
	T_UartBufferState readBufferState = {0};
	T_UartBufferState writeBufferState = {0};
	uint32_t loopCnt = 0;
#if (configUSE_TRACE_FACILITY == 1)
	int32_t i = 0;
	int32_t j = 0;
	TaskStatus_t *lastTaskStatusArray = NULL;
	TaskStatus_t *currentTaskStatusArray = NULL;
    uint8_t lastTaskStatusArraySize = 0;
    uint8_t currentTaskStatusArraySize;
    uint8_t cpuOccupyPercentage;
#endif

    while (1) {
        Osal_TaskSleepMs(1000 / RUN_INDICATE_TASK_FREQ_1HZ);

        // report UART buffer state
        if(loopCnt++ % 10 == 0)
       	{
#ifdef USING_UART_PORT_1
	        UART_GetBufferState(UART_NUM_1, &readBufferState, &writeBufferState);
	        USER_LOG_DEBUG("Uart1 read buffer state: countOfLostData %d, maxUsedCapacityOfBuffer %d.",
	                        readBufferState.countOfLostData, readBufferState.maxUsedCapacityOfBuffer);
	         USER_LOG_DEBUG("Uart1 write buffer state: countOfLostData %d, maxUsedCapacityOfBuffer %d.",
	                        writeBufferState.countOfLostData, writeBufferState.maxUsedCapacityOfBuffer);
#endif

#ifdef USING_UART_PORT_2
	        UART_GetBufferState(UART_NUM_2, &readBufferState, &writeBufferState);
	        USER_LOG_DEBUG("Uart2 read buffer state: countOfLostData %d, maxUsedCapacityOfBuffer %d.",
	                        readBufferState.countOfLostData, readBufferState.maxUsedCapacityOfBuffer);
	         USER_LOG_DEBUG("Uart2 write buffer state: countOfLostData %d, maxUsedCapacityOfBuffer %d.",
	                        writeBufferState.countOfLostData, writeBufferState.maxUsedCapacityOfBuffer);
#endif

#ifdef USING_UART_PORT_3
	        UART_GetBufferState(UART_NUM_3, &readBufferState, &writeBufferState);
	         USER_LOG_DEBUG("Uart3 read buffer state: countOfLostData %d, maxUsedCapacityOfBuffer %d.",
	                        readBufferState.countOfLostData, readBufferState.maxUsedCapacityOfBuffer);
	         USER_LOG_DEBUG("Uart3 write buffer state: countOfLostData %d, maxUsedCapacityOfBuffer %d.",
                        writeBufferState.countOfLostData, writeBufferState.maxUsedCapacityOfBuffer);
#endif
		}

        // report system performance information.
        // Attention: report system performance part is not intended for normal application runtime use but as a debug aid.
        if (USER_UTIL_IS_WORK_TURN(runIndicateTaskStep++, RUN_INDICATE_TASK_FREQ_0D1HZ, RUN_INDICATE_TASK_FREQ_1HZ)) {
#if (configUSE_TRACE_FACILITY == 1)
            currentTaskStatusArraySize = uxTaskGetNumberOfTasks();
            currentTaskStatusArray = osalHandler->Malloc(currentTaskStatusArraySize * sizeof(TaskStatus_t));
            if (currentTaskStatusArray == NULL) {
                continue;
            }

            currentTaskStatusArraySize = uxTaskGetSystemState(currentTaskStatusArray, currentTaskStatusArraySize, NULL);
            USER_LOG_DEBUG("task information:");
            USER_LOG_DEBUG("task name\trun time (%%)\tstack left (byte)\tnumber");
            for (i = 0; i < currentTaskStatusArraySize; i++) {
                cpuOccupyPercentage = 0;
                for (j = 0; j < lastTaskStatusArraySize; ++j) {
                    if (currentTaskStatusArray[i].xTaskNumber == lastTaskStatusArray[j].xTaskNumber) {
                        cpuOccupyPercentage =
                            (currentTaskStatusArray[i].ulRunTimeCounter - lastTaskStatusArray[j].ulRunTimeCounter) /
                            configTICK_RATE_HZ / RUN_INDICATE_TASK_FREQ_0D1HZ;
                        break;
                    }
                }
                USER_LOG_DEBUG("%-16s\t%u\t%u\t%u", currentTaskStatusArray[i].pcTaskName, cpuOccupyPercentage,
                               (unsigned int) currentTaskStatusArray[i].usStackHighWaterMark * sizeof(StackType_t),
                               (unsigned int) currentTaskStatusArray[i].xTaskNumber);
            }
            osalHandler->Free(lastTaskStatusArray);
            lastTaskStatusArray = currentTaskStatusArray;
            lastTaskStatusArraySize = currentTaskStatusArraySize;
#endif

            USER_LOG_DEBUG("used heap size: %d.\r\n", configTOTAL_HEAP_SIZE - xPortGetFreeHeapSize());
        }
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

/* Private functions definition-----------------------------------------------*/
static T_GduReturnCode GduUser_FillInUserInfo(T_GduUserInfo *userInfo)
{
    memset(userInfo->appName, 0, sizeof(userInfo->appName));
    memset(userInfo->appId, 0, sizeof(userInfo->appId));
    memset(userInfo->appKey, 0, sizeof(userInfo->appKey));
    memset(userInfo->appLicense, 0, sizeof(userInfo->appLicense));
    memset(userInfo->developerAccount, 0, sizeof(userInfo->developerAccount));
    memset(userInfo->baudRate, 0, sizeof(userInfo->baudRate));

    if (strlen(USER_APP_NAME) >= sizeof(userInfo->appName) ||
        strlen(USER_APP_ID) > sizeof(userInfo->appId) ||
        strlen(USER_APP_KEY) > sizeof(userInfo->appKey) ||
        strlen(USER_APP_LICENSE) > sizeof(userInfo->appLicense) ||
        strlen(USER_DEVELOPER_ACCOUNT) >= sizeof(userInfo->developerAccount) ||
        strlen(USER_BAUD_RATE) > sizeof(userInfo->baudRate)) {
        USER_LOG_ERROR("Length of user information string is beyond limit. Please check.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    if (!strcmp(USER_APP_NAME, "your_app_name") ||
        !strcmp(USER_APP_ID, "your_app_id") ||
        !strcmp(USER_APP_KEY, "your_app_key") ||
        !strcmp(USER_BAUD_RATE, "your_app_license") ||
        !strcmp(USER_DEVELOPER_ACCOUNT, "your_developer_account") ||
        !strcmp(USER_BAUD_RATE, "your_baud_rate")) {
        USER_LOG_ERROR(
            "Please fill in correct user information to 'samples/sample_c/platform/rtos_freertos/stm32f4_discovery/application/gdu_sdk_app_info.h' file.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    strncpy(userInfo->appName, USER_APP_NAME, sizeof(userInfo->appName) - 1);
    memcpy(userInfo->appId, USER_APP_ID, USER_UTIL_MIN(sizeof(userInfo->appId), strlen(USER_APP_ID)));
    memcpy(userInfo->appKey, USER_APP_KEY, USER_UTIL_MIN(sizeof(userInfo->appKey), strlen(USER_APP_KEY)));
    memcpy(userInfo->appLicense, USER_APP_LICENSE,
           USER_UTIL_MIN(sizeof(userInfo->appLicense), strlen(USER_APP_LICENSE)));
    memcpy(userInfo->baudRate, USER_BAUD_RATE, USER_UTIL_MIN(sizeof(userInfo->baudRate), strlen(USER_BAUD_RATE)));
    strncpy(userInfo->developerAccount, USER_DEVELOPER_ACCOUNT, sizeof(userInfo->developerAccount) - 1);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduUser_PrintConsole(const uint8_t *data, uint16_t dataLen)
{
    UART_Write(GDU_CONSOLE_UART_NUM, (uint8_t *) data, dataLen);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

#ifdef __cplusplus
}
#endif
/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/


