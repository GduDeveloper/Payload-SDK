/**
 ********************************************************************
 * @file    test_flight_control.c
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
#include "gdu_flight_controller.h"
#include "test_flight_control.h"
#include "gdu_fc_subscription.h"
#include "gdu_platform.h"
#include "gdu_logger.h"
#include <math.h>
//#include <widget_interaction_test/test_widget_interaction.h>
/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/
#pragma pack(1)
typedef struct {
    gdu_f32_t x;
    gdu_f32_t y;
    gdu_f32_t z;
} T_GduTestFlightControlVector3f; // pack(1)
#pragma pack()

typedef struct {
    E_GduFcSubscriptionDisplayMode displayMode;
    char *displayModeStr;
} T_GduTestFlightControlDisplayModeStr;

/* Private values -------------------------------------------------------------*/
static T_GduOsalHandler *s_osalHandler = NULL;
static const double s_earthCenter = 6378137.0;
static const double s_degToRad = 0.01745329252;

static const T_GduTestFlightControlDisplayModeStr s_flightControlDisplayModeStr[] = {
    {.displayMode = GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ATTITUDE, .displayModeStr = "attitude mode"},
    {.displayMode = GDU_FC_SUBSCRIPTION_DISPLAY_MODE_P_GPS, .displayModeStr = "p_gps mode"},
    {.displayMode = GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ASSISTED_TAKEOFF, .displayModeStr = "assisted takeoff mode"},
    {.displayMode = GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_TAKEOFF, .displayModeStr = "auto takeoff mode"},
    {.displayMode = GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_LANDING, .displayModeStr = "auto landing mode"},
    {.displayMode = GDU_FC_SUBSCRIPTION_DISPLAY_MODE_NAVI_GO_HOME, .displayModeStr = "go home mode"},
    {.displayMode = GDU_FC_SUBSCRIPTION_DISPLAY_MODE_FORCE_AUTO_LANDING, .displayModeStr = "force landing mode"},
    {.displayMode = GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ENGINE_START, .displayModeStr = "engine start mode"},
    {.displayMode = 0xFF, .displayModeStr = "unknown mode"}
};

/* Private functions declaration ---------------------------------------------*/
static uint8_t GduTest_FlightControlGetDisplayModeIndex(E_GduFcSubscriptionDisplayMode displayMode);
static T_GduFcSubscriptionFlightStatus GduTest_FlightControlGetValueOfFlightStatus(void);
static T_GduFcSubscriptionDisplaymode GduTest_FlightControlGetValueOfDisplayMode(void);
static T_GduFcSubscriptionAvoidData GduTest_FlightControlGetValueOfAvoidData(void);
static T_GduFcSubscriptionQuaternion GduTest_FlightControlGetValueOfQuaternion(void);
static T_GduFcSubscriptionPositionFused GduTest_FlightControlGetValueOfPositionFused(void);
static gdu_f32_t GduTest_FlightControlGetValueOfRelativeHeight(void);
static bool GduTest_FlightControlMotorStartedCheck(void);
static bool GduTest_FlightControlTakeOffInAirCheck(void);
static bool GduTest_FlightControlLandFinishedCheck(void);
static bool GduTest_FlightControlMonitoredTakeoff(void);
static bool GduTest_FlightControlCheckActionStarted(E_GduFcSubscriptionDisplayMode mode);
static bool GduTest_FlightControlMonitoredLanding(void);
static bool GduTest_FlightControlGoHomeAndConfirmLanding(void);
static T_GduTestFlightControlVector3f GduTest_FlightControlQuaternionToEulerAngle(T_GduFcSubscriptionQuaternion quat);
static T_GduTestFlightControlVector3f
GduTest_FlightControlLocalOffsetFromGpsAndFusedHeightOffset(T_GduFcSubscriptionPositionFused target,
                                                            T_GduFcSubscriptionPositionFused origin,
                                                            gdu_f32_t targetHeight,
                                                            gdu_f32_t originHeight);
static T_GduTestFlightControlVector3f
GduTest_FlightControlVector3FSub(T_GduTestFlightControlVector3f vectorA, T_GduTestFlightControlVector3f vectorB);
static int GduTest_FlightControlSignOfData(gdu_f32_t data);
static void GduTest_FlightControlHorizCommandLimit(gdu_f32_t speedFactor, gdu_f32_t *commandX, gdu_f32_t *commandY);
static gdu_f32_t GduTest_FlightControlVectorNorm(T_GduTestFlightControlVector3f v);
static T_GduReturnCode
GduTest_FlightControlJoystickCtrlAuthSwitchEventCallback(T_GduFlightControllerJoystickCtrlAuthorityEventInfo eventData);
static bool
GduTest_FlightControlMoveByPositionOffset(T_GduTestFlightControlVector3f offsetDesired, float yawDesiredInDeg,
                                          float posThresholdInM,
                                          float yawThresholdInDeg);
static void
GduTest_FlightControlVelocityAndYawRateCtrl(T_GduTestFlightControlVector3f offsetDesired, float yawRate,
                                            uint32_t timeMs);
static T_GduReturnCode GduTest_FlightControlInit(void);
static T_GduReturnCode GduTest_FlightControlDeInit(void);
static void GduTest_FlightControlTakeOffLandingSample(void);
static void GduTest_FlightControlPositionControlSample(void);
static void GduTest_FlightControlGoHomeForceLandingSample(void);
static void GduTest_FlightControlVelocityControlSample(void);
static void GduTest_FlightControlArrestFlyingSample(void);
static void GduTest_FlightControlSample(E_GduTestFlightCtrlSampleSelect flightCtrlSampleSelect);

/* Exported functions definition ---------------------------------------------*/
T_GduReturnCode GduTest_FlightControlRunSample(E_GduTestFlightCtrlSampleSelect flightCtrlSampleSelect)
{
    T_GduReturnCode returnCode;

    USER_LOG_DEBUG("Init flight Control Sample");
    GduTest_WidgetLogAppend("Init flight Control Sample");

    returnCode = GduTest_FlightControlInit();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Init flight Control sample failed,error code:0x%08llX", returnCode);
        return returnCode;
    }

    GduTest_FlightControlSample(flightCtrlSampleSelect);

    USER_LOG_DEBUG("Deinit Flight Control Sample");
    GduTest_WidgetLogAppend("Deinit Flight Control Sample");
    returnCode = GduTest_FlightControlDeInit();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Deinit Flight Control sample failed,error code:0x%08llX", returnCode);
        return returnCode;
    }

    return returnCode;
}

/* Private functions definition-----------------------------------------------*/
T_GduReturnCode GduTest_FlightControlInit(void)
{
    T_GduReturnCode returnCode;

    s_osalHandler = GduPlatform_GetOsalHandler();
    if (!s_osalHandler) return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;

    returnCode = GduFlightController_Init();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Init flight controller module failed, error code:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduFcSubscription_Init();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Init data subscription module failed, error code:0x%08llX", returnCode);
        return returnCode;
    }

    /*! subscribe fc data */
    returnCode = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_STATUS_FLIGHT,
                                                  GDU_DATA_SUBSCRIPTION_TOPIC_10_HZ,
                                                  NULL);

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic flight status failed, error code:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_STATUS_DISPLAYMODE,
                                                  GDU_DATA_SUBSCRIPTION_TOPIC_10_HZ,
                                                  NULL);

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic display mode failed, error code:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_AVOID_DATA,
                                                  GDU_DATA_SUBSCRIPTION_TOPIC_10_HZ,
                                                  NULL);

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic avoid data failed,error code:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_QUATERNION,
                                                  GDU_DATA_SUBSCRIPTION_TOPIC_10_HZ,
                                                  NULL);

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic quaternion failed,error code:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_POSITION_FUSED,
                                                  GDU_DATA_SUBSCRIPTION_TOPIC_10_HZ,
                                                  NULL);

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic position fused failed,error code:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_ALTITUDE_FUSED,
                                                  GDU_DATA_SUBSCRIPTION_TOPIC_10_HZ,
                                                  NULL);

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic altitude fused failed,error code:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_ALTITUDE_OF_HOMEPOINT,
                                                  GDU_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                                  NULL);

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic altitude of home point failed,error code:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = GduFlightController_RegJoystickCtrlAuthorityEventCallback(
        GduTest_FlightControlJoystickCtrlAuthSwitchEventCallback);

    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS && returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT) {
        USER_LOG_ERROR("Register joystick control authority event callback failed,error code:0x%08llX", returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_FlightControlDeInit(void)
{
    T_GduReturnCode returnCode;

    returnCode = GduFlightController_Deinit();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Deinit flight controller module failed, error code:0x%08llX",
                       returnCode);
        return returnCode;
    }

    returnCode = GduFcSubscription_DeInit();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Deinit data subscription module failed, error code:0x%08llX",
                       returnCode);
        return returnCode;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

