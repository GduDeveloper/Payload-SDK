/**
 ********************************************************************
 * @file    gdu_platform.h
 * @brief   This is the header file for "gdu_platform.c", defining the structure and
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
#ifndef GDU_PLATFORM_H
#define GDU_PLATFORM_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
* @brief Platform handle of uart operation.
*/
typedef void *T_GduUartHandle;

/**
* @brief Platform handle of usb bulk operation.
*/
typedef void *T_GduUsbBulkHandle;

/**
* @brief Platform handle of network operation.
*/
typedef void *T_GduNetworkHandle;

/**
* @brief Platform handle of thread task operation.
*/
typedef void *T_GduTaskHandle;

/**
* @brief Platform handle of mutex operation.
*/
typedef void *T_GduMutexHandle;

/**
* @brief Platform handle of semaphore operation.
*/
typedef void *T_GduSemaHandle;

/**
* @brief Platform handle of file operation.
*/
typedef void *T_GduFileHandle;

/**
* @brief Platform handle of dir operation.
*/
typedef void *T_GduDirHandle;

/**
* @brief Platform handle of socket operation.
*/
typedef void *T_GduSocketHandle;

typedef enum {
    GDU_HAL_UART_NUM_0,
    GDU_HAL_UART_NUM_1,
} E_GduHalUartNum;

typedef enum {
    GDU_SOCKET_MODE_UDP,
    GDU_SOCKET_MODE_TCP,
} E_GduSocketMode;

typedef struct {
    bool isConnect;
} T_GduUartStatus;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} T_GduTime;

typedef struct {
    uint32_t size;
    T_GduTime createTime;
    T_GduTime modifyTime;
    char path[GDU_FILE_PATH_SIZE_MAX];
    bool isDir;
} T_GduFileInfo;

