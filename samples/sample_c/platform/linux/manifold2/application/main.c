/**
 ********************************************************************
 * @file    main.c
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
#include <gdu_platform.h>
#include <gdu_logger.h>
#include <gdu_core.h>
#include <utils/util_misc.h>
#include <errno.h>
#include <signal.h>

//#include <power_management/test_power_management.h>
//#include <upgrade/test_upgrade.h>
//#include <payload_collaboration/test_payload_collaboration.h>
//#include <mop_channel/test_mop_channel.h>
//#include <xport/test_payload_xport.h>
//#include <gimbal_emu/test_payload_gimbal_emu.h>
//#include <fc_subscription/test_fc_subscription.h>
//#include <camera_emu/test_payload_cam_emu_media.h>

#include <camera_emu/test_payload_cam_emu_base.h>
#include "monitor/sys_monitor.h"
#include "osal/osal.h"
#include "osal/osal_fs.h"
#include "osal/osal_socket.h"
//#include "upgrade_platform_opt/upgrade_platform_opt_linux.h"
#include "../hal/hal_uart.h"
#include "../hal/hal_network.h"
//#include "../hal/hal_usb_bulk.h"
#include "gdu_sdk_app_info.h"
#include "gdu_aircraft_info.h"
#include "widget/test_widget.h"
#include "widget/test_widget_speaker.h"
//#include "widget_interaction_test/test_widget_interaction.h"
//#include "data_transmission/test_data_transmission.h"
#include "gdu_sdk_config.h"

/* Private constants ---------------------------------------------------------*/
#define GDU_LOG_PATH                    "Logs/GDU"
#define GDU_LOG_INDEX_FILE_NAME         "Logs/latest"
#define GDU_LOG_FOLDER_NAME             "Logs"
#define GDU_LOG_PATH_MAX_SIZE           (128)
#define GDU_LOG_FOLDER_NAME_MAX_SIZE    (32)
#define GDU_LOG_SYSTEM_CMD_MAX_SIZE     (64)
#define GDU_LOG_MAX_COUNT               (10)

#define GDU_USE_WIDGET_INTERACTION       0

/* Private types -------------------------------------------------------------*/
typedef struct {
	pid_t tid;
	char name[16];
	float pcpu;
} T_ThreadAttribute;

/* Private values -------------------------------------------------------------*/
static FILE *s_gduLogFile;
static FILE *s_gduLogFileCnt;
static pthread_t s_monitorThread = 0;

/* Private functions declaration ---------------------------------------------*/
static T_GduReturnCode GduUser_FillInUserInfo(T_GduUserInfo *userInfo);
static T_GduReturnCode GduUser_PrintConsole(const uint8_t *data, uint16_t dataLen);
static T_GduReturnCode GduUser_LocalWrite(const uint8_t *data, uint16_t dataLen);
static T_GduReturnCode GduUser_LocalWriteFsInit(const char *path);
static void *GduUser_MonitorTask(void *argument);
static T_GduReturnCode GduTest_HighPowerApplyPinInit();
//static T_GduReturnCode GduTest_WriteHighPowerApplyPin(E_GduPowerManagementPinState pinState);