void GduTest_FlightControlTakeOffLandingSample()
{
    T_GduReturnCode returnCode;

    USER_LOG_INFO("Flight control takeoff-landing sample start");
    GduTest_WidgetLogAppend("Flight control takeoff-landing sample start");
    USER_LOG_INFO("--> Step 1: Obtain joystick control authority.");
    GduTest_WidgetLogAppend("--> Step 1: Obtain joystick control authority.");
    returnCode = GduFlightController_ObtainJoystickCtrlAuthority();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Obtain joystick authority failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(1000);

    USER_LOG_INFO("--> Step 2: Take off\r\n");
    GduTest_WidgetLogAppend("--> Step 2: Take off\r\n");
    if (!GduTest_FlightControlMonitoredTakeoff()) {
        USER_LOG_ERROR("Take off failed");
        goto out;
    }
    USER_LOG_INFO("Successful take off\r\n");
    GduTest_WidgetLogAppend("Successful take off\r\n");
    s_osalHandler->TaskSleepMs(4000);

    USER_LOG_INFO("--> Step 3: Landing\r\n");
    GduTest_WidgetLogAppend("--> Step 3: Landing\r\n");
    if (!GduTest_FlightControlMonitoredLanding()) {
        USER_LOG_ERROR("Landing failed");
        goto out;
    }
    USER_LOG_INFO("Successful landing\r\n");
    GduTest_WidgetLogAppend("Successful landing\r\n");

    USER_LOG_INFO("--> Step 4: Release joystick authority");
    GduTest_WidgetLogAppend("--> Step 4: Release joystick authority");
    returnCode = GduFlightController_ReleaseJoystickCtrlAuthority();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Release joystick authority failed, error code: 0x%08X", returnCode);
        goto out;
    }

out:
    USER_LOG_INFO("Flight control takeoff-landing sample end");
    GduTest_WidgetLogAppend("Flight control takeoff-landing sample end");
}

void GduTest_FlightControlPositionControlSample()
{
    T_GduReturnCode returnCode;

    USER_LOG_INFO("Flight control move-by-position sample start");
    GduTest_WidgetLogAppend("Flight control move-by-position sample start");

    USER_LOG_INFO("--> Step 1: Obtain joystick control authority.");
    GduTest_WidgetLogAppend("--> Step 1: Obtain joystick control authority.");
    returnCode = GduFlightController_ObtainJoystickCtrlAuthority();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Obtain joystick authority failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(1000);

    USER_LOG_INFO("--> Step 2: Take off\r\n");
    GduTest_WidgetLogAppend("--> Step 2: Take off\r\n");
    if (!GduTest_FlightControlMonitoredTakeoff()) {
        USER_LOG_ERROR("Take off failed");
        goto out;
    }
    USER_LOG_INFO("Successful take off\r\n");
    GduTest_WidgetLogAppend("Successful take off\r\n");

    USER_LOG_INFO("--> Step 3: Move to north:0(m), earth:6(m), up:6(m) , yaw:30(degree) from current point");
    GduTest_WidgetLogAppend("--> Step 3: Move to north:0(m), earth:6(m), up:6(m) , yaw:30(degree) from current point");
    if (!GduTest_FlightControlMoveByPositionOffset((T_GduTestFlightControlVector3f) {0, 6, 6}, 30, 0.8, 1)) {
        USER_LOG_ERROR("Move to north:0(m), earth:6(m), up:6(m) , yaw:30(degree) from current point failed");
        goto out;
    };

    USER_LOG_INFO("--> Step 4: Move to north:6(m), earth:0(m), up:-3(m) , yaw:-30(degree) from current point");
    GduTest_WidgetLogAppend(
        "--> Step 4: Move to north:6(m), earth:0(m), up:-3(m) , yaw:-30(degree) from current point");
    if (!GduTest_FlightControlMoveByPositionOffset((T_GduTestFlightControlVector3f) {6, 0, -3}, -30, 0.8, 1)) {
        USER_LOG_ERROR("Move to north:6(m), earth:0(m), up:-3(m) , yaw:-30(degree) from current point failed");
        goto out;
    };

    USER_LOG_INFO("--> Step 5: Move to north:-6(m), earth:-6(m), up:0(m) , yaw:0(degree) from current point");
    GduTest_WidgetLogAppend("--> Step 5: Move to north:-6(m), earth:-6(m), up:0(m) , yaw:0(degree) from current point");
    if (!GduTest_FlightControlMoveByPositionOffset((T_GduTestFlightControlVector3f) {-6, -6, 0}, 0, 0.8, 1)) {
        USER_LOG_ERROR("Move to north:-6(m), earth:-6(m), up:0(m) , yaw:0(degree) from current point failed");
        goto out;
    }

    USER_LOG_INFO("--> Step 6: Landing\r\n");
    GduTest_WidgetLogAppend("--> Step 6: Landing\r\n");
    if (!GduTest_FlightControlMonitoredLanding()) {
        USER_LOG_ERROR("Landing failed");
        goto out;
    }
    USER_LOG_INFO("Successful landing\r\n");
    GduTest_WidgetLogAppend("Successful landing\r\n");

    USER_LOG_INFO("--> Step 7: Release joystick authority");
    GduTest_WidgetLogAppend("--> Step 7: Release joystick authority");
    returnCode = GduFlightController_ReleaseJoystickCtrlAuthority();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Release joystick authority failed, error code: 0x%08X", returnCode);
        goto out;
    }

out:
    USER_LOG_INFO("Flight control move-by-position sample end");
    GduTest_WidgetLogAppend("Flight control move-by-position sample end");
}

void GduTest_FlightControlGoHomeForceLandingSample()
{
    T_GduReturnCode returnCode;

    USER_LOG_INFO("Flight control go-home-force-landing sample start");
    GduTest_WidgetLogAppend("Flight control go-home-force-landing sample start");

    USER_LOG_INFO("--> Step 1: Obtain joystick control authority");
    GduTest_WidgetLogAppend("--> Step 1: Obtain joystick control authority");
    returnCode = GduFlightController_ObtainJoystickCtrlAuthority();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Obtain joystick authority failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(1000);

    USER_LOG_INFO("--> Step 2: Take off\r\n");
    GduTest_WidgetLogAppend("--> Step 2: Take off\r\n");
    if (!GduTest_FlightControlMonitoredTakeoff()) {
        USER_LOG_ERROR("Take off failed");
        goto out;
    }
    USER_LOG_INFO("Successful take off\r\n");
    GduTest_WidgetLogAppend("Successful take off\r\n");

    USER_LOG_INFO("--> Step 3: Move to north:0(m), earth:0(m), up:30(m) , yaw:0(degree) from current point");
    GduTest_WidgetLogAppend("--> Step 3: Move to north:0(m), earth:0(m), up:30(m) , yaw:0(degree) from current point");
    if (!GduTest_FlightControlMoveByPositionOffset((T_GduTestFlightControlVector3f) {0, 0, 30}, 0, 0.8, 1)) {
        USER_LOG_ERROR("Move to north:0(m), earth:0(m), up:30(m) , yaw:0(degree) from current point failed");
        goto out;
    }

    USER_LOG_INFO("--> Step 4: Move to north:10(m), earth:0(m), up:0(m) , yaw:0(degree) from current point");
    GduTest_WidgetLogAppend("--> Step 4: Move to north:10(m), earth:0(m), up:0(m) , yaw:0(degree) from current point");
    if (!GduTest_FlightControlMoveByPositionOffset((T_GduTestFlightControlVector3f) {10, 0, 0}, 0, 0.8, 1)) {
        USER_LOG_ERROR("Move to north:10(m), earth:0(m), up:0(m) , yaw:0(degree) from current point failed");
        goto out;
    }

    USER_LOG_INFO("--> Step 5: Set aircraft current position as new home location!");
    GduTest_WidgetLogAppend("--> Step 5: Set aircraft current position as new home location!");
    returnCode = GduFlightController_SetHomeLocationUsingCurrentAircraftLocation();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set aircraft current position as new home location failed, error code: 0x%08X", returnCode);
        goto out;
    }

    USER_LOG_INFO("--> Step 6: Set go home altitude to 50(m)\r\n");
    GduTest_WidgetLogAppend("--> Step 6: Set go home altitude to 50(m)\r\n");
    returnCode = GduFlightController_SetGoHomeAltitude(50);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set go home altitude to 50(m) failed, error code: 0x%08X", returnCode);
        goto out;
    }

    /*! get go home altitude */
    E_GduFlightControllerGoHomeAltitude goHomeAltitude;
    returnCode = GduFlightController_GetGoHomeAltitude(&goHomeAltitude);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get go home altitude failed, error code: 0x%08X", returnCode);
        goto out;
    }
    USER_LOG_INFO("Current go home altitude is %d m\r\n", goHomeAltitude);
    GduTest_WidgetLogAppend("Current go home altitude is %d m\r\n", goHomeAltitude);

    USER_LOG_INFO("--> Step 7: Move to north:20(m), earth:0(m), up:0(m) , yaw:0(degree) from current point");
    GduTest_WidgetLogAppend("--> Step 7: Move to north:20(m), earth:0(m), up:0(m) , yaw:0(degree) from current point");
    if (!GduTest_FlightControlMoveByPositionOffset((T_GduTestFlightControlVector3f) {20, 0, 0}, 0, 0.8, 1)) {
        USER_LOG_ERROR("Move to north:20(m), earth:0(m), up:0(m) , yaw:0(degree) from current point failed");
        goto out;
    }

    USER_LOG_INFO("--> Step 8: Go home and confirm force landing\r\n");
    GduTest_WidgetLogAppend("--> Step 8: Go home and confirm force landing\r\n");
    if (!GduTest_FlightControlGoHomeAndConfirmLanding()) {
        USER_LOG_ERROR("Go home and confirm force landing failed");
        goto out;
    }
    USER_LOG_INFO("Successful go home and confirm force landing\r\n");
    GduTest_WidgetLogAppend("Successful go home and confirm force landing\r\n");

    USER_LOG_INFO("-> Step 9: Release joystick authority");
    GduTest_WidgetLogAppend("-> Step 9: Release joystick authority");
    returnCode = GduFlightController_ReleaseJoystickCtrlAuthority();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Release joystick authority failed, error code: 0x%08X", returnCode);
        goto out;
    }

