/**
 ********************************************************************
 * @file    test_upgrade.c
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
#include <gdu_logger.h>
#include <gdu_platform.h>
#include <utils/util_misc.h>
#include "test_upgrade_common_file_transfer.h"
#include "test_upgrade_platform_opt.h"
#include "test_upgrade.h"

/* Private constants ---------------------------------------------------------*/
#define UPGRADE_TASK_STACK_SIZE                (2048)
#define GDU_TEST_UPGRADE_TASK_FREQ             (50)
#define GDU_TEST_ENTER_UPGRADE_WAIT_TIME       (10)  //wait 10s for enter upgrade process
#define GDU_TEST_UPGRADE_REBOOT_TIMEOUT        (30)   //reboot timeout 30s

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/
static T_GduUpgradeState s_upgradeState = {0};
static T_GduMutexHandle s_upgradeStateMutex = {0};
static T_GduTaskHandle s_upgradeProcessThread;
static T_GduTaskHandle s_enterUpgradeModeProcessThread;
static bool s_isNeedEnterUpgradeModeProcess = false;
static bool s_isNeedReplaceProgramBeforeReboot = false;

/* Private functions declaration ---------------------------------------------*/
static T_GduReturnCode GduTest_EnterUpgradeMode(uint16_t *waitTime);
static T_GduReturnCode GduTest_CheckFirmware(void);
static T_GduReturnCode GduTest_StartUpgrade(void);
static T_GduReturnCode GduTest_FinishUpgrade(void);
static void *GduTest_UpgradeProcessTask(void *arg);
static void *GduTest_EnterUpgradeModeProcessTask(void *arg);

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode
GduTest_UpgradeStartService(const T_GduTestUpgradePlatformOpt *upgradePlatformOpt,
                            T_GduTestUpgradeConfig testUpgradeConfig)
{
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    T_GduReturnCode returnCode;
    bool isUpgradeReboot = false;
    T_GduUpgradeEndInfo upgradeEndInfo = {0};
    T_GduUpgradeConfig upgradeConfig = {
        .currentFirmwareVersion = testUpgradeConfig.firmwareVersion,
        .firmwareTransferInfo = {
            .transferType = testUpgradeConfig.transferType,
            .ftpTransferInfo.port = 21,
            .dcftpFileTransferOpt = {
                .start = GduTestCommonFileTransfer_Start,
                .transfer = GduTestCommonFileTransfer_Transfer,
                .finish = GduTestCommonFileTransfer_Finish,
            }
        }
    };
    s_isNeedReplaceProgramBeforeReboot = testUpgradeConfig.needReplaceProgramBeforeReboot;

    T_GduUpgradeHandler s_upgradeHandler = {
        .EnterUpgradeMode = GduTest_EnterUpgradeMode,
        .CheckFirmware = GduTest_CheckFirmware,
        .StartUpgrade = GduTest_StartUpgrade,
        .FinishUpgrade = GduTest_FinishUpgrade
    };

    returnCode = GduTest_RegUpgradePlatformOpt(upgradePlatformOpt);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Reg upgrade platform opt error, return code = 0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = osalHandler->MutexCreate(&s_upgradeStateMutex);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Create mutex error");
        return returnCode;
    }

    returnCode = GduTest_GetUpgradeRebootState(&isUpgradeReboot, &upgradeEndInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get upgrade reboot state error");
        isUpgradeReboot = false;
    }

    returnCode = GduTest_CleanUpgradeRebootState();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Clean upgrade reboot state error");
    }

    osalHandler->MutexLock(s_upgradeStateMutex);
    if (isUpgradeReboot == true) {
        s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_END;
        s_upgradeState.upgradeEndInfo = upgradeEndInfo;
    } else {
        s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_IDLE;
    }
    osalHandler->MutexUnlock(s_upgradeStateMutex);

    returnCode = GduUpgrade_Init(&upgradeConfig);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("GduUpgrade_Init error, return code = %d", returnCode);
        return returnCode;
    }

    returnCode = GduUpgrade_RegHandler(&s_upgradeHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("GduUpgrade_RegHandler error, return code = %d", returnCode);
        return returnCode;
    }

    returnCode = GduUpgrade_EnableLocalUpgrade();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("GduUpgrade_EnableLocalUpgrade error, return code = %d", returnCode);
        return returnCode;
    }

    if (osalHandler->TaskCreate("upgrade_task", GduTest_UpgradeProcessTask, UPGRADE_TASK_STACK_SIZE, NULL,
                                &s_upgradeProcessThread) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Gdu upgrade test task create error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (osalHandler->TaskCreate("enter_upgrade_mode_task", GduTest_EnterUpgradeModeProcessTask, UPGRADE_TASK_STACK_SIZE,
                                NULL, &s_enterUpgradeModeProcessThread) !=
        GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Gdu upgrade test task create error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/
static T_GduReturnCode GduTest_EnterUpgradeMode(uint16_t *waitTime)
{
    // need 10s for upgrade preprocess work.
    *waitTime = GDU_TEST_ENTER_UPGRADE_WAIT_TIME;
    // enable is need enter upgrade mode process, the process is in  GduTest_EnterUpgradeModeProcessTask
    s_isNeedEnterUpgradeModeProcess = true;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduTest_CheckFirmware(void)
{
    // you can do decrypt and check firmware in this stage
    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduTest_StartUpgrade(void)
{
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    osalHandler->MutexLock(s_upgradeStateMutex);
    s_upgradeState.upgradeOngoingInfo.upgradeProgress = 0;
    s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_ONGOING;
    osalHandler->MutexUnlock(s_upgradeStateMutex);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduTest_FinishUpgrade(void)
{
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    osalHandler->MutexLock(s_upgradeStateMutex);
    s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_IDLE;
    osalHandler->MutexUnlock(s_upgradeStateMutex);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *GduTest_EnterUpgradeModeProcessTask(void *arg)
{
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    T_GduReturnCode returnCode;

    USER_UTIL_UNUSED(arg);

    while (1) {
        if (s_isNeedEnterUpgradeModeProcess) {
            // prepare enter upgrade mode
            // you can do some thing before enter upgrade mode.
            // clear upgrade program file store area
            returnCode = GduTest_CleanUpgradeProgramFileStoreArea();
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Clean upgrade file dir error, please check dir permission");
            }

            s_isNeedEnterUpgradeModeProcess = false;
            USER_LOG_INFO("Clean upgrade store area");
        }
        osalHandler->TaskSleepMs(1000 / GDU_TEST_UPGRADE_TASK_FREQ);
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *GduTest_UpgradeProcessTask(void *arg)
{
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
    T_GduUpgradeState tempUpgradeState;
    T_GduUpgradeEndInfo upgradeEndInfo;
    T_GduReturnCode returnCode;

    USER_UTIL_UNUSED(arg);

    while (1) {
        osalHandler->MutexLock(s_upgradeStateMutex);
        tempUpgradeState = s_upgradeState;
        osalHandler->MutexUnlock(s_upgradeStateMutex);

        if (tempUpgradeState.upgradeStage == GDU_UPGRADE_STAGE_ONGOING) {
            if (s_isNeedReplaceProgramBeforeReboot) {
                // Step 1 : Replace old program
                returnCode = GduTest_ReplaceOldProgram();
                if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    USER_LOG_ERROR("Replace firmware error, return code = 0x%08llX", returnCode);
                    osalHandler->MutexLock(s_upgradeStateMutex);
                    s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_END;
                    s_upgradeState.upgradeEndInfo.upgradeEndState = GDU_UPGRADE_END_STATE_UNKNOWN_ERROR;
                    osalHandler->MutexUnlock(s_upgradeStateMutex);
                    continue;
                }

                osalHandler->TaskSleepMs(1000);
                osalHandler->MutexLock(s_upgradeStateMutex);
                s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_ONGOING;
                s_upgradeState.upgradeOngoingInfo.upgradeProgress = 20;
                GduUpgrade_PushUpgradeState(&s_upgradeState);
                osalHandler->MutexUnlock(s_upgradeStateMutex);

                // Step 2 : Clean upgrade program file store area
                returnCode = GduTest_CleanUpgradeProgramFileStoreArea();
                if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    USER_LOG_ERROR("Clean upgrade file dir error, please check dir permission");
                    osalHandler->MutexLock(s_upgradeStateMutex);
                    s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_END;
                    s_upgradeState.upgradeEndInfo.upgradeEndState = GDU_UPGRADE_END_STATE_UNKNOWN_ERROR;
                    osalHandler->MutexUnlock(s_upgradeStateMutex);
                    continue;
                }

                osalHandler->TaskSleepMs(1000);
                osalHandler->MutexLock(s_upgradeStateMutex);
                s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_ONGOING;
                s_upgradeState.upgradeOngoingInfo.upgradeProgress = 30;
                GduUpgrade_PushUpgradeState(&s_upgradeState);
                osalHandler->MutexUnlock(s_upgradeStateMutex);
            }

            //attention emulation upgrade progress, user don't need this process
            do {
                osalHandler->TaskSleepMs(1000);
                osalHandler->MutexLock(s_upgradeStateMutex);
                s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_ONGOING;
                s_upgradeState.upgradeOngoingInfo.upgradeProgress += 10;
                tempUpgradeState = s_upgradeState;
                GduUpgrade_PushUpgradeState(&s_upgradeState);
                osalHandler->MutexUnlock(s_upgradeStateMutex);
            } while (tempUpgradeState.upgradeOngoingInfo.upgradeProgress < 100);

            // Step 3 : Reboot device
            osalHandler->MutexLock(s_upgradeStateMutex);
            s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_DEVICE_REBOOT;
            s_upgradeState.upgradeRebootInfo.rebootTimeout = GDU_TEST_UPGRADE_REBOOT_TIMEOUT;
            GduUpgrade_PushUpgradeState(&s_upgradeState);
            osalHandler->MutexUnlock(s_upgradeStateMutex);
            osalHandler->TaskSleepMs(1000); // sleep 1000ms to ensure push send terminal.

            upgradeEndInfo.upgradeEndState = GDU_UPGRADE_END_STATE_SUCCESS;
            returnCode = GduTest_SetUpgradeRebootState(&upgradeEndInfo);
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Set Upgrade reboot state error");
                osalHandler->MutexLock(s_upgradeStateMutex);
                s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_END;
                s_upgradeState.upgradeEndInfo.upgradeEndState = GDU_UPGRADE_END_STATE_UNKNOWN_ERROR;
                osalHandler->MutexUnlock(s_upgradeStateMutex);
                continue;
            }

            returnCode = GduTest_RebootSystem();
            if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("Reboot system error");
                osalHandler->MutexLock(s_upgradeStateMutex);
                s_upgradeState.upgradeStage = GDU_UPGRADE_STAGE_END;
                s_upgradeState.upgradeEndInfo.upgradeEndState = GDU_UPGRADE_END_STATE_UNKNOWN_ERROR;
                osalHandler->MutexUnlock(s_upgradeStateMutex);
                continue;
            }
            while (1) {
                osalHandler->TaskSleepMs(500);
            }
        } else if (s_upgradeState.upgradeStage == GDU_UPGRADE_STAGE_END) {
            osalHandler->MutexLock(s_upgradeStateMutex);
            GduUpgrade_PushUpgradeState(&s_upgradeState);
            osalHandler->MutexUnlock(s_upgradeStateMutex);
        }

        osalHandler->TaskSleepMs(500);
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
