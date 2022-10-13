/**
 ********************************************************************
 * @file    test_time_sync.h
 * @brief   This is the header file for "test_time_sync.c", defining the structure and
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
#ifndef TEST_TIME_SYNC_H
#define TEST_TIME_SYNC_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
typedef struct {
    T_GduReturnCode (*PpsSignalResponseInit)(void);
    T_GduReturnCode (*GetNewestPpsTriggerLocalTimeUs)(uint64_t *localTimeUs);
} T_GduTestTimeSyncHandler;

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode GduTest_TimeSyncStartService(void);

T_GduReturnCode GduTest_TimeSyncGetNewestPpsTriggerLocalTimeUs(uint64_t *localTimeUs);
T_GduReturnCode GduTest_TimeSyncRegHandler(T_GduTestTimeSyncHandler *timeSyncHandler);
#ifdef __cplusplus
}
#endif

#endif // TEST_TIME_SYNC_H

/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
