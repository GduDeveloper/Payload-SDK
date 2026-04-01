/**
 ********************************************************************
 * @file    gdu_ppk.h
 * @brief   This is the header file for "gdu_ppk.c", defining the structure and
 * (exported) function prototypes.
 *
 * @copyright (c) 2022 GDU. All rights reserved.
 *
 *********************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GDU_PPK_H
#define GDU_PPK_H

/* Includes ------------------------------------------------------------------*/
#include <gdu_typedef.h>
#include "gdu_time_sync.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
/*! The maximum length of a message that can be displayed by the mobile app floating window */
#define GDU_WIDGET_FLOATING_WINDOW_MSG_MAX_LEN         255

/* Exported types ------------------------------------------------------------*/

	/**
 * @brief Pin state.
 */
typedef enum {
    GDU_PPK_EVENT_PIN_STATE_RESET = 0, /*!< Specifies pin is in low level state. */
    GDU_PPK_EVENT_PIN_STATE_SET = 1, /*!< Specifies pin is in high level state. */
} E_GduPPKEventPinState;


typedef enum{
	PPK_TO_PSDK = 1, //ppk数据发给PSDK
	PPK_TO_AIRCRAFT, //ppk数据发个飞机
	PPK_TO_PSDK_AIRCRAFT,//ppk 数据发给psdk和飞机
}E_GduPPK_Switch;


/**
 * @brief ppk handler.
 */
typedef struct {
    /**
     * @brief 
     * @param 
     * @param 
     * @param 
     * @param 
     * @return 
     */
    E_GduPPK_Switch SwitchType;
    T_GduReturnCode (*EventPinInit)(void);
    T_GduReturnCode (*EventTrigger)(E_GduPPKEventPinState pinState);
} T_GduPPKHandlerList;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise ppk module, and user should call this function before using ppk features.
 * @return Execution result.
 */
T_GduReturnCode GduPPK_Init(void);

/**
 * @brief 
 * @param 
 * @param 
 * @return Execution result.
 */
T_GduReturnCode GduPPK_RegHandlerList(const T_GduPPKHandlerList *PPKHandlerList);
T_GduReturnCode GduPPK_GetRawData(uint8_t *buf, uint16_t *length);
T_GduReturnCode GduPPK_GetQianXunEventData(
		uint16_t cameraLens_id,
		uint16_t exposure_id, 
		T_GduTimeSyncAircraftTime *aircraftTime,
		char *out_event_str);

T_GduReturnCode GduPPK_GetRawDataCount(uint32_t *rawDataCount);


#ifdef __cplusplus
}
#endif

#endif // GDU_WIDGET_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
