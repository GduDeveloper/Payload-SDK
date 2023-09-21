/**
 ********************************************************************
 * @file    gdu_fc_subscription.h
 * @brief   This is the header file for "gdu_fc_subscription.c", defining the structure and
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
#ifndef GDU_FC_SUBSCRIPTION_H
#define GDU_FC_SUBSCRIPTION_H

/* Includes ------------------------------------------------------------------*/
#include "gdu_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Name of topics that can be subscribed. Topic is minimum data item subscribed.
 */
typedef enum {
    /*!
     * @brief Quaternion of aircraft topic name. Quaternion topic provides aircraft body frame (FRD) to ground frame
     * (NED) rotation. Please refer to ::T_GduFcSubscriptionQuaternion for information about data structure.
     * @details The GDU quaternion follows Hamilton convention (q0 = w, q1 = x, q2 = y, q3 = z).
     * | Angle        | Unit | Accuracy   | Notes                                           |
       |--------------|------|------------|-------------------------------------------------|
       | pitch, roll  | deg  | <1         | in NON-AHRS mode                                |
       | yaw          | deg  | <3         | in well-calibrated compass with fine aligned    |
       | yaw with rtk | deg  | around 1.2 | in RTK heading fixed mode with 1 meter baseline |
     * @datastruct \ref T_GduFcSubscriptionQuaternion
     */
    GDU_FC_SUBSCRIPTION_TOPIC_QUATERNION = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 0),
    /*!
     * @brief Provides aircraft's acceleration w.r.t a ground-fixed \b NEU frame @ up to 200Hz
     * @warning Please note that this data is not in a conventional right-handed frame of reference.
     * @details This is a fusion output from the flight control system. The output is in a right-handed NED frame, but the
     * sign of the Z-axis acceleration is flipped before publishing to this topic. So if you are looking to get acceleration
     * in an NED frame, simply flip the sign of the z-axis value. Beyond that, you can convert using rotations to
     * any right-handed frame of reference.
     * @units m/s<SUP>2</SUP>
     * @datastruct \ref T_GduFcSubscriptionAccelerationGround
     */
   // GDU_FC_SUBSCRIPTION_TOPIC_ACCELERATION_GROUND = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 1),
    /*!
     * @brief Provides aircraft's acceleration w.r.t a body-fixed \b FRU frame @ up to 200Hz
     * @warning Please note that this data is not in a conventional right-handed frame of reference.
     * @details This is a fusion output from the flight control system.
     * @units m/s<SUP>2</SUP>
     * @datastruct \ref T_GduFcSubscriptionAccelerationBody
     */
    GDU_FC_SUBSCRIPTION_TOPIC_ACCELERATION_BODY = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 2),
    /*!
     * @brief Provides aircraft's acceleration in an IMU-centered, body-fixed \b FRD frame @ up to 400Hz
     * @details This is a filtered output from the IMU on board the flight control system.
     * @sensors IMU
     * @units m/s<SUP>2</SUP>
     * @datastruct \ref T_GduFcSubscriptionAccelerationRaw
     */
    //GDU_FC_SUBSCRIPTION_TOPIC_ACCELERATION_RAW = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 3),
    /*!
     * @brief Velocity of aircraft topic name. Velocity topic provides aircraft's velocity in a ground-fixed NEU frame.
     * Please refer to ::T_GduFcSubscriptionVelocity for information about data structure.
     * @warning Please note that this data is not in a conventional right-handed frame of reference.
     * @details This velocity data is a fusion output from the aircraft. Original output is in a right-handed NED frame, but the
     * sign of the Z-axis velocity is flipped before publishing to this topic. So if you are looking to get velocity
     * in an NED frame, simply flip the sign of the z-axis value. Beyond that, you can convert using rotations to
     * any right-handed frame of reference.
     * | Axis     | Unit | Accuracy                                                                                    |
       |----------|------|---------------------------------------------------------------------------------------------|
       | vgx, vgy | m/s  | Around 5cm/s for GNSS navigation. Around 3cm/s with VO at 1 meter height                    |
       | vgz      | m/s  | 10cm/s only with barometer in steady air. 3cm/s with VO at 1 meter height with 8cm baseline |
     * @datastruct \ref T_GduFcSubscriptionVelocity
     */
    GDU_FC_SUBSCRIPTION_TOPIC_VELOCITY = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 4),
    /*!
     * @brief Provides aircraft's angular velocity in a ground-fixed \b NED frame @ up to 200Hz
     * @details This is a fusion output from the flight control system.
     * @units rad/s
     * @datastruct \ref T_GduFcSubscriptionAngularRateFusioned
     */
    GDU_FC_SUBSCRIPTION_TOPIC_ANGULAR_RATE_FUSIONED = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 5),
    /*!
     * @brief Provides aircraft's angular velocity in an IMU-centered, body-fixed \b FRD frame @ up to 400Hz
     * @details This is a filtered output from the IMU on board the flight control system.
     * @sensors IMU
     * @units rad/s
     * @datastruct \ref T_GduFcSubscriptionAngularRateRaw
     */
    GDU_FC_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 6),
    /*!
     * @brief Fused altitude of aircraft topic name. Fused altitude topic provides aircraft's fused altitude from sea
     * level. Please refer to ::T_GduFcSubscriptionAltitudeFused for information about data structure.
     * @units m
     * @datastruct \ref T_GduFcSubscriptionAltitudeFused
     */
    GDU_FC_SUBSCRIPTION_TOPIC_ALTITUDE_FUSED = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 7),
    /*!
     * @brief Provides aircraft's pressure altitude from sea level using the ICAO model @ up to 200Hz
     * @details
     * This is a filetered output from the barometer without any further fusion.
     *
     * The ICAO model gives an MSL altitude of 1013.25mBar at 15&deg; C and a temperature lapse rate of -6.5&deg; C
     * per 1000m. In your case, it may be possible that the take off altitude of the aircraft is recording a higher pressure
     * than 1013.25mBar. Let's take an example - a weather station shows that SFO (San Francisco International Airport) had
     * recently recorded a pressure of 1027.1mBar. SFO is 4m above MSL, yet, if you calculate the Pressure Altitude using
     * the ICAO model, it relates to -114m. You can use an online calculator to similarly calculate the Pressure Altitude
     * in your area.
     *
     * Another factor that may affect your altitude reading is manufacturing differences in the barometer - it is not
     * uncommon to have a variation of &plusmn;30m readings at the same physical location with two different aircraft. For a given
     * aircraft, these readings will be consistent, so you will need to calibrate the offset of your system if your code
     * relies on the accuracy of the absolute value of altitude.
     * @sensors GPS, Barometer, IMU
     * @units m
     * @datastruct \ref T_GduFcSubscriptionAltitudeBarometer
     */
    GDU_FC_SUBSCRIPTION_TOPIC_ALTITUDE_BAROMETER = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 8),
    /*!
     * @brief Provides the altitude from sea level when the aircraft last took off.
     * @details
     * This is a fusion output from the flight control system, and also uses the ICAO model.
     *
     * The ICAO model gives an MSL altitude of 1013.25mBar at 15&deg; C and a temperature lapse rate of -6.5&deg; C
     * per 1000m. In your case, it may be possible that the take off altitude of the aircraft is recording a higher pressure
     * than 1013.25mBar. Let's take an example - a weather station shows that SFO (San Francisco International Airport) had
     * recently recorded a pressure of 1027.1mBar. SFO is 4m above MSL, yet, if you calculate the Pressure Altitude using
     * the ICAO model, it relates to -114m. You can use an online calculator to similarly calculate the Pressure Altitude
     * in your area.
     *
     * Another factor that may affect your altitude reading is manufacturing differences in the barometer - it is not
     * uncommon to have a variation of &plusmn;30m readings at the same physical location with two different aircraft. For a given
     * aircraft, these readings will be consistent, so you will need to calibrate the offset of your system if your code
     * relies on the accuracy of the absolute value of altitude.
     *
     * @note This value is updated each time the drone takes off.
     *
     * @sensors Visual Odometry (M210 only), Barometer, IMU
     * @units m
     * @datastruct \ref T_GduFcSubscriptionAltitudeOfHomePoint
     */
    GDU_FC_SUBSCRIPTION_TOPIC_ALTITUDE_OF_HOMEPOINT = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 9),
    /*!
     * @brief Provides the relative height above ground at up to 100Hz.
     * @details
     * This is a fusion output from the flight control system. The height is a direct estimate of the closest large object below the aircraft's ultrasonic sensors.
     * A large object is something that covers the ultrasonic sensor for an extended duration of time.
     *
     * @warning This topic does not come with a 'valid' flag - so if the aircraft is too far from an object for the
     * ultrasonic sensors/VO to provide any meaningful data, the values will latch and there is no way for user code to
     * determine if the data is valid or not. Use with caution.
     * @sensors Visual Odometry, Ultrasonic
     * @units m
     * @datastruct \ref T_GduFcSubscriptionHeightFusion
     */
    GDU_FC_SUBSCRIPTION_TOPIC_HEIGHT_FUSION = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 10),
    /*!
     * @brief Relative height above ground of aircraft topic name. Please refer to
     * ::T_GduFcSubscriptionHeightRelative for information about data structure.
     * @details This data is a fusion output from aircraft. The height is a direct estimate of the closest large object
     * below the aircraft's ultrasonic sensors.
     * @warning This topic does not come with a 'valid' flag - so if the aircraft is too far from an object for the
     * ultrasonic sensors/VO to provide any meaningful data, the values will latch and there is no way for user to
     * determine if the data is valid or not. Please use with caution.
     * @datastruct \ref T_GduFcSubscriptionHeightRelative
     */
    GDU_FC_SUBSCRIPTION_TOPIC_HEIGHT_RELATIVE = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 11),

    /*!
     * Fused position of aircraft topic name. Please refer to ::T_GduFcSubscriptionPositionFused for information
     * about data structure.
     * @warning Please note that if GPS signal is weak (low visibleSatelliteNumber, see below), the
     * latitude/longitude values won't be updated but the altitude might still be. There is currently no way to know if
     * the lat/lon update is healthy.
     * @details The most important component of this topic is the T_GduFcSubscriptionPositionFused::visibleSatelliteNumber.
     * Use this to track your GPS satellite coverage and build some heuristics for when you might expect to lose GPS updates.
     *   | Axis | Unit | Position Sensor | Accuracy                                         |
         |------|------|-----------------|--------------------------------------------------|
         | x, y | m    | GPS             | <3m with open sky without multipath              |
         | z    | m    | GPS             | <5m with open sky without multipath              |
     * @datastruct \ref T_GduFcSubscriptionPositionFused
     */
    GDU_FC_SUBSCRIPTION_TOPIC_POSITION_FUSED = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 12),

    /*!
     * @brief GPS date topic name. Please refer to ::T_GduFcSubscriptionGpsDate for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionGpsDate
     */
    GDU_FC_SUBSCRIPTION_TOPIC_GPS_DATE = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 13),

    /*!
     * @brief GPS time topic name. Please refer to ::T_GduFcSubscriptionGpsTime for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionGpsTime
     */
    GDU_FC_SUBSCRIPTION_TOPIC_GPS_TIME = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 14),

    /*!
     * @brief GPS position topic name. Please refer to ::T_GduFcSubscriptionGpsPosition for information about data structure.
     * @details
     *   | Axis | Accuracy                                         |
         |------|--------------------------------------------------|
         | x, y | <3m with open sky without multipath              |
         | z    | <5m with open sky without multipath              |
     * @datastruct \ref T_GduFcSubscriptionGpsPosition
     */
    GDU_FC_SUBSCRIPTION_TOPIC_GPS_POSITION = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 15),

    /*!
     * @brief GPS velocity topic name. Please refer to ::T_GduFcSubscriptionGpsVelocity for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionGpsVelocity
     */
    GDU_FC_SUBSCRIPTION_TOPIC_GPS_VELOCITY = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 16),

    /*!
     * @brief GPS details topic name. GPS details topic provides GPS state and other detail information. Please refer
     * to ::T_GduFcSubscriptionGpsDetail for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionGpsDetails
     */
    GDU_FC_SUBSCRIPTION_TOPIC_GPS_DETAILS = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 17),

    /*!
     * @brief GPS signal level topic name. This topic provides a measure of the quality of GPS signal. Please refer to
     * ::T_GduFcSubscriptionGpsSignalLevel for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionGpsSignalLevel
     */
    GDU_FC_SUBSCRIPTION_TOPIC_GPS_SIGNAL_LEVEL = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 18),

    /*!
     * @brief RTK position topic name. Please refer to ::T_GduFcSubscriptionRtkPosition for information about data structure.
     * @details
     *   | Axis | Accuracy                                         |
         |------|--------------------------------------------------|
         | x, y | ~2cm with fine alignment and fix condition       |
         | z    | ~3cm with fine alignment and fix condition       |
     * @datastruct \ref T_GduFcSubscriptionRtkPosition
     */
    GDU_FC_SUBSCRIPTION_TOPIC_RTK_POSITION = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 19),

    /*!
     * @brief RTK velocity topic name. Please refer to ::T_GduFcSubscriptionRtkVelocity for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionRtkVelocity
     */
    GDU_FC_SUBSCRIPTION_TOPIC_RTK_VELOCITY = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 20),

    /*!
     * @brief RTK yaw topic name. Please refer to ::T_GduFcSubscriptionRtkYaw for information about data structure.
     * @details The RTK yaw will provide the vector from ANT1 to ANT2 as configured in GDU Assistant 2. This
     * means that the value of RTK yaw will be 90deg offset from the yaw of the aircraft.
     * @datastruct \ref T_GduFcSubscriptionRtkYaw
     */
    GDU_FC_SUBSCRIPTION_TOPIC_RTK_YAW = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 21),

    /*!
     * @brief RTK position information topic name. RTK position information topic provides a state of RTK position
     * solution. Please refer to ::T_GduFcSubscriptionRtkPositionInfo for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionRtkPositionInfo
     */
    GDU_FC_SUBSCRIPTION_TOPIC_RTK_POSITION_INFO = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 22),

    /*!
     * @brief RTK yaw topic name. RTK yaw information topic provides a state of RTK yaw solution. Please refer to
     * ::T_GduFcSubscriptionRtkYawInfo for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionRtkYawInfo
     */
    GDU_FC_SUBSCRIPTION_TOPIC_RTK_YAW_INFO = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 23),
    /*!
     * @brief Provides aircraft's magnetometer reading, fused with IMU and GPS @ up to 100Hz
     * @details This reading is the magnetic field recorded by the magnetometer in x,y,z axis, calibrated such that
     * 1000 < |m| < 2000, and fused with IMU and GPS for robustness
     * @sensors Magnetometer, IMU, GPS
     * @units N/A
     * @datastruct \ref T_GduFcSubscriptionCompass
     */
    GDU_FC_SUBSCRIPTION_TOPIC_COMPASS = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 24),
    /*!
     * @brief Provides remote controller stick inputs @ up to 100Hz
     * @details This topic will give you:
     * - Stick inputs (R,P,Y,Thr)
     * - Mode switch (P/A/F)
     * - Landing gear switch (Up/Down)
     *
     * @datastruct \ref T_GduFcSubscriptionRC
     * @also \ref TOPIC_RC_WITH_FLAG_DATA
     */
    GDU_FC_SUBSCRIPTION_TOPIC_RC = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 25),
    /*!
     * @brief Provides gimbal pitch, roll, yaw @ up to 50Hz
     * @details
     * The reference frame for gimbal angles is a NED frame attached to the gimbal.
     * This topic uses a data structure, Vector3f, that is too generic for the topic. The order of angles is :
     * |Data Structure Element| Meaning|
     * |----------------------|--------|
     * |Vector3f.x            |pitch   |
     * |Vector3f.y            |roll    |
     * |Vector3f.z            |yaw     |
     *
     * @perf
     * 0.1 deg accuracy in all axes
     *
     * @sensors Gimbal Encoder, IMU, Magnetometer
     * @units deg
     * @datastruct \ref T_GduFcSubscriptionGimbalAngles
     * @also \ref TOPIC_GIMBAL_STATUS, \ref TOPIC_GIMBAL_CONTROL_MODE
     */
    GDU_FC_SUBSCRIPTION_TOPIC_GIMBAL_ANGLES = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 26),
    /*!
     * @brief Provides gimbal status and error codes @ up to 50Hz
     * @details Please see the \ref GimbalStatus struct for the details on what data you can receive.
     *
     * @datastruct \ref T_GduFcSubscriptionGimbalStatus
     * @also \ref TOPIC_GIMBAL_ANGLES, \ref TOPIC_GIMBAL_CONTROL_MODE
     */
    GDU_FC_SUBSCRIPTION_TOPIC_GIMBAL_STATUS = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 27),
    /*!
     * @brief Flight status topic name. Please refer to ::T_GduFcSubscriptionFlightStatus for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionFlightStatus
     */
    GDU_FC_SUBSCRIPTION_TOPIC_STATUS_FLIGHT = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 28),
    /*!
     * @brief Provides a granular state representation for various tasks/flight modes @ up to 50Hz
     * @details Typically, use this topic together with \ref TOPIC_STATUS_FLIGHT to get a
     * better understanding of the overall status of the aircraft.
     *
     * @datastruct \ref T_GduFcSubscriptionDisplaymode
     * @also \ref TOPIC_STATUS_FLIGHT
     */
    GDU_FC_SUBSCRIPTION_TOPIC_STATUS_DISPLAYMODE = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 29),
    /*!
     * @brief Provides status for the landing gear state @ up to 50Hz
     *
     * @datastruct \ref T_GduFcSubscriptionLandinggear
     */
    GDU_FC_SUBSCRIPTION_TOPIC_STATUS_LANDINGGEAR = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 30),
    /*!
     * @brief If motors failed to start, this topic provides reasons why. Available @ up to 50Hz
     * @datastruct \ref T_GduFcSubscriptionMotorStartError
     * \note These enumerations show up in the ErrorCode class because they can also be returned as acknowledgements
     * for APIs that start the motors, such as \ref Control::takeoff "Takeoff" or \ref Control::armMotors "Arm"
     */
    GDU_FC_SUBSCRIPTION_TOPIC_STATUS_MOTOR_START_ERROR = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC,
                                                                                     31),
    /*!
     * @brief Battery information topic name. Please refer to ::T_GduFcSubscriptionBatteryInfo for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionBatteryInfo
     */
    GDU_FC_SUBSCRIPTION_TOPIC_BATTERY_INFO = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 32),
    /*!
     * @brief Provides states of the aircraft related to SDK/RC control
     * @details The following information is available in this topic:
     * |Data Structure Element| Meaning|
     * |----------------------|--------|
     * |controlMode           |The modes in which the aircraft is being controlled (control loops being applied on horizontal, vertical and yaw axes of the aircraft)|
     * |deviceStatus          |Which device is controlling the motion of the aircraft: RC (Manual control), MSDK (Missions kicked off through mobile), OSDK (Missions kicked off through onboard/ low-level flight control)    |
     * |flightStatus          |Has the OSDK been granted control authority? Since MSDK and RC have precedence, it is possible that deviceStatus shows RC or MSDK actually controlling the aircraft but this value is 1.     |
     * |vrcStatus             |Deprecated|
     * @datastruct \ref T_GduFcSubscriptionControlDevice
     */
    GDU_FC_SUBSCRIPTION_TOPIC_CONTROL_DEVICE = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 33),
    /*!
     * @brief Provides IMU and quaternion data time-synced with a hardware clock signal @ up to 400Hz.
     * @details This is the only data which can be synchronized with external software or hardware systems. If you want to
     * fuse an external sensor's data with the aircraft's IMU, this data along with a hardware trigger from the A3/N3's
     * expansion ports is how you would do it. You can see detailed documentation on how this process works in the [Hardware
     * Sync Guide](https://developer.gdu.com/onboard-sdk/documentation/guides/component-guide-hardware-sync.html).
     * @sensors IMU, sensor fusion output
     * @units
     * |Data Structure Element| Units|
     * |----------------------|--------|
     * |Timestamp |2.5ms, 1ns (See \ref SyncTimestamp)|
     * |Quaternion |rad (after converting to rotation matrix)|
     * |Acceleration |g|
     * |Gyroscope |rad/sec|
     * @datastruct \ref T_GduFcSubscriptionHardSync
     */
    GDU_FC_SUBSCRIPTION_TOPIC_HARD_SYNC = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 34),
    /*!
     * @brief Provides a measure of the quality of GPS signal, with a mechanism for guarding against unset homepoint @ up to 50Hz
     * @details The level varies from 0 to 5, with 0 being the worst and 5 the best GPS signal. The key difference between
     * this and TOPIC_GPS_SIGNAL_LEVEL is that this topic always returns 0 if the homepoint is not set. Once the home point is
     * set, the behavior is exactly the same as TOPIC_GPS_SIGNAL_LEVEL.
     * @sensors GPS
     * @datastruct \ref T_GduFcSubscriptionGpsControlLevel
     * @also \ref TOPIC_GPS_SIGNAL_LEVEL
     */
    GDU_FC_SUBSCRIPTION_TOPIC_GPS_CONTROL_LEVEL = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 35),
    /*!
     * @brief Provides normalized remote controller stick input data, along with connection status @ up to 50Hz
     * @note This topic was added in August 2018. Your aircraft may require a FW update to enable this feature.
     * @details This topic will give you:
     * - Stick inputs (R,P,Y,Thr)
     * - Mode switch (P/A/F)
     * - Landing gear switch (Up/Down)
     * - Connection status for air system, ground system and MSDK apps. The connection status also includes a
     * logicConnected element, which will change to false if either the air system or the ground system radios
     * are disconnected for >3s.
     * - Deadzones near the center of the stick positions are also handled in this topic.
     *
     * @datastruct \ref T_GduFcSubscriptionRCWithFlagData
     * @also \ref TOPIC_RC
     */
    GDU_FC_SUBSCRIPTION_TOPIC_RC_WITH_FLAG_DATA = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 36),
    /*!
     * @brief Provides raw data from the ESCs @ up to 50Hz
     * @note This topic was added in August 2018. Your aircraft may require a FW update to enable this feature.
     * @details This topic supports reporting data for up to 8 ESCs; note that only GDU Intelligent ESCs are supported
     * for this reporting feature. Use this topic to get data on elements close to the hardware - e.g. motor speeds,
     * ESC current and voltage, error flags at the ESC level etc.
     * @datastruct \ref T_GduFcSubscriptionEscData
     */
    GDU_FC_SUBSCRIPTION_TOPIC_ESC_DATA = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 37),
    /*!
     * @brief Provides RTK connection status @ up to 50Hz
     * @note This topic was added in August 2018. Your aircraft may require a FW update to enable this feature.
     * @details This topic will update in real time whether the RTK GPS system is connected or not; typical uses
     * include app-level logic to switch between GPS and RTK sources of positioning based on this flag.
     * @datastruct \ref T_GduFcSubscriptionRTKConnectStatus
     */
    GDU_FC_SUBSCRIPTION_TOPIC_RTK_CONNECT_STATUS = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 38),
    /*!
     * @brief Provides the mode in which the gimbal will interpret control commands @ up to 50Hz
     * @note This topic was added in August 2018. Your aircraft may require a FW update to enable this feature.
     * @details This topic will report the current control mode which can be set in the
     * GDU Go app, MSDK apps, or through Onboard SDK gimbal control APIs (see \ref Gimbal::AngleData "AngleData" struct
     * for more information)
     * @datastruct \ref T_GduFcSubscriptionGimbalControlMode
     */
    GDU_FC_SUBSCRIPTION_TOPIC_GIMBAL_CONTROL_MODE = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 39),
    /*!
     * @brief Provides a number of flags which report different errors the aircraft may encounter in flight @ up to 50Hz
     * @note This topic was added in August 2018. Your aircraft may require a FW update to enable this feature.
     * @warning Most of the errors reported by this topic are cases where immediate action is required; you can use these
     * as a baseline for implementing safety-related error-handling routines.
     * @datastruct \ref T_GduFcSubscriptionFlightAnomaly
     */
    GDU_FC_SUBSCRIPTION_TOPIC_FLIGHT_ANOMALY = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 40),
    /*!
     * @brief Provides aircraft's position in a Cartesian frame @ up to 50Hz, without the need for GPS
     * @warning This topic does not follow a standard co-ordinate convention. Please read the details below for usage.
     * @details This is the only topic which can provide positioning information without having a GPS fix; though this
     * can be a big enabler please note the caveats of using this topic:
     * - The topic will use an origin that does not have a global reference, and is not published to the SDK.
     * - The topic uses a combination of VO and compass heading to identify the X-Y axes of its reference frame. This means
     * that if your compass performance is not good in an environment, there is no guarantee the X-Y axes will point to
     * North and East.
     * - The actual directions of the X-Y axes are currently not published to the SDK.
     * - If during a flight the compass performance were to change dramatically, the orientation of the X-Y axes may change
     * to re-align with North-East. The aircraft's position in X and Y may exhibit discontinuities in these cases.
     * - The reference frame is referred to as the Navigation Frame - Cartesian X,Y axes aligned with N,E directions on a best-effort
     * basis, and Z aligned to D (down) direction.
     * - A health flag for each axis provides some granularity on whether this data is valid or not.
     *
     * The key takeaway from these details is that this topic provides a best-effort attempt at providing position
     * information in the absence of absolute references (GPS, compass etc.), without guarantees of consistency if
     * environmental conditions change. So if your application is confined to a stable environment, or if you will
     * have GPS and compass available at all times, this topic can still provide useful data that cannot be otherwise
     * had. If using for control, make sure to have guards checking for the continuity of data.
     *
     * @note Since this topic relies on visual features and/or GPS, if your environment does not provide any of these
     * sources of data, the quality of this topic will reduce significantly. VO data quality will reduce if you are too high
     * above the ground. Make sure that the Vision Positioning System is enabled in GDU Go 4 before using this topic
     * (by default it is enabled).
     * @sensors IMU, VO, GPS(if available), RTK (if available), ultrasonic, magnetometer, barometer
     * @units m
     * @datastruct \ref T_GduFcSubscriptionPositionVO
     */
    GDU_FC_SUBSCRIPTION_TOPIC_POSITION_VO = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 41),
    /*!
     * @brief Provides obstacle info around the vehicle @ up to 100Hz
     * @datastruct \ref T_GduFcSubscriptionAvoidData
     */
    GDU_FC_SUBSCRIPTION_TOPIC_AVOID_DATA = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 42),
    /*!
     * @brief Provides status of whether the home point was set or not
     * @datastruct \ref T_GduFcSubscriptionHomePointSetStatus
     */
    GDU_FC_SUBSCRIPTION_TOPIC_HOME_POINT_SET_STATUS = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 43),
    /*!
     * @brief Provides homepoint information, the valid of the home point infomation can ref to the
     * topic GDU_FC_SUBSCRIPTION_TOPIC_HOME_POINT_SET_STATUS
     * @datastruct \ref T_GduFcSubscriptionHomePointInfo
     */
    GDU_FC_SUBSCRIPTION_TOPIC_HOME_POINT_INFO = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 44),
    /*!
     * @brief Provides three gimbal information, used for M300
     * @datastruct \ref T_GduFcSubscriptionThreeGimbalData
     */
    GDU_FC_SUBSCRIPTION_TOPIC_THREE_GIMBAL_DATA = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC, 45),

    /*!
     * @brief Battery information topic name. Please refer to ::T_GduFcSubscriptionSingleBatteryInfo for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionSingleBatteryInfo
     */
    GDU_FC_SUBSCRIPTION_TOPIC_BATTERY_SINGLE_INFO_INDEX1 = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC,
                                                                                       46),
    /*!
     * @brief Battery information topic name. Please refer to ::T_GduFcSubscriptionSingleBatteryInfo for information about data structure.
     * @datastruct \ref T_GduFcSubscriptionSingleBatteryInfo
     */
    GDU_FC_SUBSCRIPTION_TOPIC_BATTERY_SINGLE_INFO_INDEX2 = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC,
                                                                                       47),
    /*!
     * @brief euler angle information topic name. Please refer to ::T_GduAttitude3d for information about data structure.
     * @datastruct \ref T_GduAttitude3d
     */
    GDU_FC_SUBSCRIPTION_TOPIC_EULER_ANGLE_INFO = GDU_DATA_SUBSCRIPTION_TOPIC(GDU_DATA_SUBSCRIPTION_MODULE_FC,
                                                                                       48),
                                                                                       
    /*! Total number of topics that can be subscribed. */
    GDU_FC_SUBSCRIPTION_TOPIC_TOTAL_NUMBER,
} E_GduFcSubscriptionTopic;