/* Exported functions definition ---------------------------------------------*/
int main(int argc, char **argv)
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
		//.GetIPAddr = Osal_GetIpAddr,

	};
	T_GduLoggerConsole printConsole = {
		.func = GduUser_PrintConsole,
		//.consoleLevel = GDU_LOGGER_CONSOLE_LOG_LEVEL_INFO,
		.consoleLevel = GDU_LOGGER_CONSOLE_LOG_LEVEL_DEBUG,
		.isSupportColor = true,
	};

	T_GduLoggerConsole localRecordConsole = {
		.consoleLevel = GDU_LOGGER_CONSOLE_LOG_LEVEL_DEBUG,
		.func = GduUser_LocalWrite,
		.isSupportColor = false,
	};

	T_GduHalUartHandler uartHandler = {
		.UartInit = HalUart_Init,
		.UartDeInit = HalUart_DeInit,
		.UartWriteData = HalUart_WriteData,
		.UartReadData = HalUart_ReadData,
		.UartGetStatus = HalUart_GetStatus,
	};

	T_GduHalNetworkHandler networkHandler = {
		.NetworkInit = HalNetWork_Init,
		.NetworkDeInit = HalNetWork_DeInit,
	};

	// T_GduHalUsbBulkHandler usbBulkHandler = {
	// 	.UsbBulkInit = HalUsbBulk_Init,
	// 	.UsbBulkDeInit = HalUsbBulk_DeInit,
	// 	.UsbBulkWriteData = HalUsbBulk_WriteData,
	// 	.UsbBulkReadData = HalUsbBulk_ReadData,
	// 	.UsbBulkGetDeviceInfo = HalUsbBulk_GetDeviceInfo,
	// };

	T_GduFileSystemHandler fileSystemHandler = {
		.FileOpen = Osal_FileOpen,
		.FileClose = Osal_FileClose,
		.FileWrite = Osal_FileWrite,
		.FileRead = Osal_FileRead,
		.FileSync = Osal_FileSync,
		.FileSeek = Osal_FileSeek,
		.DirOpen = Osal_DirOpen,
		.DirClose = Osal_DirClose,
		.DirRead = Osal_DirRead,
		.Mkdir = Osal_Mkdir,
		.Unlink = Osal_Unlink,
		.Rename = Osal_Rename,
		.Stat = Osal_Stat,
	};

	T_GduSocketHandler socketHandler = {
		.Socket = Osal_Socket,
		.Bind = Osal_Bind,
		.Close = Osal_Close,
		.UdpSendData = Osal_UdpSendData,
		.UdpRecvData = Osal_UdpRecvData,
		.TcpListen = Osal_TcpListen,
		.TcpAccept = Osal_TcpAccept,
		.TcpConnect = Osal_TcpConnect,
		.TcpSendData = Osal_TcpSendData,
		.TcpRecvData = Osal_TcpRecvData,
	};

	returnCode = GduPlatform_RegOsalHandler(&osalHandler);
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		printf("register osal handler error");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	returnCode = GduPlatform_RegHalUartHandler(&uartHandler);
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		printf("register hal uart handler error");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	// returnCode = GduPlatform_RegHalUsbBulkHandler(&usbBulkHandler);
	// if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
	// 	printf("register hal usb bulk handler error");
	// 	return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	// }

	returnCode = GduPlatform_RegHalNetworkHandler(&networkHandler);
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		printf("register hal network handler error");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	returnCode = GduPlatform_RegSocketHandler(&socketHandler);
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		printf("register osal socket handler error");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	returnCode = GduPlatform_RegFileSystemHandler(&fileSystemHandler);
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		printf("register osal filesystem handler error");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	if (GduUser_LocalWriteFsInit(GDU_LOG_PATH) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		printf("file system init error");
		return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
	}

	returnCode = GduLogger_AddConsole(&printConsole);
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		printf("add printf console error");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	returnCode = GduLogger_AddConsole(&localRecordConsole);
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		printf("add printf console error");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	returnCode = GduUser_FillInUserInfo(&userInfo);
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		osalHandler.TaskSleepMs(1000);
		USER_LOG_ERROR("fill user info error, please check user info config");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	returnCode = GduCore_Init(&userInfo);
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		USER_LOG_ERROR("core init error");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	returnCode = GduAircraftInfo_GetBaseInfo(&aircraftInfoBaseInfo);
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		USER_LOG_ERROR("get aircraft base info error");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	returnCode = GduCore_SetAlias("PSDK_APPALIAS");
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		USER_LOG_ERROR("set alias error");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	if (aircraftInfoBaseInfo.mountPosition == GDU_MOUNT_POSITION_EXTENSION_PORT) {
#if 0
		returnCode = GduTest_DataTransmissionStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("data transmission init error");
		}

		returnCode = GduTest_WidgetInteractionStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("widget interaction sample init error");
		}
#endif
	} else {
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

#ifdef CONFIG_MODULE_SAMPLE_CAMERA_EMU_ON
		returnCode = GduTest_CameraEmuBaseStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("camera emu common init error");
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_CAMERA_MEDIA_ON
		returnCode = GduTest_CameraEmuMediaStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("camera emu media init error");
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_FC_SUBSCRIPTION_ON
		returnCode = GduTest_FcSubscriptionStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("data subscription sample init error\n");
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_GIMBAL_ON
#if 0
		if (aircraftInfoBaseInfo.gduAdapterType == GDU_SDK_ADAPTER_TYPE_SKYPORT_V2 ||
				aircraftInfoBaseInfo.gduAdapterType == GDU_SDK_ADAPTER_TYPE_NONE) {
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

#ifdef CONFIG_MODULE_SAMPLE_WIDGET_ON
#if GDU_USE_WIDGET_INTERACTION
		returnCode = GduTest_WidgetInteractionStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("widget interaction test init error");
		}
#else
		returnCode = GduTest_WidgetStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("widget sample init error");
		}
