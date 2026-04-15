/**
 ********************************************************************
 * @file    gdu_error.h
 * @brief   This is the header file for "gdu_error.c", defining the structure and
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
#ifndef GDU_ERROR_H
#define GDU_ERROR_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define GDU_ERROR_MODULE_INDEX_OFFSET  32u
#define GDU_ERROR_MODULE_INDEX_MASK    0x000000FF00000000u
#define GDU_ERROR_RAW_CODE_OFFSET      0u
#define GDU_ERROR_RAW_CODE_MASK        0x00000000FFFFFFFFu

#define GDU_ERROR_CODE(moduleIndex, rawErrCode) \
((uint64_t)0 | \
((((uint64_t)(moduleIndex)) << (GDU_ERROR_MODULE_INDEX_OFFSET)) & (GDU_ERROR_MODULE_INDEX_MASK)) | \
((((uint64_t)(rawErrCode)) << (GDU_ERROR_RAW_CODE_OFFSET)) & (GDU_ERROR_RAW_CODE_MASK)))

/**
 * @brief Error objects, specifying error code, error description, error reason and error recovery suggestion below.
 * @attention Require arrange by error code from small to large.
 */
#define GDU_ERROR_OBJECTS \
/* system module error message */ \
{GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS, "Execution successfully.", NULL}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_REQUEST_PARAMETER, "Request parameters are invalid.", "Please double-check requested parameters."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_EXECUTING_HIGHER_PRIORITY_TASK, "A higher priority task is being executed.", "Please stop the higher priority task or try again later."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT, "Operation is not supported.", "Please check input parameters or contact GDU for help."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_TIMEOUT, "Execution timeout.", "Please contact GDU for help."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED, "Memory allocation failed.", "Please check system configure."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER, "Input parameters are invalid.", "Please double-check requested parameters."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE, "Operation is not supported in current state.", "Please try again later."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR, "System error.", "Please contact GDU for help."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_HARDWARE_ERR, "Hardware error.", "Please contact GDU for help."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_INSUFFICIENT_ELECTRICITY, "Low battery.", "Please replacement battery for machine and try again."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN, "Unknown error.", NULL}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND, "Parameters are not found.", NULL}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE, "Out of range.", "Please check parameters."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_BUSY, "System is busy.", "Please try again later."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_DUPLICATE, "Have existed the same object.", "Please input valid parameters."}, \
{GDU_ERROR_SYSTEM_MODULE_CODE_ADAPTER_NOT_MATCH, "PSDK adapter do not meet requirements.", "Please try again after replacing PSDK adapter."}, \
 \
/* gimbal module error message */ \
{GDU_ERROR_GIMBAL_MODULE_CODE_PITCH_REACH_POSITIVE_LIMIT, "Pitch axis gimbal reach positive limit.", "Please do not rotate towards positive direction."}, \
{GDU_ERROR_GIMBAL_MODULE_CODE_PITCH_REACH_NEGATIVE_LIMIT, "Pitch axis gimbal reach negative limit.", "Please do not rotate towards negative direction."}, \
{GDU_ERROR_GIMBAL_MODULE_CODE_ROLL_REACH_POSITIVE_LIMIT, "Roll axis gimbal reach positive limit.", "Please do not rotate towards positive direction."}, \
{GDU_ERROR_GIMBAL_MODULE_CODE_ROLL_REACH_NEGATIVE_LIMIT, "Roll axis gimbal reach negative limit.", "Please do not rotate towards negative direction."}, \
{GDU_ERROR_GIMBAL_MODULE_CODE_YAW_REACH_POSITIVE_LIMIT, "Yaw axis gimbal reach positive limit.", "Please do not rotate towards positive direction."}, \
{GDU_ERROR_GIMBAL_MODULE_CODE_YAW_REACH_NEGATIVE_LIMIT, "Yaw axis gimbal reach negative limit.", "Please do not rotate towards negative direction."}, \
{GDU_ERROR_GIMBAL_MODULE_CODE_NON_CONTROL_AUTHORITY, "Current device do not have control authority of the gimbal.", "Please do not control gimbal with other devices that have high control priority simultaneously."}, \
 \
/* payload collaboration module error message */ \
{GDU_ERROR_PAYLOAD_COLLABORATION_MODULE_CODE_POSITION_NOT_MATCH, "Payload mount position do not meet requirements.", "Please read API and user documentation, ensuring input parameters satisfy requirements."}, \
\
/* activation error message of identity module */ \
{GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_PARAMETER_ERROR, "Activation parameter error.", "Please check the validity of the activation parameters."}, \
{GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_ENCODE_ERROR, "Activation encode error", "Please check the validity of the activation parameters"}, \
{GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_NEW_DEVICE_ERROR, "Activate a new device error", \
"\r\n* Double-check your app_id and app_key. Does it match with your GDU developer account?\r\n"\
"* If this is a new device or has been previously activated with another app_id and app_key, you need to activate it through the GDU Assistant 2 with Internet.\r\n"\
"* Please make sure you download the correct version of GDU Assistant 2 for your drone.\r\n"\
}, \
{GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_SOFTWARE_NOT_CONNECTED, "Not connection to GDU Assistant 2.", "Please connect your drone to GDU Assistant 2 while activating the new device at first time."}, \
{GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_NETWORK_ERROR, "GDU Assistant 2 not connected to the internet.", "Please activate through the GDU Assistant 2 with Internet"}, \
{GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_SERVER_ACCESS_REFUSED, "GDU server refuse this activation request.", "Please check the validity of the activation parameters."}, \
{GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_ACCESS_LEVEL_ERROR, "Activate level error.", "Please check your app_id level."}, \
{GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_OSDK_VERSION_ERROR, "Activate a wrong osdk version", "Please check your OSDK version whether match the drone or not."}, \
\
/* subscription module error message */ \
{GDU_ERROR_SUBSCRIPTION_MODULE_CODE_INVALID_TOPIC_FREQ, "Frequency of topic is invalid.", "Please read API and user documentation, ensuring input parameters satisfy requirements."}, \
{GDU_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_DUPLICATE, "Topic is subscribed repeatedly.", "Please do not subscribe a topic repeatedly."}, \
{GDU_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_NOT_SUBSCRIBED, "Requested topic have not been subscribed.", "Please try to get value after subscribing topic."}, \
{GDU_ERROR_SUBSCRIPTION_MODULE_CODE_TIMESTAMP_NOT_ENABLE, "Requested topic do not have timestamp data. Did not enable timestamp when subscribe topic.", "Please subscribe topic enabled timestamp."},\
{GDU_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_NOT_SUPPORTED, "Requested topic is not supported.", "Please check the topic is supported or not on current aircraft version."}, \
\
/* mop channel module error message */ \
{GDU_ERROR_MOP_CHANNEL_MODULE_CODE_CONNECTION_CLOSE, "Connection of channel is closed. The peer channel do not work or abnormally be closed.", "Please confirm state of the peer channel and reaccept the connection request of MSDK/OSDK"}, \
\
/* flight controller module error message */ \
{GDU_ERROR_FC_MODULE_CODE_RC_MODE_ERROR, "RC_MODE_ERROR", "Please check the RC mode"}, \
{GDU_ERROR_FC_MODULE_CODE_RELEASE_CONTROL_SUCCESS, "RELEASE_CONTROL_SUCCESS", NULL} ,\
{GDU_ERROR_FC_MODULE_CODE_OBTAIN_CONTROL_SUCCESS, "OBTAIN_CONTROL_SUCCESS", NULL} ,\
{GDU_ERROR_FC_MODULE_CODE_OBTAIN_CONTROL_IN_PROGRESS, "OBTAIN_CONTROL_IN_PROGRESS", NULL} ,\
{GDU_ERROR_FC_MODULE_CODE_RELEASE_CONTROL_IN_PROGRESS, "RELEASE_CONTROL_IN_PROGRESS", NULL} ,\
{GDU_ERROR_FC_MODULE_CODE_RC_NEED_MODE_P, "RC_NEED_MODE_P", NULL} ,\
{GDU_ERROR_FC_MODULE_CODE_RC_NEED_MODE_F, "RC_NEED_MODE_F", NULL} ,\
{GDU_ERROR_FC_MODULE_CODE_PARAM_READ_WRITE_INVALID_PARAM, "read or write invalid param", "Please check hash value and param value"} ,\
{GDU_ERROR_FC_MODULE_CODE_IOC_OBTAIN_CONTROL_ERROR, "IOC_OBTAIN_CONTROL_ERROR", NULL} ,\
{GDU_ERROR_FC_MODULE_CODE_KEY_ERROR, "Activate key error", NULL} ,\
{GDU_ERROR_FC_MODULE_CODE_NO_AUTHORIZATION_ERROR, "No authorization", "Please finish activation firstly"} ,\
{GDU_ERROR_FC_MODULE_CODE_NO_RIGHTS_ERROR, "No rights error", NULL} ,\
{GDU_ERROR_FC_MODULE_CODE_SYSTEM_ERROR, "Unknown system error", NULL} ,\
\
/* flight controller action module error message */\
{GDU_ERROR_FC_JOYSTICK_MODULE_OBTAIN_RELEASE_JOYSTICK_AUTH_SUCCESS, "Obtain/Release joystick authority success", NULL} ,\
{GDU_ERROR_FC_JOYSTICK_MODULE_DEVICE_NOT_ALLOW, "The requesting device is not allowed to obtain/release joystick control authority, only support OSDK/MSDK", "Please use right devices(OSDK/MSDK)"} ,\
{GDU_ERROR_FC_JOYSTICK_MODULE_TAKING_OFF, "Not allowed to obtain/release joystick control authority when drone is taking off ", "Please do it before or after taking off"} ,\
{GDU_ERROR_FC_JOYSTICK_MODULE_LANDING, "Not allowed to obtain/release joystick control authority when drone is landing", "Please do it before or after landing"} ,\
{GDU_ERROR_FC_JOYSTICK_MODULE_CMD_INVALID, "Invalid input command", "Please check your input command which only support 0/1"} ,\
{GDU_ERROR_FC_JOYSTICK_MODULE_RC_NOT_P_MODE, "Not allowed to obtain/release joystick control authority when rc is not in P_MODE", "Please switch RC to P_MODE"} ,\
{GDU_ERROR_FC_JOYSTICK_MODULE_CMD_LENGTH_ERROR, "Invalid length of input command", "Please input valid length command"} ,\
{GDU_ERROR_FC_JOYSTICK_MODULE_HAS_NO_JOYSTICK_AUTHORITY, "Not allowed to release joystick control authority when drone has no joystick authority", "Please obtain joystick authority first"} ,\
{GDU_ERROR_FC_JOYSTICK_MODULE_IN_RC_LOST_ACTION, "Not allowed to obtain/release joystick control authority when drone is executing rc lost action", "Please change battery"} ,\
\
/* flight controller action module error message */\
{GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_MOTOR_ON, "now motor is on", "Please check motor status"} ,\
{GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_MOTOR_OFF, "now motor is off", "Please check motor status"} ,\
{GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_IN_AIR, "aircraft is in air", "Please check aircraft flight status"} ,\
{GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_NOT_IN_AIR, "aircraft is not in air", "Please check aircraft flight status"} ,\
{GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_HOME_POINT_NOT_SET, "home point not set", "Please set home point"} ,\
{GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_BAD_GPS, "bad GPS", "Please fly where the GPS signal is good"} ,\
{GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_IN_SIMULATION, "in simulation", "Please exit simulation first"} ,\
{GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_CANNOT_START_MOTOR, "can not start motor", "Please check motor status"} ,\
{GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_LOW_VOLTAGE, "low voltage", "Please change battery"} ,\
{GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_SPEED_TOO_LARGE, "speed too large", "Please slow down"} ,\
                          \