/**
 * @brief Health state of data subscribed.
 */
typedef enum {
    GDU_FC_SUBSCRIPTION_DATA_NOT_HEALTH = 0, /*!< Data subscribed is healthy and can be used. */
    GDU_FC_SUBSCRIPTION_DATA_HEALTH = 1, /*!< Data subscribed is not healthy and recommend not to use it. */
} E_GduFcSubscriptionDataHealthFlag;

/**
 * @brief Position solution property.
 */
typedef enum {
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_NOT_AVAILABLE = 0, /*!< Position solution is not available. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_FIX_POSITION = 1, /*!< Position has been fixed by the FIX POSITION command. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_FIX_HEIGHT_AUTO = 2, /*!< Position has been fixed by the FIX HEIGHT/AUTO command. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_INSTANTANEOUS_DOPPLER_COMPUTE_VELOCITY = 8, /*!< Velocity computed using instantaneous Doppler. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_SINGLE_PNT_SOLUTION = 16, /*!< Single point position solution. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_PSEUDORANGE_DIFFERENTIAL_SOLUTION = 17, /*!< Pseudorange differential solution. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_SBAS_CORRECTION_CALCULATED = 18, /*!< Solution calculated using corrections from an SBAS. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_KALMAN_FILTER_WITHOUT_OBSERVATION_PROPAGATED = 19, /*!< Propagated by a Kalman filter without new observations. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_OMNISTAR_VBS_POSITION = 20, /*!< OmniSTAR VBS position (L1 sub-metre). */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_FLOAT_L1_AMBIGUITY = 32, /*!< Floating L1 ambiguity solution. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_FLOAT_IONOSPHERIC_FREE_AMBIGUITY = 33, /*!< Floating ionospheric-free ambiguity solution. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_FLOAT_SOLUTION = 34, /*!< Float position solution. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_L1_AMBIGUITY_INT = 48, /*!< Integer L1 ambiguity solution. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_WIDE_LANE_AMBIGUITY_INT = 49, /*!< Integer wide-lane ambiguity solution. */
    GDU_FC_SUBSCRIPTION_POSITION_SOLUTION_PROPERTY_NARROW_INT = 50, /*!< Narrow fixed point position solution. */
} E_GduFcSubscriptionPositionSolutionProperty;

