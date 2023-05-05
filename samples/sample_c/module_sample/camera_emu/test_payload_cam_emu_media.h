/**
 ********************************************************************
 * @file    test_payload_cam_emu_media.h
 * @brief   This is the header file for "test_payload_cam_media.c", defining the structure and
 * (exported) function prototypes.
 *
 * @copyright (c) 2021 Gdu. All rights reserved.
 *
 * All information contained herein is, and remains, the property of Gdu.
 * The intellectual and technical concepts contained herein are proprietary
 * to Gdu and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of Gdu.
 *
 * If you receive this source code without Gdu’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify Gdu of its removal. Gdu reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TEST_PAYLOAD_CAM_EMU_MEDIA_H
#define TEST_PAYLOAD_CAM_EMU_MEDIA_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"
#include "gdu_payload_camera.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode GduTest_CameraEmuMediaStartService(void);
T_GduReturnCode GduTest_CameraMediaGetFileInfo(const char *filePath, T_GduCameraMediaFileInfo *fileInfo);

#ifdef __cplusplus
}
#endif

#endif // TEST_PAYLOAD_CAM_EMU_MEDIA_H
/************************ (C) COPYRIGHT Gdu Innovations *******END OF FILE******/
