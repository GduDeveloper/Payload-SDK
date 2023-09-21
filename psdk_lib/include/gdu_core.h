/**
 ********************************************************************
 * @file    dij_core.h
 * @brief   This is the header file for "dij_core.c", defining the structure and
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
#ifndef GDU_CORE_H
#define GDU_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
    char appName[32]; /*!< Specifies GDU SDK app name. This info can be obtained by logging in to the
                           developer website https://developer.gdu.com/user/apps/#all. End with '\0'. */
    char appId[16]; /*!< Specifies GDU SDK app ID. This info can be obtained by logging in to the
                         developer website https://developer.gdu.com/user/apps/#all. */
    char appKey[32]; /*!< Specifies GDU SDK app key. This info can be obtained by logging in to the
                          developer website https://developer.gdu.com/user/apps/#all. */
    char appLicense[512]; /*!< Specifies GDU SDK app license. This info can be obtained by logging in to the
                               developer website https://developer.gdu.com/user/apps/#all. */
    char developerAccount[64]; /*!< Specifies GDU SDK developer account email. This info can be obtained by
                                    logging in to the developer website https://developer.gdu.com/user/apps/#all.
                                    Developer's account and other related information need to be able to correspond.
                                    End with '\0'. */
    char baudRate[7]; /*!< Specifies GDU SDK communication serial port baud rate. */
} T_GduUserInfo;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize the Onboard SDK core in blocking mode.
 * @note The call location of this interface requires special attention, The call needs to be completed after the
 * registration of console/OSAL handler functions/HAL handler functions are completed. At the same time, it must be
 * initialized at the beginning of calling other functional module interfaces. You need to fill in the developer
 * information correctly to ensure the initialization is successful. For additional instructions, please refer to the
 * tutorial“PSDK Initialization”.
 * @note This function does not return until the correct aircraft type and PSDK adapter type is obtained. The logic ensures
 * that aircraft and PSDK adapter have been started up normally before PSDK functional module and user's program run.
 * General execution time of this function is 2~4 seconds.
 * @param userInfo: pointer to the PSDK application information.
 * @return Execution result.
 */
T_GduReturnCode GduCore_Init(const T_GduUserInfo *userInfo);

/**
 * @brief Set an alias that satisfies the condition for GDU application or product.
 * @details Alias will display in GDU Pilot, if exist.
 * @note Still need to pass in correct GDU APP name that is obtained from GDU SDK developer website to GduCore_Init()
 * interface. The GDU APP name will be used to bind or verification.
 * @note Alias will be effective after a while, and the max value is 1s.
 * @param productAlias: pointer to product alias string, and alias end with '\0'. The max length of the string is 31. If
 * length of alias string is greater than 31, alias string will be truncated and passed in.
 * @return Execution result.
 */
T_GduReturnCode GduCore_SetAlias(const char *productAlias);

/**
 * @brief Notify that the Onboard SDK core application starts.
 * @note The call location of this interface requires special attention, The call needs to be completed after all the
 * module initialize and register interfaces.
 * @return Execution result.
 */
T_GduReturnCode GduCore_ApplicationStart(void);

#ifdef __cplusplus
}
#endif

#endif // DIJ_CORE_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
