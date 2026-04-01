/**
 ********************************************************************
 * @file    test_upgrade_platform_opt.h
 * @brief   This is the header file for "test_upgrade_platform_opt.c", defining the structure and
 * (exported) function prototypes.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TEST_UPGRADE_PLATFORM_OPT_H
#define TEST_UPGRADE_PLATFORM_OPT_H

#include <gdu_typedef.h>
#include <gdu_upgrade.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
    T_GduReturnCode (*rebootSystem)(void);

    T_GduReturnCode (*cleanUpgradeProgramFileStoreArea)(void);
    T_GduReturnCode (*createUpgradeProgramFile)(const T_GduUpgradeFileInfo *fileInfo);
    T_GduReturnCode (*writeUpgradeProgramFile)(uint32_t offset, const uint8_t *data, uint16_t dataLen);
    T_GduReturnCode (*readUpgradeProgramFile)(uint32_t offset, uint16_t readDataLen, uint8_t *data,
                                               uint16_t *realLen);
    T_GduReturnCode (*closeUpgradeProgramFile)(void);

    T_GduReturnCode (*replaceOldProgram)(void);

    T_GduReturnCode (*setUpgradeRebootState)(const T_GduUpgradeEndInfo *upgradeEndInfo);
    T_GduReturnCode (*getUpgradeRebootState)(bool *isUpgradeReboot, T_GduUpgradeEndInfo *upgradeEndInfo);
    T_GduReturnCode (*cleanUpgradeRebootState)(void);
} T_GduTestUpgradePlatformOpt;

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode GduTest_RegUpgradePlatformOpt(const T_GduTestUpgradePlatformOpt *upgradePlatformOpt);

T_GduReturnCode GduTest_RebootSystem(void);

T_GduReturnCode GduTest_CleanUpgradeProgramFileStoreArea(void);
T_GduReturnCode GduTest_CreateUpgradeProgramFile(const T_GduUpgradeFileInfo *fileInfo);
T_GduReturnCode GduTest_WriteUpgradeProgramFile(uint32_t offset, const uint8_t *data, uint16_t dataLen);
T_GduReturnCode GduTest_ReadUpgradeProgramFile(uint32_t offset, uint16_t readDataLen, uint8_t *data,
                                                 uint16_t *realLen);
T_GduReturnCode GduTest_CloseUpgradeProgramFile(void);

T_GduReturnCode GduTest_ReplaceOldProgram(void);

T_GduReturnCode GduTest_SetUpgradeRebootState(const T_GduUpgradeEndInfo *upgradeEndInfo);
T_GduReturnCode GduTest_GetUpgradeRebootState(bool *isUpgradeReboot, T_GduUpgradeEndInfo *upgradeEndInfo);
T_GduReturnCode GduTest_CleanUpgradeRebootState(void);

#ifdef __cplusplus
}
#endif

#endif // TEST_UPGRADE_PLATFORM_OPT_H

/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
