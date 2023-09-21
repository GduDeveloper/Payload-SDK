/**
 ********************************************************************
 * @file    test_upgrade_platform_opt.c
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
#include "test_upgrade_platform_opt.h"
#include <gdu_logger.h>

/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/
static T_GduTestUpgradePlatformOpt s_upgradePlatformOpt = {0};

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode GduTest_RegUpgradePlatformOpt(const T_GduTestUpgradePlatformOpt *upgradePlatformOpt)
{
    if (upgradePlatformOpt->rebootSystem == NULL) {
        USER_LOG_ERROR("rebootSystem callback can't be NULL");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    if (upgradePlatformOpt->cleanUpgradeProgramFileStoreArea == NULL) {
        USER_LOG_ERROR("cleanUpgradeProgramFileStoreArea callback can't be NULL");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    if (upgradePlatformOpt->createUpgradeProgramFile == NULL) {
        USER_LOG_ERROR("createUpgradeProgramFile callback can't be NULL");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    if (upgradePlatformOpt->readUpgradeProgramFile == NULL) {
        USER_LOG_ERROR("readUpgradeProgramFile callback can't be NULL");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    if (upgradePlatformOpt->writeUpgradeProgramFile == NULL) {
        USER_LOG_ERROR("writeUpgradeProgramFile callback can't be NULL");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    if (upgradePlatformOpt->closeUpgradeProgramFile == NULL) {
        USER_LOG_ERROR("closeUpgradeProgramFile callback can't be NULL");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    if (upgradePlatformOpt->replaceOldProgram == NULL) {
        USER_LOG_ERROR("replaceOldProgram callback can't be NULL");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    if (upgradePlatformOpt->setUpgradeRebootState == NULL) {
        USER_LOG_ERROR("setUpgradeRebootState callback can't be NULL");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    if (upgradePlatformOpt->getUpgradeRebootState == NULL) {
        USER_LOG_ERROR("getUpgradeRebootState callback can't be NULL");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    if (upgradePlatformOpt->cleanUpgradeRebootState == NULL) {
        USER_LOG_ERROR("cleanUpgradeRebootState callback can't be NULL");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    s_upgradePlatformOpt = *upgradePlatformOpt;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_RebootSystem(void)
{
    return s_upgradePlatformOpt.rebootSystem();
}

T_GduReturnCode GduTest_CleanUpgradeProgramFileStoreArea(void)
{
    return s_upgradePlatformOpt.cleanUpgradeProgramFileStoreArea();
}

T_GduReturnCode GduTest_CreateUpgradeProgramFile(const T_GduUpgradeFileInfo *fileInfo)
{
    return s_upgradePlatformOpt.createUpgradeProgramFile(fileInfo);
}

T_GduReturnCode GduTest_WriteUpgradeProgramFile(uint32_t offset, const uint8_t *data, uint16_t dataLen)
{
    return s_upgradePlatformOpt.writeUpgradeProgramFile(offset, data, dataLen);
}

T_GduReturnCode GduTest_ReadUpgradeProgramFile(uint32_t offset, uint16_t readDataLen, uint8_t *data,
                                                 uint16_t *realLen)
{
    return s_upgradePlatformOpt.readUpgradeProgramFile(offset, readDataLen, data, realLen);
}

T_GduReturnCode GduTest_CloseUpgradeProgramFile(void)
{
    return s_upgradePlatformOpt.closeUpgradeProgramFile();
}

T_GduReturnCode GduTest_ReplaceOldProgram(void)
{
    return s_upgradePlatformOpt.replaceOldProgram();
}

T_GduReturnCode GduTest_SetUpgradeRebootState(const T_GduUpgradeEndInfo *upgradeEndInfo)
{
    return s_upgradePlatformOpt.setUpgradeRebootState(upgradeEndInfo);
}

T_GduReturnCode GduTest_GetUpgradeRebootState(bool *isUpgradeReboot, T_GduUpgradeEndInfo *upgradeEndInfo)
{
    return s_upgradePlatformOpt.getUpgradeRebootState(isUpgradeReboot, upgradeEndInfo);
}

T_GduReturnCode GduTest_CleanUpgradeRebootState(void)
{
    return s_upgradePlatformOpt.cleanUpgradeRebootState();
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