/* flight controller arrest flying module error message */\
{GDU_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_LOGOUT_SET_SUCCESS, "register/logout/execute arrest-flying success", NULL} ,\
{GDU_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_LOGOUT_NULL_POINTER, "null pointer when register/logout arrest-flying", NULL} ,\
{GDU_ERROR_FC_ARREST_FLYING_MODULE_HMS_CODE_HAS_REGISTED, "duplicate hms code", NULL} ,\
{GDU_ERROR_FC_ARREST_FLYING_MODULE_HMS_CODE_NOT_FIND, "invalid hms code", NULL} ,\
{GDU_ERROR_FC_ARREST_FLYING_MODULE_ADD_ITEM_NO_DECRIPTION, "no description when register arrest-flying", NULL} ,\
{GDU_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_ID_INVALID, "invalid request id", NULL} ,\
{GDU_ERROR_FC_ARREST_FLYING_MODULE_STOP_IN_AIR_HMSCODE_NOT_IN_WHITE_TABLE, "hms code is not in white list for allowing stop motor in the air", NULL} ,\
{GDU_ERROR_FC_ARREST_FLYING_MODULE_INVALID_FORMAT_HMSCODE, "invalid hms code format", NULL} ,\
{GDU_ERROR_FC_ARREST_FLYING_MODULE_HMSCODE_NOT_IN_WHITE_TABLE, "hms code is not in arrest-flying white list",NULL} ,\
\
/* flight controller home location module error message */\
{GDU_ERROR_FC_HOME_LOCATION_MODULE_UNKNOWN_FAILED_REASON, "set home location fail, unknown reason", NULL} ,\
{GDU_ERROR_FC_HOME_LOCATION_MODULE_INVALID_GPS_COORDINATE, "invalid GPS coordinate when set APP or RC to be home location", NULL} ,\
{GDU_ERROR_FC_HOME_LOCATION_MODULE_NOT_BE_RECORD, "home location is not inited", "Please waiting for aircraft recording home location"} ,\
{GDU_ERROR_FC_HOME_LOCATION_MODULE_GPS_NOT_READY, "GPS level < 4", NULL} ,\
{GDU_ERROR_FC_HOME_LOCATION_MODULE_DIS_TOO_FAR, "new home location is more than 20km away from current home locaton(APP/RC)", NULL} ,\
\
/* flight controller emergency stop motor module error message */\
{GDU_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_VERSION_NOT_MATCH, "emergency stop motor version not match", NULL} ,\
{GDU_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_CMD_INVALID, "emergency stop motor cmd invalid", NULL} ,\
/* camera manager module error message */\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND, "Command not supported", "Check the firmware or command validity"} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_TIMEOUT, "Camera's execution of this action has timed out", "Try again or check the firmware or command"} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_RAM_ALLOCATION_FAILED, "Camera's execution of this action is out of memory", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_INVALID_COMMAND_PARAMETER,  "Camera received invalid parameters", "Check the validity of the parameter"} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE, "Camera is busy or the command is not supported in the Camera's current state", "Check current camera state is if appropriate fot the CMD"} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_CAMERA_TIME_NOT_SYNCHRONIZED, "The time stamp of the camera is not sync", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_SET_FAILED, "Camera failed to set the parameters it received", "Please check the parameter to set is if supported in your devices."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_GET_FAILED, "Camera param get failed", "Please check the parameter to get is if supported in your devices."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_MISSING, "Camera has no SD Card", "Please install SD card."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_FULL, "The Camera's SD Card is full", "Please make sure the SD card has enough space."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_ERROR, "Error accessing the SD Card", "Please check the validity of the SD card."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_SENSOR_ERROR, "Camera sensor error", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_SYSTEM_ERROR, "Camera system error", "Please recheck all the running conditions or contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_TOTAL_TOO_LONG, "Camera param get failed", "Please check the validity of the parameter length"} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_MODULE_INACTIVATED, "Camera module is not activated", "Please activate the module first."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS, "The seq number of Firmware data is invalid", "Please check the validity of the camera firmware."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_VERIFICATION_ERROR, "Firmware check error", "Please check the validity of the camera firmware."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_FLASH_WRITE_ERROR, "Camera flash write error", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_TYPE_MISMATCH, "Firmware type is invalid", "Please check the validity of the camera firmware."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_REMOTE_CONTROL_UNCONNECTED, "Remote Control is disconnected now", "Please check the connection with remote control is if OK."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_HARDWARE_ERROR, "Camera hardware error", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_AIRCRAFT_UNCONNECTED, "Disconnect with aircraft", "Please check the connection with aircraft is if OK."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_CANNOT_UPGRADE_IN_CUR_STATE, "Camera cannot not upgrade in current status", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNDEFINE_ERROR, "Undefined error", "Please contact <dev@gdu.com> for help."} ,\
\
/* gimbal manager module error message */\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND, "Command not supported", "Check the firmware or command validity"} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_TIMEOUT, "Camera's execution of this action has timed out", "Try again or check the firmware or command"} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_RAM_ALLOCATION_FAILED, "Camera's execution of this action is out of memory", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_INVALID_COMMAND_PARAMETER,  "Camera received invalid parameters", "Check the validity of the parameter"} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE, "Camera is busy or the command is not supported in the Camera's current state", "Check current camera state is if appropriate fot the CMD"} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_CAMERA_TIME_NOT_SYNCHRONIZED, "The time stamp of the camera is not sync", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_SET_FAILED, "Camera failed to set the parameters it received", "Please check the parameter to set is if supported in your devices."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_GET_FAILED, "Camera param get failed", "Please check the parameter to get is if supported in your devices."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_MISSING, "Camera has no SD Card", "Please install SD card."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_FULL, "The Camera's SD Card is full", "Please make sure the SD card has enough space."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_ERROR, "Error accessing the SD Card", "Please check the validity of the SD card."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_SENSOR_ERROR, "Camera sensor error", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_SYSTEM_ERROR, "Camera system error", "Please recheck all the running conditions or contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_TOTAL_TOO_LONG, "Camera param get failed", "Please check the validity of the parameter length"} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_MODULE_INACTIVATED, "Camera module is not activated", "Please activate the module first."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS, "The seq number of Firmware data is invalid", "Please check the validity of the camera firmware."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_VERIFICATION_ERROR, "Firmware check error", "Please check the validity of the camera firmware."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_FLASH_WRITE_ERROR, "Camera flash write error", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_TYPE_MISMATCH, "Firmware type is invalid", "Please check the validity of the camera firmware."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_REMOTE_CONTROL_UNCONNECTED, "Remote Control is disconnected now", "Please check the connection with remote control is if OK."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_HARDWARE_ERROR, "Camera hardware error", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_AIRCRAFT_UNCONNECTED, "Disconnect with aircraft", "Please check the connection with aircraft is if OK."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_CANNOT_UPGRADE_IN_CUR_STATE, "Camera cannot not upgrade in current status", "Please contact <dev@gdu.com> for help."} ,\
{GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNDEFINE_ERROR, "Undefined error", "Please contact <dev@gdu.com> for help."} ,\
\
/* waypoint v2 module error message */   \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_SUCCESS, "Execute waypoint v2 cmd successfully", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_INVALID_DATA_LENGTH, "The length of the data is illegal based on the protocol ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_INVALD_FLOAT_NUM, "Invalid float number (NAN or INF) ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_VERSION_NO_MATCH, "Waypoint mission version can't match with firmware ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_UNKNOWN, "Fatal error	 Unexpected result	 ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_RESV, "Reserved", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_NUM_TOO_MANY, "Min_initial_waypoint_num is large than permitted_max_waypoint_num ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_NUM_TOO_FEW, "Min_initial_waypoint_num is less than permitted_min_waypoint_num ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_END_INDEX, "Waypoint_end_index is equal or large than total_waypoint_num ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_START_ID_GT_END_ID, "The start index is greater than end index of upload wps ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_END_ID_GT_TOTAL_NUM, "The end index of uplod wps is greater than inited total numbers ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DOWNLOAD_WPS_NOT_IN_STORED_RAGNE, "The index of first and end waypoint expected to download is not in range of stored in FC ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_CUR_POS_IS_FAR_AWAY_FROM_FIRST_WP, "Current position is far away from the first waypoint. ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_ADJ_WPS_TOO_CLOSE, "It is too close from two adjacent waypoints",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_ADJ_WPS_TOO_FAR, "The distance betwween two adjacent waypoints is not in[0.5m, 5000m]",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_MAX_VEL_GT_GLOBAL, "The max vel of uplod wp is greater than global max vel ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_LOCAL_MAX, "The local cruise vel of upload wp is greater than local max vel ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_GLOBAL_MAX, "The local cruise vel of upload wp is greater than global max vel ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_GLOBAL_MAX_VEL, "Global_max_vel is greater than permitted_max_vel or less than permitted_min_vel ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GLOBAL_CRUISE_VEL_GT_MAX_VEL, "Global_cruise_vel is greater than global_max_vel ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_GOTO_FIRST_FLAG, "Goto_first_point_mode is out of range of waypoint_goto_first_flag_t_enum ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_FINISHED_ACTION, "Finished_action is out of range of wp_plan_finish_action_t_enum ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_RC_LOST_ACTION, "Rc_lost_action is out of range of wp_plan_rc_lost_action_t_enum ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_MODE_INVALID, "The yaw mode of upload wp is invalid. reference to waypoint2_yaw_mode_t defined in math_waypoint_planner.h ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_CMD_NOT_IN_RANGE, "The yaw command of upload wp is not in range. the range for MR:[-180 180]",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_TURN_DIRECTION_INVALID, "The yaw turn direction of upload wp is invalid. it should be 0:clockwise or 1:anti-clockwise ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_WP_TYPE_INVALID, "The wp type of upload wp is invalid. reference to waypoint_type_t defined in math_waypoint_planner.h ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GO_STOP_CMD_INVALID, "Go/stop command is invalid. ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INVALID_PAUSE_RECOVERY_CMD, "The command of pause/recovery is not equal to any of the command enum ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INVALID_BREAK_RESTORE_CMD, "The command of break/restore is not equal to any of the command enum ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_REF_POINT, "Initial reference point position coordinate exceed set range ",NULL},    \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DAMPING_DIS_GE_DIS_OF_ADJ_POINTS,  "The damping dis is greater than or equal the distance of adjacent point ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_CANNT_SET_WP_LINE_EXIT_TYPE, "Cann't set wp_line_exit type to wp ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INFO_NOT_UPLOADED, "The init info of Ground Station is not uploaded yet ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_HAS_NOT_UPLOADED, "The wp has not uploaded yet ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOADED_WP_NOT_ENOUGH, "Min_initial_waypoint_num is not uploaded. ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_HAS_STARTED, "Waypoint plan has started when received go command. ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING, "Waypoint plan not running when received stop command. ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING_FOR_PAUSE_RECOVERY, "Ground station(GS) is not started(used by pause/recovery) ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING_FOR_BREAK_RESTORE, "Ground station(GS) is not started(used by break/restore) ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NOT_IN_WP_MIS, "Not in the waypoint mission(MIS)(cannot pause/recovery or break/restore) ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_HAS_BEEN_PAUSED, "The current status is paused",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_NOT_PAUSED, "Not in paused status",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_HAS_BEEN_BROKEN, "The current status is broken",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_NOT_BROKEN, "Not in break status",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_PAUSE_RECOVERY_NOT_SUPPORTED, "The configuration forbid using pause/recovery API ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_BREAK_RESTORE_NOT_SUPPORTED, "The configuration forbid using break/restore API ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_BREAK_POINT, "No break point is recorded for restore ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_CUR_TRAJ_PROJECT, "No current trajectory project point is recorded for restore ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_NXT_TRAJ_PROJECT, "No next trajectory project point is recorded for restore ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_NNT_TRAJ_PROJECT, "No next the next trajectory project point is recorded for restore ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_WP_ID_NOT_CONTINUE, "The index of upload wp is not continue after the store wp ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_LINE_ENTER_NOT_SET_TO_START_WP, "The WP_LINE_ENTER wp_type set to a wp which is not the init start waypoint ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_WHEN_PLAN_HAS_STARTED, "The waypoint plan has started when initializing waypoint ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DAMPING_DIS_EXCEED_RANGE, "Waypoint damping distance exceed set range ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WAYPOINT_COOR_EXCEED_RANGE, "Waypoint position coordinate exceed rational range ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_FIRST_WP_TYPE_IS_WP_TURN_NO, "First waypoint type error",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_EXCEED_RADIUS_LIMIT, "Waypoint position exceed radius limit ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_EXCEED_HEIGHT_LIMIT, "Waypoint position exceed height limit ",NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_RESV, "Reserved error code", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_WP_MIS_CHECK_FAIL, "Head_node is null or atti_not_healthy or gyro_not_healthy or horiz_vel_not healthy or horiz_abs_pos_not_healthy. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_HOME_NOT_RECORDED, "The home point is no recorded yet	 which will be executed at the first time of GPS level > 3(MR	FW). ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_LOW_LOCATION_ACCURACY, "Current location accuracy is low for bad GPS signal. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_RTK_CONDITION_IS_NOT_READY, "Use rtk_data	 but rtk is not connected or rtk_data is invalid ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_RESV, "Reserved error code", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_CROSS_NFZ, "The trajectory cross the NFZ ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_BAT_LOW, "Current capacity of smart battery or voltage of non-smart battery is lower than level 1 or level 2 threshold ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_RESV, "Reserved error code", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ID_DUPLICATED, "The ID of Action is duplicated. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ITEMS_SPACE_NOT_ENOUGH, "There is no enough memory space for new Action Item ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_SIZE_GT_BUF_SIZE, "The size of buffer used to get the info of Action is less than the size of Action. Normally users can not get this. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ID_NOT_FOUND, "The ID of Action is not found. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_DOWNLOAD_ACTION_ID_RANGE_ERROR, "The download action start id is bigger than the action end id ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_NO_ACTION_ITEMS_STORED, "Can not download or get min-max action ID for no action items stored in action kernel ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_RESV, "Reserved error code", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_TYPE_INVALID, "The type ID of Trigger is invalid. It might not defined or the information is empty. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_END_INDEX_LT_START_INDEX, "Wp_end_index is less than wp_start_index in reach_waypoint_trigger. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_INVALID_INTERVAL_WP_NUM, "Interval_wp_num is large than the difference of wp_start_index and wp_end_index in reach_waypoint_trigger. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_INVALID_AUTO_TERMINATE_WP_NUM , "Auto_terminate_wp_num is large than interval_wp_num in reach_waypoint_trigger. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_ASSOCIATE_INVALID_TYPE, "The associate_type is greater than the maximum value. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_SIMPLE_INTERVAL_INVALID_TYPE, "The interval type is greater than the maximum value. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_RESV, "Reserved error code", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_EXEC_NON_SUPPORTED, "The execution of Actuator is not supported	 e.g.	 try to stop camera shooting. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_TYPE_INVALID, "The type ID of Actuator is invalid. It might not defined or the information is empty. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_FUNC_INVALID, "The Function ID of Actuator is invalid. It might not defined or the information is empty. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_RESV, "Reserved error code", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_SINGLE_SHOT_CMD_TO_CAMERA_FAIL, "Fail to send shot cmd to camera for no camera or camera is busy. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_VIDEO_START_CMD_TO_CAMERA_FAIL, "Fail to send video start cmd to camera for no camera or camera is busy. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_VIDEO_STOP_CMD_TO_CAMERA_FAIL, "Fail to send video stop cmd to camera for no camera or camera is not busy. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_FOCUS_PARAM_XY_INVALID, "Camera focus param xy exceed valid range (0	 1). ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_FOCUS_CMD_TO_CAMERA_FAIL, "Fail to send focus cmd to camera for no camera or camera is busy. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_FOCALIZE_CMD_TO_CAMERA_FAIL, "Fail to send focalize cmd to camera for no camera or camera is busy. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_FOCAL_DISTANCE_INVALID, "Focal distance of camera focalize function exceed valid range. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_EXEC_FAIL, "This err code indicate camera fail to exec coressponding cmd	 and the low 8 bit", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_RESV, "Reserved error code", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_INVALID_RPY_ANGLE_CTRL_CMD, "Gimbal roll	pitch	yaw angle ctrl cmd param invalid	", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_INVALID_DURATION_CMD, "Gimbal duration param invalid	 unable to exec. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_FAIL_TO_ARRIVE_TGT_ANGLE, "Gimbal fail to arrive target angle . ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_FAIL_TO_SEND_CMD_TO_GIMBAL, "Fail to send cmd to gimbal for gimbal is busy or no gimbal. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_THIS_INDEX_OF_GIMBAL_NOT_DOING_UNIFORM_CTRL , "Fail to stop gimbal uniform ctrl because index error. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_RESV, "Reserved error code", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_YAW_INVALID_YAW_ANGLE, "Yaw angle is lager max yaw angle. ", NULL}, \
{GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_YAW_TO_TGT_ANGLE_TIMEOUT, "Failed to target yaw angle because of timeout. ", NULL}, \



