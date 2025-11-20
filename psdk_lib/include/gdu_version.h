/**
 ********************************************************************
 * @file    gdu_version.h
 * @brief   This is the header file for "gdu_version.c", defining the structure and
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
#ifndef GDU_VERSION_H
#define GDU_VERSION_H

/* Includes ------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define GDU_VERSION_MAJOR     3 /*!< GDU SDK major version num, when have incompatible API changes. Range from 0 to 99. */
#define GDU_VERSION_MINOR     0 /*!< GDU SDK minor version num, when add functionality in a backwards compatible manner changes. Range from 0 to 99. */
#define GDU_VERSION_BUILD     31 /*!< GDU SDK version*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif // GDU_VERSION_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