/**
 * @brief GPS fix state.
 */
typedef enum {
    GDU_FC_SUBSCRIPTION_GPS_FIX_STATE_NO_FIX = 0, /*!< GPS position has not been fixed. */
    GDU_FC_SUBSCRIPTION_GPS_FIX_STATE_DEAD_RECKONING_ONLY = 1, /*!< GPS position is dead reckoned only. */
    GDU_FC_SUBSCRIPTION_GPS_FIX_STATE_2D_FIX = 2, /*!< The horizontal position with latitude/longitude (or northing/easting or X/Y) is fixed. */
    GDU_FC_SUBSCRIPTION_GPS_FIX_STATE_3D_FIX = 3, /*!< The horizontal and vertical position with latitude/longitude/altitude (northing/easting/altitude or X/Y/Z) is fixed. */
    GDU_FC_SUBSCRIPTION_GPS_FIX_STATE_GPS_PLUS_DEAD_RECKONING = 4, /*!< Position is calculated by GPS and combined with dead reckoning. */
    GDU_FC_SUBSCRIPTION_GPS_FIX_STATE_TIME_ONLY_FIX = 5, /*!< Only time is fixed. */
} E_GduFcSubscriptionGpsFixState;

/**
 * @brief Flight status of aircraft.
 */
typedef enum {
    GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_STOPED = 0, /*!< Aircraft is on ground and motors are still. */
    GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_ON_GROUND = 1, /*!< Aircraft is on ground but motors are rotating. */
    GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_IN_AIR = 2, /*!< Aircraft is in air. */
} E_GduFcSubscriptionFlightStatus;

