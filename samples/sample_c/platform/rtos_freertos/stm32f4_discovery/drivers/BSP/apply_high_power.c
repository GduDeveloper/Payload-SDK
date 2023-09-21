/**
 ********************************************************************
 * @file    apply_high_power.c
 * @version V2.0.0
 * @date    2019/9/20
 * @brief
 *
 * @copyright (c) 2021 gdu. All rights reserved.
 *
 * All information contained herein is, and remains, the property of gdu.
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

/* Includes ------------------------------------------------------------------*/
#include "apply_high_power.h"
#include "stm32f4xx_hal.h"

/* Private constants ---------------------------------------------------------*/
#define HIGH_POWER_APPLY_PORT     GPIOD
#define HIGH_POWER_APPLY_PIN      GPIO_PIN_1

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode GduTest_HighPowerApplyPinInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOD clock */
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* Configure pin as input floating */
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    GPIO_InitStructure.Pin = HIGH_POWER_APPLY_PIN;
    HAL_GPIO_Init(HIGH_POWER_APPLY_PORT, &GPIO_InitStructure);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_WriteHighPowerApplyPin(E_GduPowerManagementPinState pinState)
{
    GPIO_PinState state;

    switch (pinState) {
        case GDU_POWER_MANAGEMENT_PIN_STATE_RESET:
            state = GPIO_PIN_RESET;
            break;
        case GDU_POWER_MANAGEMENT_PIN_STATE_SET:
            state = GPIO_PIN_SET;
            break;
        default:
            USER_LOG_ERROR("pin state unknown: %d.", pinState);
            return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    HAL_GPIO_WritePin(HIGH_POWER_APPLY_PORT, HIGH_POWER_APPLY_PIN, state);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
