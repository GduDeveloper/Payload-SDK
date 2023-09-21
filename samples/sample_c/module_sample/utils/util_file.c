/**
 ********************************************************************
 * @file    util_file.c
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
#ifdef SYSTEM_ARCH_LINUX

#include "util_file.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* Private constants ---------------------------------------------------------*/


/* Private types -------------------------------------------------------------*/


/* Private functions declaration ---------------------------------------------*/


/* Private values ------------------------------------------------------------*/


/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode UtilFile_GetCreateTime(const char *filePath, T_UtilFileCreateTime *createTime)
{
    struct stat st;
    struct tm *fileTm;

    if (filePath == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    if (stat(filePath, &st) != 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    fileTm = localtime(&(st.st_ctime));
    if (fileTm == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    createTime->year = fileTm->tm_year + 1900 - 1980;
    createTime->month = fileTm->tm_mon;
    createTime->day = fileTm->tm_mday;
    createTime->hour = fileTm->tm_hour;
    createTime->minute = fileTm->tm_min;
    createTime->second = fileTm->tm_sec;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode UtilFile_GetFileSizeByPath(const char *filePath, uint32_t *fileSize)
{
    struct stat st;

    if (filePath == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    if (stat(filePath, &st) != 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    *fileSize = st.st_size;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode UtilFile_GetFileDataByPath(const char *filePath, uint32_t offset, uint16_t len,
                                           uint8_t *data, uint16_t *realLen)
{
    FILE *pF;
    T_GduReturnCode psdkStat;
    uint32_t readRtn;

    if (filePath == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    pF = fopen(filePath, "rb+");
    if (pF == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    if (fseek(pF, offset, SEEK_SET) != 0) {
        psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        goto out;
    }

    readRtn = fread(data, 1, len, pF);
    if (readRtn == 0 || readRtn > len) {
        psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        goto out;
    }

    *realLen = readRtn;

    psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;

out:
    fclose(pF);

    return psdkStat;
}

T_GduReturnCode UtilFile_Delete(const char *filePath)
{
    int ret;

    if (filePath == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = unlink(filePath);

    if (ret != 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    } else {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }
}

T_GduReturnCode UtilFile_GetFileSize(FILE *file, uint32_t *fileSize)
{
    int result;

    if (file == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    long int curSeek = ftell(file);

    result = fseek(file, 0L, SEEK_END);
    if (result != 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    *fileSize = ftell(file);

    if (curSeek < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    result = fseek(file, curSeek, SEEK_SET);
    if (result != 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode UtilFile_GetFileData(FILE *file, uint32_t offset, uint16_t len, uint8_t *data, uint16_t *realLen)
{
    T_GduReturnCode psdkStat;
    uint32_t readRtn;

    if (file == NULL) {
        psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
        goto out;
    }

    if (fseek(file, offset, SEEK_SET) != 0) {
        psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        goto out;
    }

    readRtn = fread(data, 1, len, file);
    if (readRtn == 0 || readRtn > len) {
        psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        goto out;
    }

    *realLen = readRtn;

    psdkStat = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;

out:
    return psdkStat;
}

/* Private functions definition-----------------------------------------------*/

#endif

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
