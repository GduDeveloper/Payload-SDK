## 目录

-   宏定义、枚举和结构体
    
    T_GduGimbalManagerRotation
    
-   函数原型
    
    GduGimbalManager_Init
    
    GduGimbalManager_Deinit

    GduGimbalManager_LookAt
    
    GduGimbalManager_SetMode
    
    GduGimbalManager_Reset
    
    GduGimbalManager_Rotate

    GduGimbalManager_RestoreFactorySettings
    

## 宏定义、枚举和结构体

### typedef struct T_GduGimbalManagerRotation

定义云台管理旋转属性。

```
typedef struct {
E_GduGimbalRotationMode rotationMode; /*!< 云台旋转模式 */
gdu_f32_t pitch; /*!< 俯仰角，单位：度 */
gdu_f32_t roll; /*!< 翻滚角，单位：度 */
gdu_f32_t yaw; /*!< 偏航角，单位：度 */
gdu_f64_t time; /*!< 预计执行时间，单位：秒 */
} T_GduGimbalManagerRotation;
```

## 函数原型

### function GduGimbalManager_Init

|功能：初始化云台管理模块|适用产品：所有|
|---|---|

> **注：** 请在 GduCore_Init 之后初始化此接口。

```
T_GduReturnCode GduGimbalManager_Init(void);
```

返回值

返回码详情请参见：GduErrorCode

### function GduGimbalManager_Deinit

|功能：反初始化云台管理模块|适用产品：所有|
|---|---|

```
T_GduReturnCode GduGimbalManager_Deinit(void);
```

返回值

返回码详情请参见： GduErrorCode

### function GduGimbalManager_LookAt

|功能：设置云台工作模式|适用产品：所有|
|---|---|

```
T_GduReturnCode GduGimbalManager_LookAt(E_GduMountPosition mountPosition, T_GduGimbalManagerLookAt info);
```

参数

info：目标点，输入限制参见 T_GduGimbalManagerLookAt

mountPosition：云台挂载位置，输入限制参见E_GduMountPosition。

返回值

返回码详情请参见：GduErrorCode

### function GduGimbalManager_SetMode

|功能：设置云台工作模式|适用产品：所有|
|---|---|

```
T_GduReturnCode GduGimbalManager_SetMode(E_GduMountPosition mountPosition, E_GduGimbalMode mode);
```

参数

mode：云台工作模式，输入限制参见 E_GduGimbalMode。

mountPosition：云台挂载位置，输入限制参见E\_GduMountPosition。

返回值

返回码详情请参见：GduErrorCode

### function GduGimbalManager_Reset

|功能：重置云台的俯仰和偏航角|适用产品：所有|
|---|---|

```
T_GduReturnCode GduGimbalManager_Reset(E_GduMountPosition mountPosition, E_GduGimbalResetMode resetMode);
```

参数

mode：重置模式，输入限制参见 E\_GduGimbalResetMode。

mountPosition：云台挂载位置，输入限制参见 E\_GduMountPosition。

返回值

返回码详情请参见：GduErrorCode

### function GduGimbalManager_Rotate

|功能：旋转云台角度|适用产品：所有|
|---|---|

```
T_GduReturnCode GduGimbalManager_Rotate(E_GduMountPosition mountPosition, T_GduGimbalManagerRotation rotation);
```

参数

rotation：执行目标云台上的旋转参数，包括旋转模式、目标角度值和执行时间，参见 T\_GduGimbalManagerRotation。

mountPosition：云台安装位置，输入限制参见 E\_GduMountPosition。

返回值

返回码详情请参见：GduErrorCode

### function GduGimbalManager_RestoreFactorySettings

|功能：恢复云台的出厂设置|适用产品：所有|
|---|---|

```
T_GduReturnCode GduGimbalManager_RestoreFactorySettings(E_GduMountPosition mountPosition);
```

参数

mountPosition：云台安装位置，输入限制参见 E_GduMountPosition。

返回值