/*!
 * @brief "Mode" of the vehicle's state machine, as displayed on GDU Go
 * @details Available through Telemetry::TOPIC_STATUS_DISPLAYMODE
 */
typedef enum {
    /*! This mode requires the user to manually
     * control the aircraft to remain stable in air. */
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_MANUAL_CTRL = 0,
    /*! In this mode, the aircraft can keep
     * attitude stabilization and only use the
     * barometer for positioning to control the altitude. <br>
     * The aircraft can not autonomously locate and hover stably.*/
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ATTITUDE = 1,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_2 = 2,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_3 = 3,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_4 = 4,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_5 = 5,
    /*! The aircraft is in normal GPS mode. <br>
     * In normal GPS mode, the aircraft can
     * autonomously locate and hover stably. <br>
     *  The sensitivity of the aircraft to the
     *  command response is moderate.
     */
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_P_GPS = 6,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_7 = 7,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_8 = 8,
    /*! In hotpoint mode */
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_HOTPOINT_MODE = 9,
    /*! In this mode, user can push the throttle
     * stick to complete stable take-off. */
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ASSISTED_TAKEOFF = 10,
    /*! In this mode, the aircraft will autonomously
     * start motor, ascend and finally hover. */
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_TAKEOFF = 11,
    /*! In this mode, the aircraft can land autonomously. */
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_AUTO_LANDING = 12,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_13 = 13,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_14 = 14,
    /*! In this mode, the aircraft can antonomously return the
     * last recorded Home Point. <br>
     * There are three types of this mode: Smart RTH(Return-to-Home),
     * Low Batterry RTH, and Failsafe RTTH.  */
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_NAVI_GO_HOME = 15,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_16 = 16,
    /*! In this mode, the aircraft is controled by SDK API. <br>
     * User can directly define the control mode of horizon
     * and vertical directions and send control datas to aircraft. */
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_NAVI_SDK_CTRL = 17,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_18 = 18,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_19 = 19,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_20 = 20,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_21 = 21,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_22 = 22,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_23 = 23,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_24 = 24,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_25 = 25,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_26 = 26,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_27 = 27,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_28 = 28,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_29 = 29,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_30 = 30,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_31 = 31,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_32 = 32,
    /*! drone is forced to land, might due to low battery */
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_FORCE_AUTO_LANDING = 33,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_34 = 34,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_35 = 35,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_36 = 36,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_37 = 37,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_38 = 38,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_39 = 39,
    /*! drone will search for the last position where the rc is not lost */
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_SEARCH_MODE = 40,
    /*! Mode for motor starting. <br>
     * Every time user unlock the motor, this will be the first mode. */
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_ENGINE_START = 41,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_42 = 42,
    GDU_FC_SUBSCRIPTION_DISPLAY_MODE_RESERVED_43 = 42,
} E_GduFcSubscriptionDisplayMode;