out:
    USER_LOG_INFO("Flight control go-home-force-landing sample end");
    GduTest_WidgetLogAppend("Flight control go-home-force-landing sample end");
}

void GduTest_FlightControlVelocityControlSample()
{
    T_GduReturnCode returnCode;

    USER_LOG_INFO("Flight control move-by-velocity sample start");
    GduTest_WidgetLogAppend("Flight control move-by-velocity sample start");

    USER_LOG_INFO("--> Step 1: Obtain joystick control authority");
    GduTest_WidgetLogAppend("--> Step 1: Obtain joystick control authority");
    returnCode = GduFlightController_ObtainJoystickCtrlAuthority();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Obtain joystick authority failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(1000);

    USER_LOG_INFO("--> Step 2: Take off\r\n");
    GduTest_WidgetLogAppend("--> Step 2: Take off\r\n");
    if (!GduTest_FlightControlMonitoredTakeoff()) {
        USER_LOG_ERROR("Take off failed");
        goto out;
    }
    USER_LOG_INFO("Successful take off\r\n");
    GduTest_WidgetLogAppend("Successful take off\r\n");

    USER_LOG_INFO(
        "--> Step 3: Move with north:0(m/s), earth:0(m/s), up:5(m/s), yaw:0(deg/s) from current point for 2s!");
    GduTest_WidgetLogAppend(
        "--> Step 3: Move with north:0(m/s), earth:0(m/s), up:5(m/s), yaw:0(deg/s) from current point for 2s!");
    GduTest_FlightControlVelocityAndYawRateCtrl((T_GduTestFlightControlVector3f) {0, 0, 5.0}, 0, 2000);

    USER_LOG_INFO("--> Step 4: Emergency brake for 2s");
    GduTest_WidgetLogAppend("--> Step 4: Emergency brake for 2s");
    returnCode = GduFlightController_ExecuteEmergencyBrakeAction();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Emergency brake failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(2000);
    returnCode = GduFlightController_CancelEmergencyBrakeAction();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Cancel emergency brake action failed, error code: 0x%08X", returnCode);
        goto out;
    }

    USER_LOG_INFO(
        "--> Step 5: Move with north:-1.5(m/s), earth:2(m/s), up:0(m/s), yaw:20(deg/s) from current point for 2s!");
    GduTest_WidgetLogAppend(
        "--> Step 5: Move with north:-1.5(m/s), earth:2(m/s), up:0(m/s), yaw:20(deg/s) from current point for 2s!");
    GduTest_FlightControlVelocityAndYawRateCtrl((T_GduTestFlightControlVector3f) {-1.5, 2, 0}, 20, 2000);

    USER_LOG_INFO("--> Step 6: Emergency brake for 2s");
    GduTest_WidgetLogAppend("--> Step 6: Emergency brake for 2s");
    returnCode = GduFlightController_ExecuteEmergencyBrakeAction();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Emergency brake failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(2000);
    returnCode = GduFlightController_CancelEmergencyBrakeAction();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Cancel emergency brake action failed, error code: 0x%08X", returnCode);
        goto out;
    }

    USER_LOG_INFO(
        "--> Step 7: Move with north:3(m/s), earth:0(m/s), up:0(m/s), yaw:0(deg/s) from current point for 2.5s!");
    GduTest_WidgetLogAppend(
        "--> Step 7: Move with north:3(m/s), earth:0(m/s), up:0(m/s), yaw:0(deg/s) from current point for 2.5s!");
    GduTest_FlightControlVelocityAndYawRateCtrl((T_GduTestFlightControlVector3f) {3, 0, 0}, 0, 2500);

    USER_LOG_INFO("--> Step 8: Emergency brake for 2s");
    GduTest_WidgetLogAppend("--> Step 8: Emergency brake for 2s");
    returnCode = GduFlightController_ExecuteEmergencyBrakeAction();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Emergency brake failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(2000);
    returnCode = GduFlightController_CancelEmergencyBrakeAction();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Cancel emergency brake action failed, error code: 0x%08X", returnCode);
        goto out;
    }

    USER_LOG_INFO(
        "--> Step 9: Move with north:-1.6(m/s), earth:-2(m/s), up:0(m/s), yaw:0(deg/s) from current point for 2.2s!");
    GduTest_WidgetLogAppend(
        "--> Step 9: Move with north:-1.6(m/s), earth:-2(m/s), up:0(m/s), yaw:0(deg/s) from current point for 2.2s!");
    GduTest_FlightControlVelocityAndYawRateCtrl((T_GduTestFlightControlVector3f) {-1.6, -2, 0}, 0, 2200);

    USER_LOG_INFO("--> Step 10: Emergency brake for 2s");
    GduTest_WidgetLogAppend("--> Step 10: Emergency brake for 2s");
    returnCode = GduFlightController_ExecuteEmergencyBrakeAction();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Emergency brake failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(2000);
    returnCode = GduFlightController_CancelEmergencyBrakeAction();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Cancel emergency brake action failed, error code: 0x%08X", returnCode);
        goto out;
    }

    USER_LOG_INFO("--> Step 11: Landing\r\n");
    GduTest_WidgetLogAppend("--> Step 11: Landing\r\n");
    if (!GduTest_FlightControlMonitoredLanding()) {
        USER_LOG_ERROR("Landing failed");
        goto out;
    }
    USER_LOG_INFO("Successful landing\r\n");
    GduTest_WidgetLogAppend("Successful landing\r\n");

    USER_LOG_INFO("--> Step 12: Release joystick authority");
    GduTest_WidgetLogAppend("--> Step 12: Release joystick authority");
    returnCode = GduFlightController_ReleaseJoystickCtrlAuthority();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Release joystick authority failed, error code: 0x%08X", returnCode);
        goto out;
    }

out:
    USER_LOG_INFO("Flight control move-by-velocity sample end");
    GduTest_WidgetLogAppend("Flight control move-by-velocity sample end");
}

