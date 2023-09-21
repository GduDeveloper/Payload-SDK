/**
 ********************************************************************
 * @file    gdu_aircraft_info.h
 * @brief   This is the header file for "gdu_aircraft_info.c", defining the structure and
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
#ifndef GDU_AIRCRAFT_INFO_H
#define GDU_AIRCRAFT_INFO_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/**
 * @brief Information related to mobile APP.
 */
typedef struct {
    E_GduMobileAppLanguage appLanguage; /*!< Mobile APP system language */
    E_GduMobileAppScreenType appScreenType; /*!< Mobile APP screen size type. */
} T_GduMobileAppInfo;

/**
 * @brief Some base information of aircraft system, mainly including some constant parameters information of system.
 */
typedef struct {
    E_GduAircraftType aircraftType; /*!< Aircraft type. */
    E_GduSdkAdapterType gduAdapterType; /*!< GDU adapter type. */
    E_GduMountPosition mountPosition; /*!< Payload mount position. */
} T_GduAircraftInfoBaseInfo;


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Get base information of aircraft system, including aircraft type and GDU adapter type.
 * @param baseInfo: pointer to memory space used to store base information of the aircraft system.
 * @return Execution result.
 */
T_GduReturnCode GduAircraftInfo_GetBaseInfo(T_GduAircraftInfoBaseInfo *baseInfo);

/**
 * @brief Get information related to mobile APP.
 * @note The mobile APP language and screen type is unknown if RC or APP is not connected to the aircraft system.
 * @param mobileAppInfo: pointer to memory space used to store information related to mobile APP.
 * @return Execution result.
 */
T_GduReturnCode GduAircraftInfo_GetMobileAppInfo(T_GduMobileAppInfo *mobileAppInfo);

#ifdef __cplusplus
}
#endif

#endif //GDU_AIRCRAFT_INFO_H

/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
