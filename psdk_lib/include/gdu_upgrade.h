/**
 ********************************************************************
 * @file    gdu_upgrade.h
 * @brief   This is the header file for "gdu_upgrade.c", defining the structure and
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
#ifndef GDU_UPGRADE_H
#define GDU_UPGRADE_H

/* Includes ------------------------------------------------------------------*/
#include <gdu_typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif
/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum {
    /*!
     * @brief FTP firmware transfer type.
     * @note This transfer type only support linux platform and use network port. Users need to deploy FTP service on
     * payload. The ftp user info used to transfer upgrade firmware is :
     * username:gdu_payload_ftp
     * password:
     * You can get guide about FTP service deployment on https://developer.gdu.com/payload-sdk/documentation
     */
    GDU_FIRMWARE_TRANSFER_TYPE_UART = 0,
} E_GduFirmwareTransferType;

typedef enum {
    GDU_UPGRADE_STAGE_IDLE = 0, /*!< Idle stage means not in upgrade mode. */
    GDU_UPGRADE_STAGE_ONGOING = 3, /*!< Ongoing stage means payload is upgrading. */
    GDU_UPGRADE_STAGE_DEVICE_REBOOT = 6, /*!< Device reboot stage means device is rebooting. */
    GDU_UPGRADE_STAGE_END = 4, /*!< End Stage means upgrade finish and reporting upgrade result to the terminal APP. */
} E_GduUpgradeStage;

typedef enum {
    GDU_UPGRADE_END_STATE_SUCCESS = 1, /*!< Upgrade success. */
    GDU_UPGRADE_END_STATE_UNKNOWN_ERROR = 2, /*!< Upgrade failure due to unknown reason. */
} E_GduUpgradeEndState;

/**
 * @brief The firmware version of payload.
 * @note If majorVersion = AA, minorVersion = BB, modifyVersion = CC, debugVersion = DD, The version show in
 * terminal APP is AA.BB.CC.DD
 */
typedef struct {
    uint8_t majorVersion; /*!< The major version of firmware, the range is 0 ~ 99. */
    uint8_t minorVersion; /*!< The minor version of firmware, the range is 0 ~ 99. */
    uint8_t modifyVersion; /*!< The modify version of firmware, the range is 0 ~ 99. */
    uint8_t debugVersion; /*!< The debug version of firmware, the range is 0 ~ 99. */
} T_GduVersion;

typedef struct {
    uint8_t upgradeProgress; /*!< The upgrade progress, the range is 0 ~ 100. */
} T_GduUpgradeOngoingInfo;

typedef struct {
    uint8_t rebootTimeout; /*!< The timeout value of rebooting device. unit: s */
} T_GduUpgradeRebootInfo;

typedef struct {
    E_GduUpgradeEndState upgradeEndState; /*!< The upgrade end state */
} T_GduUpgradeEndInfo;

typedef struct {
    uint32_t port; /*!< FTP port used to transfer file, default is 21 */
} T_GduUpgradeFtpFileTransferInfo;

typedef struct {
    uint32_t fileSize; /*! The size of file. */
    char fileName[GDU_FILE_NAME_SIZE_MAX]; /*! The name of file. */
} T_GduUpgradeFileInfo;

typedef struct {
    /**
      * @brief Prototype of callback function used to start file transfer.
      * @param fileInfo: the file info about the file to be transferred.
      * @return Execution result.
      */
    T_GduReturnCode (*start)(const T_GduUpgradeFileInfo *fileInfo);
    /**
      * @brief Prototype of callback function used to transfer file data.
      * @details After start transfer, this callback function will be called several times to transfer file data sequentially.
      * @param data: pointer to memory space used to store file data.
      * @param dataLen: the data length of data.
      * @return Execution result.
      */
    T_GduReturnCode (*transfer)(const uint8_t *data, uint16_t dataLen);
    /**
      * @brief Prototype of callback function used to finish file transfer.
      * @param md5: the md5 value of file, used to check the correctness of the file transfer .
      * @return Execution result.
      */
    T_GduReturnCode (*finish)(const uint8_t md5[GDU_MD5_BUFFER_LEN]);
} T_GduUpgradeDcftpFileTransferOpt;

typedef struct {
    /*! The firmware transfer type for upgrade. */
    E_GduFirmwareTransferType transferType;

    /*! If transferType is GDU_FIRMWARE_TRANSFER_TYPE_DCFTP, need support dcftpFileTransferOpt. */
    T_GduUpgradeDcftpFileTransferOpt dcftpFileTransferOpt;
} T_GduFirmwareTransferInfo;

