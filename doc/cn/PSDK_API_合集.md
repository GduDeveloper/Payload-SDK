# PSDK API 合集文档

> 本文档汇集了 PSDK (Payload SDK) 的所有 API 文档，包含完整的函数原型、数据结构和枚举定义。

---

# 目录

## GDU Core 模块
1. [PSDK API 概览](#1-psdk-api-概览)
2. [GDU Core 模块](#2-gdu-core-模块)
3. [版本信息](#3-版本信息)
4. [错误码](#4-错误码)
5. [类型定义](#5-类型定义)

## 基础功能模块
6. [日志管理](#6-日志管理)
7. [信息管理](#7-信息管理)
8. [消息订阅](#8-消息订阅)
9. [飞行控制](#9-飞行控制)
10. [自定义控件](#10-自定义控件)
11. [HMS 功能](#11-hms-功能)
12. [时间同步](#12-时间同步)
13. [数据传输](#13-数据传输)
14. [本地升级](#14-本地升级)
15. [航迹规划](#15-航迹规划)
16. [精准定位](#16-精准定位)
17. [云台功能](#17-云台功能)
18. [相机功能](#18-相机功能)
19. [相机管理](#19-相机管理)

---

# 1. PSDK API 概览

---
title: PSDK API 概览
date: 2026-04-16
---

> **说明：**
>
> GDU 为支持开发者开发出可挂载在GDU 无人机上的负载设备，提供了开发工具包Payload SDK（即PSDK）以及开发配件FlyPort转接环，方便开发者利用GDU 无人机上如电源、通讯链路及状态信息等**资源**，开发出可挂载在GDU 无人机上的负载设备。开发者能够根据行业的应用需求，基于PSDK提供的功能接口，结合具体的结构设计、硬件设计、软件逻辑实现和算法优化，开发出如**自动巡检系统**、**红外相机**、**测绘相机**、**多光谱相机**、**喊话器**、**探照灯**等满足不同细分领域的负载设备。

## 主要特性

- 使用C 语言开发
- 支持主流的嵌入式系统，如Linux 和RTOS
- 模块化的设计思路，易于跨平台移植

---

# 2. GDU Core 模块

---
title: GDU Core
date: 2026-04-16
---

PSDK Core相关功能的头文件为`gdu_core.h`，本文档描述了`gdu_core.h` 文件中结构体和函数原型的关键信息和使用方法。

### 目录

- 宏定义、枚举与结构体
  T_GduUserInfo
- 函数原型
  GduCore_Init
  GduCore_SetAlias
  GduCore_ApplicationStart

### 宏定义、枚举与结构体)宏定义、枚举与结构体

#### typedef struct T_GduUserInfo

本结构体用于描述基于PSDK开发的负载的信息。

> **说明：** 请确认Name、ID 和 Key等信息宇开发者网站一致。更多信息请查看用户中心

```c
typedef struct {
    char appName[32]; /*!< 指定 GDU SDK 应用名称。 该信息可通过登录开发者网站 获取。 以"\0"结尾。 */
    char appId[16]; /*!< 指定 GDU SDK 应用 ID。 该信息可通过登录开发者网站 获取。*/
    char appKey[32]; /*!< 指定 GDU SDK 应用密钥。 该信息可通过登录开发者网站  获取。 */
    char appLicense[512]; /*!< 指定 GDU SDK 应用证书. 该信息可通过登录开发者网站  获取。 */
    char developerAccount[64]; /*!< 指定 GDU SDK 开发者账号邮箱。 该信息可通过登录开发者网站  获取。 开发者账号等相关信息需要能够对应。 以"\0"结尾。 */
    char baudRate[7]; /*!< 指定 GDU SDK 通信串口波特率。*/
} T_GduUserInfo;
```

### 函数原型

#### GduCore_Init

| 功能：初始化 Payload SDK 内核。 | product:all |
| ------------------------------- | ----------- |
|                                 |             |

以阻塞模式初始化 Payload SDK 内核。

> **说明：**
>
> - 这个接口的调用位置需要特别注意，需要在console/OSAL handler函数/HAL handler函数注册完成后调用。 同时，必须在调用其他功能模块接口开始时进行初始化。 您需要正确填写开发者信息，以确保初始化成功。 如需更多说明，请参阅教程"PSDK 初始化"。
> - 该函数在获得正确的飞行器类型和 PSDK 适配器类型之前不会返回。 该逻辑保证飞行器和PSDK适配器在PSDK功能模块和用户程序运行前已经正常启动。 该函数的一般执行时间为 2~4 秒。

```c
T_GduReturnCode GduCore_Init(const T_GduUserInfo *userInfo);
```

参数
： 指向 PSDK 应用程序信息的指针。返回值根据程序执行的情况输出对应的返回值，详情请参见：

####  GduCore_SetAlias

| 功能：为 GDU 应用或产品设置一个满足条件的别名。 | product:all |
| ----------------------------------------------- | ----------- |
|                                                 |             |

别名将在 GDU Pilot 中显示（如果存在）。

> **说明：**
>
> - 仍然需要将从 GDU SDK 开发者网站获取的正确的 GDU APP 名称传递给 GduCore_Init() 接口。 GDU APP 名称将用于绑定或验证。
> - 别名会在一段时间后生效，最大值为1s。

```c
T_GduReturnCode GduCore_SetAlias(const char *productAlias);
```

参数
： 指向产品别名字符串的指针，别名以 '\0' 结尾。 字符串的最大长度为 30。如果别名字符串的长度大于 30，别名字符串将被截断并传入。返回值根据程序执行的情况输出对应的返回值，详情请参见：

####  GduCore_ApplicationStart

| 功能：通知 Payload SDK 核心应用程序启动。 | product:all |
| ----------------------------------------- | ----------- |
|                                           |             |

> **说明：** 该接口的调用位置需要特别注意，需要在所有模块初始化和注册接口后完成调用。同时该接口为阻塞式接口，直到和无人机通信上才会继续往下执行。

```c
T_GduReturnCode GduCore_ApplicationStart(void);
```


返回值
根据程序执行的情况输出对应的返回值，详情请参见：GduErrorCode

---

# 3. 版本信息

---
title: GDU 版本信息
date: 2026-04-16
---

PSDK 获取版本信息相关功能的头文件为`gdu_version.h`，本文档描述了`gdu_version.h` 文件中的宏定义。

### 宏定义

- GDU SDK 主版本号

GDU SDK 主版本号，当有不兼容的 API 更改时。 范围从 0 到 99。

```c
#define GDU_VERSION_MAJOR     3  
```

- GDU SDK 次版本号

GDU SDK 次版本号，当以向后兼容的方式添加功能时会发生变化。 范围从 0 到 99。

```c
#define GDU_VERSION_MINOR     1
```

- GDU SDK 修改版本号

GDU SDK 修改版本号，当有向后兼容的错误修复更改时。 范围从 0 到 99。

```c
#define GDU_VERSION_MODIFY    0
```

- GDU SDK beta版本信息

GDU SDK beta 版本信息，发布版本为0，当beta版本发布变化时。 范围从 0 到 255。

```c
#define GDU_VERSION_BETA      0 
```

- GDU SDK 构建版本号，范围为0～65535

GDU SDK 版本构建信息，当 jenkins 触发构建更改时。 范围从 0 到 65535。

```c
#define GDU_VERSION_BUILD     1503
```

---

# 4. 错误码

---
title: GDU 错误码
date: 2026-04-16
---

错误码的头文件为`gdu_error.h`，本文档描述了错误码相关的宏定义和结构体的关键信息以及使用方法。

### 宏定义

- 错误模块索引值偏移量

```C
#define GDU_ERROR_MODULE_INDEX_OFFSET  32u
```

- 错误模块索引值掩码

```c
#define GDU_ERROR_MODULE_INDEX_MASK    0x000000FF00000000u
```

- 原始错误码偏移量

```c
#define GDU_ERROR_RAW_CODE_OFFSET      0u
```

- 原始错误码掩码

```c
#define GDU_ERROR_RAW_CODE_MASK        0x00000000FFFFFFFFu
```

- 错误码生成宏

```c
#define GDU_ERROR_CODE(moduleIndex, rawErrCode) \
((uint64_t)0 | \
((((uint64_t)(moduleIndex)) << (GDU_ERROR_MODULE_INDEX_OFFSET)) & (GDU_ERROR_MODULE_INDEX_MASK)) | \
((((uint64_t)(rawErrCode)) << (GDU_ERROR_RAW_CODE_OFFSET)) & (GDU_ERROR_RAW_CODE_MASK)))
```

### PSDK 错误码

PSDK 错误码全集。用户可以搜索到所有错误码信息。请参考错误对象获取错误描述、错误原因以及错误恢复建议。

```c
enum GduErrorCode {
    /* system module error code, including some common error code */
    GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_SUCCESS),
    GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_REQUEST_PARAMETER = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_INVALID_REQUEST_PARAMETER),
    GDU_ERROR_SYSTEM_MODULE_CODE_EXECUTING_HIGHER_PRIORITY_TASK = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_EXECUTING_HIGHER_PRIORITY_TASK),
    GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_NONSUPPORT),
    GDU_ERROR_SYSTEM_MODULE_CODE_TIMEOUT = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_TIMEOUT),
    GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_MEMORY_ALLOC_FAILED),
    GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_INVALID_PARAMETER),
    GDU_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_NONSUPPORT_IN_CURRENT_STATE),
    GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_SYSTEM_ERROR),
    GDU_ERROR_SYSTEM_MODULE_CODE_HARDWARE_ERR = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_HARDWARE_ERR),
    GDU_ERROR_SYSTEM_MODULE_CODE_INSUFFICIENT_ELECTRICITY = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_INSUFFICIENT_ELECTRICITY),
    GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_UNKNOWN),
    GDU_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_NOT_FOUND),
    GDU_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_OUT_OF_RANGE),
    GDU_ERROR_SYSTEM_MODULE_CODE_BUSY = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_BUSY),
    GDU_ERROR_SYSTEM_MODULE_CODE_DUPLICATE = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_DUPLICATE),
    GDU_ERROR_SYSTEM_MODULE_CODE_ADAPTER_NOT_MATCH = GDU_ERROR_CODE(GDU_ERROR_MODULE_SYSTEM, GDU_ERROR_SYSTEM_MODULE_RAW_CODE_ADAPTER_NOT_MATCH),
    // ... 更多错误码定义请参考原始文档
};
```

---

# 5. 类型定义

---
title: GDU Typedef
date: 2026-04-16
---

Typedef 的头文件为`gdu_typedef.h`，本文档描述了`gdu_typedef.h` 文件中宏定义和结构体的关键信息和使用方法。

### 宏定义

- 圆周率

```C
#define GDU_PI                     (3.14159265358979323846f)
```

- 文件名大小

```c
#define GDU_FILE_NAME_SIZE_MAX     256
```

### 类型

- 返回值类型定义（uint64）

```c
typedef uint64_t T_GduReturnCode;
```

### 枚举值

#### enum E_GduMountPosition

安装位置

```c
typedef enum {
    GDU_MOUNT_POSITION_UNKNOWN = 0,
    GDU_MOUNT_POSITION_PAYLOAD_PORT_NO1 = 1,
    GDU_MOUNT_POSITION_PAYLOAD_PORT_NO2 = 2,
    GDU_MOUNT_POSITION_PAYLOAD_PORT_NO3 = 3,
    GDU_MOUNT_POSITION_EXTENSION_PORT = 4
} E_GduMountPosition;
```

#### enum E_GduAircraftType

无人机型号

```c
typedef enum {
    GDU_AIRCRAFT_TYPE_UNKNOWN = 0,
    GDU_AIRCRAFT_TYPE_S100 = 1, 
    GDU_AIRCRAFT_TYPE_W300 = 2, 
    GDU_AIRCRAFT_TYPE_600 = 3,
    GDU_AIRCRAFT_TYPE_S400 = 4, 
    GDU_AIRCRAFT_TYPE_S700 = 5,
    GDU_AIRCRAFT_TYPE_S220 = 6,
    GDU_AIRCRAFT_TYPE_S450 = 7, 
    GDU_AIRCRAFT_TYPE_S480 = 8, 
    GDU_AIRCRAFT_TYPE_S200 = 9,
    // ... 更多类型
} E_GduAircraftType;
```

---

# 6. 日志管理

---
title: 日志管理
date: 2026-04-16
---

日志管理相关功能的头文件为`gdu_logger.h`，本文档描述了`gdu_logger.h` 文件中结构体和函数原型的关键信息和使用方法。

### 目录

- 宏定义、枚举与结构体
  ConsoleFunc
  E_GduLoggerConsoleLogLevel
  T_GduLoggerConsole
- 函数原型
  GduLogger_AddConsole
  GduLogger_UserLogOutput

### 枚举

#### typedef enum E_GduLoggerConsoleLogLevel

日志级别。

```c
typedef enum {
    GDU_LOGGER_CONSOLE_LOG_LEVEL_ERROR = 0,      // 打印系统错误类型（Error）的日志
    GDU_LOGGER_CONSOLE_LOG_LEVEL_WARN = 1,       // 打印警告信息类型（Warning）的日志
    GDU_LOGGER_CONSOLE_LOG_LEVEL_INFO = 2,       // 打印关键信息类型（Info）的日志
    GDU_LOGGER_CONSOLE_LOG_LEVEL_DEBUG = 3,      // 打印调试信息类型（Debug）的日志
} E_GduLoggerConsoleLogLevel;
```

### 函数原型

#### GduLogger_AddConsole

| 功能：为OSDK添加控制台功能与等级。 | product：all |
| ---------------------------------- | ------------ |

```c
T_GduReturnCode GduLogger_AddConsole(T_GduLoggerConsole *console);
```

#### function GduLogger_UserLogOutput

| 功能：通过注册方法打印输出特定格式的选定等级的日志。 | product：all |
| ---------------------------------------------------- | ------------ |

```c
void PsdkLogger_UserLogInfo(const char *fmt, ...);
```

---

# 7. 信息管理

---
title: 信息管理
date: 2026-04-16
---

无人机信息（信息管理）相关功能的头文件为`gdu_aircraft_info.h`，本文档描述了`gdu_aircraft_info.h` 文件中结构体和函数原型的关键信息和使用方法。

### 目录

- 宏定义、枚举与结构体
  T_GduMobileAppInfo
  T_GduAircraftInfoBaseInfo
- 函数原型
  GduAircraftInfo_GetBaseInfo
  GduAircraftInfo_GetMobileAppInfo

### 结构体

#### typedef struct T_GduAircraftInfoBaseInfo

飞行器系统的一些基础信息，主要包括系统的一些常数参数信息。

```c
typedef struct {
    E_GduAircraftType aircraftType; /*!< 无人机类型 */
    E_GduSdkAdapterType gduAdapterType; /*!< GDU 适配器类型 */
    E_GduMountPosition mountPosition; /*!< 负载安装位置 */
} T_GduAircraftInfoBaseInfo;
```

### 函数原型

#### function GduAircraftInfo_GetBaseInfo

| 功能：获取飞行器系统的基本信息 | product：all |
| ------------------------------ | ------------ |

```c
T_GduReturnCode GduAircraftInfo_GetBaseInfo(T_GduAircraftInfoBaseInfo *baseInfo);
```

---

# 8. 消息订阅

---
title: 消息订阅
date: 2026-04-16
---

消息订阅相关功能的头文件为`gdu_fc_subscription.h`，本文档描述了`gdu_fc_subscription.h` 文件中结构体和函数原型的关键信息和使用方法。

### 目录

- 枚举
  E_GduFcSubscriptionTopic
  E_GduFcSubscriptionFlightStatus
  E_GduFcSubscriptionDisplayMode
- 结构体
  T_GduFcSubscriptionQuaternion
  T_GduFcSubscriptionVelocity
  T_GduFcSubscriptionPositionFused
  // ... 更多结构体
- 函数原型
  GduFcSubscription_Init
  GduFcSubscription_SubscribeTopic
  GduFcSubscription_GetLatestValueOfTopic

### 枚举

#### typedef enum E_GduFcSubscriptionTopic

订阅主题枚举，包含大量飞行器状态数据主题，如加速度、速度、高度、位置、GPS、RTK等。

```c
typedef enum {
    GDU_FC_SUBSCRIPTION_TOPIC_ACCELERATION_BODY = ...,
    GDU_FC_SUBSCRIPTION_TOPIC_VELOCITY = ...,
    GDU_FC_SUBSCRIPTION_TOPIC_POSITION_FUSED = ...,
    GDU_FC_SUBSCRIPTION_TOPIC_GPS_POSITION = ...,
    GDU_FC_SUBSCRIPTION_TOPIC_RTK_POSITION = ...,
    // ... 更多主题
} E_GduFcSubscriptionTopic;
```

#### typedef enum E_GduFcSubscriptionFlightStatus

无人机飞行状态

```c
typedef enum {
    GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_STOPED = 0,         // 无人机在地面上且电机静止
    GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_ON_GROUND = 1,      // 无人机在地面上，但电机仍在转动 
    GDU_FC_SUBSCRIPTION_FLIGHT_STATUS_IN_AIR = 2,         // 无人机在空中 
} E_GduFcSubscriptionFlightStatus;
```

### 函数原型

#### function GduFcSubscription_Init

| 功能：消息订阅模块初始化 | product：all |
| ------------------------ | ------------ |

```c
T_GduReturnCode GduFcSubscription_Init(void);
```

#### function GduFcSubscription_SubscribeTopic

| 功能：订阅主题 | product：all |
| -------------- | ------------ |

```c
T_GduReturnCode GduFcSubscription_SubscribeTopic(E_GduFcSubscriptionTopic topic,
                                                 E_GduDataSubscriptionTopicFreq frequency,
                                                 GduReceiveDataOfTopicCallback callback);
```

#### function GduFcSubscription_GetLatestValueOfTopic

| 功能：获取最新的数据和时间戳 | product：all |
| ---------------------------- | ------------ |

```c
T_GduReturnCode GduFcSubscription_GetLatestValueOfTopic(E_GduFcSubscriptionTopic topic,
                                                        uint8_t *data, uint16_t dataSizeOfTopic,
                                                        T_GduDataTimestamp *timestamp);
```

---

# 9. 飞行控制

---
title: 飞行控制
date: 2026-04-16
---

飞行控制相关功能的头文件为`gdu_flight_controller.h`，本文档描述了`gdu_flight_controller.h` 文件中结构体和函数原型的关键信息和使用方法。

### 目录

- 枚举
  E_GduFlightControllerRCLostAction
  E_GduFlightControllerHorizontalControlMode
  E_GduFlightControllerVerticalControlMode
  E_GduFlightControllerYawControlMode
- 结构体
  T_GduFlightControllerJoystickCommand
  T_GduFlightControllerHomeLocation
- 函数原型
  GduFlightController_Init
  GduFlightController_StartTakeoff
  GduFlightController_StartLanding
  GduFlightController_StartGoHome
  // ... 更多函数

### 枚举

#### typedef enum E_GduFlightControllerRCLostAction

遥控器断连动作

```c
typedef enum {
    GDU_FLIGHT_CONTROLLER_RC_LOST_ACTION_HOVER = 0,  // 当遥控器连接失败，飞机将悬停
    GDU_FLIGHT_CONTROLLER_RC_LOST_ACTION_LANDING = 1,  // 当遥控器连接失败，飞机将执行下降操作
    GDU_FLIGHT_CONTROLLER_RC_LOST_ACTION_GOHOME = 2,  // 当遥控器连接是被，飞机将返航
} E_GduFlightControllerRCLostAction;
```

#### typedef enum E_GduFlightControllerHorizontalControlMode

水平控制模式

```c
typedef enum {
    GDU_FLIGHT_CONTROLLER_HORIZONTAL_ANGLE_CONTROL_MODE = 0,
    GDU_FLIGHT_CONTROLLER_HORIZONTAL_VELOCITY_CONTROL_MODE = 1,
    GDU_FLIGHT_CONTROLLER_HORIZONTAL_POSITION_CONTROL_MODE = 2,
    GDU_FLIGHT_CONTROLLER_HORIZONTAL_ANGULAR_RATE_CONTROL_MODE = 3
} E_GduFlightControllerHorizontalControlMode;
```

### 函数原型

#### function GduFlightController_Init

| 功能：飞行控制模块初始化 | product：all |
| ------------------------ | ------------ |

```c
T_GduReturnCode GduFlightController_Init(void);
```

#### function GduFlightController_StartTakeoff

| 功能：无人机在地面时请求起飞 | product：all |
| ---------------------------- | ------------ |

```c
T_GduReturnCode GduFlightController_StartTakeoff(void);
```

#### function GduFlightController_StartLanding

| 功能：无人机在空中时请求降落 | product：all |
| ---------------------------- | ------------ |

```c
T_GduReturnCode GduFlightController_StartLanding(void);
```

#### function GduFlightController_StartGoHome

| 功能：无人机在空中时请求返航 | product：all |
| ---------------------------- | ------------ |

```c
T_GduReturnCode GduFlightController_StartGoHome(void);
```

---

# 10. 自定义控件

---
title: 自定义控件
date: 2026-04-16
---

自定义控件相关功能的头文件为`gdu_widget.h`，本文档描述了`gdu_widget.h` 文件中结构体和函数原型的关键信息和使用方法。

### 目录

- 枚举
  E_GduWidgetType
  E_GduWidgetButtonState
  E_GduWidgetSwitchState
- 结构体
  T_GduWidgetHandlerListItem
- 函数原型
  GduWidget_Init
  GduWidget_RegHandlerList
  GduWidgetFloatingWindow_ShowMessage

### 枚举

#### typedef enum E_GduWidgetType

控件类型

```c
typedef enum {
    PSDK_WIDGET_TYPE_BUTTON = 1,        // 按钮类型
    PSDK_WIDGET_TYPE_SWITCH = 2,        // 开关类型
    PSDK_WIDGET_TYPE_SCALE = 3,         // 范围条类型
    PSDK_WIDGET_TYPE_LIST = 4,          // 列表类型
    PSDK_WIDGET_TYPE_INT_INPUT_BOX = 5, // 整型值输入框
    GDU_WIDGET_TYPE_TEXT_INPUT_BOX = 6, // 文本输入框
} E_GduWidgetType;
```

### 函数原型

#### function GduWidget_Init

| 功能：初始化自定义控件模块 | product：all |
| -------------------------- | ------------ |

```c
T_GduReturnCode GduWidget_Init(void);
```

#### function GduWidget_RegHandlerList

| 功能：注册处理函数列表 | product：all |
| ---------------------- | ------------ |

```c
T_GduReturnCode GduWidget_RegHandlerList(const T_GduWidgetHandlerListItem *widgetHandlerList, uint32_t itemCount);
```

#### function GduWidgetFloatingWindow_ShowMessage

| 功能：向移动端App 的浮窗发送消息。 | product：all |
| ---------------------------------- | ------------ |

```c
T_GduReturnCode GduWidgetFloatingWindow_ShowMessage(const char *str);
```

---

# 11. HMS 功能

---
title: HMS
date: 2026-04-16
---

健康管理系统相关功能的头文件为`gdu_hms.h`，本文档描述了`gdu_hms.h` 文件中结构体和函数原型的关键信息和使用方法。

### 目录

- 结构体
  T_GduHmsInfo
  T_GduHmsInfoTable
- 函数原型
  GduHms_Init
  GduHms_RegHmsInfoCallback

### 结构体

#### typedef struct T_GduHmsInfo

```c
typedef struct {
    uint32_t errorCode;
    uint8_t componentIndex;
    uint8_t errorLevel;
} T_GduHmsInfo;
```

### 函数原型

#### function GduHms_Init

| 功能：初始化 hms 模块 | product：all |
| --------------------- | ------------ |

```c
T_GduReturnCode GduHms_Init(void);
```

#### function GduHms_RegHmsInfoCallback

| 功能：注册回调以获取 hms 信息 | product：all |
| ----------------------------- | ------------ |

> **说明：** 数据以 1Hz 的频率推送。

```c
T_GduReturnCode GduHms_RegHmsInfoCallback(GduHmsInfoCallback callback);
```

---

# 12. 时间同步

---
title: 时间同步
date: 2026-04-16
---

时间同步相关功能的头文件为`gdu_time_sync.h`，本文档描述了`gdu_time_sync.h` 文件中结构体和函数原型的关键信息和使用方法。

### 目录

- 结构体
  T_GduTimeSyncAircraftTime
- 函数原型
  GduTimeSync_Init
  GduTimeSync_RegGetNewestPpsTriggerTimeCallback
  GduTimeSync_TransferToAircraftTime

### 结构体

#### typedef struct T_GduTimeSyncAircraftTime

无人机中时间系统的数据结构

```c
typedef struct {
    uint16_t year;          // 年
    uint8_t month;          // 月，1～12
    uint8_t day;            // 日，1～31
    uint8_t hour;           // 小时，0～23
    uint8_t minute;         // 分钟，0～59
    uint8_t second;         // 秒，0～59
    uint32_t microsecond;   // 微秒，0～999999.
} T_GduTimeSyncAircraftTime;
```

### 函数原型

#### function GduTimeSync_Init

| 功能：初始化时间同步功能模块 | product：all |
| ---------------------------- | ------------ |

```c
T_GduReturnCode GduTimeSync_Init(void);
```

#### function GduTimeSync_TransferToAircraftTime

| 功能：时间转换 | product：all |
| -------------- | ------------ |

将负载设备的本地时间转换为无人机上的时间。

```c
T_GduReturnCode GduTimeSync_TransferToAircraftTime(uint64_t localTimeUs, T_GduTimeSyncAircraftTime *aircraftTime);
```

---

# 13. 数据传输

---
title: 数据传输
date: 2026-04-16
---

## 低速数据通道

低速数据通道相关功能的头文件为`gdu_low_speed_data_channel.h`，本文档描述了`gdu_low_speed_data_channel.h` 文件中结构体和函数原型的关键信息和使用方法。

### 函数原型

#### function GduLowSpeedDataChannel_Init

| 功能：初始化低速数据通道模块 | product：all |
| ---------------------------- | ------------ |

```c
T_GduReturnCode GduLowSpeedDataChannel_Init(void);
```

#### function GduLowSpeedDataChannel_SendData

| 功能：通过命令通道将数据发送到选定的通道地址端 | product：all |
| ---------------------------------------------- | ------------ |

```c
T_GduReturnCode GduLowSpeedDataChannel_SendData(E_GduChannelAddress channelAddress, const uint8_t *data, uint8_t len);
```

---

# 14. 本地升级

---
title: 本地升级
date: 2026-04-16
---

本地升级相关功能的头文件为`gdu_upgrade.h`，本文档描述了`gdu_upgrade.h` 文件中结构体和函数原型的关键信息和使用方法。

> 注意：该功能目前暂未正式开放，请勿使用。

### 目录

- 枚举
  E_GduFirmwareTransferType
  E_GduUpgradeStage
  E_GduUpgradeEndState
- 结构体
  T_GduFirmwareVersion
  T_GduUpgradeHandler
- 函数原型
  GduUpgrade_Init
  GduUpgrade_EnableLocalUpgrade
  GduUpgrade_RegHandler

### 函数原型

#### function GduUpgrade_Init

| 功能：升级模块初始化 | product：all |
| -------------------- | ------------ |

```c
T_GduReturnCode GduUpgrade_Init(const T_GduUpgradeConfig *upgradeConfig);
```

#### function GduUpgrade_EnableLocalUpgrade

| 功能：启动本地升级模块 | product：all |
| ---------------------- | ------------ |

开启本地升级模式后，GDU Assistant 2 将显示负载升级界面。

```c
T_GduReturnCode GduUpgrade_EnableLocalUpgrade(void);
```

---

# 15. 航迹规划

---
title: 航迹规划
date: 2026-04-16
---

waypoint V3相关功能的头文件为`gdu_waypoint_v3.h`，本文档描述了`gdu_waypoint_v3.h` 文件中结构体和函数原型的关键信息和使用方法。

### 目录

- 枚举
  E_GduWaypointV3Action
  E_GduWaypointV3State
- 结构体
  T_GduWaypointV3MissionState
- 函数原型
  GduWaypointV3_Init
  GduWaypointV3_UploadXmlFile
  GduWaypointV3_Action

### 枚举

#### typedef enum E_GduWaypointV3Action

waypoint V3任务行为

```c
typedef enum {
    GDU_WAYPOINT_V3_ACTION_START = 1, // Waypoint v3 mission start action.
    GDU_WAYPOINT_V3_ACTION_PAUSE = 2, // Waypoint v3 mission pause action.
    GDU_WAYPOINT_V3_ACTION_RESUME = 3, // Waypoint v3 mission resume action.
    GDU_WAYPOINT_V3_ACTION_STOP = 4, // Waypoint v3 mission stop action.
} E_GduWaypointV3Action;
```

### 函数原型

#### function GduWaypointV3_Init

| 功能：waypoint V3模块初始化 | product：all |
| --------------------------- | ------------ |

```c
T_GduReturnCode GduWaypointV3_Init(void);
```

#### function GduWaypointV3_Action

| 功能：开始执行任务 | product：all |
| ------------------ | ------------ |

```c
T_GduReturnCode GduWaypointV3_Action(E_GduWaypointV3Action action);
```

---

# 16. 精准定位

---
title: 精准定位
date: 2026-04-16
---

精准定位相关功能的头文件为`gdu_positioning.h`，本文档描述了`gdu_positioning.h` 文件中结构体和函数原型的关键信息和使用方法。

### 目录

- 结构体
  T_GduPositioningEventInfo
  T_GduPositioningPosition
  T_GduPositioningPositionInfo
- 函数原型
  GduPositioning_Init
  GduPositioning_GetPositionInformationSync

### 结构体

#### typedef struct T_GduPositioningPosition

用于描述一个定位点的结构体

```c
typedef struct {
    gdu_f64_t longitude;     // 定位点的经度，单位：度
    gdu_f64_t latitude;      // 定位点的维度，单位：度
    gdu_f64_t height;        // 定位点海平面以上的高度，单位：米
} T_GduPositioningPosition;
```

### 函数原型

#### function GduPositioning_Init

| 功能：初始化精准定位模块 | product：all |
| ------------------------ | ------------ |

```c
T_GduReturnCode GduPositioning_Init(void);
```

#### function GduPositioning_GetPositionInformationSync

| 功能：获取精准定位信息 | product：all |
| ---------------------- | ------------ |

```c
T_GduReturnCode GduPositioning_GetPositionInformationSync(uint8_t eventCount, 
                                                          T_GduPositioningEventInfo *eventInfo,
                                                          T_GduPositioningPositionInfo *positionInfo);
```

---

# 17. 云台功能

---
title: 云台功能
date: 2026-04-16
---

PSDK 云台相关功能的头文件为`gdu_gimbal.h`，本文档描述了`gdu_gimbal.h` 文件中结构体和函数原型的关键信息和使用方法。

> **说明：** 仅使用FlyPort 开发的负载设备可使用`gdu_gimbal.h`。

### 目录

- 枚举
  E_GduGimbalCalibrationStage
  E_GduGimbalAxis
  E_GduGimbalResetMode
- 结构体
  T_GduGimbalSystemState
  T_GduGimbalAttitudeInformation
  T_GduGimbalCommonHandler
- 函数原型
  GduGimbal_Init
  GduGimbal_RegCommonHandler

### 枚举

#### typedef enum E_GduGimbalAxis

云台轴

```c
typedef enum {
    GDU_GIMBAL_AXIS_PITCH = 0,  // 俯仰轴
    GDU_GIMBAL_AXIS_ROLL = 1,  // 横滚轴
    GDU_GIMBAL_AXIS_YAW = 2,  // 偏航轴
} E_GduGimbalAxis;
```

### 函数原型

#### function GduGimbal_Init

| 功能：初始化云台模块 | product：all |
| -------------------- | ------------ |

```c
T_GduReturnCode GduGimbal_Init(void);
```

#### function GduGimbal_RegCommonHandler

| 功能：注册云台控制功能 | product：all |
| ---------------------- | ------------ |

注册控制云台的回调函数。

```c
T_GduReturnCode GduGimbal_RegCommonHandler(const T_GduGimbalCommonHandler *commonHandler);
```

---

# 18. 相机功能

---
title: 相机功能
date: 2026-04-16
---

PSDK 相机相关功能的头文件为`gdu_payload_camera.h`，本文档描述了`gdu_payload_camera.h` 文件中结构体和函数原型的关键信息和使用方法。

### 目录

- 枚举
  E_GduCameraMode
  E_GduCameraShootPhotoMode
  E_GduCameraShootingState
  E_GduCameraMeteringMode
  E_GduCameraFocusMode
  E_GduCameraVideoStreamType
- 结构体
  T_GduCameraSystemState
  T_GduCameraSDCardState
  T_GduCameraCommonHandler
  T_GduCameraFocusHandler
  T_GduCameraOpticalZoomHandler
- 函数原型
  GduPayloadCamera_Init
  GduPayloadCamera_RegCommonHandler
  GduPayloadCamera_SendVideoStream
  // ... 更多函数

### 枚举

#### typedef enum E_GduCameraMode

相机类负载设备的模式

```c
typedef enum {
    GDU_CAMERA_MODE_SHOOT_PHOTO = 0,      // 拍照模式 
    GDU_CAMERA_MODE_RECORD_VIDEO = 1,      // 录像模式 
    GDU_CAMERA_MODE_PLAYBACK = 2,      // 媒体文件回放 
} E_GduCameraMode;
```

#### typedef enum E_GduCameraShootPhotoMode

相机类负载设备的拍照模式

```c
typedef enum {
    GDU_CAMERA_SHOOT_PHOTO_MODE_SINGLE = 1,      // 单张拍摄模式 
    GDU_CAMERA_SHOOT_PHOTO_MODE_BURST = 4,      // 连拍模式 
    GDU_CAMERA_SHOOT_PHOTO_MODE_INTERVAL = 6,      // 间隔拍摄模式
} E_GduCameraShootPhotoMode;
```

### 函数原型

#### function GduPayloadCamera_Init

| 功能：PSDK 相机模块初始化 | product：all |
| ------------------------- | ------------ |

```c
T_GduReturnCode GduPayloadCamera_Init(void);
```

#### function GduPayloadCamera_RegCommonHandler

| 功能：注册相机类负载设备的基础功能 | product：all |
| ---------------------------------- | ------------ |

```c
T_GduReturnCode GduPayloadCamera_RegCommonHandler(const T_GduCameraCommonHandler *cameraCommonHandler);
```

#### function GduPayloadCamera_SendVideoStream

| 功能：通过数据通道的视频流将视频发送到移动端 | product：all |
| -------------------------------------------- | ------------ |

```c
T_GduReturnCode GduPayloadCamera_SendVideoStream(const uint8_t *data, uint16_t len);
```

---

# 19. 相机管理

---
title: 相机管理
date: 2026-04-16
---

## 目录

- 结构体
  T_GduGimbalManagerRotation
- 函数原型
  GduGimbalManager_Init
  GduGimbalManager_Deinit
  GduGimbalManager_LookAt
  GduGimbalManager_SetMode
  GduGimbalManager_Reset
  GduGimbalManager_Rotate

### 结构体

#### typedef struct T_GduGimbalManagerRotation

定义云台管理旋转属性。

```c
typedef struct {
    E_GduGimbalRotationMode rotationMode; /*!< 云台旋转模式 */
    gdu_f32_t pitch; /*!< 俯仰角，单位：度 */
    gdu_f32_t roll; /*!< 翻滚角，单位：度 */
    gdu_f32_t yaw; /*!< 偏航角，单位：度 */
    gdu_f64_t time; /*!< 预计执行时间，单位：秒 */
} T_GduGimbalManagerRotation;
```

### 函数原型

#### function GduGimbalManager_Init

|功能：初始化云台管理模块|适用产品：所有|

> **注：** 请在 GduCore_Init 之后初始化此接口。

```c
T_GduReturnCode GduGimbalManager_Init(void);
```

#### function GduGimbalManager_Rotate

|功能：旋转云台角度|适用产品：所有|

```c
T_GduReturnCode GduGimbalManager_Rotate(E_GduMountPosition mountPosition, 
                                         T_GduGimbalManagerRotation rotation);
```

---

# 附录：常用数据结构速查

## 无人机基础信息

```c
typedef struct {
    E_GduAircraftType aircraftType;           // 无人机类型
    E_GduSdkAdapterType gduAdapterType;       // GDU 适配器类型
    E_GduMountPosition mountPosition;          // 负载安装位置
} T_GduAircraftInfoBaseInfo;
```

## 飞行器姿态

```c
typedef struct {
    gdu_f32_t pitch; // 俯仰姿态
    gdu_f32_t roll;  // 横滚姿态
    gdu_f32_t yaw;  // 偏航姿态
} T_GduAttitude3f;
```

## 位置信息

```c
typedef struct {
    gdu_f64_t longitude; // 经度，单位: rad
    gdu_f64_t latitude;  // 纬度，单位: rad
    gdu_f32_t altitude; // 高度，WGS 84 参考椭球体，单位：m
} T_GduFcSubscriptionPositionFused;
```

## 时间戳

```c
typedef struct {
    uint32_t millisecond; // 毫秒
    uint32_t microsecond; // 微秒
} T_GduDataTimestamp;
```

---

*文档生成时间：2026-04-17*
*PSDK 版本：3.1.0*