#define GDU_RETURN_CODE_OK                 GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS /*!< Payload SDK return code represents as status is ok. */
#define GDU_RETURN_CODE_ERR_ALLOC          GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED /*!< Payload SDK return code represents as status alloc error. */
#define GDU_RETURN_CODE_ERR_TIMEOUT        GDU_ERROR_SYSTEM_MODULE_CODE_TIMEOUT /*!< Payload SDK return code represents as status timeout error. */
#define GDU_RETURN_CODE_ERR_NOT_FOUND      GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND /*!< Payload SDK return code represents as status not found error. */
#define GDU_RETURN_CODE_ERR_OUT_OF_RANGE   GDU_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE /*!< Payload SDK return code represents as status out of range error. */
#define GDU_RETURN_CODE_ERR_PARAM          GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER /*!< Payload SDK return code represents as status parameter error. */
#define GDU_RETURN_CODE_ERR_SYSTEM         GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR /*!< Payload SDK return code represents as status system error. */
#define GDU_RETURN_CODE_ERR_BUSY           GDU_ERROR_SYSTEM_MODULE_CODE_BUSY /*!< Payload SDK return code represents as status busy error. */
#define GDU_RETURN_CODE_ERR_UNSUPPORT      GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT /*!< Payload SDK return code represents as status nonsupport error. */
#define GDU_RETURN_CODE_ERR_UNKNOWN        GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN /*!< Payload SDK return code represents as status unknown error. */

/* Exported types ------------------------------------------------------------*/
/**
 * @brief GDU module enum for defining error code.
 */
typedef enum {
    GDU_ERROR_MODULE_SYSTEM = 0,
    GDU_ERROR_MODULE_PLATFORM,
    GDU_ERROR_MODULE_LOGGER,
    GDU_ERROR_MODULE_TIME_SYNC,
    GDU_ERROR_MODULE_COMMAND,
    GDU_ERROR_MODULE_CAMERA,
    GDU_ERROR_MODULE_GIMBAL,
    GDU_ERROR_MODULE_XPORT,
    GDU_ERROR_MODULE_PAYLOAD_COLLABORATION,
    GDU_ERROR_MODULE_WIDGET,
    GDU_ERROR_MODULE_CORE,
    GDU_ERROR_MODULE_IDENTITY,
    GDU_ERROR_MODULE_TRANSMISSION,
    GDU_ERROR_MODULE_DATA_CHANNEL,
    GDU_ERROR_MODULE_SUBSCRIPTION,
    GDU_ERROR_MODULE_MOP_CHANNEL,
    GDU_ERROR_MODULE_POSITIONING,
    GDU_ERROR_MODULE_POWER_MANAGEMENT,
    GDU_ERROR_MODULE_AIRCRAFTINFO,
    GDU_ERROR_MODULE_PRODUCTINFO,
    GDU_ERROR_MODULE_FLOWCONTROLLER,
    GDU_ERROR_MODULE_DOWNLOADER,
    GDU_ERROR_MODULE_PARAMETER,
    GDU_ERROR_MODULE_UTIL,
    GDU_ERROR_MODULE_USER,
    GDU_ERROR_MODULE_NEGOTIATE,
    GDU_ERROR_MODULE_UPGRADE,
    GDU_ERROR_MODULE_FC_BASIC,
    GDU_ERROR_MODULE_FC_JOYSTICK,
    GDU_ERROR_MODULE_FC_ACTION,
    GDU_ERROR_MODULE_FC_ARREST_FLYING,
    GDU_ERROR_MODULE_FC_HOME_LOCATION,
    GDU_ERROR_MODULE_FC_EMERGENCY_STOP_MOTOR,
    GDU_ERROR_MODULE_CAMERA_MANAGER,
    GDU_ERROR_MODULE_GIMBAL_MANAGER,
    GDU_ERROR_MODULE_WAYPOINT_V2,
    GDU_ERROR_MODULE_ERROR,
} E_GduErrorModule;

