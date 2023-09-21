/**
 ********************************************************************
 * @file    test_payload_gimbal_emu.h
 * @brief   This is the header file for "test_payload_gimbal_emu.c", defining the structure and
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
#ifndef TEST_PAYLOAD_GIMBAL_EMU_H
#define TEST_PAYLOAD_GIMBAL_EMU_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"
#include "gdu_gimbal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/
T_GduReturnCode GduTest_GimbalStartService(void);

T_GduReturnCode GduTest_GimbalDeInit(void);
T_GduReturnCode GduTest_GimbalRotate(E_GduGimbalRotationMode rotationMode,
                                     T_GduGimbalRotationProperty rotationProperty,
                                     T_GduAttitude3d rotationValue); // unit if angle control: 0.1 degree, unit if speed control: 0.1 degree/s

#ifdef __cplusplus
}
#endif

#endif // TEST_PAYLOAD_GIMBAL_EMU_H

/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
