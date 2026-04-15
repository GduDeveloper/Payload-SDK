/**
 ********************************************************************
 * @file    osal_socket.c
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
#include "osal_socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "stdlib.h"

/* Private constants ---------------------------------------------------------*/
typedef struct {
    int socketFd;
} T_SocketHandleStruct;

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode Osal_Socket(E_GduSocketMode mode, T_GduSocketHandle *socketHandle)
{
    T_SocketHandleStruct *socketHandleStruct;

    if (socketHandle == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    socketHandleStruct = malloc(sizeof(T_SocketHandleStruct));
    if (socketHandleStruct == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    if (mode == GDU_SOCKET_MODE_UDP) {
        socketHandleStruct->socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    } else if (mode == GDU_SOCKET_MODE_TCP) {
        socketHandleStruct->socketFd = socket(AF_INET, SOCK_STREAM, 0);
    } else {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    *socketHandle = socketHandleStruct;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_Close(T_GduSocketHandle socketHandle)
{
    T_SocketHandleStruct *socketHandleStruct = (T_SocketHandleStruct *) socketHandle;
    int32_t ret;

    if (socketHandleStruct->socketFd <= 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = close(socketHandleStruct->socketFd);
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_Bind(T_GduSocketHandle socketHandle, const char *ipAddr, uint32_t port)
{
    T_SocketHandleStruct *socketHandleStruct = (T_SocketHandleStruct *) socketHandle;
    struct sockaddr_in addr;
    int32_t ret;

    if (socketHandle == NULL || ipAddr == NULL || port == 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ipAddr);

    ret = bind(socketHandleStruct->socketFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_UdpSendData(T_GduSocketHandle socketHandle, const char *ipAddr, uint32_t port,
                                 const uint8_t *buf, uint32_t len, uint32_t *realLen)
{
    struct sockaddr_in addr;
    T_SocketHandleStruct *socketHandleStruct = (T_SocketHandleStruct *) socketHandle;
    int32_t ret;

    if (socketHandle <= 0 || ipAddr == NULL || port == 0 || buf == NULL || len == 0 || realLen == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ipAddr);

    ret = sendto(socketHandleStruct->socketFd, buf, len, 0, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if (ret >= 0) {
        *realLen = ret;
    } else {
        printf("send err , err:%d\n", ret);
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_UdpRecvData(T_GduSocketHandle socketHandle, char *ipAddr, uint32_t *port,
                                 uint8_t *buf, uint32_t len, uint32_t *realLen)
{
    struct sockaddr_in addr;
    T_SocketHandleStruct *socketHandleStruct = (T_SocketHandleStruct *) socketHandle;
    uint32_t addrLen = 0;
    int32_t ret;

    if (socketHandle == NULL || ipAddr == NULL || port == NULL || buf == NULL || len == 0 || realLen == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = recvfrom(socketHandleStruct->socketFd, buf, len, 0, (struct sockaddr *) &addr, &addrLen);
    if (ret >= 0) {
        *realLen = ret;
        strcpy(ipAddr, inet_ntoa(addr.sin_addr));
        *port = ntohs(addr.sin_port);
    } else {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_TcpListen(T_GduSocketHandle socketHandle)
{
    int32_t ret;
    T_SocketHandleStruct *socketHandleStruct = (T_SocketHandleStruct *) socketHandle;

    if (socketHandle == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = listen(socketHandleStruct->socketFd, 5);
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_TcpAccept(T_GduSocketHandle socketHandle, char *ipAddr, uint32_t *port,
                               T_GduSocketHandle *outSocketHandle)
{
    T_SocketHandleStruct *socketHandleStruct = (T_SocketHandleStruct *) socketHandle;
    T_SocketHandleStruct *outSocketHandleStruct;
    struct sockaddr_in addr;
    uint32_t addrLen = 0;

    if (socketHandle == NULL || ipAddr == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    outSocketHandleStruct = malloc(sizeof(T_SocketHandleStruct));
    if (outSocketHandleStruct == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    outSocketHandleStruct->socketFd = accept(socketHandleStruct->socketFd, (struct sockaddr *) &addr, &addrLen);
    if ((int32_t *) outSocketHandle == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    *port = ntohs(addr.sin_port);
    *outSocketHandle = outSocketHandleStruct;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_TcpConnect(T_GduSocketHandle socketHandle, const char *ipAddr, uint32_t port)
{
    struct sockaddr_in addr;
    T_SocketHandleStruct *socketHandleStruct = (T_SocketHandleStruct *) socketHandle;
    int32_t ret;

    if (socketHandle == NULL || ipAddr == NULL || port == 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ipAddr);

    ret = connect(socketHandleStruct->socketFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_TcpSendData(T_GduSocketHandle socketHandle,
                                 const uint8_t *buf, uint32_t len, uint32_t *realLen)
{
    T_SocketHandleStruct *socketHandleStruct = (T_SocketHandleStruct *) socketHandle;
    int32_t ret;

    if (socketHandle == NULL || buf == NULL || len == 0 || realLen == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = send(socketHandleStruct->socketFd, buf, len, 0);
    if (ret >= 0) {
        *realLen = ret;
    } else {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode Osal_TcpRecvData(T_GduSocketHandle socketHandle,
                                 uint8_t *buf, uint32_t len, uint32_t *realLen)
{
    T_SocketHandleStruct *socketHandleStruct = (T_SocketHandleStruct *) socketHandle;
    int32_t ret;

    if (socketHandle == NULL || buf == NULL || len == 0 || realLen == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    ret = recv(socketHandleStruct->socketFd, buf, len, 0);
    if (ret >= 0) {
        *realLen = ret;
    } else {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