/**
 * @brief Setting status of homepoint.
 */
typedef enum {
    GDU_FC_SUBSCRIPTION_HOME_POINT_SET_STATUS_FAILED = 0, /*!< The home point was not set yet. */
    GDU_FC_SUBSCRIPTION_HOME_POINT_SET_STATUS_SUCCESS = 1, /*!< The home point was set successfully. */
} E_GduFcSubscriptionHomePointSetStatus;

#pragma pack(1)

/**
 * @brief Quaternion topic data structure.
 */
typedef struct Quaternion {
    gdu_f32_t q0; /*!< w, rad (when converted to a rotation matrix or Euler angles). */
    gdu_f32_t q1; /*!< x, rad (when converted to a rotation matrix or Euler angles). */
    gdu_f32_t q2; /*!< y, rad (when converted to a rotation matrix or Euler angles). */
    gdu_f32_t q3; /*!< z, rad (when converted to a rotation matrix or Euler angles). */
} T_GduFcSubscriptionQuaternion;

/**
 * @brief GDU_FC_SUBSCRIPTION_TOPIC_ACCELERATION_GROUND topic data structure.
 */
typedef T_GduVector3f T_GduFcSubscriptionAccelerationGround;

/**
 * @brief GDU_FC_SUBSCRIPTION_TOPIC_ACCELERATION_BODY topic data structure.
 */
typedef T_GduVector3f T_GduFcSubscriptionAccelerationBody;

/**
 * @brief GDU_FC_SUBSCRIPTION_TOPIC_ACCELERATION_RAW topic data structure.
 */
typedef T_GduVector3f T_GduFcSubscriptionAccelerationRaw;

/**
 * @brief GDU_FC_SUBSCRIPTION_TOPIC_VELOCITY topic data structure.
 */
typedef struct Velocity {
    /*! Velocity of aircraft. */
    T_GduVector3f data;
    /*! Health state of aircraft velocity data. It can be any value of ::E_GduFcSubscriptionDataHealthFlag. */
    uint8_t health: 1;
    /*! Reserved. */
    uint8_t reserve: 7;
} T_GduFcSubscriptionVelocity;

/**
 * @brief GDU_FC_SUBSCRIPTION_TOPIC_ANGULAR_RATE_FUSIONED topic data structure.
 */
typedef T_GduVector3f T_GduFcSubscriptionAngularRateFusioned;

/**
 * @brief GDU_FC_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW topic data structure.
 */
typedef T_GduVector3f T_GduFcSubscriptionAngularRateRaw;

/**
 * @brief GDU_FC_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW topic data structure.
 */
typedef gdu_f32_t T_GduFcSubscriptionAltitudeFused;

/**
 * @brief GDU_FC_SUBSCRIPTION_TOPIC_ALTITUDE_BAROMETER topic data structure.
 */
typedef gdu_f32_t T_GduFcSubscriptionAltitudeBarometer;

/**
 * @brief GDU_FC_SUBSCRIPTION_TOPIC_ALTITUDE_OF_HOMEPOINT topic data structure.
 */
typedef gdu_f32_t T_GduFcSubscriptionAltitudeOfHomePoint;

/**
 * @brief GDU_FC_SUBSCRIPTION_TOPIC_HEIGHT_FUSION topic data structure.
 */