/**
 * @brief Raw error code of system module.
 */
typedef enum {
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_SUCCESS = 0x000,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_INVALID_REQUEST_PARAMETER = 0x0D4,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_EXECUTING_HIGHER_PRIORITY_TASK = 0x0D7,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_NONSUPPORT = 0x0E0,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_TIMEOUT = 0x0E1,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_MEMORY_ALLOC_FAILED = 0x0E2,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_INVALID_PARAMETER = 0x0E3,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_NONSUPPORT_IN_CURRENT_STATE = 0x0E4,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_SYSTEM_ERROR = 0x0EC,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_HARDWARE_ERR = 0x0FA,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_INSUFFICIENT_ELECTRICITY = 0x0FB,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_UNKNOWN = 0x0FF,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_NOT_FOUND = 0x100,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_OUT_OF_RANGE = 0x101,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_BUSY = 0x102,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_DUPLICATE = 0x103,
    GDU_ERROR_SYSTEM_MODULE_RAW_CODE_ADAPTER_NOT_MATCH = 0x104,
} E_GduErrorSystemModuleRawCode;

/**
 * @brief Raw error code of activation of identity module.
 */
typedef enum {
    GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_PARAMETER_ERROR = 0x0001,
    GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_ENCODE_ERROR = 0x0002,
    GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_NEW_DEVICE_ERROR = 0x0003,
    GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_SOFTWARE_NOT_CONNECTED = 0x0004,
    GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_NETWORK_ERROR = 0x0005,
    GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_SERVER_ACCESS_REFUSED = 0x0006,
    GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_ACCESS_LEVEL_ERROR = 0x0007,
    GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_OSDK_VERSION_ERROR = 0x0008,
} E_GduErrorIdentityModuleActivationRawCode;

/**
 * @brief Raw error code of gimbal module.
 */
typedef enum {
    GDU_ERROR_GIMBAL_MODULE_RAW_CODE_PITCH_REACH_POSITIVE_LIMIT = 0x000,
    GDU_ERROR_GIMBAL_MODULE_RAW_CODE_PITCH_REACH_NEGATIVE_LIMIT = 0x001,
    GDU_ERROR_GIMBAL_MODULE_RAW_CODE_ROLL_REACH_POSITIVE_LIMIT = 0x002,
    GDU_ERROR_GIMBAL_MODULE_RAW_CODE_ROLL_REACH_NEGATIVE_LIMIT = 0x003,
    GDU_ERROR_GIMBAL_MODULE_RAW_CODE_YAW_REACH_POSITIVE_LIMIT = 0x004,
    GDU_ERROR_GIMBAL_MODULE_RAW_CODE_YAW_REACH_NEGATIVE_LIMIT = 0x005,
    GDU_ERROR_GIMBAL_MODULE_RAW_CODE_NON_CONTROL_AUTHORITY = 0x006,
} E_GduErrorGimbalModuleRawCode;

/**
 * @brief Raw error code of payload collaboration module.
 */
typedef enum {
    GDU_ERROR_PAYLOAD_COLLABORATION_MODULE_RAW_CODE_POSITION_NOT_MATCH = 0x000,
} E_GduErrorPayloadCollaborationModuleRawCode;

/**
 * @brief Raw error code of subscription module.
 */
typedef enum {
    GDU_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_INVALID_TOPIC_FREQ = 0x000,
    GDU_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_DUPLICATE = 0x001,
    GDU_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_NOT_SUBSCRIBED = 0x002,
    GDU_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TIMESTAMP_NOT_ENABLE = 0x003,
    GDU_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_NOT_SUPPORTED = 0x004,
} E_GduErrorSubscriptionModuleRawCode;

/**
 * @brief Raw error code of mop channel module.
 */
typedef enum {
    GDU_ERROR_MOP_CHANNEL_MODULE_RAW_CODE_CONNECTION_CLOSE = 0x00B,
} E_GduErrorMopChannelModuleRawCode;

/**
 * @brief Raw error code of flight controller module.
 */
typedef enum {
    GDU_ERROR_FC_MODULE_RAW_CODE_RC_MODE_ERROR = 0x0000,
    GDU_ERROR_FC_MODULE_RAW_CODE_RELEASE_CONTROL_SUCCESS = 0x0001,
    GDU_ERROR_FC_MODULE_RAW_CODE_OBTAIN_CONTROL_SUCCESS = 0x0002,
    GDU_ERROR_FC_MODULE_RAW_CODE_OBTAIN_CONTROL_IN_PROGRESS = 0x0003,
    GDU_ERROR_FC_MODULE_RAW_CODE_RELEASE_CONTROL_IN_PROGRESS = 0x0004,
    GDU_ERROR_FC_MODULE_RAW_CODE_RC_NEED_MODE_P = 0x0005,
    GDU_ERROR_FC_MODULE_RAW_CODE_RC_NEED_MODE_F = 0x0006,
    GDU_ERROR_FC_MODULE_RAW_CODE_PARAM_READ_WRITE_INVALID_PARAM = 0x0007,
    GDU_ERROR_FC_MODULE_RAW_CODE_IOC_OBTAIN_CONTROL_ERROR = 0x00C9,
    GDU_ERROR_FC_MODULE_RAW_CODE_KEY_ERROR = 0xFF00,
    GDU_ERROR_FC_MODULE_RAW_CODE_NO_AUTHORIZATION_ERROR = 0xFF01,
    GDU_ERROR_FC_MODULE_RAW_CODE_NO_RIGHTS_ERROR = 0xFF02,
    GDU_ERROR_FC_MODULE_RAW_CODE_SYSTEM_ERROR = 0xFF03,
} E_GduErrorFCModuleRawCode;

/**
 * @brief Raw error code of flight controller joystick module.
 */
typedef enum {
    GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_SUCCESS = 0x00,
    GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_DEVICE_NOT_ALLOW = 0x01,
    GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_ENGINE_STARTING = 0x02,
    GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_TAKING_OFF = 0x03,
    GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_LANDING = 0x04,
    GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_CMD_INVALID = 0x05,
    GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_RC_NOT_P_MODE = 0x06,
    GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_CMD_LENGTH_ERROR = 0x07,
    GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_HAS_NO_JOYSTICK_AUTHORITY = 0x08,
    GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_IN_RC_LOST_ACTION = 0x09,
} E_GduErrorFCJoystickModuleRawCode;

/**
 * @brief Raw error code of flight controller action module.
 */
typedef enum {
    GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_MOTOR_ON = 0x01,
    GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_MOTOR_OFF = 0x02,
    GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_IN_AIR = 0x03,
    GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_NOT_IN_AIR = 0x04,
    GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_HOME_POINT_NOT_SET = 0x05,
    GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_BAD_GPS = 0x06,
    GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_IN_SIMULATION = 0x07,
    GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_CANNOT_START_MOTOR = 0x11,
    GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_LOW_VOLTAGE = 0x12,
    GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_SPEED_TOO_LARGE = 0x14,
} E_GduErrorFCActionModuleRawCode;

/**
 * @brief Raw error code of flight controller arrest flying module.
 */
typedef enum {
    GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_LOGOUT_SET_SUCCESS = 0x00,
    GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_LOGOUT_NULL_POINTER = 0x01,
    GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMS_CODE_HAS_REGISTED = 0x02,
    GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMS_CODE_NOT_FIND = 0x03,
    GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_ADD_ITEM_NO_DECRIPTION = 0x04,
    GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_ID_INVALID = 0x05,
    GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_STOP_IN_AIR_HMSCODE_NOT_IN_WHITE_TABLE = 0x06,
    GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_INVALID_FORMAT_HMSCODE = 0x07,
    GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMSCODE_NOT_IN_WHITE_TABLE = 0x08,
} E_GduErrorFCArrestFlyingModuleRawCode;

/**
 * @brief Raw error code of flight controller home location module.
 */
typedef enum {
    GDU_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_UNKNOWN_FAILED_REASON = 0x01,
    GDU_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_INVALID_GPS_COORDINATE = 0x02,
    GDU_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_NOT_BE_RECORD = 0x03,
    GDU_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_GPS_NOT_READY = 0x04,
    GDU_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_DIS_TOO_FAR = 0x05,
} E_GduErrorFCHomeLocationModuleRawCode;

/**
 * @brief Raw error code of flight controller emergency stop motor module.
 */
typedef enum {
    GDU_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_RAW_CODE_VERSION_NOT_MATCH = 0x01,
    GDU_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_RAW_CODE_CMD_INVALID = 0x02,
} E_GduErrorFCKillSwitchModuleRawCode;

/**
 * @brief Raw error code of camera manager
 */
typedef enum {
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND = 0xE0, /*!< Do not support this command */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_TIMEOUT = 0xE1, /*!< Execution timeout */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_RAM_ALLOCATION_FAILED = 0xE2, /*!< Memory alloc failed */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_INVALID_COMMAND_PARAMETER = 0xE3, /*!< Invalid parameter for the command */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE = 0xE4, /*!< Do not support this command in the current state */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_CAMERA_TIME_NOT_SYNCHRONIZED = 0xE5, /*!< Timestamp of camera is not synchronized */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_SET_FAILED = 0xE6, /*!< Setting parameter failed */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_GET_FAILED = 0xE7, /*!< Getting parameter failed */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_MISSING = 0xE8, /*!< SD card is not installed */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_FULL = 0xE9, /*!< SD card is full */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_ERROR = 0xEA, /*!< Error accessing the SD Card */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SENSOR_ERROR = 0xEB, /*!< Sensor go wrong */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SYSTEM_ERROR = 0xEC, /*!< System error */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_TOTAL_TOO_LONG = 0xED, /*!< Length of the parameter is too long */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_MODULE_INACTIVATED = 0xEE, /*!< Module is too not activated yet */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS = 0xF0, /*!< Fireware data number is a discontinuous number */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_VERIFICATION_ERROR = 0xF2, /*!< Error verifying fireware */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FLASH_WRITE_ERROR = 0xF4, /*!< Error writing flash */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_TYPE_MISMATCH = 0xF6, /*!< Firmware type don't match */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_REMOTE_CONTROL_UNCONNECTED = 0xF8, /*!< Not connect remote control yet */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_HARDWARE_ERROR = 0xFA, /*!< Hardware fault */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_AIRCRAFT_UNCONNECTED = 0xFC, /*!< Aircraft is not connected yet */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_CANNOT_UPGRADE_IN_CUR_STATE = 0xFE, /*!< Cannot upgrade in current status (Please reboot or contact with GDU support */
    GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNDEFINE_ERROR = 0xFF, /*!< Undefined error */
} E_GduErrorCameraManagerModuleRawCode;

