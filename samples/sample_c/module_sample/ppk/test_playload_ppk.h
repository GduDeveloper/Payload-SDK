
/**
 ********************************************************************
 * @file    test_widget.h
 * @brief   This is the header file for "test_playload_ppk.c", defining the structure and
 * (exported) function prototypes.
 *
 * @copyright (c) 2022 GDU. All rights reserved.
 *
 *********************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TEST_PPK_H
#define TEST_PPK_H

/* Includes ------------------------------------------------------------------*/
#include <gdu_typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode GduTest_PPKStartService(void);
T_GduReturnCode GduTest_PPKRawData(uint8_t *data,uint16_t length);

#ifdef __cplusplus
}
#endif

#endif // TEST_WIDGET_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