#endif
#endif

#ifdef CONFIG_MODULE_SAMPLE_WIDGET_SPEAKER_ON
		returnCode = GduTest_WidgetSpeakerStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("widget speaker test init error");
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_DATA_TRANSMISSION_ON
		returnCode = GduTest_DataTransmissionStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("widget sample init error");
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_MOP_CHANNEL_ON
		returnCode = GduTest_MopChannelStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("mop channel sample init error");
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_PAYLOAD_COLLABORATION_ON
		returnCode = GduTest_PayloadCollaborationStartService();
		if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("Payload collaboration sample init error\n");
		}
#endif

#ifdef CONFIG_MODULE_SAMPLE_UPGRADE_ON
		T_GduTestUpgradePlatformOpt linuxUpgradePlatformOpt = {
			.rebootSystem = GduUpgradePlatformLinux_RebootSystem,
			.cleanUpgradeProgramFileStoreArea = GduUpgradePlatformLinux_CleanUpgradeProgramFileStoreArea,
			.createUpgradeProgramFile = GduUpgradePlatformLinux_CreateUpgradeProgramFile,
			.writeUpgradeProgramFile = GduUpgradePlatformLinux_WriteUpgradeProgramFile,
			.readUpgradeProgramFile = GduUpgradePlatformLinux_ReadUpgradeProgramFile,
			.closeUpgradeProgramFile = GduUpgradePlatformLinux_CloseUpgradeProgramFile,
			.replaceOldProgram = GduUpgradePlatformLinux_ReplaceOldProgram,
			.setUpgradeRebootState = GduUpgradePlatformLinux_SetUpgradeRebootState,
			.getUpgradeRebootState = GduUpgradePlatformLinux_GetUpgradeRebootState,
			.cleanUpgradeRebootState = GduUpgradePlatformLinux_CleanUpgradeRebootState,
		};
		T_GduTestUpgradeConfig testUpgradeConfig = {
			.firmwareVersion = {1, 0, 0, 0},
			.transferType = GDU_FIRMWARE_TRANSFER_TYPE_FTP,
			.needReplaceProgramBeforeReboot = true
		};
		if (GduTest_UpgradeStartService(&linuxUpgradePlatformOpt, testUpgradeConfig) !=
				GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
			USER_LOG_ERROR("psdk upgrade init error");
		}
#endif
	}

	returnCode = GduCore_ApplicationStart();
	if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
		USER_LOG_ERROR("start sdk application error");
	}

#if 0
	if (pthread_create(&s_monitorThread, NULL, GduUser_MonitorTask, NULL) != 0) {
		USER_LOG_ERROR("create monitor task fail.");
	}

	if (pthread_setname_np(s_monitorThread, "monitor task") != 0) {
		USER_LOG_ERROR("set name for monitor task fail.");
	}
#endif

	while (1) {
		sleep(1);
	}
}

/* Private functions definition-----------------------------------------------*/
static T_GduReturnCode GduUser_PrintConsole(const uint8_t *data, uint16_t dataLen)
{
	USER_UTIL_UNUSED(dataLen);

	printf("%s", data);

	return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduUser_LocalWrite(const uint8_t *data, uint16_t dataLen)
{
	int32_t realLen;

	if (s_gduLogFile == NULL) {
		return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
	}

	realLen = fwrite(data, 1, dataLen, s_gduLogFile);
	fflush(s_gduLogFile);
	if (realLen == dataLen) {
		return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
	} else {
		return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
	}
}

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
				"Please fill in correct user information to 'samples/sample_c/platform/linux/manifold2/application/gdu_sdk_app_info.h' file.");
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