/**
 * @brief Raw error code of gimbal manager
 */
typedef enum {
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND = 0xE0, /*!< Do not support this command */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_TIMEOUT = 0xE1, /*!< Execution timeout */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_RAM_ALLOCATION_FAILED = 0xE2, /*!< Memory alloc failed */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_INVALID_COMMAND_PARAMETER = 0xE3, /*!< Invalid parameter for the command */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE = 0xE4, /*!< Do not support this command in the current state */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_CAMERA_TIME_NOT_SYNCHRONIZED = 0xE5, /*!< Timestamp of camera is not synchronized */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_SET_FAILED = 0xE6, /*!< Setting parameter failed */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_GET_FAILED = 0xE7, /*!< Getting parameter failed */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_MISSING = 0xE8, /*!< SD card is not installed */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_FULL = 0xE9, /*!< SD card is full */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_ERROR = 0xEA, /*!< Error accessing the SD Card */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SENSOR_ERROR = 0xEB, /*!< Sensor go wrong */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SYSTEM_ERROR = 0xEC, /*!< System error */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_TOTAL_TOO_LONG = 0xED, /*!< Length of the parameter is too long */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_MODULE_INACTIVATED = 0xEE, /*!< Module is too not activated yet */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS = 0xF0, /*!< Fireware data number is a discontinuous number */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_VERIFICATION_ERROR = 0xF2, /*!< Error verifying fireware */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FLASH_WRITE_ERROR = 0xF4, /*!< Error writing flash */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_TYPE_MISMATCH = 0xF6, /*!< Firmware type don't match */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_REMOTE_CONTROL_UNCONNECTED = 0xF8, /*!< Not connect remote control yet */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_HARDWARE_ERROR = 0xFA, /*!< Hardware fault */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_AIRCRAFT_UNCONNECTED = 0xFC, /*!< Aircraft is not connected yet */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_CANNOT_UPGRADE_IN_CUR_STATE = 0xFE, /*!< Cannot upgrade in current status (Please reboot or contact with GDU support */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNDEFINE_ERROR = 0xFF, /*!< Undefined error */
} E_GduErrorGimbalManagerModuleRawCode;

/**
 * @brief Waypoint v2 total error raw code.
 */
typedef enum {
    /* Waypoint v2 common error raw code */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_SUCCESS = 0x00000,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_INVALID_DATA_LENGTH = 0x00001,   /*!< the length of the data is illegal based on the protocol */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_INVALD_FLOAT_NUM = 0x00002,   /*!< invalid float number (NAN or INF) */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_VERSION_NO_MATCH = 0x00003,   /*!< waypoint mission version can't match with firmware*/
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_UNKNOWN = 0x0ffff,   /*!< Fatal error! Unexpected result! */

    /* The enum that defines the detail error code of flight-trajectory module */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_RESV = 0x10000,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_NUM_TOO_MANY = 0x10001,   /*!< min_initial_waypoint_num is large than permitted_max_waypoint_num */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_NUM_TOO_FEW = 0x10002,   /*!< min_initial_waypoint_num is less than permitted_min_waypoint_num */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_END_INDEX = 0x10003,   /*!< waypoint_end_index is equal or large than total_waypoint_num */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_START_ID_GT_END_ID = 0x10004,   /*!< the start index is greater than end index of upload wps */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_END_ID_GT_TOTAL_NUM = 0x10005,   /*!< the end index of uplod wps is greater than inited total numbers */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DOWNLOAD_WPS_NOT_IN_STORED_RAGNE = 0x10006,   /*!< the index of first and end waypoint expected to download is not in range of stored in FC */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_CUR_POS_IS_FAR_AWAY_FROM_FIRST_WP = 0x10008,   /*!< current position is far away from the first waypoint. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_ADJ_WPS_TOO_CLOSE = 0x1000a,   /*!< it is too close from two adjacent waypoints, the value of which might be distinguish from products */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_ADJ_WPS_TOO_FAR = 0x1000b,   /*!< the distance betwween two adjacent waypoints is not in[0.5m, 5000m] the value of which might be distinguish from products */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_MAX_VEL_GT_GLOBAL = 0x1000c,   /*!< the max vel of uplod wp is greater than global max vel */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_LOCAL_MAX = 0x1000d,   /*!< the local cruise vel of upload wp is greater than local max vel */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_GLOBAL_MAX = 0x1000e,   /*!< the local cruise vel of upload wp is greater than global max vel */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_GLOBAL_MAX_VEL = 0x1000f,   /*!< global_max_vel is greater than permitted_max_vel or less than permitted_min_vel */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GLOBAL_CRUISE_VEL_GT_MAX_VEL = 0x10010,   /*!< global_cruise_vel is greater than global_max_vel */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_GOTO_FIRST_FLAG = 0x10011,   /*!< goto_first_point_mode is out of range of waypoint_goto_first_flag_t_enum */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_FINISHED_ACTION = 0x10012,   /*!< finished_action is out of range of wp_plan_finish_action_t_enum */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_RC_LOST_ACTION = 0x10013,   /*!< rc_lost_action is out of range of wp_plan_rc_lost_action_t_enum */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_MODE_INVALID = 0x10014,   /*!< the yaw mode of upload wp is invalid. reference to waypoint2_yaw_mode_t defined in math_waypoint_planner.h */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_CMD_NOT_IN_RANGE = 0x10015,   /*!< the yaw command of upload wp is not in range. the range for MR:[-180 180],unit:degree */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_TURN_DIRECTION_INVALID = 0x10016,   /*!< the yaw turn direction of upload wp is invalid. it should be 0:clockwise or 1:anti-clockwise */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_WP_TYPE_INVALID = 0x10017,   /*!< the wp type of upload wp is invalid. reference to waypoint_type_t defined in math_waypoint_planner.h */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GO_STOP_CMD_INVALID = 0x10018,   /*!< go/stop command is invalid. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INVALID_PAUSE_RECOVERY_CMD = 0x10019,   /*!< the command of pause/recovery is not equal to any of the command enum */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INVALID_BREAK_RESTORE_CMD = 0x1001a,   /*!< the command of break/restore is not equal to any of the command enum */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_REF_POINT = 0x1001b,   /*!< initial reference point position coordinate exceed set range */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DAMPING_DIS_GE_DIS_OF_ADJ_POINTS = 0x1001c,    /*!< the damping dis is greater than or equal the distance of adjacent point */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_CANNT_SET_WP_LINE_EXIT_TYPE = 0x1001d,   /*!< cann't set wp_line_exit type to wp */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INFO_NOT_UPLOADED = 0x1001e,   /*!< the init info of Ground Station is not uploaded yet */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_HAS_NOT_UPLOADED = 0x1001f,   /*!< the wp has not uploaded yet */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOADED_WP_NOT_ENOUGH = 0x10020,   /*!< min_initial_waypoint_num is not uploaded. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_HAS_STARTED = 0x10021,   /*!< waypoint plan has started when received go command. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING = 0x10022,   /*!< waypoint plan not running when received stop command. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING_FOR_PAUSE_RECOVERY = 0x10023,   /*!< ground station(GS) is not started(used by pause/recovery) */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING_FOR_BREAK_RESTORE = 0x10024,   /*!< ground station(GS) is not started(used by break/restore) */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NOT_IN_WP_MIS = 0x10025,   /*!< not in the waypoint mission(MIS)(cannot pause/recovery or break/restore) */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_HAS_BEEN_PAUSED = 0x10026,   /*!< the current status is paused, cannot command pause again */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_NOT_PAUSED = 0x10027,   /*!< not in paused status, cannot command recovery */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_HAS_BEEN_BROKEN = 0x10028,   /*!< the current status is broken, cannot command break again */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_NOT_BROKEN = 0x10029,   /*!< not in break status, cannot command restore */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_PAUSE_RECOVERY_NOT_SUPPORTED = 0x1002a,   /*!< the configuration forbid using pause/recovery API */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_BREAK_RESTORE_NOT_SUPPORTED = 0x1002b,   /*!< the configuration forbid using break/restore API */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_BREAK_POINT = 0x1002c,   /*!< no break point is recorded for restore */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_CUR_TRAJ_PROJECT = 0x1002d,   /*!< no current trajectory project point is recorded for restore */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_NXT_TRAJ_PROJECT = 0x1002e,   /*!< no next trajectory project point is recorded for restore */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_NNT_TRAJ_PROJECT = 0x1002f,   /*!< no next the next trajectory project point is recorded for restore */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_WP_ID_NOT_CONTINUE = 0x10030,   /*!< the index of upload wp is not continue after the store wp */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_LINE_ENTER_NOT_SET_TO_START_WP = 0x10031,   /*!< the WP_LINE_ENTER wp_type set to a wp which is not the init start waypoint */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_WHEN_PLAN_HAS_STARTED = 0x10032,   /*!< the waypoint plan has started when initializing waypoint */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DAMPING_DIS_EXCEED_RANGE = 0x10033,   /*!< waypoint damping distance exceed set range */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WAYPOINT_COOR_EXCEED_RANGE = 0x10034,   /*!< waypoint position coordinate exceed rational range */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_FIRST_WP_TYPE_IS_WP_TURN_NO = 0x10035,   /*!< first waypoint type error, it can not be WP_TURN_NO */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_EXCEED_RADIUS_LIMIT = 0x10038,   /*!< waypoint position exceed radius limit */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_EXCEED_HEIGHT_LIMIT = 0x10039,   /*!< waypoint position exceed height limit */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOADED_WP_EXCEED_HEIGHT_LIMIT = 0x10040,   /*!< the uploaded waypoint exceed height limit */

    /* The enum that defines the detail error code of flight-status module */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_RESV = 0x20000,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_WP_MIS_CHECK_FAIL = 0x20001,   /*!< head_node is null or atti_not_healthy or gyro_not_healthy or horiz_vel_not healthy or horiz_abs_pos_not_healthy. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_HOME_NOT_RECORDED = 0x20002,   /*!< the home point is no recorded yet, which will be executed at the first time of GPS level > 3(MR/FW). */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_LOW_LOCATION_ACCURACY = 0x20003,   /*!< current location accuracy is low for bad GPS signal. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_RTK_CONDITION_IS_NOT_READY = 0x20005,   /*!< use rtk_data, but rtk is not connected or rtk_data is invalid */

    /* The enum that defines the detail error code of flight-secure module */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_RESV = 0x30000,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_CROSS_NFZ = 0x30001,   /*!< the trajectory cross the NFZ */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_BAT_LOW = 0x30002,   /*!< current capacity of smart battery or voltage of non-smart battery is lower than level 1 or level 2 threshold */

    /* The enum that defines the detail error code of action-common module */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_RESV = 0x400000,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_DUPLICATED = 0x400001,   /*!< the ID of Action is duplicated. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ITEMS_SPACE_NOT_ENOUGH = 0x400002,   /*!< there is no enough memory space for new Action Item */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_SIZE_GT_BUF_SIZE = 0x400003,   /*!< the size of buffer used to get the info of Action is less than the size of Action. Normally users can not get this. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_NOT_FOUND = 0x400004,   /*!< the ID of Action is not found. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_DOWNLOAD_ACTION_ID_RANGE_ERROR = 0x400005,   /*!< the download action start id is bigger than the action end id */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_NO_ACTION_ITEMS_STORED = 0x400006,   /*!< can not download or get min-max action ID for no action items stored in action kernel */

    /* The enum that defines the detail error code of trigger module */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_RESV = 0x410000,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_TYPE_INVALID = 0x410001,   /*!< the type ID of Trigger is invalid. It might not defined or the information is empty. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_END_INDEX_LT_START_INDEX = 0x410021,   /*!< wp_end_index is less than wp_start_index in reach_waypoint_trigger. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_INVALID_INTERVAL_WP_NUM = 0x410022,   /*!< interval_wp_num is large than the difference of wp_start_index and wp_end_index in reach_waypoint_trigger. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_INVALID_AUTO_TERMINATE_WP_NUM = 0x410023,   /*!< auto_terminate_wp_num is large than interval_wp_num in reach_waypoint_trigger. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_ASSOCIATE_INVALID_TYPE = 0x410041,   /*!< the associate_type is greater than the maximum value.  */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_SIMPLE_INTERVAL_INVALID_TYPE = 0x410081,   /*!< the interval type is greater than the maximum value. */

    /* The enum that defines the detail error code of actuator-common module */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_RESV = 0x420000,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_EXEC_NON_SUPPORTED = 0x420001,   /*!< the execution of Actuator is not supported, e.g., try to stop camera shooting. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_TYPE_INVALID = 0x420002,   /*!< the type ID of Actuator is invalid. It might not defined or the information is empty. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_FUNC_INVALID = 0x420003,   /*!< the Function ID of Actuator is invalid. It might not defined or the information is empty. */

    /* The enum that defines the detail error code of action-camera module */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_RESV = 0x430000,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_SINGLE_SHOT_CMD_TO_CAMERA_FAIL = 0x430001,   /*!< fail to send shot cmd to camera for no camera or camera is busy. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_VIDEO_START_CMD_TO_CAMERA_FAIL = 0x430002,    /*!< fail to send video start cmd to camera for no camera or camera is busy. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_VIDEO_STOP_CMD_TO_CAMERA_FAIL = 0x430003,    /*!< fail to send video stop cmd to camera for no camera or camera is not busy. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_FOCUS_PARAM_XY_INVALID = 0x430004,   /*!< camera focus param xy exceed valid range (0, 1). */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_FOCUS_CMD_TO_CAMERA_FAIL = 0x430005,   /*!< fail to send focus cmd to camera for no camera or camera is busy. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_FOCALIZE_CMD_TO_CAMERA_FAIL = 0x430006,   /*!< fail to send focalize cmd to camera for no camera or camera is busy. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_FOCAL_DISTANCE_INVALID = 0x430007,   /*!< focal distance of camera focalize function exceed valid range. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_EXEC_FAIL = 0x430100,   /*!< this err code indicate camera fail to exec coressponding cmd, and the low 8 bit
                                                                                  will be replaced by the return code from camera, for example: 0x01E0 means current cmd
                                                                                  is not supported, 0x01E8 means SD card is not inserted and so on, the detailed camera
                                                                                  return code could be found in camera protocal. */

    /* The enum that defines the detail error code of action-gimbal module */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_RESV = 0x440000,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_INVALID_RPY_ANGLE_CTRL_CMD = 0x440001,   /*!< gimbal roll/pitch/yaw angle ctrl cmd param invalid, unable to exec. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_INVALID_DURATION_CMD = 0x440002,   /*!< gimbal duration param invalid, unable to exec. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_FAIL_TO_ARRIVE_TGT_ANGLE = 0x440003,   /*!< gimbal fail to arrive target angle . */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_FAIL_TO_SEND_CMD_TO_GIMBAL = 0x440004,   /*!< fail to send cmd to gimbal for gimbal is busy or no gimbal. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_THIS_INDEX_OF_GIMBAL_NOT_DOING_UNIFORM_CTRL = 0x440005, /*!< fail to stop gimbal uniform ctrl because index error.*/

    /* The enum that defines the detail error code of action-flight module */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_RESV = 0x460000,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_YAW_INVALID_YAW_ANGLE = 0x460001,   /*!< yaw angle is lager max yaw angle. */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_YAW_TO_TGT_ANGLE_TIMEOUT = 0x460002,   /*!< faile to target yaw angle, because of timeout.*/
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_ACTION_YAW_OCCUPIED = 0x460003,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_CUR_AND_TGT_VEL_CLE_STATUE_EQUAL = 0x460004,

    /* The enum that defines the detail error code of payload module */
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_RESV = 0x470000,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_FAIL_TO_SEND_CMD_TO_PAYLOAD = 0x470001,
    GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_EXEC_FAILED = 0x470002,
} E_GduErrorWaypointV2ModuleRawCode;

