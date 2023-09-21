/**
 ********************************************************************
 * @file    test_upgrade_common_file_transfer.h
 * @brief   This is the header file for "test_upgrade_common_file_transfer.c", defining the structure and
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
#ifndef TEST_UPGRADE_COMMON_FILE_TRANSFER_H
#define TEST_UPGRADE_COMMON_FILE_TRANSFER_H

/* Includes ------------------------------------------------------------------*/
#include <gdu_upgrade.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode GduTestCommonFileTransfer_Start(const T_GduUpgradeFileInfo *fileInfo);
T_GduReturnCode GduTestCommonFileTransfer_Transfer(const uint8_t *data, uint16_t dataLen);
T_GduReturnCode GduTestCommonFileTransfer_Finish(const uint8_t md5[GDU_MD5_BUFFER_LEN]);
int GduTestCommonFileTransfer_getTransferProccess(void);

#ifdef __cplusplus
}
#endif

#endif // TEST_UPGRADE_COMMON_FILE_TRANSFER_LINUX_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
