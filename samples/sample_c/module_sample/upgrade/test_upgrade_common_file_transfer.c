/**
 ********************************************************************
 * @file    test_upgrade_common_file_transfer.c
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
#include "test_upgrade_common_file_transfer.h"
#include "gdu_logger.h"
#include <utils/util_md5.h>
#include "test_upgrade_platform_opt.h"

/* Private constants ---------------------------------------------------------*/
#define GDU_TEST_FILE_MD5_BUFFER_SIZE              256

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/
static T_GduUpgradeFileInfo s_upgradeFileInfo = {0};
static uint32_t s_alreadyTransferFileSize = 0;

/* Private functions declaration ---------------------------------------------*/
static T_GduReturnCode GduTestFile_GetUpgradeFileMd5(uint8_t md5[GDU_MD5_BUFFER_LEN]);

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode GduTestCommonFileTransfer_Start(const T_GduUpgradeFileInfo *fileInfo)
{
    T_GduReturnCode returnCode;

    s_upgradeFileInfo.fileSize = 0;
    memset(s_upgradeFileInfo.fileName, 0, sizeof(s_upgradeFileInfo.fileName));
    s_alreadyTransferFileSize = 0;

    returnCode = GduTest_CreateUpgradeProgramFile(fileInfo);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Create upgrade program file error");
        return returnCode;
    }

    s_upgradeFileInfo = *fileInfo;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

int GduTestCommonFileTransfer_getTransferProccess(void)
{
	int proccess = (int)((s_alreadyTransferFileSize * 100 / s_upgradeFileInfo.fileSize));

	return proccess;
}

T_GduReturnCode GduTestCommonFileTransfer_Transfer(const uint8_t *data, uint16_t dataLen)
{
    T_GduReturnCode returnCode;

    if (s_alreadyTransferFileSize >= s_upgradeFileInfo.fileSize) {
        USER_LOG_ERROR("Already transfer file size is more than file real size");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = GduTest_WriteUpgradeProgramFile(s_alreadyTransferFileSize, data, dataLen);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Write upgrade program file error, return code = 0x%08llX", returnCode);
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    s_alreadyTransferFileSize += dataLen;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTestCommonFileTransfer_Finish(const uint8_t md5[GDU_MD5_BUFFER_LEN])
{
    uint8_t localFileMd5[GDU_MD5_BUFFER_LEN] = {0};
    T_GduReturnCode returnCode;

    if (s_alreadyTransferFileSize != s_upgradeFileInfo.fileSize) {
        USER_LOG_ERROR("Transfer finish error, transfer file size is not equal");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = GduTestFile_GetUpgradeFileMd5(localFileMd5);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file md5 error, return code = 0x%08llX", returnCode);
        goto out;
    }

    if (memcmp(md5, localFileMd5, GDU_MD5_BUFFER_LEN) == 0) {
        returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    } else {
        returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

out:
    GduTest_CloseUpgradeProgramFile();
    s_upgradeFileInfo.fileSize = 0;
    memset(s_upgradeFileInfo.fileName, 0, sizeof(s_upgradeFileInfo.fileName));
    s_alreadyTransferFileSize = 0;
    return returnCode;
}

/* Private functions definition-----------------------------------------------*/
static T_GduReturnCode GduTestFile_GetUpgradeFileMd5(uint8_t md5[GDU_MD5_BUFFER_LEN])
{
    uint8_t fileBuffer[GDU_TEST_FILE_MD5_BUFFER_SIZE] = {0};
    T_GduReturnCode returnCode;
    uint32_t offset;
    MD5_CTX fileMd5Ctx;
    uint16_t realLen = 0;

    offset = 0;
    UtilMd5_Init(&fileMd5Ctx);
    while (s_upgradeFileInfo.fileSize - offset > GDU_TEST_FILE_MD5_BUFFER_SIZE) {
        returnCode = GduTest_ReadUpgradeProgramFile(offset, GDU_TEST_FILE_MD5_BUFFER_SIZE,
                                                     fileBuffer, &realLen);
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS || realLen != GDU_TEST_FILE_MD5_BUFFER_SIZE) {
            USER_LOG_ERROR("Get file data error, return code = 0x%08llX", returnCode);
            return returnCode;
        }

        UtilMd5_Update(&fileMd5Ctx, fileBuffer, GDU_TEST_FILE_MD5_BUFFER_SIZE);

        offset += GDU_TEST_FILE_MD5_BUFFER_SIZE;
    }

    returnCode = GduTest_ReadUpgradeProgramFile(offset, s_upgradeFileInfo.fileSize - offset, fileBuffer, &realLen);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS || realLen != s_upgradeFileInfo.fileSize - offset) {
        USER_LOG_ERROR("Get file data error, return code = 0x%08llX", returnCode);
        return returnCode;
    }

    UtilMd5_Update(&fileMd5Ctx, fileBuffer, s_upgradeFileInfo.fileSize - offset);
    UtilMd5_Final(&fileMd5Ctx, md5);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
