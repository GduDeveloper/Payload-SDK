/**
 ********************************************************************
 * @file    test_payload_cam_emu_common.h
 * @brief   This is the header file for "test_payload_cam_emu.c", defining the structure and
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
#ifndef TEST_PAYLOAD_CAM_EMU_BASE_H
#define TEST_PAYLOAD_CAM_EMU_BASE_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"
#include "gdu_payload_camera.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode GduTest_CameraEmuBaseStartService(void);
T_GduReturnCode GduTest_CameraGetDigitalZoomFactor(gdu_f32_t *factor);
T_GduReturnCode GduTest_CameraGetOpticalZoomFactor(gdu_f32_t *factor);
T_GduReturnCode GduTest_CameraGetMode(E_GduCameraMode *mode);
T_GduReturnCode GduTest_CameraGetVideoStreamType(E_GduCameraVideoStreamType *type);
bool GduTest_CameraIsInited(void);

#ifdef __cplusplus
}
#endif

#endif // TEST_PAYLOAD_CAM_EMU_BASE_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