typedef gdu_f32_t T_GduFcSubscriptionHeightFusion;

/**
 * @brief Relative height above ground topic data structure, unit: m.
 */
typedef gdu_f32_t T_GduFcSubscriptionHeightRelative;

/**
 * @brief Fused position topic data structure.
 */
typedef struct PositionFused {
    gdu_f64_t longitude; /*!< Longitude, unit: rad. */
    gdu_f64_t latitude; /*!< Latitude, unit: rad. */
    gdu_f32_t altitude; /*!< Altitude, WGS 84 reference ellipsoid, unit: m. */
    uint16_t visibleSatelliteNumber; /*!< Number of visible satellites. */
} T_GduFcSubscriptionPositionFused;

/**
 * @brief GPS date topic data structure, format: yyyymmdd.
 */
typedef uint32_t T_GduFcSubscriptionGpsDate;

/**
 * @brief GPS time topic data structure, format: hhmmss.
 */
typedef uint32_t T_GduFcSubscriptionGpsTime;

/**
 * @brief GPS position topic data structure. x = Longitude, y = Latitude, z = Altitude, unit: deg*10<SUP>-7</SUP>
 * (Lat,Lon), mm (Alt)
 */
typedef T_GduVector3d T_GduFcSubscriptionGpsPosition;

/**
 * @brief GPS velocity topic data structure, unit: cm/s.
 */
typedef T_GduVector3f T_GduFcSubscriptionGpsVelocity;

/**
 * @brief GPS details topic data structure.
 */
typedef struct GpsDetail {
    gdu_f32_t hdop; /*!< Horizontal dilution of precision, <1: ideal, 1-2: excellent, 2-5: good, 5-10: moderate, 10-20: fair, >20: poor. */
    gdu_f32_t pdop; /*!< Position dilution of precision, <1: ideal, 1-2: excellent, 2-5: good, 5-10: moderate, 10-20: fair, >20: poor. */
    gdu_f32_t fixState; /*!< GPS fix state, and can be any value of ::E_GduFcSubscriptionGpsFixState. Value other than ::E_GduFcSubscriptionGpsFixState is invalid. */
    gdu_f32_t vacc; /*!< Vertical position accuracy (mm), the smaller, the better. */
    gdu_f32_t hacc; /*!< Horizontal position accuracy (mm), the smaller, the better. */
    gdu_f32_t sacc; /*!< Speed accuracy (cm/s), the smaller, the better. */
    uint32_t gpsSatelliteNumberUsed; /*!< Number of GPS satellites used for fixing position. */
    uint32_t glonassSatelliteNumberUsed; /*!< Number of GLONASS satellites used for fixing position. */
    uint16_t totalSatelliteNumberUsed; /*!< Total number of satellites used for fixing position. */
    uint16_t gpsCounter; /*!< Accumulated times of sending GPS data. */
} T_GduFcSubscriptionGpsDetails;

/**
 * @brief GPS signal level topic data structure. Signal level of GPS. The level varies from 0 to 5, with 0 being the
 * worst and 5 the best GPS signal.
 */
typedef uint8_t T_GduFcSubscriptionGpsSignalLevel;

/**
 * @brief RTK position topic data structure.
 */
typedef struct PositionData {
    gdu_f64_t longitude; /*!< Longitude, unit: deg. */
    gdu_f64_t latitude;  /*!< Latitude, unit: deg. */
    gdu_f32_t hfsl;      /*!< Height above mean sea level, unit: m. */
} T_GduFcSubscriptionRtkPosition;

/**
 * @brief RTK velocity topic data structure, unit: cm/s.
 */
typedef T_GduVector3f T_GduFcSubscriptionRtkVelocity;

/**
 * @brief RTK yaw topic data structure, unit: deg.
 */
typedef int16_t T_GduFcSubscriptionRtkYaw;

/**
 * @brief RTK position information topic data structure. Specifies RTK position solution state, it can be any value of
 * ::E_GduFcSubscriptionPositionSolutionProperty.
 */
typedef uint8_t T_GduFcSubscriptionRtkPositionInfo;

/**
 * @brief RTK yaw information topic data structure. Specifies RTK yaw solution state, it can be any value of
 * ::E_GduFcSubscriptionPositionSolutionProperty.
 */
typedef uint8_t T_GduFcSubscriptionRtkYawInfo;

/*!
 * @brief struct for data broadcast, return magnetometer reading
 *
 * @note returned value is calibrated mag data,
 * 1000 < |mag| < 2000 for normal operation
 */
typedef struct Mag {
    int16_t x;
    int16_t y;
    int16_t z;
} T_GduFcSubscriptionCompass;

/*!
 * @brief struct for data broadcast and data subscription, return RC reading
 */
typedef struct RC {
    int16_t roll;     /*!< [-10000,10000] */
    int16_t pitch;    /*!< [-10000,10000] */
    int16_t yaw;      /*!< [-10000,10000] */
    int16_t throttle; /*!< [-10000,10000] */
    int16_t mode;     /*!< [-10000,10000] */
    /*!< M100 [P: -8000, A: 0, F: 8000] */
    int16_t gear;     /*!< [-10000,10000] */
    /*!< M100 [Up: -10000, Down: -4545] */
} T_GduFcSubscriptionRC;

typedef T_GduVector3f T_GduFcSubscriptionGimbalAngles;

/*!
 * @brief struct for TOPIC_GIMBAL_STATUS
 */
typedef struct GimbalStatus {
    uint32_t mountStatus: 1; /*!< 1 - gimbal mounted, 0 - gimbal not mounted*/
    uint32_t isBusy: 1;
    uint32_t pitchLimited: 1;           /*!< 1 - axis reached limit, 0 - no */
    uint32_t rollLimited: 1;            /*!< 1 - axis reached limit, 0 - no */
    uint32_t yawLimited: 1;             /*!< 1 - axis reached limit, 0 - no */
    uint32_t calibrating: 1;            /*!< 1 - calibrating, 0 - no */
    uint32_t prevCalibrationgResult: 1; /*!< 1 - success, 0 - fail */
    uint32_t installedDirection: 1;     /*!< 1 - reversed for OSMO, 0 - normal */
    uint32_t disabled_mvo: 1;
    uint32_t gear_show_unable: 1;
    uint32_t gyroFalut: 1;       /*!< 1 - fault, 0 - normal */
    uint32_t escPitchStatus: 1;  /*!< 1 - Pitch data is normal, 0 - fault */
    uint32_t escRollStatus: 1;   /*!< 1 - Roll data is normal, 0 - fault */
    uint32_t escYawStatus: 1;    /*!< 1 - Yaw data is normal , 0 - fault */
    uint32_t droneDataRecv: 1;   /*!< 1 - normal , 0 - at fault */
    uint32_t initUnfinished: 1;  /*!< 1 - init complete, 0 - not complete */
    uint32_t FWUpdating: 1;      /*!< 1 - updating, 0 - not updating */
    uint32_t reserved2: 15;
} T_GduFcSubscriptionGimbalStatus;

/**
 * @brief Flight status information topic data structure. It can be any value of ::E_GduFcSubscriptionFlightStatus.
 */
typedef uint8_t T_GduFcSubscriptionFlightStatus;

typedef uint8_t T_GduFcSubscriptionDisplaymode;

typedef uint8_t T_GduFcSubscriptionLandinggear;

typedef uint16_t T_GduFcSubscriptionMotorStartError;