void GduTest_FlightControlArrestFlyingSample()
{
    T_GduReturnCode returnCode;

    USER_LOG_INFO("Flight control arrest-flying sample start");
    GduTest_WidgetLogAppend("Flight control arrest-flying sample start");

    USER_LOG_INFO("--> Step 1: Enable arrest-flying");
    GduTest_WidgetLogAppend("--> Step 1: Enable arrest-flying");
    returnCode = GduFlightController_ArrestFlying();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Enable arrest-flying failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(2000);

    //you can replace with takeoff to test in air.
    USER_LOG_INFO("--> Step 2: Turn on motors\r\n");
    GduTest_WidgetLogAppend("--> Step 2: Turn on motors\r\n");
    returnCode = GduFlightController_TurnOnMotors();
    if (returnCode == GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Turn on motors successfully, but arrest-flying failed");
        s_osalHandler->TaskSleepMs(4000);
        USER_LOG_INFO("--> Step 3: Turn off motors\r\n");
        GduTest_WidgetLogAppend("--> Step 3: Turn off motors\r\n");
        returnCode = GduFlightController_TurnOffMotors();
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("Turn off motors failed, error code: 0x%08X", returnCode);
        }
        goto out;
    }

    USER_LOG_INFO("Turn on motors failed.Arrest-flying successfully\r\n");
    GduTest_WidgetLogAppend("Turn on motors failed.Arrest-flying successfully\r\n");
    s_osalHandler->TaskSleepMs(2000);

    USER_LOG_INFO("--> Step 3: Disable arrest-flying");
    GduTest_WidgetLogAppend("--> Step 3: Disable arrest-flying");
    returnCode = GduFlightController_CancelArrestFlying();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Disable arrest-flying failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(2000);

    USER_LOG_INFO("--> Step 4: Turn on motors\r\n");
    GduTest_WidgetLogAppend("--> Step 4: Turn on motors\r\n");
    returnCode = GduFlightController_TurnOnMotors();
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Turn on motors failed and disable arrest-flying failed, error code: 0x%08X", returnCode);
        goto out;
    } else {
        USER_LOG_INFO("Turn on motors successfully and disable arrest-flying successfully\r\n");
        s_osalHandler->TaskSleepMs(4000);
        USER_LOG_INFO("--> Step 5: Turn off motors");
        GduTest_WidgetLogAppend("--> Step 5: Turn off motors");
        returnCode = GduFlightController_TurnOffMotors();
        if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("Turn off motors failed, error code: 0x%08X", returnCode);
        }
    }

out:
    USER_LOG_INFO("Flight control arrest-flying sample end");
    GduTest_WidgetLogAppend("Flight control arrest-flying sample end");
}

void GduTest_FlightControlSetGetParamSample()
{
    T_GduReturnCode returnCode;
    E_GduFlightControllerObstacleAvoidanceEnableStatus horizontalVisualObstacleAvoidanceStatus;
    E_GduFlightControllerObstacleAvoidanceEnableStatus horizontalRadarObstacleAvoidanceStatus;
    E_GduFlightControllerObstacleAvoidanceEnableStatus upwardsVisualObstacleAvoidanceStatus;
    E_GduFlightControllerObstacleAvoidanceEnableStatus upwardsRadarObstacleAvoidanceStatus;
    E_GduFlightControllerObstacleAvoidanceEnableStatus downloadsVisualObstacleAvoidanceStatus;
    E_GduFlightControllerGoHomeAltitude goHomeAltitude;
    E_GduFlightControllerRtkPositionEnableStatus rtkEnableStatus;
    E_GduFlightControllerRCLostAction rcLostAction;

    USER_LOG_INFO("Flight control set-get-param sample start");
    GduTest_WidgetLogAppend("Flight control set-get-param sample start");

    /*! Turn on horizontal vision avoid enable */
    USER_LOG_INFO("--> Step 1: Turn on horizontal visual obstacle avoidance");
    GduTest_WidgetLogAppend("--> Step 1: Turn on horizontal visual obstacle avoidance");
    returnCode = GduFlightController_SetHorizontalVisualObstacleAvoidanceEnableStatus(
        GDU_FLIGHT_CONTROLLER_ENABLE_OBSTACLE_AVOIDANCE);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Turn on horizontal visual obstacle avoidance failed, error code: 0x%08X", returnCode);
        goto out;
    };
    s_osalHandler->TaskSleepMs(1000);

    USER_LOG_INFO("--> Step 2: Get horizontal horizontal visual obstacle status\r\n");
    GduTest_WidgetLogAppend("--> Step 2: Get horizontal horizontal visual obstacle status\r\n");
    returnCode = GduFlightController_GetHorizontalVisualObstacleAvoidanceEnableStatus(
        &horizontalVisualObstacleAvoidanceStatus);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get horizontal visual obstacle avoidance failed, error code: 0x%08X", returnCode);
        goto out;
    }
    USER_LOG_INFO("Current horizontal visual obstacle avoidance status is %d\r\n",
                  horizontalVisualObstacleAvoidanceStatus);
    s_osalHandler->TaskSleepMs(1000);

    /*! Turn on horizontal radar avoid enable */
    USER_LOG_INFO("--> Step 3: Turn on horizontal radar obstacle avoidance");
    GduTest_WidgetLogAppend("--> Step 3: Turn on horizontal radar obstacle avoidance");
    returnCode = GduFlightController_SetHorizontalRadarObstacleAvoidanceEnableStatus(
        GDU_FLIGHT_CONTROLLER_ENABLE_OBSTACLE_AVOIDANCE);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Turn on horizontal radar obstacle avoidance failed, error code: 0x%08X", returnCode);
        goto out;
    };
    s_osalHandler->TaskSleepMs(1000);

    USER_LOG_INFO("--> Step 4: Get horizontal radar obstacle avoidance status\r\n");
    GduTest_WidgetLogAppend("--> Step 4: Get horizontal radar obstacle avoidance status\r\n");
    returnCode = GduFlightController_GetHorizontalRadarObstacleAvoidanceEnableStatus(
        &horizontalRadarObstacleAvoidanceStatus);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get horizontal radar obstacle avoidance failed, error code: 0x%08X", returnCode);
        goto out;
    }
    USER_LOG_INFO("Current horizontal radar obstacle avoidance status is %d\r\n",
                  horizontalRadarObstacleAvoidanceStatus);
    s_osalHandler->TaskSleepMs(1000);

    /*! Turn on upwards vision avoid enable */
    USER_LOG_INFO("--> Step 5: Turn on upwards visual obstacle avoidance.");
    GduTest_WidgetLogAppend("--> Step 5: Turn on upwards visual obstacle avoidance.");
    returnCode = GduFlightController_SetUpwardsVisualObstacleAvoidanceEnableStatus(
        GDU_FLIGHT_CONTROLLER_ENABLE_OBSTACLE_AVOIDANCE);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Turn on upwards visual obstacle avoidance failed, error code: 0x%08X", returnCode);
        goto out;
    };
    s_osalHandler->TaskSleepMs(1000);

    USER_LOG_INFO("--> Step 6: Get upwards visual obstacle avoidance status\r\n");
    GduTest_WidgetLogAppend("--> Step 6: Get upwards visual obstacle avoidance status\r\n");
    returnCode = GduFlightController_GetUpwardsVisualObstacleAvoidanceEnableStatus(
        &upwardsVisualObstacleAvoidanceStatus);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get upwards visual obstacle avoidance failed, error code: 0x%08X", returnCode);
        goto out;
    }
    USER_LOG_INFO("Current upwards visual obstacle avoidance status is %d\r\n", upwardsVisualObstacleAvoidanceStatus);
    s_osalHandler->TaskSleepMs(1000);

    /*! Turn on upwards radar avoid enable */
    USER_LOG_INFO("--> Step 7: Turn on upwards radar obstacle avoidance.");
    GduTest_WidgetLogAppend("--> Step 7: Turn on upwards radar obstacle avoidance.");
    returnCode = GduFlightController_SetUpwardsRadarObstacleAvoidanceEnableStatus(
        GDU_FLIGHT_CONTROLLER_ENABLE_OBSTACLE_AVOIDANCE);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Turn on upwards radar obstacle avoidance failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(1000);

    USER_LOG_INFO("--> Step 8: Get upwards radar obstacle avoidance status\r\n");
    GduTest_WidgetLogAppend("--> Step 8: Get upwards radar obstacle avoidance status\r\n");
    returnCode = GduFlightController_GetUpwardsRadarObstacleAvoidanceEnableStatus(&upwardsRadarObstacleAvoidanceStatus);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get upwards radar obstacle avoidance failed, error code: 0x%08X", returnCode);
        goto out;
    }
    USER_LOG_INFO("Current upwards radar obstacle avoidance status is %d\r\n", upwardsRadarObstacleAvoidanceStatus);
    s_osalHandler->TaskSleepMs(1000);

    /*! Turn on downwards vision avoid enable */
    USER_LOG_INFO("--> Step 9: Turn on downwards visual obstacle avoidance.");
    GduTest_WidgetLogAppend("--> Step 9: Turn on downwards visual obstacle avoidance.");
    returnCode = GduFlightController_SetDownwardsVisualObstacleAvoidanceEnableStatus(
        GDU_FLIGHT_CONTROLLER_ENABLE_OBSTACLE_AVOIDANCE);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Turn on downwards visual obstacle avoidance failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(1000);

    USER_LOG_INFO("--> Step 10: Get downwards visual obstacle avoidance status\r\n");
    GduTest_WidgetLogAppend("--> Step 10: Get downwards visual obstacle avoidance status\r\n");
    returnCode = GduFlightController_GetDownwardsVisualObstacleAvoidanceEnableStatus(
        &downloadsVisualObstacleAvoidanceStatus);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get downwards visual obstacle avoidance failed, error code: 0x%08X", returnCode);
        goto out;
    }
    USER_LOG_INFO("Current downwards visual obstacle avoidance status is %d\r\n",
                  downloadsVisualObstacleAvoidanceStatus);
    s_osalHandler->TaskSleepMs(1000);

    /*! Set new go home altitude */
    USER_LOG_INFO("--> Step 11: Set go home altitude to 50(m)");
    GduTest_WidgetLogAppend("--> Step 11: Set go home altitude to 50(m)");
    returnCode = GduFlightController_SetGoHomeAltitude(50);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set go home altitude to 50(m) failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(1000);

    /*! get go home altitude */
    USER_LOG_INFO("--> Step 12: Get go home altitude\r\n");
    GduTest_WidgetLogAppend("--> Step 12: Get go home altitude\r\n");
    returnCode = GduFlightController_GetGoHomeAltitude(&goHomeAltitude);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get go home altitude failed, error code: 0x%08X", returnCode);
        goto out;
    }
    USER_LOG_INFO("Current go home altitude is %d m\r\n", goHomeAltitude);
    s_osalHandler->TaskSleepMs(2000);

    /*! Set rtk enable */
    USER_LOG_INFO("--> Step 13: Set rtk enable status");
    GduTest_WidgetLogAppend("--> Step 13: Set rtk enable status");
    returnCode = GduFlightController_SetRtkPositionEnableStatus(GDU_FLIGHT_CONTROLLER_ENABLE_RTK_POSITION);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set rtk enable failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(1000);

    USER_LOG_INFO("--> Step 14: Get rtk enable status\r\n");
    GduTest_WidgetLogAppend("--> Step 14: Get rtk enable status\r\n");
    returnCode = GduFlightController_GetRtkPositionEnableStatus(&rtkEnableStatus);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get rtk enable failed, error code: 0x%08X", returnCode);
        goto out;
    }
    USER_LOG_INFO("Current rtk enable status is %d\r\n", rtkEnableStatus);
    s_osalHandler->TaskSleepMs(1000);

    /*! Set rc lost action */
    USER_LOG_INFO("--> Step 15: Set rc lost action");
    GduTest_WidgetLogAppend("--> Step 15: Set rc lost action");
    returnCode = GduFlightController_SetRCLostAction(GDU_FLIGHT_CONTROLLER_RC_LOST_ACTION_GOHOME);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set rc lost action failed, error code: 0x%08X", returnCode);
        goto out;
    }
    s_osalHandler->TaskSleepMs(1000);

    USER_LOG_INFO("--> Step 16: Get rc lost action\r\n");
    GduTest_WidgetLogAppend("--> Step 16: Get rc lost action\r\n");
    returnCode = GduFlightController_GetRCLostAction(&rcLostAction);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get rc lost action failed, error code: 0x%08X", returnCode);
        goto out;
    }
    USER_LOG_INFO("Current rc lost action is %d\r\n", rcLostAction);
    GduTest_WidgetLogAppend("Current rc lost action is %d\r\n", rcLostAction);
    s_osalHandler->TaskSleepMs(1000);