//@formatter:off
/**
 * @brief GDU error code complete works. Users can search all error messages here.
 * @details Please get error description, error reason and error recovery suggestion of every error code from
 * ::GDU_ERROR_OBJECTS macro.
 */
enum GduErrorCode {
    /* system module error code, including some common error code */
    GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_SUCCESS),
    GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_REQUEST_PARAMETER = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_INVALID_REQUEST_PARAMETER),
    GDU_ERROR_SYSTEM_MODULE_CODE_EXECUTING_HIGHER_PRIORITY_TASK = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_EXECUTING_HIGHER_PRIORITY_TASK),
    GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_NONSUPPORT),
    GDU_ERROR_SYSTEM_MODULE_CODE_TIMEOUT = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_TIMEOUT),
    GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_MEMORY_ALLOC_FAILED),
    GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_INVALID_PARAMETER),
    GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_NONSUPPORT_IN_CURRENT_STATE),
    GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_SYSTEM_ERROR),
    GDU_ERROR_SYSTEM_MODULE_CODE_HARDWARE_ERR = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_HARDWARE_ERR),
    GDU_ERROR_SYSTEM_MODULE_CODE_INSUFFICIENT_ELECTRICITY = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_INSUFFICIENT_ELECTRICITY),
    GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_UNKNOWN),
    GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_NOT_FOUND),
    GDU_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_OUT_OF_RANGE),
    GDU_ERROR_SYSTEM_MODULE_CODE_BUSY = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_BUSY),
    GDU_ERROR_SYSTEM_MODULE_CODE_DUPLICATE = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_DUPLICATE),
    GDU_ERROR_SYSTEM_MODULE_CODE_ADAPTER_NOT_MATCH = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_ADAPTER_NOT_MATCH),

    /* gimbal module error code */
    GDU_ERROR_GIMBAL_MODULE_CODE_PITCH_REACH_POSITIVE_LIMIT = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL, GDU_ERROR_GIMBAL_MODULE_RAW_CODE_PITCH_REACH_POSITIVE_LIMIT),
    GDU_ERROR_GIMBAL_MODULE_CODE_PITCH_REACH_NEGATIVE_LIMIT = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL, GDU_ERROR_GIMBAL_MODULE_RAW_CODE_PITCH_REACH_NEGATIVE_LIMIT),
    GDU_ERROR_GIMBAL_MODULE_CODE_ROLL_REACH_POSITIVE_LIMIT = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL, GDU_ERROR_GIMBAL_MODULE_RAW_CODE_ROLL_REACH_POSITIVE_LIMIT),
    GDU_ERROR_GIMBAL_MODULE_CODE_ROLL_REACH_NEGATIVE_LIMIT = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL, GDU_ERROR_GIMBAL_MODULE_RAW_CODE_ROLL_REACH_NEGATIVE_LIMIT),
    GDU_ERROR_GIMBAL_MODULE_CODE_YAW_REACH_POSITIVE_LIMIT = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL, GDU_ERROR_GIMBAL_MODULE_RAW_CODE_YAW_REACH_POSITIVE_LIMIT),
    GDU_ERROR_GIMBAL_MODULE_CODE_YAW_REACH_NEGATIVE_LIMIT = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL, GDU_ERROR_GIMBAL_MODULE_RAW_CODE_YAW_REACH_NEGATIVE_LIMIT),
    GDU_ERROR_GIMBAL_MODULE_CODE_NON_CONTROL_AUTHORITY = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL, GDU_ERROR_GIMBAL_MODULE_RAW_CODE_NON_CONTROL_AUTHORITY),

    /* payload collaboration module error code */
    GDU_ERROR_PAYLOAD_COLLABORATION_MODULE_CODE_POSITION_NOT_MATCH = GDU_ERROR_CODE(GDU_ERROR_MODULE_PAYLOAD_COLLABORATION, GDU_ERROR_PAYLOAD_COLLABORATION_MODULE_RAW_CODE_POSITION_NOT_MATCH),

    /* activation error code of identity module */
    GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_PARAMETER_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_IDENTITY, GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_PARAMETER_ERROR),
    GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_ENCODE_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_IDENTITY, GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_ENCODE_ERROR),
    GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_NEW_DEVICE_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_IDENTITY, GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_NEW_DEVICE_ERROR),
    GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_SOFTWARE_NOT_CONNECTED = GDU_ERROR_CODE(GDU_ERROR_MODULE_IDENTITY, GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_SOFTWARE_NOT_CONNECTED),
    GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_NETWORK_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_IDENTITY, GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_NETWORK_ERROR),
    GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_SERVER_ACCESS_REFUSED = GDU_ERROR_CODE(GDU_ERROR_MODULE_IDENTITY, GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_SERVER_ACCESS_REFUSED),
    GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_ACCESS_LEVEL_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_IDENTITY, GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_ACCESS_LEVEL_ERROR),
    GDU_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_OSDK_VERSION_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_IDENTITY, GDU_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_OSDK_VERSION_ERROR),

    /* subscription module error code */
    GDU_ERROR_SUBSCRIPTION_MODULE_CODE_INVALID_TOPIC_FREQ = GDU_ERROR_CODE(GDU_ERROR_MODULE_SUBSCRIPTION, GDU_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_INVALID_TOPIC_FREQ),
    GDU_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_DUPLICATE = GDU_ERROR_CODE(GDU_ERROR_MODULE_SUBSCRIPTION, GDU_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_DUPLICATE),
    GDU_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_NOT_SUBSCRIBED = GDU_ERROR_CODE(GDU_ERROR_MODULE_SUBSCRIPTION, GDU_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_NOT_SUBSCRIBED),
    GDU_ERROR_SUBSCRIPTION_MODULE_CODE_TIMESTAMP_NOT_ENABLE = GDU_ERROR_CODE(GDU_ERROR_MODULE_SUBSCRIPTION, GDU_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TIMESTAMP_NOT_ENABLE),
    GDU_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_NOT_SUPPORTED = GDU_ERROR_CODE(GDU_ERROR_MODULE_SUBSCRIPTION, GDU_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_NOT_SUPPORTED),

    /* mop channel module error code */
    GDU_ERROR_MOP_CHANNEL_MODULE_CODE_CONNECTION_CLOSE = GDU_ERROR_CODE(GDU_ERROR_MODULE_MOP_CHANNEL, GDU_ERROR_MOP_CHANNEL_MODULE_RAW_CODE_CONNECTION_CLOSE),

    /* Flight controller basic errors */
    GDU_ERROR_FC_MODULE_CODE_RC_MODE_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_RC_MODE_ERROR),
    GDU_ERROR_FC_MODULE_CODE_RELEASE_CONTROL_SUCCESS= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_RELEASE_CONTROL_SUCCESS),
    GDU_ERROR_FC_MODULE_CODE_OBTAIN_CONTROL_SUCCESS= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_OBTAIN_CONTROL_SUCCESS),
    GDU_ERROR_FC_MODULE_CODE_OBTAIN_CONTROL_IN_PROGRESS= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_OBTAIN_CONTROL_IN_PROGRESS),
    GDU_ERROR_FC_MODULE_CODE_RELEASE_CONTROL_IN_PROGRESS= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_RELEASE_CONTROL_IN_PROGRESS),
    GDU_ERROR_FC_MODULE_CODE_RC_NEED_MODE_P= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_RC_NEED_MODE_P),
    GDU_ERROR_FC_MODULE_CODE_RC_NEED_MODE_F= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_RC_NEED_MODE_F),
    GDU_ERROR_FC_MODULE_CODE_PARAM_READ_WRITE_INVALID_PARAM= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_PARAM_READ_WRITE_INVALID_PARAM),
    GDU_ERROR_FC_MODULE_CODE_IOC_OBTAIN_CONTROL_ERROR= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_IOC_OBTAIN_CONTROL_ERROR),
    GDU_ERROR_FC_MODULE_CODE_KEY_ERROR= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_KEY_ERROR),
    GDU_ERROR_FC_MODULE_CODE_NO_AUTHORIZATION_ERROR= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_NO_AUTHORIZATION_ERROR),
    GDU_ERROR_FC_MODULE_CODE_NO_RIGHTS_ERROR= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_NO_RIGHTS_ERROR),
    GDU_ERROR_FC_MODULE_CODE_SYSTEM_ERROR= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_BASIC, GDU_ERROR_FC_MODULE_RAW_CODE_SYSTEM_ERROR),

    /* Flight controller joystick errors */
    GDU_ERROR_FC_JOYSTICK_MODULE_OBTAIN_RELEASE_JOYSTICK_AUTH_SUCCESS = GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_JOYSTICK, GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_SUCCESS),
    GDU_ERROR_FC_JOYSTICK_MODULE_DEVICE_NOT_ALLOW= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_JOYSTICK, GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_DEVICE_NOT_ALLOW),
    GDU_ERROR_FC_JOYSTICK_MODULE_TAKING_OFF= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_JOYSTICK, GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_TAKING_OFF),
    GDU_ERROR_FC_JOYSTICK_MODULE_LANDING= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_JOYSTICK, GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_LANDING),
    GDU_ERROR_FC_JOYSTICK_MODULE_CMD_INVALID = GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_JOYSTICK, GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_CMD_INVALID),
    GDU_ERROR_FC_JOYSTICK_MODULE_RC_NOT_P_MODE = GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_JOYSTICK, GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_RC_NOT_P_MODE),
    GDU_ERROR_FC_JOYSTICK_MODULE_CMD_LENGTH_ERROR= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_JOYSTICK, GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_CMD_LENGTH_ERROR),
    GDU_ERROR_FC_JOYSTICK_MODULE_HAS_NO_JOYSTICK_AUTHORITY= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_JOYSTICK, GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_HAS_NO_JOYSTICK_AUTHORITY),
    GDU_ERROR_FC_JOYSTICK_MODULE_IN_RC_LOST_ACTION= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_JOYSTICK, GDU_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_IN_RC_LOST_ACTION),

    /* Flight controller action errors */
    GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_MOTOR_ON= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ACTION, GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_MOTOR_ON),
    GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_MOTOR_OFF= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ACTION, GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_MOTOR_OFF),
    GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_IN_AIR= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ACTION, GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_IN_AIR),
    GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_NOT_IN_AIR= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ACTION, GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_NOT_IN_AIR),
    GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_HOME_POINT_NOT_SET= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ACTION, GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_HOME_POINT_NOT_SET),
    GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_BAD_GPS= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ACTION, GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_BAD_GPS),
    GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_IN_SIMULATION= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ACTION, GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_IN_SIMULATION),
    GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_CANNOT_START_MOTOR= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ACTION, GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_CANNOT_START_MOTOR),
    GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_LOW_VOLTAGE= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ACTION, GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_LOW_VOLTAGE),
    GDU_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_SPEED_TOO_LARGE= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ACTION, GDU_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_SPEED_TOO_LARGE),

    /* Flight controller arrest flying errors */
    GDU_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_LOGOUT_SET_SUCCESS = GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ARREST_FLYING, GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_LOGOUT_SET_SUCCESS),
    GDU_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_LOGOUT_NULL_POINTER= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ARREST_FLYING, GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_LOGOUT_NULL_POINTER),
    GDU_ERROR_FC_ARREST_FLYING_MODULE_HMS_CODE_HAS_REGISTED= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ARREST_FLYING, GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMS_CODE_HAS_REGISTED),
    GDU_ERROR_FC_ARREST_FLYING_MODULE_HMS_CODE_NOT_FIND = GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ARREST_FLYING, GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMS_CODE_NOT_FIND),
    GDU_ERROR_FC_ARREST_FLYING_MODULE_ADD_ITEM_NO_DECRIPTION = GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ARREST_FLYING, GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_ADD_ITEM_NO_DECRIPTION),
    GDU_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_ID_INVALID= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ARREST_FLYING, GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_ID_INVALID),
    GDU_ERROR_FC_ARREST_FLYING_MODULE_STOP_IN_AIR_HMSCODE_NOT_IN_WHITE_TABLE= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ARREST_FLYING, GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_STOP_IN_AIR_HMSCODE_NOT_IN_WHITE_TABLE),
    GDU_ERROR_FC_ARREST_FLYING_MODULE_INVALID_FORMAT_HMSCODE= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ARREST_FLYING, GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_INVALID_FORMAT_HMSCODE),
    GDU_ERROR_FC_ARREST_FLYING_MODULE_HMSCODE_NOT_IN_WHITE_TABLE= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_ARREST_FLYING, GDU_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMSCODE_NOT_IN_WHITE_TABLE),

    /* Flight controller home location errors */
    GDU_ERROR_FC_HOME_LOCATION_MODULE_UNKNOWN_FAILED_REASON= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_HOME_LOCATION, GDU_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_UNKNOWN_FAILED_REASON),
    GDU_ERROR_FC_HOME_LOCATION_MODULE_INVALID_GPS_COORDINATE= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_HOME_LOCATION, GDU_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_INVALID_GPS_COORDINATE),
    GDU_ERROR_FC_HOME_LOCATION_MODULE_NOT_BE_RECORD = GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_HOME_LOCATION, GDU_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_NOT_BE_RECORD),
    GDU_ERROR_FC_HOME_LOCATION_MODULE_GPS_NOT_READY = GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_HOME_LOCATION, GDU_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_GPS_NOT_READY),
    GDU_ERROR_FC_HOME_LOCATION_MODULE_DIS_TOO_FAR = GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_HOME_LOCATION, GDU_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_DIS_TOO_FAR),

    /* Flight controller emergency stop motor errors */
    GDU_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_VERSION_NOT_MATCH= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_EMERGENCY_STOP_MOTOR, GDU_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_RAW_CODE_VERSION_NOT_MATCH),
    GDU_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_CMD_INVALID= GDU_ERROR_CODE(GDU_ERROR_MODULE_FC_EMERGENCY_STOP_MOTOR, GDU_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_RAW_CODE_CMD_INVALID),

    /* Camera manager errors */
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_TIMEOUT = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_TIMEOUT ),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_RAM_ALLOCATION_FAILED = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_RAM_ALLOCATION_FAILED),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_INVALID_COMMAND_PARAMETER = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_INVALID_COMMAND_PARAMETER),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_CAMERA_TIME_NOT_SYNCHRONIZED = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_CAMERA_TIME_NOT_SYNCHRONIZED),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_SET_FAILED = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_SET_FAILED),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_GET_FAILED = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_GET_FAILED),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_MISSING = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_MISSING),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_FULL = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_FULL),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_ERROR),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_SENSOR_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SENSOR_ERROR),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_SYSTEM_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SYSTEM_ERROR),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_TOTAL_TOO_LONG = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_TOTAL_TOO_LONG),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_MODULE_INACTIVATED = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_MODULE_INACTIVATED),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_VERIFICATION_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_VERIFICATION_ERROR),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_FLASH_WRITE_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FLASH_WRITE_ERROR),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_TYPE_MISMATCH = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_TYPE_MISMATCH),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_REMOTE_CONTROL_UNCONNECTED = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_REMOTE_CONTROL_UNCONNECTED),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_HARDWARE_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_HARDWARE_ERROR),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_AIRCRAFT_UNCONNECTED = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_AIRCRAFT_UNCONNECTED),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_CANNOT_UPGRADE_IN_CUR_STATE = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_CANNOT_UPGRADE_IN_CUR_STATE),
    GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNDEFINE_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_CAMERA_MANAGER, GDU_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNDEFINE_ERROR),

    /* Gimbal manager errors */
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_TIMEOUT = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_TIMEOUT ),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_RAM_ALLOCATION_FAILED = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_RAM_ALLOCATION_FAILED),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_INVALID_COMMAND_PARAMETER = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_INVALID_COMMAND_PARAMETER),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_CAMERA_TIME_NOT_SYNCHRONIZED = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_CAMERA_TIME_NOT_SYNCHRONIZED),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_SET_FAILED = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_SET_FAILED),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_GET_FAILED = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_GET_FAILED),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_MISSING = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_MISSING),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_FULL = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_FULL),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_ERROR),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_SENSOR_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SENSOR_ERROR),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_SYSTEM_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SYSTEM_ERROR),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_TOTAL_TOO_LONG = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_TOTAL_TOO_LONG),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_MODULE_INACTIVATED = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_MODULE_INACTIVATED),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_VERIFICATION_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_VERIFICATION_ERROR),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_FLASH_WRITE_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FLASH_WRITE_ERROR),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_TYPE_MISMATCH = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_TYPE_MISMATCH),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_REMOTE_CONTROL_UNCONNECTED = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_REMOTE_CONTROL_UNCONNECTED),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_HARDWARE_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_HARDWARE_ERROR),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_AIRCRAFT_UNCONNECTED = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_AIRCRAFT_UNCONNECTED),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_CANNOT_UPGRADE_IN_CUR_STATE = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_CANNOT_UPGRADE_IN_CUR_STATE),
    GDU_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNDEFINE_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_GIMBAL_MANAGER, GDU_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNDEFINE_ERROR),

	/* Waypoint v2 total errors */
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_SUCCESS = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_SUCCESS),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_INVALID_DATA_LENGTH = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_INVALID_DATA_LENGTH),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_INVALD_FLOAT_NUM = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_INVALD_FLOAT_NUM),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_VERSION_NO_MATCH = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_VERSION_NO_MATCH),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_UNKNOWN = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_UNKNOWN),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_RESV = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_RESV),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_NUM_TOO_MANY = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_NUM_TOO_MANY),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_NUM_TOO_FEW = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_NUM_TOO_FEW  ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_END_INDEX = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_END_INDEX ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_START_ID_GT_END_ID = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_START_ID_GT_END_ID),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_END_ID_GT_TOTAL_NUM = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_END_ID_GT_TOTAL_NUM),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DOWNLOAD_WPS_NOT_IN_STORED_RAGNE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DOWNLOAD_WPS_NOT_IN_STORED_RAGNE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_CUR_POS_IS_FAR_AWAY_FROM_FIRST_WP = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_CUR_POS_IS_FAR_AWAY_FROM_FIRST_WP),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_ADJ_WPS_TOO_CLOSE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_ADJ_WPS_TOO_CLOSE ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_ADJ_WPS_TOO_FAR = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_ADJ_WPS_TOO_FAR  ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_MAX_VEL_GT_GLOBAL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_MAX_VEL_GT_GLOBAL ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_LOCAL_MAX = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_LOCAL_MAX),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_GLOBAL_MAX = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_GLOBAL_MAX),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_GLOBAL_MAX_VEL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_GLOBAL_MAX_VEL  ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GLOBAL_CRUISE_VEL_GT_MAX_VEL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GLOBAL_CRUISE_VEL_GT_MAX_VEL ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_GOTO_FIRST_FLAG = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_GOTO_FIRST_FLAG ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_FINISHED_ACTION = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_FINISHED_ACTION ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_RC_LOST_ACTION = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_RC_LOST_ACTION),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_MODE_INVALID = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_MODE_INVALID),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_CMD_NOT_IN_RANGE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_CMD_NOT_IN_RANGE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_TURN_DIRECTION_INVALID = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_TURN_DIRECTION_INVALID),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_WP_TYPE_INVALID = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_WP_TYPE_INVALID),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GO_STOP_CMD_INVALID = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GO_STOP_CMD_INVALID),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INVALID_PAUSE_RECOVERY_CMD = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INVALID_PAUSE_RECOVERY_CMD),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INVALID_BREAK_RESTORE_CMD = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INVALID_BREAK_RESTORE_CMD),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_REF_POINT = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_REF_POINT),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DAMPING_DIS_GE_DIS_OF_ADJ_POINTS = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DAMPING_DIS_GE_DIS_OF_ADJ_POINTS),
    GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_CANNT_SET_WP_LINE_EXIT_TYPE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_CANNT_SET_WP_LINE_EXIT_TYPE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INFO_NOT_UPLOADED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INFO_NOT_UPLOADED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_HAS_NOT_UPLOADED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_HAS_NOT_UPLOADED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOADED_WP_NOT_ENOUGH = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOADED_WP_NOT_ENOUGH),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_HAS_STARTED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_HAS_STARTED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING_FOR_PAUSE_RECOVERY = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING_FOR_PAUSE_RECOVERY),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING_FOR_BREAK_RESTORE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING_FOR_BREAK_RESTORE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NOT_IN_WP_MIS = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NOT_IN_WP_MIS ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_HAS_BEEN_PAUSED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_HAS_BEEN_PAUSED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_NOT_PAUSED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_NOT_PAUSED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_HAS_BEEN_BROKEN = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_HAS_BEEN_BROKEN),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_NOT_BROKEN = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_NOT_BROKEN),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_PAUSE_RECOVERY_NOT_SUPPORTED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_PAUSE_RECOVERY_NOT_SUPPORTED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_BREAK_RESTORE_NOT_SUPPORTED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_BREAK_RESTORE_NOT_SUPPORTED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_BREAK_POINT = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_BREAK_POINT ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_CUR_TRAJ_PROJECT = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_CUR_TRAJ_PROJECT),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_NXT_TRAJ_PROJECT = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_NXT_TRAJ_PROJECT),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_NNT_TRAJ_PROJECT = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_NNT_TRAJ_PROJECT),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_WP_ID_NOT_CONTINUE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_WP_ID_NOT_CONTINUE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_LINE_ENTER_NOT_SET_TO_START_WP = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_LINE_ENTER_NOT_SET_TO_START_WP ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_WHEN_PLAN_HAS_STARTED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_WHEN_PLAN_HAS_STARTED ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DAMPING_DIS_EXCEED_RANGE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DAMPING_DIS_EXCEED_RANGE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WAYPOINT_COOR_EXCEED_RANGE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WAYPOINT_COOR_EXCEED_RANGE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_FIRST_WP_TYPE_IS_WP_TURN_NO = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_FIRST_WP_TYPE_IS_WP_TURN_NO),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_EXCEED_RADIUS_LIMIT = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_EXCEED_RADIUS_LIMIT ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_EXCEED_HEIGHT_LIMIT = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_EXCEED_HEIGHT_LIMIT ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_RESV = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_RESV),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_WP_MIS_CHECK_FAIL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_WP_MIS_CHECK_FAIL),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_HOME_NOT_RECORDED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_HOME_NOT_RECORDED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_LOW_LOCATION_ACCURACY = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_LOW_LOCATION_ACCURACY),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_RTK_CONDITION_IS_NOT_READY = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_RTK_CONDITION_IS_NOT_READY),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_RESV = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_RESV),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_CROSS_NFZ = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_CROSS_NFZ),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_BAT_LOW = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_BAT_LOW),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_RESV = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_RESV),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ID_DUPLICATED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_DUPLICATED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ITEMS_SPACE_NOT_ENOUGH = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ITEMS_SPACE_NOT_ENOUGH),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_SIZE_GT_BUF_SIZE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_SIZE_GT_BUF_SIZE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ID_NOT_FOUND = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_NOT_FOUND),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_DOWNLOAD_ACTION_ID_RANGE_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_DOWNLOAD_ACTION_ID_RANGE_ERROR),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_NO_ACTION_ITEMS_STORED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_NO_ACTION_ITEMS_STORED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_RESV = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_RESV),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_TYPE_INVALID = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_TYPE_INVALID),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_END_INDEX_LT_START_INDEX = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_END_INDEX_LT_START_INDEX),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_INVALID_INTERVAL_WP_NUM = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_INVALID_INTERVAL_WP_NUM),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_INVALID_AUTO_TERMINATE_WP_NUM = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_INVALID_AUTO_TERMINATE_WP_NUM),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_ASSOCIATE_INVALID_TYPE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_ASSOCIATE_INVALID_TYPE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_SIMPLE_INTERVAL_INVALID_TYPE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_SIMPLE_INTERVAL_INVALID_TYPE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_RESV = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_RESV),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_EXEC_NON_SUPPORTED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_EXEC_NON_SUPPORTED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_TYPE_INVALID = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_TYPE_INVALID),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_FUNC_INVALID = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_FUNC_INVALID),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_RESV = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_RESV),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_SINGLE_SHOT_CMD_TO_CAMERA_FAIL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_SINGLE_SHOT_CMD_TO_CAMERA_FAIL),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_VIDEO_START_CMD_TO_CAMERA_FAIL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_VIDEO_START_CMD_TO_CAMERA_FAIL),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_VIDEO_STOP_CMD_TO_CAMERA_FAIL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_VIDEO_STOP_CMD_TO_CAMERA_FAIL),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_FOCUS_PARAM_XY_INVALID = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_FOCUS_PARAM_XY_INVALID),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_FOCUS_CMD_TO_CAMERA_FAIL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_FOCUS_CMD_TO_CAMERA_FAIL),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_FOCALIZE_CMD_TO_CAMERA_FAIL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_FOCALIZE_CMD_TO_CAMERA_FAIL),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_FOCAL_DISTANCE_INVALID = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_FOCAL_DISTANCE_INVALID),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_EXEC_FAIL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_EXEC_FAIL),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_RESV = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_RESV),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_INVALID_RPY_ANGLE_CTRL_CMD = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_INVALID_RPY_ANGLE_CTRL_CMD),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_INVALID_DURATION_CMD = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_INVALID_DURATION_CMD),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_FAIL_TO_ARRIVE_TGT_ANGLE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_FAIL_TO_ARRIVE_TGT_ANGLE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_FAIL_TO_SEND_CMD_TO_GIMBAL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_FAIL_TO_SEND_CMD_TO_GIMBAL),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_THIS_INDEX_OF_GIMBAL_NOT_DOING_UNIFORM_CTRL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_THIS_INDEX_OF_GIMBAL_NOT_DOING_UNIFORM_CTRL),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_RESV = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_RESV),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_YAW_INVALID_YAW_ANGLE = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_YAW_INVALID_YAW_ANGLE),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_YAW_TO_TGT_ANGLE_TIMEOUT = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_YAW_TO_TGT_ANGLE_TIMEOUT),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_ACTION_YAW_OCCUPIED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_ACTION_YAW_OCCUPIED),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_CUR_AND_TGT_VEL_CLE_STATUE_EQUAL = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_CUR_AND_TGT_VEL_CLE_STATUE_EQUAL),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_PAYLOAD_RESV = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_RESV),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_PAYLOAD_FAIL_TO_SEND_CMD_TO_PAYLOAD = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_FAIL_TO_SEND_CMD_TO_PAYLOAD),
	GDU_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_PAYLOAD_EXEC_FAILED = GDU_ERROR_CODE(GDU_ERROR_MODULE_WAYPOINT_V2, GDU_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_EXEC_FAILED),
};
//@formatter:on

/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif // GDU_ERROR_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
