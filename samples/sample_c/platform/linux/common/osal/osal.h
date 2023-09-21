/**
 ********************************************************************
 * @file    osal.h
 * @brief   This is the header file for "osal.c", defining the structure and
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
#ifndef OSAL_H
#define OSAL_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>
#include "gdu_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode Osal_TaskCreate(const char *name, void *(*taskFunc)(void *),
                                uint32_t stackSize, void *arg, T_GduTaskHandle *task);
T_GduReturnCode Osal_TaskDestroy(T_GduTaskHandle task);
T_GduReturnCode Osal_TaskSleepMs(uint32_t timeMs);

T_GduReturnCode Osal_MutexCreate(T_GduMutexHandle *mutex);
T_GduReturnCode Osal_MutexDestroy(T_GduMutexHandle mutex);
T_GduReturnCode Osal_MutexLock(T_GduMutexHandle mutex);
T_GduReturnCode Osal_MutexUnlock(T_GduMutexHandle mutex);

T_GduReturnCode Osal_SemaphoreCreate(uint32_t initValue, T_GduSemaHandle *semaphore);
T_GduReturnCode Osal_SemaphoreDestroy(T_GduSemaHandle semaphore);
T_GduReturnCode Osal_SemaphoreWait(T_GduSemaHandle semaphore);
T_GduReturnCode Osal_SemaphoreTimedWait(T_GduSemaHandle semaphore, uint32_t waitTime);
T_GduReturnCode Osal_SemaphorePost(T_GduSemaHandle semaphore);

T_GduReturnCode Osal_GetTimeMs(uint32_t *ms);
T_GduReturnCode Osal_GetTimeUs(uint64_t *us);

void *Osal_Malloc(uint32_t size);
void Osal_Free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif // OSAL_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