out:
    USER_LOG_INFO("Flight control set-get-param sample end");
    GduTest_WidgetLogAppend("Flight control set-get-param sample end");
}

void GduTest_FlightControlSample(E_GduTestFlightCtrlSampleSelect flightCtrlSampleSelect)
{
    switch (flightCtrlSampleSelect) {
        case E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_LANDING: {
            GduTest_FlightControlTakeOffLandingSample();
            break;
        }
        case E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_POSITION_CTRL_LANDING: {
            GduTest_FlightControlPositionControlSample();
            break;
        }
        case E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_GO_HOME_FORCE_LANDING: {
            GduTest_FlightControlGoHomeForceLandingSample();
            break;
        }
        case E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_VELOCITY_CTRL_LANDING: {
            GduTest_FlightControlVelocityControlSample();
            break;
        }
        case E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_ARREST_FLYING: {
            GduTest_FlightControlArrestFlyingSample();
            break;
        }
        case E_GDU_TEST_FLIGHT_CTRL_SAMPLE_SELECT_SET_GET_PARAM: {
            GduTest_FlightControlSetGetParamSample();
            break;
        }
        default:
            break;
    }
}

uint8_t GduTest_FlightControlGetDisplayModeIndex(E_GduFcSubscriptionDisplayMode displayMode)
{
    uint8_t i;

    for (i = 0; i < sizeof(s_flightControlDisplayModeStr) / sizeof(T_GduTestFlightControlDisplayModeStr); i++) {
        if (s_flightControlDisplayModeStr[i].displayMode == displayMode) {
            return i;
        }
    }

    return i;
}

T_GduFcSubscriptionFlightStatus GduTest_FlightControlGetValueOfFlightStatus(void)
{
    T_GduReturnCode gduStat;
    T_GduFcSubscriptionFlightStatus flightStatus;
    T_GduDataTimestamp flightStatusTimestamp = {0};

    gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_STATUS_FLIGHT,
                                                      (uint8_t *) &flightStatus,
                                                      sizeof(T_GduFcSubscriptionFlightStatus),
                                                      &flightStatusTimestamp);

    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get value of topic flight status error, error code: 0x%08X", gduStat);
        flightStatus = 0;
    } else {
        USER_LOG_DEBUG("Timestamp: millisecond %u microsecond %u.", flightStatusTimestamp.millisecond,
                       flightStatusTimestamp.microsecond);
        USER_LOG_DEBUG("Flight status: %d.", flightStatus);
    }

    return flightStatus;
}

T_GduFcSubscriptionDisplaymode GduTest_FlightControlGetValueOfDisplayMode(void)
{
    T_GduReturnCode gduStat;
    T_GduFcSubscriptionDisplaymode displayMode;
    T_GduDataTimestamp displayModeTimestamp = {0};

    gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_STATUS_DISPLAYMODE,
                                                      (uint8_t *) &displayMode,
                                                      sizeof(T_GduFcSubscriptionDisplaymode),
                                                      &displayModeTimestamp);

    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get value of topic display mode error, error code: 0x%08X", gduStat);
        displayMode = 0;
    } else {
        USER_LOG_DEBUG("Timestamp: millisecond %u microsecond %u.", displayModeTimestamp.millisecond,
                       displayModeTimestamp.microsecond);
        USER_LOG_DEBUG("Display mode : %d.", displayMode);
    }

    return displayMode;
}

T_GduFcSubscriptionAvoidData GduTest_FlightControlGetValueOfAvoidData(void)
{
    T_GduReturnCode gduStat;
    T_GduFcSubscriptionAvoidData avoidData = {0};
    T_GduDataTimestamp avoidDataTimestamp = {0};

    gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_AVOID_DATA,
                                                      (uint8_t *) &avoidData,
                                                      sizeof(T_GduFcSubscriptionAvoidData),
                                                      &avoidDataTimestamp);

    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get value of topic avoid data error, error code: 0x%08X", gduStat);
    } else {
        USER_LOG_DEBUG("Timestamp: millisecond %u microsecond %u.", avoidDataTimestamp.millisecond,
                       avoidDataTimestamp.microsecond);
        USER_LOG_DEBUG("Avoid downwards distance is %f m", avoidData.down);
    }

    return avoidData;
}

