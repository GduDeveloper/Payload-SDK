/**
 ********************************************************************
 * @file    osal.c
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
#include "limits.h"
#include "osal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Private constants ---------------------------------------------------------*/
#define SEM_MUTEX_WAIT_FOREVER      0xFFFFFFFF
#define TASK_PRIORITY_NORMAL        0

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/

/* Private functions definition-----------------------------------------------*/
T_GduReturnCode Osal_TaskCreate(const char *name, void *(*taskFunc)(void *), uint32_t stackSize,
                                void *arg, T_GduTaskHandle *task)
{
    uint32_t stackDepth;
    char nameDealed[16] = {0};

    //attention :  freertos use stack depth param, stack size = (stack depth) * sizeof(StackType_t)
    if (stackSize % sizeof(StackType_t) == 0) {
        stackDepth = stackSize / sizeof(StackType_t);
    } else {
        stackDepth = stackSize / sizeof(StackType_t) + 1;
    }

    if (name != NULL)
        strncpy(nameDealed, name, sizeof(nameDealed) - 1);
    if (xTaskCreate((TaskFunction_t) taskFunc, nameDealed, stackDepth, arg, TASK_PRIORITY_NORMAL, (TaskHandle_t *)task) != pdPASS) {
        *task = NULL;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_TaskDestroy(T_GduTaskHandle task)
{
    vTaskDelete(task);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_TaskSleepMs(uint32_t timeMs)
{
    TickType_t ticks;

    ticks = timeMs / portTICK_PERIOD_MS;

    /* Minimum delay = 1 tick */
    vTaskDelay(ticks ? ticks : 1);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_MutexCreate(T_GduMutexHandle *mutex)
{
    *mutex = xSemaphoreCreateMutex();
    if (*mutex == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_MutexDestroy(T_GduMutexHandle mutex)
{
    vQueueDelete((SemaphoreHandle_t) mutex);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_MutexLock(T_GduMutexHandle mutex)
{
    TickType_t ticks;

    if (mutex == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ticks = portMAX_DELAY;

    if (xSemaphoreTake(mutex, ticks) != pdTRUE) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_MutexUnlock(T_GduMutexHandle mutex)
{
    if (xSemaphoreGive(mutex) != pdTRUE) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_SemaphoreCreate(uint32_t initValue, T_GduSemaHandle *semaphore)
{
    uint32_t maxCount = UINT_MAX;

    *semaphore = xSemaphoreCreateCounting(maxCount, initValue);

    if (*semaphore == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_SemaphoreDestroy(T_GduSemaHandle semaphore)
{
    vSemaphoreDelete(semaphore);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_SemaphoreTimedWait(T_GduSemaHandle semaphore, uint32_t waitTimeMs)
{
    TickType_t ticks;

    if (semaphore == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ticks = 0;
    if (waitTimeMs == SEM_MUTEX_WAIT_FOREVER) {
        ticks = portMAX_DELAY;
    } else if (waitTimeMs != 0) {
        ticks = waitTimeMs / portTICK_PERIOD_MS;
        if (ticks == 0) {
            ticks = 1;
        }
    }
    if (xSemaphoreTake(semaphore, ticks) != pdTRUE) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_SemaphoreWait(T_GduSemaHandle semaphore)
{
    Osal_SemaphoreTimedWait(semaphore, SEM_MUTEX_WAIT_FOREVER);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_SemaphorePost(T_GduSemaHandle semaphore)
{
    if (xSemaphoreGive(semaphore) != pdTRUE) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_GetTimeMs(uint32_t *ms)
{
    *ms = xTaskGetTickCount();

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_GetTimeUs(uint64_t *us)
{
    *us = xTaskGetTickCount() * 1000;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

void *Osal_Malloc(uint32_t size)
{
    return pvPortMalloc(size);
}

void Osal_Free(void *ptr)
{
    vPortFree(ptr);
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