typedef struct {
    uint32_t reserved: 12;
    uint32_t cellBreak: 5;            /*! 0:normal;other:Undervoltage core index(0x01-0x1F)*/
    uint32_t selfCheckError: 3;       /*! enum-type: GDUSmartBatterySelfCheck*/
    uint32_t reserved1: 7;
    uint32_t batteryClosedReason: 5;   /*! enum-type: GDU_BETTERY_CLOSED_REASON*/
    uint8_t reserved2: 6;             /*! [0]CHG state；[1]DSG state；[2]ORING state*/
    uint8_t batSOHState: 2;                /*! enum-type: GDUSmartBatterySohState*/
    uint8_t maxCycleLimit: 6;          /*! APP:cycle_limit*10*/
    uint8_t reserved3: 2;
    uint16_t lessBattery: 1;
    uint16_t batteryCommunicationAbnormal: 1;
    uint16_t reserved4: 3;
    uint16_t hasCellBreak: 1;
    uint16_t reserved5: 4;
    uint16_t isBatteryEmbed: 1;        /*! 0:embed;1:unmebed*/
    uint16_t heatState: 2;             /*!enum-type: GDUSmartBatteryHeatState*/
    uint16_t socState: 3;             /*!enum-type: GDUSmartBatterySocWarning*/
} T_GduFcSubscriptionSingleBatteryState;

/**
 * @brief Battery information topic data structure.
 */
typedef struct BatteryWholeInfo {
    uint32_t capacity; /*!< Battery capacity, unit: mAh. */
    int32_t voltage; /*!< Battery voltage, unit: mV. */
    int32_t current; /*!< Battery current, unit: mA. */
    uint8_t percentage; /*!< Battery capacity percentage, unit: 1%. */
} T_GduFcSubscriptionWholeBatteryInfo;

typedef struct BatterySingleInfo {
    uint8_t reserve;
    uint8_t batteryIndex;
    int32_t currentVoltage;          /*! uint:mV*/
    int32_t currentElectric;         /*!uint:mA*/
    uint32_t fullCapacity;            /*!uint:mAh*/
    uint32_t remainedCapacity;        /*!uint:mAh*/
    int16_t batteryTemperature;      /*!uint:℃*/
    uint8_t cellCount;
    uint8_t batteryCapacityPercent;  /*!uint:%*/
    T_GduFcSubscriptionSingleBatteryState batteryState;
    uint8_t reserve1;
    uint8_t reserve2;
    uint8_t SOP;                     /*!Relative power percentage*/
} T_GduFcSubscriptionSingleBatteryInfo;

/*!
 * @brief struct for TOPIC_CONTROL_DEVICE and data broadcast, return SDK info
 */
typedef struct SDKCtrlInfo {
    uint8_t controlMode;      /*!< See CtlrMode in gdu_status.hpp*/
    uint8_t deviceStatus: 3; /*!< For M300 and M210V2(firmware version V01.00.0690 and later):0->rc  1->app  4->serial;
                                 Other: 0->rc  1->app  2->serial*/
    uint8_t flightStatus: 1; /*!< 1->opensd  0->close */
    uint8_t vrcStatus: 1;
    uint8_t reserved: 3;
} T_GduFcSubscriptionControlDevice; // pack(1)

/*!
 * @brief sub struct for TOPIC_HARD_SYNC
 */
typedef struct SyncTimestamp {
    uint32_t time2p5ms; /*!< clock time in multiples of 2.5ms. Sync timer runs at
                         400Hz, this field increments in integer steps */
    uint32_t time1ns;   /*!< nanosecond time offset from the 2.5ms pulse */
    uint32_t resetTime2p5ms; /*!< clock time in multiple of 2.5ms elapsed since the
                              hardware sync started */
    uint16_t index;   /*!< This is the tag field you filled out when using the
                       setSyncFreq API above; use it to identify the packets that
                       have sync data. This is useful when you call the
                       setSyncFreq API with freqInHz = 0, so you get a single
                       pulse that can be uniquely identified with a tag - allowing
                       you to create your own pulse train with uniquely
                       identifiable pulses. */
    uint8_t flag;     /*!< This is true when the packet corresponds to a hardware
                       pulse and false otherwise. This is useful because you can
                       request the software packet to be sent at a higher frequency
                       that the hardware line.*/
} SyncTimestamp;    // pack(1)

/*!
 * @brief struct for TOPIC_HARD_SYNC
 */
typedef struct HardSyncData {
    SyncTimestamp ts;  /*!< time stamp for the incoming data */
    struct Quaternion q;  /*!< quaternion */
    T_GduVector3f a;  /*!< accelerometer reading unit: g */
    T_GduVector3f w;  /*!< gyro reading unit: rad/sec */
} T_GduFcSubscriptionHardSync;

typedef uint8_t T_GduFcSubscriptionGpsControlLevel;

/*!
 * @brief struct for TOPIC_RC_WITH_FLAG_DATA
 */
typedef struct RCWithFlagData {
    gdu_f32_t pitch;       /*!< down = -0.999, middle = 0.000, up   =0.999 */
    gdu_f32_t roll;        /*!< left = -0.999, middle = 0.000, right=0.999 */
    gdu_f32_t yaw;         /*!< left = -0.999, middle = 0.000, right=0.999 */
    gdu_f32_t throttle;    /*!< down = -0.999, middle = 0.000, up   =0.999 */
    struct {
        uint8_t logicConnected: 1;  /*!< 0 if sky or ground side is disconnected for 3 seconds   */
        uint8_t skyConnected: 1;  /*!< Sky side is connected, i.e., receiver is connected to FC */
        uint8_t groundConnected: 1;  /*!< Ground side is connected, i.e., RC is on and connected to FC */
        uint8_t appConnected: 1;  /*!< Mobile App is connected to RC */
        uint8_t reserved: 4;
    } flag;
} T_GduFcSubscriptionRCWithFlagData;

/*!
 * @brief struct for status of each individual esc
 */
typedef struct ESCStatusIndividual {
    int16_t current;              /*!< ESC current, unit: mA */
    int16_t speed;                /*!< ESC speed, unit: rpm */
    uint16_t voltage;              /*!< Input power from battery to ESC, unit: mV */
    int16_t temperature;          /*!< ESC temperature, unit: degree C */
    uint16_t stall: 1; /*!< Motor is stall */
    uint16_t empty: 1; /*!< Motor has no load */
    uint16_t unbalanced: 1; /*!< Motor speed is unbalanced */
    uint16_t escDisconnected: 1; /*!< ESC is disconnected */
    uint16_t temperatureHigh: 1; /*!< Temperature is high */
    uint16_t reserved: 11;
} ESCStatusIndividual;

/*!
 * @brief struct for TOPIC_ESC_DATA
 */
typedef struct EscData {
    ESCStatusIndividual esc[8];
} T_GduFcSubscriptionEscData;

/*!
 * @brief struct indicating RTK GPS Connection
 */
typedef struct RTKConnectStatus {
    uint16_t rtkConnected: 1;
    uint16_t reserve: 15;
} T_GduFcSubscriptionRTKConnectStatus;

typedef uint8_t T_GduFcSubscriptionGimbalControlMode;

/*!
 * @brief struct for TOPIC_FLIGHT_ANOMALY
 */
typedef struct FlightAnomaly {
    uint32_t impactInAir: 1;  /*!< 0: No impact,                      1: Impact happens in Air */
    uint32_t randomFly: 1;  /*!< 0: Normal,                         1: Randomly fly in GPS mode without stick input*/
    uint32_t heightCtrlFail: 1;  /*!< 0: Height control normal,          1: Height control failed */
    uint32_t rollPitchCtrlFail: 1;  /*!< 0: Tilt control normal,            1: Tilt control failed */
    uint32_t yawCtrlFail: 1;  /*!< 0: Yaw control normal,             1: Yaw control failed */
    uint32_t aircraftIsFalling: 1;  /*!< 0: Aircraft is not falling,        1: Aircraft is falling */
    uint32_t strongWindLevel1: 1;  /*!< 0: Wind is under big wind level 1, 1: wind is stronger than  big wind level 1*/
    uint32_t strongWindLevel2: 1;  /*!< 0: Wind is under big wind level 2, 1: wind is stronger than  big wind level 2*/
    uint32_t compassInstallationError: 1;  /*!< 0: Compass install right,          1: Compass install error */
    uint32_t imuInstallationError: 1;  /*!< 0: IMU install right,              1: IMU install error */
    uint32_t escTemperatureHigh: 1;  /*!< 0: ESC temperature is normal,      1: ESC temperature is high */
    uint32_t atLeastOneEscDisconnected: 1;  /*!< 0: No ESC disconnected,            1: At least one ESC is disconnected */
    uint32_t gpsYawError: 1;  /*!< 0: No GPS yaw error,               1: GPS yaw error */
    uint32_t reserved: 19;
} T_GduFcSubscriptionFlightAnomaly;

