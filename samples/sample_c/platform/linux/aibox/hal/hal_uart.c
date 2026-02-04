/**
 ********************************************************************
 * @file    hal_uart.c
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
#include <gdu_logger.h>
#include "hal_uart.h"

/* Private constants ---------------------------------------------------------*/
#define UART_DEV_NAME_STR_SIZE             (128)
#define GDU_SYSTEM_CMD_MAX_SIZE            (64)
#define GDU_SYSTEM_RESULT_STR_MAX_SIZE     (128)

/* Private types -------------------------------------------------------------*/
typedef struct {
    int uartFd;
} T_UartHandleStruct;

/* Private values -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode HalUart_Init(E_GduHalUartNum uartNum, uint32_t baudRate, T_GduUartHandle *uartHandle)
{
    T_UartHandleStruct *uartHandleStruct;
    struct termios options;
    struct flock lock;
    T_GduReturnCode returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    char uartName[UART_DEV_NAME_STR_SIZE];
    char systemCmd[GDU_SYSTEM_CMD_MAX_SIZE];
    char *ret = NULL;
    char lineBuf[GDU_SYSTEM_RESULT_STR_MAX_SIZE] = {0};
    FILE *fp;

    uartHandleStruct = malloc(sizeof(T_UartHandleStruct));

    if (uartNum == GDU_HAL_UART_NUM_0) {
        strcpy(uartName, LINUX_UART_DEV1);
    } else if (uartNum == GDU_HAL_UART_NUM_1) {
        strcpy(uartName, LINUX_UART_DEV2);
    } else {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

#ifdef USE_CLION_DEBUG
    sprintf(systemCmd, "ls -l %s", uartName);
    fp = popen(systemCmd, "r");
    if (fp == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    ret = fgets(lineBuf, sizeof(lineBuf), fp);
    if (ret == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (strstr(lineBuf, "crwxrwxrwx") == NULL) {
        USER_LOG_ERROR("Can't operation the device. "
                       "Probably the device has not operation permission. "
                       "Please execute command 'sudo chmod 777 %s' to add permission. ", uartName);
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
#else
    sprintf(systemCmd, "chmod 777 %s", uartName);
    fp = popen(systemCmd, "r");
    if (fp == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
#endif

    uartHandleStruct->uartFd = open(uartName, (unsigned) O_RDWR | (unsigned) O_NOCTTY | (unsigned) O_NDELAY);
    if (uartHandleStruct->uartFd == -1) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    // Forbid multiple psdk programs to access the serial port
    lock.l_type = F_WRLCK;
    lock.l_pid = getpid();
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(uartHandleStruct->uartFd, F_GETLK, &lock) < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
    if (lock.l_type != F_UNLCK) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
    lock.l_type = F_WRLCK;
    lock.l_pid = getpid();
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    if (fcntl(uartHandleStruct->uartFd, F_SETLKW, &lock) < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (tcgetattr(uartHandleStruct->uartFd, &options) != 0) {
        close(uartHandleStruct->uartFd);
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    switch (baudRate) {
        case 115200:
            cfsetispeed(&options, B115200);
            cfsetospeed(&options, B115200);
            break;
        case 230400:
            cfsetispeed(&options, B230400);
            cfsetospeed(&options, B230400);
            break;
        case 460800:
            cfsetispeed(&options, B460800);
            cfsetospeed(&options, B460800);
            break;
        case 921600:
            cfsetispeed(&options, B921600);
            cfsetospeed(&options, B921600);
            break;
        case 1000000:
            cfsetispeed(&options, B1000000);
            cfsetospeed(&options, B1000000);
            break;
        default:
            return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    options.c_cflag |= (unsigned) CLOCAL;
    options.c_cflag |= (unsigned) CREAD;
    options.c_cflag &= ~(unsigned) CRTSCTS;
    options.c_cflag &= ~(unsigned) CSIZE;
    options.c_cflag |= (unsigned) CS8;
    options.c_cflag &= ~(unsigned) PARENB;
    options.c_iflag &= ~(unsigned) INPCK;
    options.c_cflag &= ~(unsigned) CSTOPB;
    options.c_oflag &= ~(unsigned) OPOST;
    options.c_lflag &= ~((unsigned) ICANON | (unsigned) ECHO | (unsigned) ECHOE | (unsigned) ISIG);
    options.c_iflag &= ~((unsigned) BRKINT | (unsigned) ICRNL | (unsigned) INPCK | (unsigned) ISTRIP | (unsigned) IXON);
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 0;

    tcflush(uartHandleStruct->uartFd, TCIFLUSH);

    if (tcsetattr(uartHandleStruct->uartFd, TCSANOW, &options) != 0) {
        close(uartHandleStruct->uartFd);
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    *uartHandle = uartHandleStruct;

    return returnCode;
}

T_GduReturnCode HalUart_DeInit(T_GduUartHandle uartHandle)
{
    int32_t ret;
    T_UartHandleStruct *uartHandleStruct = (T_UartHandleStruct *) uartHandle;

    if (uartHandle == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    ret = close(uartHandleStruct->uartFd);
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode HalUart_WriteData(T_GduUartHandle uartHandle, const uint8_t *buf, uint32_t len, uint32_t *realLen)
{
    int32_t ret;
    T_UartHandleStruct *uartHandleStruct = (T_UartHandleStruct *) uartHandle;

    if (uartHandle == NULL || buf == NULL || len == 0 || realLen == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = write(uartHandleStruct->uartFd, buf, len);
    if (ret >= 0) {
        *realLen = ret;
    } else {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode HalUart_ReadData(T_GduUartHandle uartHandle, uint8_t *buf, uint32_t len, uint32_t *realLen)
{
    int32_t ret;
    T_UartHandleStruct *uartHandleStruct = (T_UartHandleStruct *) uartHandle;

    if (uartHandle == NULL || buf == NULL || len == 0 || realLen == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = read(uartHandleStruct->uartFd, buf, len);
    if (ret >= 0) {
        *realLen = ret;
    } else {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode HalUart_GetStatus(E_GduHalUartNum uartNum, T_GduUartStatus *status)
{
    if (uartNum == GDU_HAL_UART_NUM_0) {
        status->isConnect = true;
    } else if (uartNum == GDU_HAL_UART_NUM_1) {
        status->isConnect = true;
    } else {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
