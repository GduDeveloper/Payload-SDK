/**
 ********************************************************************
 * @file    test_waypoint_v3.c
 * @brief
 *
 * @copyright (c) 2018 GDU. All rights reserved.
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
#include <utils/util_file.h>
#include <utils/util_misc.h>
#include "test_waypoint_v3.h"
#include "gdu_logger.h"
#include "gdu_waypoint_v3.h"
#include "waypoint_file_c/__20221230134822_xml.h"

/* Private constants ---------------------------------------------------------*/
#define GDU_TEST_WAYPOINT_V3_KMZ_FILE_PATH_LEN_MAX         (256)

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static T_GduReturnCode GduTest_WaypointV3StateCallback(T_GduWaypointV3MissionState missionState);

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode GduTest_WaypointV3RunSample(void)
{
    T_GduReturnCode returnCode;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();
#ifdef SYSTEM_ARCH_LINUX
    FILE *xmlFile;
    uint32_t xmlFileSize = 0;
    uint8_t *xmlFileBuf;
    uint16_t readLen;
    char curFileDirPath[GDU_TEST_WAYPOINT_V3_KMZ_FILE_PATH_LEN_MAX];
    char tempPath[GDU_TEST_WAYPOINT_V3_KMZ_FILE_PATH_LEN_MAX];

    returnCode = GduWaypointV3_Init();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Waypoint v3 init failed.");
        return returnCode;
    }

    returnCode = GduWaypointV3_RegMissionStateCallback(GduTest_WaypointV3StateCallback);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Register waypoint v3 state callback failed.");
        goto out;
    }

    returnCode = GduUserUtil_GetCurrentFileDirPath(__FILE__, GDU_TEST_WAYPOINT_V3_KMZ_FILE_PATH_LEN_MAX,
                                                   curFileDirPath);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file current path error, stat = 0x%08llX", returnCode);
        goto out;
    }

    /*! Attention: suggest use the exported xml file by GDU pilot. If use this test file, you need set the longitude as
     * 113.94755, latitude as 22.51853 on GDU Assistant 2 simulator */
    snprintf(tempPath, GDU_TEST_WAYPOINT_V3_KMZ_FILE_PATH_LEN_MAX, "%s/waypoint_file/20221230134822.gz",
             curFileDirPath);

    xmlFile = fopen(tempPath, "rb");
    if (xmlFile == NULL) {
        USER_LOG_ERROR("Open xml file failed.  %s", tempPath);
        goto out;
    }

    returnCode = UtilFile_GetFileSize(xmlFile, &xmlFileSize);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get xml file size failed.");
        goto out;
    }

    xmlFileBuf = osalHandler->Malloc(xmlFileSize);
    if (xmlFileBuf == NULL) {
        USER_LOG_ERROR("Malloc xml file buf error.");
        goto out;
    }

    readLen = fread(xmlFileBuf, 1, xmlFileSize, xmlFile);
    if (readLen != xmlFileSize) {
        USER_LOG_ERROR("Read xml file data failed.");
        goto out;
    }

    returnCode = GduWaypointV3_UploadXmlFile(xmlFileBuf, xmlFileSize, "20221230134822.gz");
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Upload xml file failed.");
        goto out;
    }

    osalHandler->Free(xmlFileBuf);
#else
    returnCode = GduWaypointV3_UploadXmlFile(__20221230134822_xml_fileBinaryArray,
                                             __20221230134822_xml_fileSize, "20221230134822.gz");
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Upload xml file failed.");
        goto out;
    }
#endif

    USER_LOG_INFO("Execute start action");
    returnCode = GduWaypointV3_Action(GDU_WAYPOINT_V3_ACTION_START, GDU_WAYPOINT_V3_NORMAL_MISSION);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Execute start action failed.");
        goto out;
    }

    for (int i = 0; i < 30; ++i) {
        osalHandler->TaskSleepMs(1000);
    }

    USER_LOG_INFO("Execute pause action");
    returnCode = GduWaypointV3_Action(GDU_WAYPOINT_V3_ACTION_PAUSE, GDU_WAYPOINT_V3_NORMAL_MISSION);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Execute start action failed.");
        goto out;
    }

    for (int i = 0; i < 5; ++i) {
        osalHandler->TaskSleepMs(1000);
    }

    USER_LOG_INFO("Execute resume action");
    returnCode = GduWaypointV3_Action(GDU_WAYPOINT_V3_ACTION_RESUME, GDU_WAYPOINT_V3_NORMAL_MISSION);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Execute start action failed.");
        goto out;
    }

out:
    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/
static T_GduReturnCode GduTest_WaypointV3StateCallback(T_GduWaypointV3MissionState missionState)
{
    USER_LOG_INFO("Waypoint v3 mission state: %d, current waypoint index: %d, wayLine id: %d", missionState.state,
                  missionState.currentWaypointIndex, missionState.wayLineId);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
