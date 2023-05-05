/**
 ********************************************************************
 * @file    hal_usb_bulk.c
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
#include "hal_usb_bulk.h"
#include "gdu_logger.h"

/* Private constants ---------------------------------------------------------*/
#define LINUX_USB_BULK_TRANSFER_TIMEOUT_MS    (50)
#define LINUX_USB_BULK_TRANSFER_WAIT_FOREVER  (-1)

#define LINUX_USB_BULK_DEV1                   "/dev/usb-ffs/bulk"

/* Private types -------------------------------------------------------------*/
typedef struct {
    libusb_device_handle *handle;
    int32_t ep1;
    int32_t ep2;
    T_GduHalUsbBulkInfo usbBulkInfo;
} T_HalUsbBulkObj;

/* Private values -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode HalUsbBulk_Init(T_GduHalUsbBulkInfo usbBulkInfo, T_GduUsbBulkHandle *usbBulkHandle)
{
    int32_t ret;
    struct libusb_device_handle *handle = NULL;

    *usbBulkHandle = malloc(sizeof(T_HalUsbBulkObj));
    if (*usbBulkHandle == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (usbBulkInfo.isUsbHost == true) {
        ret = libusb_init(NULL);
        if (ret < 0) {
            return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }

        handle = libusb_open_device_with_vid_pid(NULL, usbBulkInfo.vid, usbBulkInfo.pid);
        if (handle == NULL) {
            return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }

        ret = libusb_claim_interface(handle, usbBulkInfo.channelInfo.interfaceNum);
        if (ret != LIBUSB_SUCCESS) {
            printf("libusb claim interface error");
            libusb_close(handle);
            return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }

        ((T_HalUsbBulkObj *) *usbBulkHandle)->handle = handle;
        memcpy(&((T_HalUsbBulkObj *) *usbBulkHandle)->usbBulkInfo, &usbBulkInfo, sizeof(usbBulkInfo));

    } else {
        ((T_HalUsbBulkObj *) *usbBulkHandle)->handle = handle;
        memcpy(&((T_HalUsbBulkObj *) *usbBulkHandle)->usbBulkInfo, &usbBulkInfo, sizeof(usbBulkInfo));

        ((T_HalUsbBulkObj *) *usbBulkHandle)->ep1 = open("/dev/usb-ffs/bulk/ep1", O_RDWR);
        if (((T_HalUsbBulkObj *) *usbBulkHandle)->ep1 < 0) {
            return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }

        ((T_HalUsbBulkObj *) *usbBulkHandle)->ep2 = open("/dev/usb-ffs/bulk/ep2", O_RDWR);
        if (((T_HalUsbBulkObj *) *usbBulkHandle)->ep2 < 0) {
            return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode HalUsbBulk_DeInit(T_GduUsbBulkHandle usbBulkHandle)
{
    struct libusb_device_handle *handle = NULL;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    if (usbBulkHandle == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    handle = ((T_HalUsbBulkObj *) usbBulkHandle)->handle;

    if (((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.isUsbHost == true) {
        libusb_release_interface(handle, ((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.channelInfo.interfaceNum);
        osalHandler->TaskSleepMs(100);
        libusb_exit(NULL);
    } else {
        close(((T_HalUsbBulkObj *) usbBulkHandle)->ep1);
        close(((T_HalUsbBulkObj *) usbBulkHandle)->ep2);
    }

    free(usbBulkHandle);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode HalUsbBulk_WriteData(T_GduUsbBulkHandle usbBulkHandle, const uint8_t *buf, uint32_t len,
                                     uint32_t *realLen)
{
    int32_t ret;
    int32_t actualLen;
    struct libusb_device_handle *handle = NULL;

    if (usbBulkHandle == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    handle = ((T_HalUsbBulkObj *) usbBulkHandle)->handle;

    if (((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.isUsbHost == true) {
        ret = libusb_bulk_transfer(handle, ((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.channelInfo.endPointOut,
                                   (uint8_t *) buf, len, &actualLen, LINUX_USB_BULK_TRANSFER_TIMEOUT_MS);
        if (ret < 0) {
            return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
        }

        *realLen = actualLen;
    } else {
        *realLen = write(((T_HalUsbBulkObj *) usbBulkHandle)->ep1, buf, len);
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
}

T_GduReturnCode HalUsbBulk_ReadData(T_GduUsbBulkHandle usbBulkHandle, uint8_t *buf, uint32_t len,
                                    uint32_t *realLen)
{
    int32_t ret;
    struct libusb_device_handle *handle = NULL;
    int32_t actualLen;

    if (usbBulkHandle == NULL) {
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    handle = ((T_HalUsbBulkObj *) usbBulkHandle)->handle;

    if (((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.isUsbHost == true) {
        ret = libusb_bulk_transfer(handle, ((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.channelInfo.endPointIn,
                                   buf, len, &actualLen, LINUX_USB_BULK_TRANSFER_WAIT_FOREVER);
        if (ret < 0) {
            return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }

        *realLen = actualLen;
    } else {
        *realLen = read(((T_HalUsbBulkObj *) usbBulkHandle)->ep2, buf, len);
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode HalUsbBulk_GetDeviceInfo(T_GduHalUsbBulkDeviceInfo *deviceInfo)
{
    //attention: need confirm your usb config in device mode.
    deviceInfo->vid = 0x7020;
    deviceInfo->pid = 0x9055;

    deviceInfo->bulkChannelNum = 2;

    deviceInfo->channelInfo[0].interfaceNum = 0;
    deviceInfo->channelInfo[0].endPointIn = 0x01;
    deviceInfo->channelInfo[0].endPointOut = 0x81;

    deviceInfo->channelInfo[1].interfaceNum = 0;
    deviceInfo->channelInfo[1].endPointIn = 0x02;
    deviceInfo->channelInfo[1].endPointOut = 0x82;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
