/**
 ********************************************************************
 * @file    gdu_power_management.h
 * @brief   This is the header file for "gdu_power_management.c", defining the structure and
 * (exported) function prototypes.
 *
 * @copyright (c) 2021GDU. All rights reserved.
 *
 * All information contained herein is, and remains, the property ofGDU.
 * The intellectual and technical concepts contained herein are proprietary
 * toGDU and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent ofGDU.
 *
 * If you receive this source code withoutGDU’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notifyGDU of its removal.GDU reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GDU_POWER_MANAGEMENT_H
#define GDU_POWER_MANAGEMENT_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Pin state.
 */
typedef enum {
   GDU_POWER_MANAGEMENT_PIN_STATE_RESET = 0, /*!< Specifies pin is in low level state. */
   GDU_POWER_MANAGEMENT_PIN_STATE_SET = 1, /*!< Specifies pin is in high level state. */
} E_GduPowerManagementPinState;

/**
 * @brief Prototype of callback function used to set level of high power application pin.
 * @param pinState: level state of pin to be set.
 * @return Execution result.
 */
typedef T_GduReturnCode (*GduWriteHighPowerApplyPinCallback)(E_GduPowerManagementPinState pinState);

/**
 * @brief Prototype of callback function used to notify payload that the aircraft will power off soon and get state
 * whether the payload is ready to power off or not.
 * @warning User can not execute blocking style operations or functions in a callback function, because that will blockGDU
 * root thread, causing problems such as slow system response, payload disconnection or infinite loop.
 * @param powerOffPreparationFlag: pointer to memory space used to store power off preparation flag. True represents
 * that payload has completed all work and is ready to power off. False represents that payload is not ready to power
 * off.
 * @return Execution result.
 */
typedef T_GduReturnCode (*GduPowerOffNotificationCallback)(bool *powerOffPreparationFlag);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise power management module, and user should call this function before using power management features.
 * @return Execution result.
 */
T_GduReturnCode GduPowerManagement_Init(void);

/**
 * @brief Apply high power from aircraft in blocking mode.
 * @details Before applying, user should register callback function used to set level state of high power application
 * pin using GduPowerManagement_RegWriteHighPowerApplyPinCallback() function. After applying high power, power pin of
 *GDU adapter will output high power based predetermined specification.
 * @note Max execution time of this function is slightly larger than 600ms.
 * @return Execution result.
 */
T_GduReturnCode GduPowerManagement_ApplyHighPowerSync(void);

/**
 * @brief Register callback function used to set level state of high power application pin. Must be called before
 * applying high power.
 * @param callback: pointer to the callback function.
 * @return Execution result.
 */
T_GduReturnCode GduPowerManagement_RegWriteHighPowerApplyPinCallback(GduWriteHighPowerApplyPinCallback callback);

/**
 * @brief Register callback function used to notify payload that the aircraft will power off soon and get state
 * whether the payload is ready to power off or not.
 * @details After registering, the callback function will be called at a fixed frequency when aircraft will power off.
 * User fill in the power off preparation flag, and once the payload is ready to power off, the callback function will not
 * be called. After a specified time, if the payload is not ready to power off, the aircraft will power off immediately. The
 * specified time is 3s.
 * @param callback: pointer to callback function used to notify aircraft power off message and get payload power off
 * preparation flag.
 * @return Execution result.
 */
T_GduReturnCode GduPowerManagement_RegPowerOffNotificationCallback(GduPowerOffNotificationCallback callback);

#ifdef __cplusplus
}
#endif

#endif //GDU_POWER_MANAGEMENT_H

/************************ (C) COPYRIGHTGDU Innovations *******END OF FILE******/
