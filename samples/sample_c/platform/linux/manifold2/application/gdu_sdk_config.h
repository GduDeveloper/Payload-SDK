/**
 ********************************************************************
 * @file    gdu_sdk_config.h
 * @brief   This is the header file for "gdu_config.c", defining the structure and
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
#ifndef GDU_SDK_CONFIG_H
#define GDU_SDK_CONFIG_H

/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
//#define CONFIG_MODULE_SAMPLE_POWER_MANAGEMENT_ON

#define CONFIG_MODULE_SAMPLE_CAMERA_EMU_ON

#define CONFIG_MODULE_SAMPLE_CAMERA_MEDIA_ON

#define CONFIG_MODULE_SAMPLE_GIMBAL_ON

//#define CONFIG_MODULE_SAMPLE_XPORT_ON

#define CONFIG_MODULE_SAMPLE_FC_SUBSCRIPTION_ON

//#define CONFIG_MODULE_SAMPLE_WIDGET_ON

//#define CONFIG_MODULE_SAMPLE_DATA_TRANSMISSION_ON

//#define CONFIG_MODULE_SAMPLE_PAYLOAD_COLLABORATION_ON

//#define CONFIG_MODULE_SAMPLE_UPGRADE_ON

/*!< Attention: This function needs to be used together with mobile sdk mop sample.
* */
//#define CONFIG_MODULE_SAMPLE_MOP_CHANNEL_ON

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif // GDU_SDK_CONFIG_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
