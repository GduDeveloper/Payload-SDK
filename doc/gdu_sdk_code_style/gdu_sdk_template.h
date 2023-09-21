/**
 ********************************************************************
 * @file    gdu_sdk_template.h
 * @brief   This is the header file for "gdu_sdk_template.c", defining the structure and
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
#ifndef GDU_SDK_TEMPLATE_H
#define GDU_SDK_TEMPLATE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define GDU_TEMPLATE_MAX_BUFFER_SIZE      256

/**
 * @defgroup psdk_some_flag
 * @{
 */
#define GDU_TEMPLATE_SUCCESS               (0)
#define GDU_TEMPLATE_FAILURE               (-1)

#define GDU_SOME_FLAG_AAA                   1    /*!< Define line comment. */
#define GDU_SOME_FLAG_BBB                   2    /*!< Define line comment,
                                                          if comment is too long, split line. */
#define GDU_SOME_FLAG_CCC                   3

#define IS_GDU_SOME_FLAG(FLAG)       (((FLAG) == GDU_SOME_FLAG_AAA) || \
                                     ((FLAG) == GDU_SOME_FLAG_BBB) || \
                                     ((FLAG) == GDU_SOME_FLAG_CCC))

/**
 * @}
 */
#define GDU_TEMPLATE_MIN(a, b) (((a)<(b))?(a):(b))

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Enum brief comment.
 */
typedef enum {
    TEMPLATE_COLOR_RED = 0,
    TEMPLATE_COLOR_GREED = 1,
    TEMPLATE_COLOR_BLUE = 2,
} E_TemplateColor;

/**
  * @brief Struct brief comment.
  */
typedef struct {
    uint8_t structMember1;        /*!< structure member comment. */
    uint8_t longStructMember2;    /*!< structure member comment. */

    uint8_t someFlag;             /*!< if this value use marco define ,must use defgroup to indicate, and comment here
                                        use ref. for example:
                                        someFlag of GDU template. This parameter can be a value of @ref psdk_some_flag */
} T_StructDefine;

typedef void (*GduTemplateFunc)(void);

typedef int Inttype;

/* Use pack(1) to transfer structure. */
#pragma pack(1)

typedef struct {
    uint8_t structMember1;
    uint32_t structMember2;
} T_StructProtocolCommunication;

#pragma pack()

/* Exported functions --------------------------------------------------------*/
void GduSdkTemplate_Init(void);
int GduSdkTemplate_Send(const uint8_t *buffer, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif // GDU_SDK_TEMPLATE_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