typedef struct {
    /*! The upgrade stage in upgrade process. */
    E_GduUpgradeStage upgradeStage;

    union {
        /*! If upgradeStage is GDU_UPGRADE_STAGE_ONGOING, need support upgradeOngoingInfo. */
        T_GduUpgradeOngoingInfo upgradeOngoingInfo;
        /*! If upgradeStage is GDU_UPGRADE_STAGE_DEVICE_REBOOT, need support upgradeRebootInfo. */
        T_GduUpgradeRebootInfo upgradeRebootInfo;
        /*! If upgradeStage is GDU_UPGRADE_STAGE_END, need support upgradeEndInfo. */
        T_GduUpgradeEndInfo upgradeEndInfo;
    };
} T_GduUpgradeState;

typedef struct {
    T_GduVersion currentFirmwareVersion; /*!< The current firmware version of payload. */
    T_GduFirmwareTransferInfo firmwareTransferInfo; /*!< The firmware transfer info of payload. */
} T_GduUpgradeConfig;

typedef struct {
    /**
      * @brief Prototype of callback function used to enter upgrade mode.
      * @param waitTime: the wait time for enter upgrade mode. The terminal APP will wait these time before do other
      * upgrade actions. You can use this time to prepare for firmware upgrade in other task, such as clean firmware
      * store area. unit: s
      * @return Execution result.
      */
    T_GduReturnCode (*EnterUpgradeMode)(uint16_t *waitTime);
    /**
      * @brief Prototype of callback function used to check transferred firmware.
      * @details You can verify signature and decrypt firmware in this callback function.
      * @return Execution result.
      */
    T_GduReturnCode (*CheckFirmware)(void);
    /**
      * @brief Prototype of callback function used to start firmware upgrade.
      * @note After start upgrade, the upgrade stage need change to ::GDU_UPGRADE_STAGE_ONGOING
      * @return Execution result.
      */
    T_GduReturnCode (*StartUpgrade)(void);
    /**
      * @brief Prototype of callback function used to finish firmware upgrade.
      * @note After call finish upgrade, the upgrade stage need change from ::GDU_UPGRADE_STAGE_END to ::GDU_UPGRADE_STAGE_IDLE
      * @return Execution result.
      */
    T_GduReturnCode (*FinishUpgrade)(void);
} T_GduUpgradeHandler;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise upgrade module, and user should call this function before using upgrade features.
 * @param upgradeConfig: upgrade init configuration.
 * @return Execution result.
 */
T_GduReturnCode GduUpgrade_Init(const T_GduUpgradeConfig *upgradeConfig);

/**
 * @brief Enable local upgrade mode.
 * @details After enable local upgrade mode, GDU Assistant 2 will display payload upgrade interface. Users can choose
 * firmware file to upgrade payload.
 * @return Execution result.
 */
T_GduReturnCode GduUpgrade_EnableLocalUpgrade(void);

/**
 * @brief Register the handler for upgrade process.
 * @param upgradeHandler: pointer to structure of handler functions for upgrade process.
 * @return Execution result.
 */
T_GduReturnCode GduUpgrade_RegHandler(const T_GduUpgradeHandler *upgradeHandler);

/**
 * @brief Push upgrade state to terminal app.
 * @param upgradeState: upgrade state in upgrade process.
 * @note When start upgrade, you need push upgrade state until finish upgrade. The upgrade state push logic is :
 * 1. StartUpgrade callback called;
 * 2. The upgrade stage change to :: GDU_UPGRADE_STAGE_ONGOING. Push upgrade state upgradeOngoingInfo when upgradeOngoingInfo changed;
 * 3. After upgrade ongoing, change upgrade stage to GDU_UPGRADE_STAGE_DEVICE_REBOOT and push upgrade state upgradeRebootInfo;
 * 4. Device reboot;
 * 4. After device reboot, change upgrade stage to GDU_UPGRADE_STAGE_END and continuous push upgrade state upgradeEndInfo (recommended 1Hz);
 * 5. FinishUpgrade Callback called, stop push upgrade state. The upgrade process finished.
 * @return Execution result.
 */
T_GduReturnCode GduUpgrade_PushUpgradeState(const T_GduUpgradeState *upgradeState);

#ifdef __cplusplus
}
#endif

#endif // GDU_UPGRADE_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
