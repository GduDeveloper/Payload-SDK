/**
 ********************************************************************
 * @file    osal_fs.c
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
#include "osal_fs.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <sys/stat.h>
#include <dirent.h>
#include "time.h"

/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode Osal_FileOpen(const char *fileName, const char *fileMode, T_GduFileHandle *fileObj)
{
    if (fileName == NULL || fileMode == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    *fileObj = malloc(sizeof(FILE));
    if (*fileObj == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    *fileObj = fopen(fileName, fileMode);
    if (*fileObj == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_FileClose(T_GduFileHandle fileObj)
{
    int32_t ret;

    if (fileObj == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = fclose(fileObj);
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_FileWrite(T_GduFileHandle fileObj, const uint8_t *buf, uint32_t len, uint32_t *realLen)
{
    int32_t ret;

    if (fileObj == NULL || buf == NULL || len == 0 || realLen == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = fwrite(buf, 1, len, fileObj);
    if (ret >= 0) {
        *realLen = ret;
    } else {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_FileRead(T_GduFileHandle fileObj, uint8_t *buf, uint32_t len, uint32_t *realLen)
{
    int32_t ret;

    if (fileObj == NULL || buf == NULL || len == 0 || realLen == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = fread(buf, 1, len, fileObj);
    if (ret >= 0) {
        *realLen = ret;
    } else {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_FileSeek(T_GduFileHandle fileObj, uint32_t offset)
{
    int32_t ret;

    if (fileObj == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = fseek(fileObj, offset, SEEK_SET);
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_FileSync(T_GduFileHandle fileObj)
{
    int32_t ret;

    if (fileObj == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = fflush(fileObj);
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_DirOpen(const char *filePath, T_GduDirHandle *dirObj)
{
    if (filePath == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    *dirObj = opendir(filePath);
    if (*dirObj == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_DirClose(T_GduDirHandle dirObj)
{
    int32_t ret;

    if (dirObj == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = closedir((DIR *) dirObj);
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_DirRead(T_GduDirHandle dirObj, T_GduFileInfo *fileInfo)
{
    struct dirent *dirent;

    if (dirObj == NULL || fileInfo == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    dirent = readdir((DIR *) dirObj);
    if (!dirent) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (dirent->d_type == DT_DIR) {
        fileInfo->isDir = true;
    } else {
        fileInfo->isDir = false;
    }
    strcpy(fileInfo->path, dirent->d_name);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_Mkdir(const char *filePath)
{
    int32_t ret;

    if (filePath == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = mkdir(filePath, S_IRWXU);
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_Unlink(const char *filePath)
{
    int32_t ret;

    if (filePath == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    if (filePath[strlen(filePath) - 1] == '/') {
        ret = rmdir(filePath);
        if (ret < 0) {
            return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
        }
    } else {
        ret = unlink(filePath);
        if (ret < 0) {
            return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
        }
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_Rename(const char *oldFilePath, const char *newFilePath)
{
    int32_t ret;

    if (oldFilePath == NULL || newFilePath == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = rename(oldFilePath, newFilePath);
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_Stat(const char *filePath, T_GduFileInfo *fileInfo)
{
    struct stat st;
    int32_t ret;
    struct tm *fileTm;

    if (filePath == NULL || fileInfo == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = stat(filePath, &st);
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    fileTm = localtime(&(st.st_ctime));
    if (fileTm == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    fileInfo->size = st.st_size;

    fileInfo->createTime.year = fileTm->tm_year + 1900 - 1980;
    fileInfo->createTime.month = fileTm->tm_mon;
    fileInfo->createTime.day = fileTm->tm_mday;
    fileInfo->createTime.hour = fileTm->tm_hour;
    fileInfo->createTime.minute = fileTm->tm_min;
    fileInfo->createTime.second = fileTm->tm_sec;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
