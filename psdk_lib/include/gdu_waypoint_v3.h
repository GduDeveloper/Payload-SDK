/**
 ********************************************************************
 * @file    gdu_waypoint_v3.h
 * @brief   This is the header file for "gdu_waypoint_v3.c", defining the structure and
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
#ifndef GDU_WAYPOINT_V3_H
#define GDU_WAYPOINT_V3_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
* @brief Waypoint v3 mission action.
*/
typedef enum {
    GDU_WAYPOINT_V3_ACTION_START = 1, /*!< Waypoint v3 mission start action. */
    GDU_WAYPOINT_V3_ACTION_PAUSE = 2, /*!< Waypoint v3 mission pause action. */
    GDU_WAYPOINT_V3_ACTION_RESUME = 3, /*!< Waypoint v3 mission resume action. */
	GDU_WAYPOINT_V3_ACTION_STOP = 4, /*!< Waypoint v3 mission stop action. */
} E_GduWaypointV3Action;

/**
*  Waypoint v3 current aircraft state.
*/
typedef enum {
    GDU_WAYPOINT_V3_STATE_GOING = 2, /*!< Waypoint v3 aircraft in RUNNING state. */
    GDU_WAYPOINT_V3_STATE_PAUSE = 3, /*!< Waypoint v3 aircraft in PAUSE state. */
    GDU_WAYPOINT_V3_STATE_COMPLETE = 4, /*!< Waypoint v3 aircraft in complete state. */
    GDU_WAYPOINT_V3_STATE_FINISHED_ACTIVE = 5, /*!< Waypoint v3 aircraft active finish state. */
    GDU_WAYPOINT_V3_STATE_FAILED = 6, /*!< Waypoint v3 aircraft in failed state. */
} E_GduWaypointV3State;

/**
*  Waypoint v3 mission state.
*/
typedef struct {
    E_GduWaypointV3State state; /*!< Waypoint v3 current aircraft state, #E_GduWaypointV3State. */
    uint32_t wayLineId; /*!< Waypoint v3 current way line id. */
    uint16_t currentWaypointIndex; /*!< Waypoint v3 current waypoint index. */
} T_GduWaypointV3MissionState;

/**
 * @brief Prototype of callback function used to receive the waypoint v3 mission state.
 * @warning User can not execute blocking style operations or functions in callback function, because that will block
 * root thread, causing problems such as slow system response, payload disconnection or infinite loop.
 * @param missionState: current waypoint v3 mission state.
 * @return Execution result.
 */
typedef T_GduReturnCode (*WaypointV3StateCallback)(T_GduWaypointV3MissionState missionState);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise waypoint v3 module, and user should call this function.
 * before using waypoint v3 features.
 * @return Execution result.
 */
T_GduReturnCode GduWaypointV3_Init(void);

/**
 * @brief Deinitialise waypoint v3 module.
 * @return Execution result.
 */
T_GduReturnCode GduWaypointV3_DeInit(void);

/**
 * @brief Upload kmz file by raw data.
 * @note The size of xml file is very small, you can use this interface to upload quickly.
 * @return Execution result.
 */
T_GduReturnCode GduWaypointV3_UploadXmlFile(const uint8_t *data, uint32_t dataLen);

/**
 * @brief Execute the mission action.
 * @note This action should be called after uploading the kmz file.
 * @return Execution result.
 */
T_GduReturnCode GduWaypointV3_Action(E_GduWaypointV3Action action);

/**
 * @brief Register the mission state callback for waypoint mission.
 * @note If you want to monitor the state of waypoint mission, this interface should be called before uploading kmz
 * file or executing this mission action.
 * @return Execution result.
 */
T_GduReturnCode GduWaypointV3_RegMissionStateCallback(WaypointV3StateCallback callback);

#ifdef __cplusplus
}
#endif

#endif // GDU_WAYPOINT_V3_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
