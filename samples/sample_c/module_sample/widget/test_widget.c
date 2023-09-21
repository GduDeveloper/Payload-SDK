/**
 ********************************************************************
 * @file    test_widget.c
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
#include "test_widget.h"
#include <gdu_widget.h>
#include <gdu_logger.h>
#include "../utils/util_misc.h"
#include <gdu_platform.h>
#include <stdio.h>
#include "file_binary_array_list_en.h"

/* Private constants ---------------------------------------------------------*/
#define WIDGET_DIR_PATH_LEN_MAX         (256)
#define WIDGET_TASK_STACK_SIZE          (2048)

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static void *GduTest_WidgetTask(void *arg);
static T_GduReturnCode GduTestWidget_SetWidgetValue(E_GduWidgetType widgetType, uint32_t index, int32_t value,
                                                    void *userData);
static T_GduReturnCode GduTestWidget_GetWidgetValue(E_GduWidgetType widgetType, uint32_t index, int32_t *value,
                                                    void *userData);

/* Private values ------------------------------------------------------------*/
static T_GduTaskHandle s_widgetTestThread;
static bool s_isWidgetFileDirPathConfigured = false;
static char s_widgetFileDirPath[GDU_FILE_PATH_SIZE_MAX] = {0};

static const T_GduWidgetHandlerListItem s_widgetHandlerList[] = {
    {0, GDU_WIDGET_TYPE_BUTTON,        GduTestWidget_SetWidgetValue, GduTestWidget_GetWidgetValue, NULL},
    {1, GDU_WIDGET_TYPE_BUTTON,        GduTestWidget_SetWidgetValue, GduTestWidget_GetWidgetValue, NULL},
    {2, GDU_WIDGET_TYPE_LIST,          GduTestWidget_SetWidgetValue, GduTestWidget_GetWidgetValue, NULL},
    {3, GDU_WIDGET_TYPE_SWITCH,        GduTestWidget_SetWidgetValue, GduTestWidget_GetWidgetValue, NULL},
    {4, GDU_WIDGET_TYPE_SCALE,         GduTestWidget_SetWidgetValue, GduTestWidget_GetWidgetValue, NULL},
    {5, GDU_WIDGET_TYPE_BUTTON,        GduTestWidget_SetWidgetValue, GduTestWidget_GetWidgetValue, NULL},
    {6, GDU_WIDGET_TYPE_SCALE,         GduTestWidget_SetWidgetValue, GduTestWidget_GetWidgetValue, NULL},
    {7, GDU_WIDGET_TYPE_INT_INPUT_BOX, GduTestWidget_SetWidgetValue, GduTestWidget_GetWidgetValue, NULL},
    {8, GDU_WIDGET_TYPE_SWITCH,        GduTestWidget_SetWidgetValue, GduTestWidget_GetWidgetValue, NULL},
    {9, GDU_WIDGET_TYPE_LIST,          GduTestWidget_SetWidgetValue, GduTestWidget_GetWidgetValue, NULL},
};

static char *s_widgetTypeNameArray[] = {
    "Unknown",
    "Button",
    "Switch",
    "Scale",
    "List",
    "Int input box"
};

static const uint32_t s_widgetHandlerListCount = sizeof(s_widgetHandlerList) / sizeof(T_GduWidgetHandlerListItem);
static int32_t s_widgetValueList[sizeof(s_widgetHandlerList) / sizeof(T_GduWidgetHandlerListItem)] = {0};

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode GduTest_WidgetStartService(void)
{
    T_GduReturnCode gduStat;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    //Step 1 : Init GDU Widget
    gduStat = GduWidget_Init();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Gdu test widget init error, stat = 0x%08llX", gduStat);
        return gduStat;
    }