/*!
 * @brief struct for TOPIC_POSITION_VO
 */
typedef struct PositionVO {
    gdu_f32_t x;              /*!< North (best effort), unit: m */
    gdu_f32_t y;              /*!< East (best effort),  unit: m */
    gdu_f32_t z;              /*!< Down,  unit: m */
    uint8_t xHealth: 1;
    uint8_t yHealth: 1;
    uint8_t zHealth: 1;
    uint8_t reserved: 5;
} T_GduFcSubscriptionPositionVO;

/*!
 * @brief struct for data broadcast and subscription, return obstacle info around the vehicle
 *
 * @note available in M210 (front, up, down)
 */
typedef struct RelativePosition {
    gdu_f32_t down;            /*!< distance from obstacle (m) */
    gdu_f32_t front;           /*!< distance from obstacle (m) */
    gdu_f32_t right;           /*!< distance from obstacle (m) */
    gdu_f32_t back;            /*!< distance from obstacle (m) */
    gdu_f32_t left;            /*!< distance from obstacle (m) */
    gdu_f32_t up;              /*!< distance from obstacle (m) */
    uint8_t downHealth: 1;  /*!< Down sensor flag: 0 - not working, 1 - working */
    uint8_t frontHealth: 1; /*!< Front sensor flag: 0 - not working, 1 - working */
    uint8_t rightHealth: 1; /*!< Right sensor flag: 0 - not working, 1 - working */
    uint8_t backHealth: 1;  /*!< Back sensor flag: 0 - not working, 1 - working */
    uint8_t leftHealth: 1;  /*!< Left sensor flag: 0 - not working, 1 - working */
    uint8_t upHealth: 1;    /*!< Up sensor health flag: 0 - not working, 1 - working */
    uint8_t reserved: 2;    /*!< Reserved sensor health flag*/
} T_GduFcSubscriptionAvoidData;

/**
 * @brief Struct for the topic GDU_FC_SUBSCRIPTION_TOPIC_HOME_POINT_SET_STATUS. Specifies homepoint was set or not,
 * it can be any value of ::E_GduFcSubscriptionHomePointSetStatus.
 */
typedef uint8_t T_GduFcSubscriptionHomePointSetStatus;

/**
 * @brief Struct for the topic GDU_FC_SUBSCRIPTION_TOPIC_HOME_POINT_INFO. Specifies homepoint.
 */
typedef struct HomeLocationData {
    gdu_f64_t latitude;  /*!< unit: rad */
    gdu_f64_t longitude; /*!< unit: rad */
} T_GduFcSubscriptionHomePointInfo; // pack(1)

typedef struct GimbalSingleData {
    gdu_f32_t pitch;
    gdu_f32_t roll;
    gdu_f32_t yaw;
    uint32_t status;
    uint8_t mode;
} GimbalSingleData;

/**
 * @brief Struct for the topic GDU_FC_SUBSCRIPTION_TOPIC_THREE_GIMBAL_DATA. Used in M300
 */
typedef struct GimbalThreeData {
    GimbalSingleData gbData[3];
} T_GduFcSubscriptionThreeGimbalData;

#pragma pack()

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise data subscription module in blocking mode. This function has to be called before subscribing any
 * data, to initialize run environment of data subscription module, if need to subscribe data from aircraft.
 * @note Max execution time of this function is slightly larger than 500ms.
 * @note This function has to be called in user task, rather than main() function, and after scheduler being started.
 * @return Execution result.
 */
T_GduReturnCode GduFcSubscription_Init(void);

/**
 * @brief Deinitialize data subscription module. When data subscription will no longer be used, can call this function
 * to deinitialize the data subscription module. Deinitialization function will help to release all system resource data
 * subscription has occupied.
 * @return Execution result.
 */
T_GduReturnCode GduFcSubscription_DeInit(void);

/**
 * @brief Subscribe a topic in blocking mode. Before subscribing any data from aircraft, GduFcSubscription_Init()
 * function has to be called.
 * @details User can subscribe a topic by specifying topic name, push frequency and callback function used to receive
 * data of topic (if needed). After subscribing successfully, the user can call
 * GduFcSubscription_GetLatestValueOfTopic() function to get the latest data of the topic have been
 * subscribed and the corresponding timestamp when aircraft sends the data out, and the callback function will be called to
 * push data of the topic and corresponding timestamp if the callback function is specified.
 * @note Max execution time of this function is slightly larger than 1200ms.
 * @note Topic to be subscribed can not have been subscribed, that is, one topic can not be subscribed repeatedly.
 * @note User must ensure that types of subscription frequency of all topics have been subscribed is less than or
 * equal to 4. Otherwise, the subscribing topic will fail.
 * @note User must ensure that the data length sum of all topics of the same subscription frequency is less than or equal to 242.
 * @param topic: topic name to be subscribed.
 * @param frequency: subscription frequency of topic to be subscribed. Subscription frequency can not beyond max
 * frequency limitation of the topic and must be the value of enum E_GduFcSubscriptionTopicFreq. And, subscription
 * frequency has to be larger than 0. Users can find max frequency of topics in data subscription part of documentation
 * on developer website (developer.gdu.com).
 * @param callback: callback function used to receive data of topic to be subscribed. If the callback function is not needed,
 * this item can be set as NULL.
 * @return Execution result.
 */
T_GduReturnCode GduFcSubscription_SubscribeTopic(E_GduFcSubscriptionTopic topic,
                                                 E_GduDataSubscriptionTopicFreq frequency,
                                                 GduReceiveDataOfTopicCallback callback);

/**
 * @brief Get the latest data value and timestamp in aircraft time system when sending the data from aircraft of specified
 * topic. If the specified topic has not been subscribed successfully, this function will return the error code.
 * @note After calling this function, user need transfer type of data pointer that pointer to data of topic to
 * corresponding data structure pointer for getting every item of the topic conveniently.
 * @param topicName: topic name to be gotten value.
 * @param data: pointer to memory space used to store data of the topic. The memory space used to store data of topic
 * have to have been allocated correctly and should ensure its size is equal to data structure size corresponding to
 * the topic, otherwise, this function will not be able to return data and timestamp (return error code).
 * @param dataSizeOfTopic: the size of memory space used to store data of topic. Normally, this size is equal to data
 * structure size corresponding to the topic. If this size is not equal to the size of the memory space, may cause memory
 * overflow event
 * @param timestamp: pointer to memory space used to store timestamps. The memory space used to store timestamps
 * have to have been allocated correctly, and should ensure its size is equal to data structure size of timestamp,
 * otherwise, this function will not be able to return data and timestamp (return error code) or even cause memory
 * overflow event. If the user does not need timestamp information, can fill in NULL.
 * @return Execution result.
 */
T_GduReturnCode GduFcSubscription_GetLatestValueOfTopic(E_GduFcSubscriptionTopic topic,
                                                        uint8_t *data, uint16_t dataSizeOfTopic,
                                                        T_GduDataTimestamp *timestamp);
#ifdef __cplusplus
}
#endif

#endif // GDU_FC_SUBSCRIPTION_H
/************************ (C) COPYRIGHT GDU Innovations *******END OF FILE******/
