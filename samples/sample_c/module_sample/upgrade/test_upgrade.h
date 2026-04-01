/**
 ********************************************************************
 * @file    test_upgrade.h
 * @brief   This is the header file for "test_upgrade.c", defining the structure and
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
#ifndef TEST_UPGRADE_H
#define TEST_UPGRADE_H

/* Includes ------------------------------------------------------------------*/
#include <gdu_upgrade.h>
#include "test_upgrade_platform_opt.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
    T_GduVersion firmwareVersion;
    E_GduFirmwareTransferType transferType;
    //For linux: need replace program before reboot system
    //For mcu: don't need replace program before reboot system, replace program in loader
    bool needReplaceProgramBeforeReboot;
} T_GduTestUpgradeConfig;

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode GduTest_UpgradeStartService(const T_GduTestUpgradePlatformOpt *upgradePlatformOpt,
                                            T_GduTestUpgradeConfig testUpgradeConfig);

#ifdef __cplusplus
}
#endif

#endif
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