T_GduFcSubscriptionQuaternion GduTest_FlightControlGetValueOfQuaternion(void)
{
    T_GduReturnCode gduStat;
    T_GduFcSubscriptionQuaternion quaternion = {0};
    T_GduDataTimestamp quaternionTimestamp = {0};

    gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_QUATERNION,
                                                      (uint8_t *) &quaternion,
                                                      sizeof(T_GduFcSubscriptionQuaternion),
                                                      &quaternionTimestamp);

    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get value of topic quaternion error, error code: 0x%08X", gduStat);
    } else {
        USER_LOG_DEBUG("Timestamp: millisecond %u microsecond %u.", quaternionTimestamp.millisecond,
                       quaternionTimestamp.microsecond);
        USER_LOG_DEBUG("Quaternion: %f %f %f %f.", quaternion.q0, quaternion.q1, quaternion.q2, quaternion.q3);
    }

    return quaternion;
}

T_GduFcSubscriptionPositionFused GduTest_FlightControlGetValueOfPositionFused(void)
{
    T_GduReturnCode gduStat;
    T_GduFcSubscriptionPositionFused positionFused = {0};
    T_GduDataTimestamp positionFusedTimestamp = {0};

    gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_POSITION_FUSED,
                                                      (uint8_t *) &positionFused,
                                                      sizeof(T_GduFcSubscriptionPositionFused),
                                                      &positionFusedTimestamp);

    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get value of topic position fused error, error code: 0x%08X", gduStat);
    } else {
        USER_LOG_DEBUG("Timestamp: millisecond %u microsecond %u.", positionFusedTimestamp.millisecond,
                       positionFusedTimestamp.microsecond);
        USER_LOG_DEBUG("PositionFused: %f, %f,%f,%d.", positionFused.latitude, positionFused.longitude,
                       positionFused.altitude, positionFused.visibleSatelliteNumber);
    }

    return positionFused;
}

gdu_f32_t GduTest_FlightControlGetValueOfRelativeHeight(void)
{
    T_GduReturnCode gduStat;
    T_GduFcSubscriptionAltitudeFused altitudeFused = 0;
    T_GduFcSubscriptionAltitudeOfHomePoint homePointAltitude = 0;
    gdu_f32_t relativeHeight = 0;
    T_GduDataTimestamp relativeHeightTimestamp = {0};

    gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_ALTITUDE_OF_HOMEPOINT,
                                                      (uint8_t *) &homePointAltitude,
                                                      sizeof(T_GduFcSubscriptionAltitudeOfHomePoint),
                                                      &relativeHeightTimestamp);

    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get value of topic altitude of home point error, error code: 0x%08X", gduStat);
        return -1;
    } else {
        USER_LOG_DEBUG("Timestamp: millisecond %u microsecond %u.", relativeHeightTimestamp.millisecond,
                       relativeHeightTimestamp.microsecond);
    }

    gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_ALTITUDE_FUSED,
                                                      (uint8_t *) &altitudeFused,
                                                      sizeof(T_GduFcSubscriptionAltitudeFused),
                                                      &relativeHeightTimestamp);

    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get value of topic altitude fused error, error code: 0x%08X", gduStat);
        return -1;
    } else {
        USER_LOG_DEBUG("Timestamp: millisecond %u microsecond %u.", relativeHeightTimestamp.millisecond,
                       relativeHeightTimestamp.microsecond);
    }

    relativeHeight = altitudeFused - homePointAltitude;

    return relativeHeight;
}

bool GduTest_FlightControlMotorStartedCheck(void)
{
    int motorsNotStarted = 0;
    int timeoutCycles = 20;

    while (GduTest_FlightControlGetValueOfFlightStatus() != GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_ON_GROUND &&
           GduTest_FlightControlGetValueOfDisplayMode() != GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ENGINE_START &&
           motorsNotStarted < timeoutCycles) {
        motorsNotStarted++;
        s_osalHandler->TaskSleepMs(100);
    }
    return motorsNotStarted != timeoutCycles ? true : false;
}

bool GduTest_FlightControlTakeOffInAirCheck(void)
{
    int stillOnGround = 0;
    int timeoutCycles = 110;

    while (GduTest_FlightControlGetValueOfFlightStatus() != GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_IN_AIR &&
           (GduTest_FlightControlGetValueOfDisplayMode() != GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ASSISTED_TAKEOFF ||
            GduTest_FlightControlGetValueOfDisplayMode() != GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_TAKEOFF) &&
           stillOnGround < timeoutCycles) {
        stillOnGround++;
        s_osalHandler->TaskSleepMs(100);
    }

    return stillOnGround != timeoutCycles ? true : false;
}

bool takeoffFinishedCheck(void)
{
    while (GduTest_FlightControlGetValueOfDisplayMode() == GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_TAKEOFF ||
           GduTest_FlightControlGetValueOfDisplayMode() == GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ASSISTED_TAKEOFF) {
        s_osalHandler->TaskSleepMs(1000);
    }

    return (GduTest_FlightControlGetValueOfDisplayMode() == GDU_FC_SUBSCRIPTION_DISPLAY_MODE_P_GPS ||
            GduTest_FlightControlGetValueOfDisplayMode() == GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ATTITUDE) ? true : false;
}

bool GduTest_FlightControlLandFinishedCheck(void)
{
    while (GduTest_FlightControlGetValueOfDisplayMode() == GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_LANDING ||
           GduTest_FlightControlGetValueOfFlightStatus() == GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_IN_AIR) {
        s_osalHandler->TaskSleepMs(1000);
    }

    return (GduTest_FlightControlGetValueOfDisplayMode() != GDU_FC_SUBSCRIPTION_DISPLAY_MODE_P_GPS ||
            GduTest_FlightControlGetValueOfDisplayMode() != GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ATTITUDE) ? true : false;
}

bool GduTest_FlightControlCheckActionStarted(E_GduFcSubscriptionDisplayMode mode)
{
    int actionNotStarted = 0;
    int timeoutCycles = 20;

    while (GduTest_FlightControlGetValueOfDisplayMode() != mode && actionNotStarted < timeoutCycles) {
        actionNotStarted++;
        s_osalHandler->TaskSleepMs(100);
    }

    if (actionNotStarted == timeoutCycles) {
        USER_LOG_ERROR("%s start failed, now flight is in %s.",
                       s_flightControlDisplayModeStr[GduTest_FlightControlGetDisplayModeIndex(mode)].displayModeStr,
                       s_flightControlDisplayModeStr[GduTest_FlightControlGetDisplayModeIndex(
                           GduTest_FlightControlGetValueOfDisplayMode())].displayModeStr);
        return false;
    } else {
        USER_LOG_INFO("Now flight is in %s.",
                      s_flightControlDisplayModeStr[GduTest_FlightControlGetDisplayModeIndex(mode)].displayModeStr);
        return true;
    }
}

bool GduTest_FlightControlMonitoredTakeoff(void)
{
    T_GduReturnCode gduStat;

    //! Start takeoff
    gduStat = GduFlightController_StartTakeoff();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Request to take off failed, error code: 0x%08X", gduStat);
        return false;
    }

    //! Motors start check
    if (!GduTest_FlightControlMotorStartedCheck()) {
        USER_LOG_ERROR("Takeoff failed. Motors are not spinning.");
        return false;
    } else {
        USER_LOG_INFO("Motors spinning...");
    }
    //! In air check
    if (!GduTest_FlightControlTakeOffInAirCheck()) {
        USER_LOG_ERROR("Takeoff failed. Aircraft is still on the ground, but the "
                       "motors are spinning");
        return false;
    } else {
        USER_LOG_INFO("Ascending...");
    }
    //! Finished takeoff check
    if (!takeoffFinishedCheck()) {
        USER_LOG_ERROR("Takeoff finished, but the aircraft is in an unexpected mode. "
                       "Please connect GDU GO.");
        return false;
    }

    return true;
}

bool GduTest_FlightControlMonitoredLanding(void)
{
    T_GduReturnCode gduStat;
    /*! Step 1: Start landing */
    gduStat = GduFlightController_StartLanding();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Start landing failed, error code: 0x%08X", gduStat);
        return false;
    }

    /*! Step 2: check Landing start*/
    if (!GduTest_FlightControlCheckActionStarted(GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_LANDING)) {
        USER_LOG_ERROR("Fail to execute Landing action!");
        return false;
    } else {
        /*! Step 3: check Landing finished*/
        if (!GduTest_FlightControlLandFinishedCheck()) {
            USER_LOG_ERROR("Landing finished, but the aircraft is in an unexpected mode. "
                           "Please connect GDU Assistant.");
            return false;
        }
    }

    return true;
}

