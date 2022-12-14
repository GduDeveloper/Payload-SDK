/**
 ********************************************************************
 * @file    upgrade_platform_opt_stm32.h
 * @version V2.0.0
 * @date    3/25/20
 * @brief   This is the header file for "upgrade_platform_opt_stm32.c", defining the structure and
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
#ifndef UPGRADE_PLATFORM_OPT_STM32_H
#define UPGRADE_PLATFORM_OPT_STM32_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"
#include "gdu_upgrade.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode GduUpgradePlatformStm32_RebootSystem(void);

T_GduReturnCode GduUpgradePlatformStm32_CleanUpgradeProgramFileStoreArea(void);
T_GduReturnCode GduUpgradePlatformStm32_CreateUpgradeProgramFile(const T_GduUpgradeFileInfo *fileInfo);
T_GduReturnCode GduUpgradePlatformStm32_WriteUpgradeProgramFile(uint32_t offset, const uint8_t *data,
                                                                uint16_t dataLen);
T_GduReturnCode GduUpgradePlatformStm32_ReadUpgradeProgramFile(uint32_t offset, uint16_t readDataLen, uint8_t *data,
                                                               uint16_t *realLen);
T_GduReturnCode GduUpgradePlatformStm32_CloseUpgradeProgramFile(void);

T_GduReturnCode GduUpgradePlatformStm32_ReplaceOldProgram(void);

T_GduReturnCode GduUpgradePlatformStm32_SetUpgradeRebootState(const T_GduUpgradeEndInfo *upgradeEndInfo);
T_GduReturnCode GduUpgradePlatformStm32_GetUpgradeRebootState(bool *isUpgradeReboot,
                                                              T_GduUpgradeEndInfo *upgradeEndInfo);
T_GduReturnCode GduUpgradePlatformStm32_CleanUpgradeRebootState(void);


#ifdef __cplusplus
}
#endif

#endif // UPGRADE_PLATFORM_OPT_STM32_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