static T_GduReturnCode GduUser_LocalWriteFsInit(const char *path)
{
	T_GduReturnCode gduReturnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
	char filePath[GDU_LOG_PATH_MAX_SIZE];
	char systemCmd[GDU_LOG_SYSTEM_CMD_MAX_SIZE];
	char folderName[GDU_LOG_FOLDER_NAME_MAX_SIZE];
	time_t currentTime = time(NULL);
	struct tm *localTime = localtime(&currentTime);
	uint16_t logFileIndex = 0;
	uint16_t currentLogFileIndex;
	uint8_t ret;

	if (localTime == NULL) {
		printf("Get local time error.\r\n");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	if (access(GDU_LOG_FOLDER_NAME, F_OK) != 0) {
		sprintf(folderName, "mkdir %s", GDU_LOG_FOLDER_NAME);
		ret = system(folderName);
		if (ret != 0) {
			return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
		}
	}

	s_gduLogFileCnt = fopen(GDU_LOG_INDEX_FILE_NAME, "rb+");
	if (s_gduLogFileCnt == NULL) {
		s_gduLogFileCnt = fopen(GDU_LOG_INDEX_FILE_NAME, "wb+");
		if (s_gduLogFileCnt == NULL) {
			return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
		}
	} else {
		ret = fseek(s_gduLogFileCnt, 0, SEEK_SET);
		if (ret != 0) {
			printf("Seek log count file error, ret: %d, errno: %d.\r\n", ret, errno);
			return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
		}

		ret = fread((uint16_t *) &logFileIndex, 1, sizeof(uint16_t), s_gduLogFileCnt);
		if (ret != sizeof(uint16_t)) {
			printf("Read log file index error.\r\n");
		}
	}

	currentLogFileIndex = logFileIndex;
	logFileIndex++;

	ret = fseek(s_gduLogFileCnt, 0, SEEK_SET);
	if (ret != 0) {
		printf("Seek log file error, ret: %d, errno: %d.\r\n", ret, errno);
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	ret = fwrite((uint16_t *) &logFileIndex, 1, sizeof(uint16_t), s_gduLogFileCnt);
	if (ret != sizeof(uint16_t)) {
		printf("Write log file index error.\r\n");
		fclose(s_gduLogFileCnt);
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	fclose(s_gduLogFileCnt);

	sprintf(filePath, "%s_%04d_%04d%02d%02d_%02d-%02d-%02d.log", path, currentLogFileIndex,
			localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
			localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

	s_gduLogFile = fopen(filePath, "wb+");
	if (s_gduLogFile == NULL) {
		USER_LOG_ERROR("Open filepath time error.");
		return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
	}

	if (logFileIndex >= GDU_LOG_MAX_COUNT) {
		sprintf(systemCmd, "rm -rf %s_%04d*.log", path, currentLogFileIndex - GDU_LOG_MAX_COUNT);
		system(systemCmd);
	}

	return gduReturnCode;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"

static void *GduUser_MonitorTask(void *argument)
{
	unsigned int i = 0;
	unsigned int threadCount = 0;
	pid_t *tidList = NULL;
	T_ThreadAttribute *threadAttribute = NULL;
	T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

	USER_UTIL_UNUSED(argument);

	while (1) {
		threadCount = Monitor_GetThreadCountOfProcess(getpid());
		tidList = osalHandler->Malloc(threadCount * sizeof(pid_t));
		if (tidList == NULL) {
			USER_LOG_ERROR("malloc fail.");
			goto delay;
		}
		Monitor_GetTidListOfProcess(getpid(), tidList, threadCount);

		threadAttribute = osalHandler->Malloc(threadCount * sizeof(T_ThreadAttribute));
		if (threadAttribute == NULL) {
			USER_LOG_ERROR("malloc fail.");
			goto freeTidList;
		}
		for (i = 0; i < threadCount; ++i) {
			threadAttribute[i].tid = tidList[i];
		}

		USER_LOG_DEBUG("thread pcpu:");
		USER_LOG_DEBUG("tid\tname\tpcpu");
		for (i = 0; i < threadCount; ++i) {
			threadAttribute[i].pcpu = Monitor_GetPcpuOfThread(getpid(), tidList[i]);
			Monitor_GetNameOfThread(getpid(), tidList[i], threadAttribute[i].name, sizeof(threadAttribute[i].name));
			USER_LOG_DEBUG("%d\t%15s\t%f %%.", threadAttribute[i].tid, threadAttribute[i].name,
					threadAttribute[i].pcpu);
		}

		USER_LOG_DEBUG("heap used: %d B.", Monitor_GetHeapUsed(getpid()));
		USER_LOG_DEBUG("stack used: %d B.", Monitor_GetStackUsed(getpid()));

		osalHandler->Free(threadAttribute);
freeTidList:
		osalHandler->Free(tidList);

delay:
		sleep(10);
	}
}

static T_GduReturnCode GduTest_HighPowerApplyPinInit()
{
	return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

#if 0
static T_GduReturnCode GduTest_WriteHighPowerApplyPin(E_GduPowerManagementPinState pinState)
{
	//attention: please pull up the HWPR pin state by hardware.
	return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
#endif

#pragma GCC diagnostic pop

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
