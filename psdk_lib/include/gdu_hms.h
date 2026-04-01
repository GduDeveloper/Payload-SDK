/**
 ********************************************************************
 * @file    gdu_hms.h
 * @brief   This is the header file for "gdu_hms.c", defining the structure and
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
#ifndef GDU_HMS_H
#define GDU_HMS_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint32_t errorCode;
    uint8_t componentIndex;
    uint8_t errorLevel;
} T_GduHmsInfo;

typedef struct {
    T_GduHmsInfo *hmsInfo;
    uint32_t hmsInfoNum;
} T_GduHmsInfoTable;

typedef T_GduReturnCode (*GduHmsInfoCallback)(T_GduHmsInfoTable hmsInfoTable);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise hms module, and user should call this function
 * before using hms features.
 * @return Execution result.
 */
T_GduReturnCode GduHms_Init(void);

/**
 * @brief Deinitialise hms module.
 * @return Execution result.
 */
T_GduReturnCode GduHms_DeInit(void);

/**
 * @brief Register callback to get hms info.
 * @note: Data is pushed with a frequency of 1Hz.
 * @param callback: see reference of GduHmsInfoCallback.
 * @return Execution result.
 */
T_GduReturnCode GduHms_RegHmsInfoCallback(GduHmsInfoCallback callback);

#ifdef __cplusplus
}
#endif

#endif // GDU_HMS_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
