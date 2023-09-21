/**
 ********************************************************************
 * @file    file_binary_array_list_en.h
 * @brief   This is the header file for "file_binary_array_list_en.c", defining the structure and
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
#ifndef FILE_BINARY_ARRAY_LIST_EN_H
#define FILE_BINARY_ARRAY_LIST_EN_H

#include <stdint.h>
#include <gdu_widget.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/

extern uint32_t g_EnBinaryArrayCount;
extern T_GduWidgetFileBinaryArray * g_EnFileBinaryArrayList;

#ifdef __cplusplus
}
#endif

#endif // FILE_BINARY_ARRAY_LIST_EN_H

/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
