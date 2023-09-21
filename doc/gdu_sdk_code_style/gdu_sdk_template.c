/**
 ********************************************************************
 * @file    gdu_sdk_template.c
 * @brief
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

/* Includes ------------------------------------------------------------------*/
#include "gdu_sdk_template.h"

/* Private constants ---------------------------------------------------------*/
#define MAX_PACKAGE_LEN  256

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/
static uint8_t s_SendBuffer[GDU_TEMPLATE_MAX_BUFFER_SIZE];

/* Private functions declaration ---------------------------------------------*/
static int GduTemplate_PrivateSend(const uint8_t *buffer, uint16_t len);

/* Exported functions definition ---------------------------------------------*/
/**
 * @brief GDU protocol initialization.
 * @param None.
 * @return None.
 */
void GduSdkTemplate_Init(void)
{
    /* NOTE : This function should be implemented in the user file. */
}

/**
 * @brief Send data function.
 * @param buffer : the buffer point send data
 * @param len : send data length
 * @return if >= 0, indicate the real send data length; if < 0, indicate the -ErrorCode for send failure reason.
 */
int GduSdkTemplate_Send(const uint8_t *buffer, uint16_t len)
{
    uint16_t realLen;

    if (len > MAX_PACKAGE_LEN) {
        realLen = MAX_PACKAGE_LEN;
    } else {
        realLen = len;
    }

    GduTemplate_PrivateSend(buffer, len);

    return realLen;
}

/* Private functions definition-----------------------------------------------*/
/**
 * @brief GDU low level send function.
 * @param buffer Pointer to send data buffer.
 * @param len Size of data to be sent.
 * @return Size of data to be sent.
 */
static int GduTemplate_PrivateSend(const uint8_t *buffer, uint16_t len)
{
    /* NOTE : This function should be implemented in the user file. */
    return len;
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