bool GduTest_FlightControlGoHomeAndConfirmLanding(void)
{
    T_GduReturnCode gduStat;

    /*! Step 1: Start go home */
    USER_LOG_INFO("Start go home action");
    gduStat = GduFlightController_StartGoHome();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Start to go home failed, error code: 0x%08X", gduStat);
        return false;;
    }

    if (!GduTest_FlightControlCheckActionStarted(GDU_FC_SUBSCRIPTION_DISPLAY_MODE_NAVI_GO_HOME)) {
        return false;
    } else {
        while (GduTest_FlightControlGetValueOfFlightStatus() == GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_IN_AIR &&
               GduTest_FlightControlGetValueOfDisplayMode() == GDU_FC_SUBSCRIPTION_DISPLAY_MODE_NAVI_GO_HOME) {
            s_osalHandler->TaskSleepMs(1000);// waiting for this action finished
        }
    }

    /*! Step 2: Start landing */
    USER_LOG_INFO("Start landing action");
    if (!GduTest_FlightControlCheckActionStarted(GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_LANDING)) {
        USER_LOG_ERROR("Fail to execute Landing action");
        return false;
    } else {
        while (GduTest_FlightControlGetValueOfDisplayMode() == GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_LANDING &&
               GduTest_FlightControlGetValueOfFlightStatus() == GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_IN_AIR) {
            T_GduFcSubscriptionAvoidData avoidData = GduTest_FlightControlGetValueOfAvoidData();
            s_osalHandler->TaskSleepMs(1000);
            if (((gdu_f64_t) 0.65 < avoidData.down && avoidData.down < (gdu_f64_t) 0.75) &&
                avoidData.downHealth == 1) {
                break;
            }
        }
    }

    /*! Step 4: Confirm Landing */
    USER_LOG_INFO("Start confirm Landing and avoid ground action");
    gduStat = GduFlightController_StartConfirmLanding();
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Fail to execute confirm landing avoid ground action, error code: 0x%08X", gduStat);
        return false;
    }

    if (!GduTest_FlightControlCheckActionStarted(GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_LANDING)) {
        return false;
    } else {
        while (GduTest_FlightControlGetValueOfFlightStatus() == GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_IN_AIR &&
               GduTest_FlightControlGetValueOfDisplayMode() == GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_LANDING) {
            s_osalHandler->TaskSleepMs(1000);
        }
    }

    /*! Step 5: Landing finished check*/
    if (GduTest_FlightControlGetValueOfDisplayMode() != GDU_FC_SUBSCRIPTION_DISPLAY_MODE_P_GPS ||
        GduTest_FlightControlGetValueOfDisplayMode() != GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ATTITUDE) {
        USER_LOG_INFO("Successful landing");
    } else {
        USER_LOG_ERROR("Landing finished, but the aircraft is in an unexpected mode. "
                       "Please connect GDU Assistant.");
        return false;
    }

    return true;
}

T_GduTestFlightControlVector3f GduTest_FlightControlQuaternionToEulerAngle(const T_GduFcSubscriptionQuaternion quat)
{
    T_GduTestFlightControlVector3f eulerAngle;
    double q2sqr = quat.q2 * quat.q2;
    double t0 = -2.0 * (q2sqr + quat.q3 * quat.q3) + 1.0;
    double t1 = (gdu_f64_t) 2.0 * (quat.q1 * quat.q2 + quat.q0 * quat.q3);
    double t2 = -2.0 * (quat.q1 * quat.q3 - quat.q0 * quat.q2);
    double t3 = (gdu_f64_t) 2.0 * (quat.q2 * quat.q3 + quat.q0 * quat.q1);
    double t4 = -2.0 * (quat.q1 * quat.q1 + q2sqr) + 1.0;
    t2 = (t2 > 1.0) ? 1.0 : t2;
    t2 = (t2 < -1.0) ? -1.0 : t2;
    eulerAngle.x = asin(t2);
    eulerAngle.y = atan2(t3, t4);
    eulerAngle.z = atan2(t1, t0);
    return eulerAngle;
}

T_GduTestFlightControlVector3f
GduTest_FlightControlLocalOffsetFromGpsAndFusedHeightOffset(const T_GduFcSubscriptionPositionFused target,
                                                            const T_GduFcSubscriptionPositionFused origin,
                                                            const gdu_f32_t targetHeight,
                                                            const gdu_f32_t originHeight)
{
    T_GduTestFlightControlVector3f deltaNed;
    double deltaLon = target.longitude - origin.longitude;
    double deltaLat = target.latitude - origin.latitude;
    deltaNed.x = deltaLat * s_earthCenter;
    deltaNed.y = deltaLon * s_earthCenter * cos(target.latitude);
    deltaNed.z = targetHeight - originHeight;

    return deltaNed;
}

T_GduTestFlightControlVector3f
GduTest_FlightControlVector3FSub(const T_GduTestFlightControlVector3f vectorA,
                                 const T_GduTestFlightControlVector3f vectorB)
{
    T_GduTestFlightControlVector3f result;
    result.x = vectorA.x - vectorB.x;
    result.y = vectorA.y - vectorB.y;
    result.z = vectorA.z - vectorB.z;
    return result;
}

int GduTest_FlightControlSignOfData(gdu_f32_t data)
{
    return data < 0 ? -1 : 1;
}

void GduTest_FlightControlHorizCommandLimit(gdu_f32_t speedFactor, gdu_f32_t *commandX, gdu_f32_t *commandY)
{
    if (fabs(*commandX) > speedFactor)
        *commandX = speedFactor * GduTest_FlightControlSignOfData(*commandX);
    if (fabs(*commandY) > speedFactor)
        *commandY = speedFactor * GduTest_FlightControlSignOfData(*commandY);
}

gdu_f32_t GduTest_FlightControlVectorNorm(T_GduTestFlightControlVector3f v)
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

bool
GduTest_FlightControlMoveByPositionOffset(const T_GduTestFlightControlVector3f offsetDesired, float yawDesiredInDeg,
                                          float posThresholdInM, float yawThresholdInDeg)
{
    int timeoutInMilSec = 20000;
    int controlFreqInHz = 50;  // Hz
    int cycleTimeInMs = 1000 / controlFreqInHz;
    int outOfControlBoundsTimeLimit = 10 * cycleTimeInMs;    // 10 cycles
    int withinControlBoundsTimeReqmt = 100 * cycleTimeInMs;  // 100 cycles
    int elapsedTimeInMs = 0;
    int withinBoundsCounter = 0;
    int outOfBounds = 0;
    int brakeCounter = 0;
    int speedFactor = 2;

    //! get origin position and relative height(from home point)of aircraft.
    T_GduFcSubscriptionPositionFused originGPSPosition = GduTest_FlightControlGetValueOfPositionFused();
    gdu_f32_t originHeightBaseHomePoint = GduTest_FlightControlGetValueOfRelativeHeight();
    if (originHeightBaseHomePoint == -1) {
        USER_LOG_ERROR("Relative height is invalid!");
        return false;
    }

    T_GduFlightControllerJoystickMode joystickMode = {
        GDU_FLIGHT_CONTROLLER_HORIZONTAL_POSITION_CONTROL_MODE,
        GDU_FLIGHT_CONTROLLER_VERTICAL_POSITION_CONTROL_MODE,
        GDU_FLIGHT_CONTROLLER_YAW_ANGLE_CONTROL_MODE,
        GDU_FLIGHT_CONTROLLER_HORIZONTAL_GROUND_COORDINATE,
        GDU_FLIGHT_CONTROLLER_STABLE_CONTROL_MODE_ENABLE,
    };
    GduFlightController_SetJoystickMode(joystickMode);

    while (elapsedTimeInMs < timeoutInMilSec) {
        T_GduFcSubscriptionPositionFused currentGPSPosition = GduTest_FlightControlGetValueOfPositionFused();
        T_GduFcSubscriptionQuaternion currentQuaternion = GduTest_FlightControlGetValueOfQuaternion();
        gdu_f32_t currentHeight = GduTest_FlightControlGetValueOfRelativeHeight();
        if (originHeightBaseHomePoint == -1) {
            USER_LOG_ERROR("Relative height is invalid!");
            return false;
        }

        float yawInRad = GduTest_FlightControlQuaternionToEulerAngle(currentQuaternion).z;
        //! get the vector between aircraft and origin point.

        T_GduTestFlightControlVector3f localOffset = GduTest_FlightControlLocalOffsetFromGpsAndFusedHeightOffset(
            currentGPSPosition,
            originGPSPosition,
            currentHeight,
            originHeightBaseHomePoint);
        //! get the vector between aircraft and target point.
        T_GduTestFlightControlVector3f offsetRemaining = GduTest_FlightControlVector3FSub(offsetDesired, localOffset);

        T_GduTestFlightControlVector3f positionCommand = offsetRemaining;
        GduTest_FlightControlHorizCommandLimit(speedFactor, &positionCommand.x, &positionCommand.y);

        T_GduFlightControllerJoystickCommand joystickCommand = {positionCommand.x, positionCommand.y,
                                                                offsetDesired.z + originHeightBaseHomePoint,
                                                                yawDesiredInDeg};
        GduFlightController_ExecuteJoystickAction(joystickCommand);

        if (GduTest_FlightControlVectorNorm(offsetRemaining) < posThresholdInM &&
            fabs(yawInRad / s_degToRad - yawDesiredInDeg) < yawThresholdInDeg) {
            //! 1. We are within bounds; start incrementing our in-bound counter
            withinBoundsCounter += cycleTimeInMs;
        } else {
            if (withinBoundsCounter != 0) {
                //! 2. Start incrementing an out-of-bounds counter
                outOfBounds += cycleTimeInMs;
            }
        }
        //! 3. Reset withinBoundsCounter if necessary
        if (outOfBounds > outOfControlBoundsTimeLimit) {
            withinBoundsCounter = 0;
            outOfBounds = 0;
        }
        //! 4. If within bounds, set flag and break
        if (withinBoundsCounter >= withinControlBoundsTimeReqmt) {
            break;
        }
        s_osalHandler->TaskSleepMs(cycleTimeInMs);
        elapsedTimeInMs += cycleTimeInMs;
    }

    while (brakeCounter < withinControlBoundsTimeReqmt) {
        s_osalHandler->TaskSleepMs(cycleTimeInMs);
        brakeCounter += cycleTimeInMs;
    }

    if (elapsedTimeInMs >= timeoutInMilSec) {
        USER_LOG_ERROR("Task timeout!");
        return false;
    }

    return true;
}