#ifdef SYSTEM_ARCH_LINUX
    //Step 2 : Set UI Config (Linux environment)
    char curFileDirPath[WIDGET_DIR_PATH_LEN_MAX];
    char tempPath[WIDGET_DIR_PATH_LEN_MAX];
    gduStat = GduUserUtil_GetCurrentFileDirPath(__FILE__, WIDGET_DIR_PATH_LEN_MAX, curFileDirPath);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file current path error, stat = 0x%08llX", gduStat);
        return gduStat;
    }

    if (s_isWidgetFileDirPathConfigured == true) {
        snprintf(tempPath, WIDGET_DIR_PATH_LEN_MAX, "%swidget_file/en_big_screen", s_widgetFileDirPath);
    } else {
        snprintf(tempPath, WIDGET_DIR_PATH_LEN_MAX, "%swidget_file/en_big_screen", curFileDirPath);
    }

    //set default ui config path
    gduStat = GduWidget_RegDefaultUiConfigByDirPath(tempPath);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add default widget ui config error, stat = 0x%08llX", gduStat);
        return gduStat;
    }

    //set ui config for English language
    gduStat = GduWidget_RegUiConfigByDirPath(GDU_MOBILE_APP_LANGUAGE_ENGLISH,
                                             GDU_MOBILE_APP_SCREEN_TYPE_BIG_SCREEN,
                                             tempPath);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add widget ui config error, stat = 0x%08llX", gduStat);
        return gduStat;
    }

    //set ui config for Chinese language
    if (s_isWidgetFileDirPathConfigured == true) {
        snprintf(tempPath, WIDGET_DIR_PATH_LEN_MAX, "%swidget_file/cn_big_screen", s_widgetFileDirPath);
    } else {
        snprintf(tempPath, WIDGET_DIR_PATH_LEN_MAX, "%swidget_file/cn_big_screen", curFileDirPath);
    }

    gduStat = GduWidget_RegUiConfigByDirPath(GDU_MOBILE_APP_LANGUAGE_CHINESE,
                                             GDU_MOBILE_APP_SCREEN_TYPE_BIG_SCREEN,
                                             tempPath);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add widget ui config error, stat = 0x%08llX", gduStat);
        return gduStat;
    }
#else
    //Step 2 : Set UI Config (RTOS environment)
    T_GduWidgetBinaryArrayConfig enWidgetBinaryArrayConfig = {
        .binaryArrayCount = g_EnBinaryArrayCount,
        .fileBinaryArrayList = g_EnFileBinaryArrayList
    };

    //set default ui config
    gduStat = GduWidget_RegDefaultUiConfigByBinaryArray(&enWidgetBinaryArrayConfig);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add default widget ui config error, stat = 0x%08llX", gduStat);
        return gduStat;
    }
#endif
    //Step 3 : Set widget handler list
    gduStat = GduWidget_RegHandlerList(s_widgetHandlerList, s_widgetHandlerListCount);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set widget handler list error, stat = 0x%08llX", gduStat);
        return gduStat;
    }

    //Step 4 : Run widget api sample task
    if (osalHandler->TaskCreate("user_widget_task", GduTest_WidgetTask, WIDGET_TASK_STACK_SIZE, NULL,
                                &s_widgetTestThread) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Gdu widget test task create error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_WidgetSetConfigFilePath(const char *path)
{
    memset(s_widgetFileDirPath, 0, sizeof(s_widgetFileDirPath));
    memcpy(s_widgetFileDirPath, path, USER_UTIL_MIN(strlen(path), sizeof(s_widgetFileDirPath) - 1));
    s_isWidgetFileDirPathConfigured = true;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

__attribute__((weak)) void GduTest_WidgetLogAppend(const char *fmt, ...)
{

}

#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wformat"
#endif

/* Private functions definition-----------------------------------------------*/
static void *GduTest_WidgetTask(void *arg)
{
    char message[GDU_WIDGET_FLOATING_WINDOW_MSG_MAX_LEN];
    uint32_t sysTimeMs = 0;
    T_GduReturnCode gduStat;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    USER_UTIL_UNUSED(arg);

    while (1) {
			
        gduStat = osalHandler->GetTimeMs(&sysTimeMs);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("Get system time ms error, stat = 0x%08llX", gduStat);
        }
        snprintf(message, GDU_WIDGET_FLOATING_WINDOW_MSG_MAX_LEN, "System time:%ums\nSystem time2:%ums\nSystem time3:%u ms\nSystem time4:%ums", sysTimeMs,sysTimeMs,sysTimeMs,sysTimeMs);

        gduStat = GduWidgetFloatingWindow_ShowMessage(message);
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("Floating window show message error, stat = 0x%08llX", gduStat);
        }

        osalHandler->TaskSleepMs(1000);
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

static T_GduReturnCode GduTestWidget_SetWidgetValue(E_GduWidgetType widgetType, uint32_t index, int32_t value,
                                                    void *userData)
{
    USER_UTIL_UNUSED(userData);

    USER_LOG_INFO("Set widget value, widgetType = %s, widgetIndex = %d ,widgetValue = %d",
                  s_widgetTypeNameArray[widgetType], index, value);
    s_widgetValueList[index] = value;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode GduTestWidget_GetWidgetValue(E_GduWidgetType widgetType, uint32_t index, int32_t *value,
                                                    void *userData)
{
    USER_UTIL_UNUSED(userData);
    USER_UTIL_UNUSED(widgetType);

    *value = s_widgetValueList[index];

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/

