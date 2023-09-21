/**
 ********************************************************************
 * @file    gdu_hms_info_table.h
 * @brief   This is the header file for "gdu_hms_info_table.c", defining the structure and
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
#ifndef GDU_HMS_INFO_TABLE_H
#define GDU_HMS_INFO_TABLE_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint32_t alarmId;            /*! error code*/
    const char *groundAlarmInfo; /*! alarm information when the flight is on the ground*/
    const char *flyAlarmInfo;    /*! alarm information when the flight is in the air*/
} T_GduHmsErrCodeInfo;

/*! HMS's error code table*/
const T_GduHmsErrCodeInfo hmsErrCodeInfoTbl[] = {
    {0x10000001, "Aircraft front binocular status error. Fly with caution",                                                                   ""},
    {0x10000002, "Aircraft behind binocular status error. Fly with caution",                                                                  ""},
    {0x10000003, "Aircraft left binocular status error. Fly with caution",                                                                    ""},
    {0x10000004, "Aircraft right binocular status error. Fly with caution",                                                                   ""},
    {0x10000005, "Aircraft up binocular status error. Fly with caution",                                                                      ""},
    {0x10000006, "Aircraft lower binocular status error. Fly with caution",                                                                   ""},
	{0x10000007, "Aircraft high radar status error. Fly with caution",																		  ""},
	{0x10000008, "Aircraft lower TOF status error. Fly with caution",  																		  ""},
	{0x20000001, "RC not connect",                                                   												      ""},
    {0x20000002, "battery electric quantity fault",                                                                                                         ""},
    {0x20000003, "battery voltage fault",                                                                                                     ""},
    {0x20000004, "battery alarm",                                                                                                             ""},
    {0x20000005, "battery temperature fault",                                                                                                 ""},
    {0x20000006, "aircraft IMU fault",                                                                                                        ""},
    {0x20000007, "aircraft IMU uncalibrated",                                                                                                 ""},
    {0x20000008, "aircraft IMU calibrated. Restart aircraft",                                                                                 ""},
    {0x20000009, "magnetometer sensor fault. Restart aircraft",                                                                               ""},
    {0x2000000A, "The ambient magnetic interference is large",                                                                                ""},
    {0x2000000B, "barometer sensor fault",                                                                                                    ""},
    {0x2000000C, "optical module sensor fault",                                                                                               ""},
    {0x2000000D, "ultrasound/tof fault",                                                                                                      ""},
    {0x2000000E, "EEPROM connection error",                                                                                                   ""},
    {0x2000000F, "memorizer connection error",                                                                                                ""},
    {0x20000010, "memorizer full",                                                                                                            ""},
    {0x20000011, "gps fault",                                                                                                                 ""},
    {0x20000012, "foc main controller fault",                                                                                                 ""},
    {0x20000013, "main controller and cordinate controller communicat error",                                                                 ""},
};

/* Exported functions --------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // GDU_HMS_INFO_TABLE_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