void GduTest_FlightControlVelocityAndYawRateCtrl(const T_GduTestFlightControlVector3f offsetDesired, float yawRate,
                                                 uint32_t timeMs)
{
    uint32_t originTime = 0;
    uint32_t currentTime = 0;
    uint32_t elapsedTimeInMs = 0;
    s_osalHandler->GetTimeMs(&originTime);
    s_osalHandler->GetTimeMs(&currentTime);
    elapsedTimeInMs = currentTime - originTime;
    T_GduFlightControllerJoystickMode joystickMode = {
        GDU_FLIGHT_CONTROLLER_HORIZONTAL_VELOCITY_CONTROL_MODE,
        GDU_FLIGHT_CONTROLLER_VERTICAL_VELOCITY_CONTROL_MODE,
        GDU_FLIGHT_CONTROLLER_YAW_ANGLE_RATE_CONTROL_MODE,
        GDU_FLIGHT_CONTROLLER_HORIZONTAL_GROUND_COORDINATE,
        GDU_FLIGHT_CONTROLLER_STABLE_CONTROL_MODE_ENABLE,
    };

    GduFlightController_SetJoystickMode(joystickMode);
    T_GduFlightControllerJoystickCommand joystickCommand = {offsetDesired.x, offsetDesired.y, offsetDesired.z,
                                                            yawRate};

    while (elapsedTimeInMs <= timeMs) {
        GduFlightController_ExecuteJoystickAction(joystickCommand);
        s_osalHandler->TaskSleepMs(2);
        s_osalHandler->GetTimeMs(&currentTime);
        elapsedTimeInMs = currentTime - originTime;
    }
}

T_GduReturnCode
GduTest_FlightControlJoystickCtrlAuthSwitchEventCallback(T_GduFlightControllerJoystickCtrlAuthorityEventInfo eventData)
{
    switch (eventData.joystickCtrlAuthoritySwitchEvent) {
        case GDU_FLIGHT_CONTROLLER_MSDK_GET_JOYSTICK_CTRL_AUTH_EVENT: {
            if (eventData.curJoystickCtrlAuthority == GDU_FLIGHT_CONTROLLER_JOYSTICK_CTRL_AUTHORITY_MSDK) {
                USER_LOG_INFO("[Event]Msdk request to obtain joystick ctrl authority\r\n");
            } else {
                USER_LOG_INFO("[Event]Msdk request to release joystick ctrl authority\r\n");
            }
            break;
        }
        case GDU_FLIGHT_CONTROLLER_INTERNAL_GET_JOYSTICK_CTRL_AUTH_EVENT: {
            if (eventData.curJoystickCtrlAuthority == GDU_FLIGHT_CONTROLLER_JOYSTICK_CTRL_AUTHORITY_INTERNAL) {
                USER_LOG_INFO("[Event]Internal request to obtain joystick ctrl authority\r\n");
            } else {
                USER_LOG_INFO("[Event]Internal request to release joystick ctrl authority\r\n");
            }
            break;
        }
        case GDU_FLIGHT_CONTROLLER_OSDK_GET_JOYSTICK_CTRL_AUTH_EVENT: {
            if (eventData.curJoystickCtrlAuthority == GDU_FLIGHT_CONTROLLER_JOYSTICK_CTRL_AUTHORITY_OSDK) {
                USER_LOG_INFO("[Event]Osdk request to obtain joystick ctrl authority\r\n");
            } else {
                USER_LOG_INFO("[Event]Osdk request to release joystick ctrl authority\r\n");
            }
            break;
        }
        case GDU_FLIGHT_CONTROLLER_RC_LOST_GET_JOYSTICK_CTRL_AUTH_EVENT :
            USER_LOG_INFO("[Event]Current joystick ctrl authority is reset to rc due to rc lost\r\n");
            break;
        case GDU_FLIGHT_CONTROLLER_RC_NOT_P_MODE_RESET_JOYSTICK_CTRL_AUTH_EVENT :
            USER_LOG_INFO("[Event]Current joystick ctrl authority is reset to rc for rc is not in P mode\r\n");
            break;
        case GDU_FLIGHT_CONTROLLER_RC_SWITCH_MODE_GET_JOYSTICK_CTRL_AUTH_EVENT :
            USER_LOG_INFO("[Event]Current joystick ctrl authority is reset to rc due to rc switching mode\r\n");
            break;
        case GDU_FLIGHT_CONTROLLER_RC_PAUSE_GET_JOYSTICK_CTRL_AUTH_EVENT :
            USER_LOG_INFO("[Event]Current joystick ctrl authority is reset to rc due to rc pausing\r\n");
            break;
        case GDU_FLIGHT_CONTROLLER_RC_REQUEST_GO_HOME_GET_JOYSTICK_CTRL_AUTH_EVENT :
            USER_LOG_INFO("[Event]Current joystick ctrl authority is reset to rc due to rc request for return\r\n");
            break;
        case GDU_FLIGHT_CONTROLLER_LOW_BATTERY_GO_HOME_RESET_JOYSTICK_CTRL_AUTH_EVENT :
            USER_LOG_INFO("[Event]Current joystick ctrl authority is reset to rc for low battery return\r\n");
            break;
        case GDU_FLIGHT_CONTROLLER_LOW_BATTERY_LANDING_RESET_JOYSTICK_CTRL_AUTH_EVENT :
            USER_LOG_INFO("[Event]Current joystick ctrl authority is reset to rc for low battery land\r\n");
            break;
        case GDU_FLIGHT_CONTROLLER_OSDK_LOST_GET_JOYSTICK_CTRL_AUTH_EVENT:
            USER_LOG_INFO("[Event]Current joystick ctrl authority is reset to rc due to osdk lost\r\n");
            break;
        case GDU_FLIGHT_CONTROLLER_NERA_FLIGHT_BOUNDARY_RESET_JOYSTICK_CTRL_AUTH_EVENT :
            USER_LOG_INFO("[Event]Current joystick ctrl authority is reset to rc due to near boundary\r\n");
            break;
        default:
            USER_LOG_INFO("[Event]Unknown joystick ctrl authority event\r\n");
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT GDU Innovations *****END OF FILE****/
