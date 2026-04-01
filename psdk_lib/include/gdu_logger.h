/**
 ********************************************************************
 * @file    gdu_logger.h
 * @brief   This is the header file for "gdu_logger.c", defining the structure and
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
#ifndef GDU_LOGGER_H
#define GDU_LOGGER_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/**
* @brief The console method that needs to be registered.
* @note  Before registering the console method, you need to test the methods that need to be registered to ensure
*        that they can be used normally.
*/
typedef T_GduReturnCode (*ConsoleFunc)(const uint8_t *data, uint16_t dataLen);

/**
 * @brief Logger console level.
 */
typedef enum {
    GDU_LOGGER_CONSOLE_LOG_LEVEL_ERROR = 0, /*!< Logger console error level. The method and level of the console are
                                                  associated with each other. If the level of the registered console
                                                  method is lower than this level, the level interface will not be
                                                  printed successfully. */
    GDU_LOGGER_CONSOLE_LOG_LEVEL_WARN = 1, /*!< Logger console warning level.The method and level of the console are
                                                    associated with each other. If the level of the registered console
                                                    method is lower than this level, the level interface will not be
                                                    printed successfully. */
    GDU_LOGGER_CONSOLE_LOG_LEVEL_INFO = 2, /*!< Logger console info level. The method and level of the console are
                                                 associated with each other. If the level of the registered console
                                                 method is lower than this level, the level interface will not be
                                                 printed successfully. */
    GDU_LOGGER_CONSOLE_LOG_LEVEL_DEBUG = 3, /*!< Logger console debug level. The method and level of the console are
                                                  associated with each other. If the level of the registered console
                                                  method is lower than this level, the level interface will not be
                                                  printed successfully. */
} E_GduLoggerConsoleLogLevel;

/**
 * @brief Logger console content.
 */
typedef struct {
    ConsoleFunc func;
    uint8_t consoleLevel;
    bool isSupportColor;
} T_GduLoggerConsole;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Add the console function and level for Onboard SDK.
 * @note When registering the console, you need to provide the method of the console and the level corresponding to
 * the method. Log levels from high to low are Debug, Info, Warn, and Error, the log function module can print all
 * logs not higher than the specified level. Maximum support for registering eight different console methods at the
 * same time.Before registering the console method, you should test the registration method ensure that all the method
 * are normally. If you registered multiple methods at the same time, all the methods will be print.
 * @param console: pointer to the console function.
 * @return Execution result.
 */
T_GduReturnCode GduLogger_AddConsole(T_GduLoggerConsole *console);

/**
 * @brief Print out the selected level log of the specified format by the registration method.
 * @note The registered method is printed according to the corresponding level. If the level of the console is lower
 * than the level at which the log needs to be printed, it will not be printed successfully.
 * @param fmt: pointer to the format string that needs print out.
 * @param ...: Variable parameters, consistent with the use of the system interface print out.
 */
//void GduLogger_UserLogOutput(E_GduLoggerConsoleLogLevel level, const char *fmt, ...);

void GduLogger_LogOutput(E_GduLoggerConsoleLogLevel level,const char *type_name, const char *fmt, ...);
/* Exported constants --------------------------------------------------------*/
#define USER_LOG_DEBUG(fmt, ...)    \
        GduLogger_LogOutput(GDU_LOGGER_CONSOLE_LOG_LEVEL_DEBUG,"user", "[%s:%d) " fmt, __FUNCTION__, __LINE__ , ##__VA_ARGS__)
#define USER_LOG_INFO(fmt, ...)     \
        GduLogger_LogOutput(GDU_LOGGER_CONSOLE_LOG_LEVEL_INFO,"user", "[%s:%d) " fmt, __FUNCTION__, __LINE__ , ##__VA_ARGS__)
#define USER_LOG_WARN(fmt, ...)     \
        GduLogger_LogOutput(GDU_LOGGER_CONSOLE_LOG_LEVEL_WARN,"user", "[%s:%d) " fmt, __FUNCTION__, __LINE__ , ##__VA_ARGS__)
#define USER_LOG_ERROR(fmt, ...)    \
        GduLogger_LogOutput(GDU_LOGGER_CONSOLE_LOG_LEVEL_ERROR,"user", "[%s:%d) " fmt, __FUNCTION__, __LINE__ , ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // GDU_LOGGER_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