typedef struct {
    T_GduReturnCode (*UartInit)(E_GduHalUartNum uartNum, uint32_t baudRate, T_GduUartHandle *uartHandle);

    T_GduReturnCode (*UartDeInit)(T_GduUartHandle uartHandle);

    T_GduReturnCode (*UartWriteData)(T_GduUartHandle uartHandle, const uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_GduReturnCode (*UartReadData)(T_GduUartHandle uartHandle, uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_GduReturnCode (*UartGetStatus)(E_GduHalUartNum uartNum, T_GduUartStatus *status);
} T_GduHalUartHandler;

typedef struct {
    uint16_t interfaceNum;
    uint16_t endPointIn;
    uint16_t endPointOut;
} T_GduHalUsbBulkChannelInfo;

typedef struct {
    bool isUsbHost;
    // attention: if 'isUsbHost' equals false, the following parameters is not valid.
    uint16_t pid;
    uint16_t vid;
    T_GduHalUsbBulkChannelInfo channelInfo;
} T_GduHalUsbBulkInfo;

typedef struct {
    uint16_t pid;
    uint16_t vid;
    uint8_t bulkChannelNum;
    T_GduHalUsbBulkChannelInfo *channelInfo;
} T_GduHalUsbBulkDeviceInfo;

typedef struct {
    T_GduReturnCode (*UsbBulkInit)(T_GduHalUsbBulkInfo usbBulkInfo, T_GduUsbBulkHandle *usbBulkHandle);

    T_GduReturnCode (*UsbBulkDeInit)(T_GduUsbBulkHandle usbBulkHandle);

    T_GduReturnCode (*UsbBulkWriteData)(T_GduUsbBulkHandle usbBulkHandle, const uint8_t *buf,
                                        uint32_t len, uint32_t *realLen);

    T_GduReturnCode (*UsbBulkReadData)(T_GduUsbBulkHandle usbBulkHandle, uint8_t *buf,
                                       uint32_t len, uint32_t *realLen);

    T_GduReturnCode (*UsbBulkGetDeviceInfo)(T_GduHalUsbBulkDeviceInfo *deviceInfo);
} T_GduHalUsbBulkHandler;

typedef struct {
    T_GduReturnCode (*NetworkInit)(const char *ipAddr, const char *netMask, T_GduNetworkHandle *networkHandle);

    T_GduReturnCode (*NetworkDeInit)(T_GduNetworkHandle networkHandle);
} T_GduHalNetworkHandler;

typedef struct {
    T_GduReturnCode (*TaskCreate)(const char *name, void *(*taskFunc)(void *),
                                  uint32_t stackSize, void *arg, T_GduTaskHandle *task);

    T_GduReturnCode (*TaskDestroy)(T_GduTaskHandle task);

    T_GduReturnCode (*TaskSleepMs)(uint32_t timeMs);

    T_GduReturnCode (*MutexCreate)(T_GduMutexHandle *mutex);

    T_GduReturnCode (*MutexDestroy)(T_GduMutexHandle mutex);

    T_GduReturnCode (*MutexLock)(T_GduMutexHandle mutex);

    T_GduReturnCode (*MutexUnlock)(T_GduMutexHandle mutex);

    T_GduReturnCode (*SemaphoreCreate)(uint32_t initValue, T_GduSemaHandle *semaphore);

    T_GduReturnCode (*SemaphoreDestroy)(T_GduSemaHandle semaphore);

    T_GduReturnCode (*SemaphoreWait)(T_GduSemaHandle semaphore);

    T_GduReturnCode (*SemaphoreTimedWait)(T_GduSemaHandle semaphore, uint32_t waitTimeMs);

    T_GduReturnCode (*SemaphorePost)(T_GduSemaHandle semaphore);

    T_GduReturnCode (*GetTimeMs)(uint32_t *ms);

    T_GduReturnCode (*GetTimeUs)(uint64_t *us);

    T_GduReturnCode (*GetIPAddr)(uint8_t *addr);

    void *(*Malloc)(uint32_t size);

    void (*Free)(void *ptr);
} T_GduOsalHandler;

typedef struct {
    T_GduReturnCode (*FileOpen)(const char *fileName, const char *fileMode, T_GduFileHandle *fileObj);

    T_GduReturnCode (*FileClose)(T_GduFileHandle fileObj);

    T_GduReturnCode (*FileWrite)(T_GduFileHandle fileObj, const uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_GduReturnCode (*FileRead)(T_GduFileHandle fileObj, uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_GduReturnCode (*FileSeek)(T_GduFileHandle fileObj, uint32_t offset);

    T_GduReturnCode (*FileSync)(T_GduFileHandle fileObj);

    T_GduReturnCode (*DirOpen)(const char *filePath, T_GduDirHandle *dirObj);

    T_GduReturnCode (*DirClose)(T_GduDirHandle dirObj);

    T_GduReturnCode (*DirRead)(T_GduDirHandle dirObj, T_GduFileInfo *fileInfo);

    T_GduReturnCode (*Mkdir)(const char *filePath);

    T_GduReturnCode (*Unlink)(const char *filePath);

    T_GduReturnCode (*Rename)(const char *oldFilePath, const char *newFilePath);

    T_GduReturnCode (*Stat)(const char *filePath, T_GduFileInfo *fileInfo);
} T_GduFileSystemHandler;

typedef struct {
    T_GduReturnCode (*Socket)(E_GduSocketMode mode, T_GduSocketHandle *socketHandle);

    T_GduReturnCode (*Close)(T_GduSocketHandle socketHandle);

    T_GduReturnCode (*Bind)(T_GduSocketHandle socketHandle, const char *ipAddr, uint32_t port);

    T_GduReturnCode (*UdpSendData)(T_GduSocketHandle socketHandle, const char *ipAddr, uint32_t port,
                                   const uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_GduReturnCode (*UdpRecvData)(T_GduSocketHandle socketHandle, char *ipAddr, uint32_t *port,
                                   uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_GduReturnCode (*TcpListen)(T_GduSocketHandle socketHandle);

    T_GduReturnCode (*TcpAccept)(T_GduSocketHandle socketHandle, char *ipAddr, uint32_t *port,
                                 T_GduSocketHandle *outSocketHandle);

    T_GduReturnCode (*TcpConnect)(T_GduSocketHandle socketHandle, const char *ipAddr, uint32_t port);

    T_GduReturnCode (*TcpSendData)(T_GduSocketHandle socketHandle,
                                   const uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_GduReturnCode (*TcpRecvData)(T_GduSocketHandle socketHandle,
                                   uint8_t *buf, uint32_t len, uint32_t *realLen);
} T_GduSocketHandler;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Register the handler for hal uart interfaces by your platform.
 * @note It should be noted that the interface in hal is written and tested well. Users need to implement all the
 * interfaces. Otherwise, the user interface cannot be successfully registered, and then the user interface is registered
 * through the interface. If the registration fails, it needs to be based on the return code. To judge the problem. Make
 * sure that the feature is available after a successful registration. The interface needs to be called at the beginning of
 * the application for registration, otherwise, the subsequent functions will not work properly.
 * @param halUartHandler: pointer to the handler for hal uart interfaces by your platform.
 * @return Execution result.
 */
T_GduReturnCode GduPlatform_RegHalUartHandler(const T_GduHalUartHandler *halUartHandler);

/**
 * @brief Register the handler for usb bulk interfaces by your platform.
 * @param fileSystemHandler: pointer to the handler for usb bulk interfaces by your platform.
 * @return Execution result.
 */
T_GduReturnCode GduPlatform_RegHalUsbBulkHandler(const T_GduHalUsbBulkHandler *halUsbBulkHandler);

/**
 * @brief Register the handler for hal network interfaces by your platform.
 * @note It should be noted that the interface in hal is written and tested well. Users need to implement all the
 * interfaces. Otherwise, the user interface cannot be successfully registered, and then the user interface is registered
 * through the interface. If the registration fails, it needs to be based on the return code. To judge the problem. Make
 * sure that the feature is available after a successful registration.
 * @attention The interface needs to be called at the beginning of the application for registration, otherwise, the
 * subsequent functions will not work properly.
 * @param osalHandler: pointer to the handler for network handler interfaces by your platform.
 * @return Execution result.
 */
T_GduReturnCode GduPlatform_RegHalNetworkHandler(const T_GduHalNetworkHandler *halNetworkHandler);

/**
 * @brief Register the handler for osal interfaces by your platform.
 * @note It should be noted that the interface in osal is written and tested well. Users need to implement all the
 * interfaces. Otherwise, the user interface cannot be successfully registered, and then the user interface is registered
 * through the interface. If the registration fails, it needs to be based on the return code. To judge the problem. Make
 * sure that the feature is available after a successful registration. The interface needs to be called at the beginning of
 * the application for registration, otherwise, the subsequent functions will not work properly.
 * @param osalHandler: pointer to the handler for osal interfaces by your platform.
 * @return Execution result.
 */
T_GduReturnCode GduPlatform_RegOsalHandler(const T_GduOsalHandler *osalHandler);

/**
 * @brief Register the handler for file-system interfaces by your platform.
 * @param fileSystemHandler: pointer to the handler for file-system interfaces by your platform.
 * @return Execution result.
 */
T_GduReturnCode GduPlatform_RegFileSystemHandler(const T_GduFileSystemHandler *fileSystemHandler);

/**
 * @brief Register the handler for socket interfaces by your platform.
 * @param fileSystemHandler: pointer to the handler for socket interfaces by your platform.
 * @return Execution result.
 */
T_GduReturnCode GduPlatform_RegSocketHandler(const T_GduSocketHandler *socketHandler);

/**
 * @brief Get the handler of osal interfaces.
 * @return Execution result.
 */
T_GduOsalHandler *GduPlatform_GetOsalHandler(void);

/**
 * @brief Get the handler of file-system interfaces.
 * @return Execution result.
 */
T_GduFileSystemHandler *GduPlatform_GetFileSystemHandler(void);

/**
 * @brief Get the handler of socket interfaces.
 * @return Execution result.
 */
T_GduSocketHandler *GduPlatform_GetSocketHandler(void);

#ifdef __cplusplus
}
#endif

#endif // GDU_PLATFORM_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
