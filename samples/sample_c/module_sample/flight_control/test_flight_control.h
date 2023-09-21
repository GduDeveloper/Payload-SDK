/**
 ********************************************************************
 * @file    test_flight_control.h
 * @brief   This is the header file for "test_flight_control.c", defining the structure and
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
#ifndef TEST_FLIGHT_CONTROL_H
#define TEST_FLIGHT_CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
typedef enum {
    E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_LANDING,
    E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_POSITION_CTRL_LANDING,
    E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_GO_HOME_FORCE_LANDING,
    E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_VELOCITY_CTRL_LANDING,
    E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_ARREST_FLYING,
    E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_SET_GET_PARAM,
} E_GduTestFlightCtrlSampleSelect;

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_GduReturnCode GduTest_FlightControlRunSample(E_GduTestFlightCtrlSampleSelect flightCtrlSampleSelect);

#ifdef __cplusplus
}
#endif

#endif // TEST_FLIGHT_CONTROL_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
