## 概述

为方便开发者在 PSDK 的基础上快速开发出控制 GDU 无人机上相机的功能，GDU PSDK 提供了相机管理功能，开发者基于相机管理的接口能够同时设置并获取无人机上多个相机的感光度、光圈、快门和曝光等参数的值，控制相机实现拍照、录像及指点变焦等功能。


## 相机管理功能

在使用相机管理功能时，开发者需要先**初始化** PSDK 中的相机管理功能模块，再根据实际的使用需要**设置相机的模式**，最后根据用户的使用逻辑实现所需使用的功能，如设置相机的参数或检查功能的状态等。

|功能|8K|8KC|30X|PFL_ONE|PDL_300G|PDL_1K|PQL_02|SX_DL|SX_TL|GD_FL|GD_1K_FL|GD_DL|GD_TL|PSDK|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|基础信息|获取相机类型|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|获取固件版本号|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|获取相机连接状态|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|拍摄功能|设置/获取工作模式|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|设置/获取拍照模式|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|开始/停止拍照|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|设置/获取连拍参数|\-|\-|\-|\-|\-|\-|\-|✓|✓|\-|✓|\-|\-|\-|✓|
|设置/获取定时拍参数|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|开始/停止录像|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|相机拍照状态|✓|✓|✓|✓|✓|\-|\-|✓|✓|✓|✓|✓|✓|✓|✓|
|多镜头拍照存储设置|✓|✓|✓|✓|✓|\-|\-|\-|\-|\-|\-|✓|✓|✓|✓|
|获取相机的照片格式范围|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|设置和获取相机的照片格式|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|获取定时拍照的倒计时|✓|✓|✓|✓|✓|\-|\-|✓|✓|✓|✓|✓|✓|✓|✓|
|录像状态|✓|✓|✓|✓|✓|\-|\-|✓|✓|✓|✓|✓|✓|✓|✓|
|相机当前录制视频的时长|✓|✓|✓|✓|✓|\-|\-|✓|✓|✓|✓|✓|✓|✓|✓|
|多镜头录像存储设置|✓|✓|✓|✓|✓|\-|\-|\-|\-|\-|\-|✓|✓|✓|✓|
|获取相机的录像格式范围|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|设置和获取相机支持的视频格式|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|获取相机视频源范围|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|设置相机视频源|✓|✓|✓|✓|✓|✓|✓|\-|\-|\-|\-|✓|✓|✓|✓|
|获取相机镜头照片比例范围|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|设置相机镜头照片比例|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|获取相机镜头视频分辨率和帧率|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|设置开启夜景模式|✓|✓|\-|✓|✓|\-|\-|\-|\-|\-|✓|✓|✓|\-|\-|
|变焦/对焦功能|设置/获取对焦模式|✓|✓|✓|✓|✓|✓|✓|✓|\-|\-|✓|✓|✓|✓|✓|
|设置/获取对焦目标点|✓|✓|✓|✓|✓|✓|✓|✓|\-|\-|\-|✓|✓|✓|✓|
|开始/停止持续光学变焦|✓|✓|✓|✓|✓|\-|\-|✓|\-|\-|✓|✓|✓|✓|✓|
|设置/获取光学变焦参数|✓|✓|✓|✓|✓|\-|\-|✓|\-|\-|✓|✓|✓|✓|✓|
|设置/获取指点变焦开关|✓|✓|✓|✓|✓|\-|✓|✓|\-|\-|✓|✓|✓|✓|✓|
|设置/获取指点变焦系数|\-|\-|✓|✓|✓|\-|✓|✓|\-|\-|✓|✓|✓|✓|✓|
|基于目标点执行指点变焦|✓|✓|✓|✓|✓|✓|✓|✓|\-|\-|✓|✓|✓|\-|\-|
|获取相机手动对焦时所能设置的最小数值|✓|✓|✓|✓|✓|✓|✓|✓|\-|\-|✓|✓|✓|✓|✓|
|获取相机手动对焦时所能设置的最大数值|✓|✓|✓|✓|✓|✓|✓|✓|\-|\-|✓|✓|✓|✓|✓|
|设置相机的手动对焦值|✓|✓|✓|✓|✓|✓|✓|✓|\-|\-|✓|✓|✓|✓|✓|
|相机基础参数|设置/获取曝光模式|✓|✓|✓|✓|✓|✓|✓|\-|\-|\-|✓|✓|✓|✓|✓|
|设置/获取 ISO|✓|✓|✓|✓|✓|✓|✓|\-|\-|\-|✓|✓|✓|✓|✓|
|设置/获取光圈|✓|✓|✓|✓|✓|✓|✓|\-|\-|\-|✓|✓|✓|✓|✓|
|设置/获取快门速度|✓|✓|✓|✓|✓|✓|✓|\-|\-|\-|✓|✓|✓|✓|✓|
|设置/获取曝光补偿参数|✓|✓|✓|✓|✓|✓|✓|\-|\-|\-|✓|✓|✓|✓|✓|
|相机镜头的自动曝光锁定|✓|✓|✓|✓|✓|✓|✓|\-|\-|\-|✓|✓|✓|✓|✓|
|重置相机参数|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|媒体文件管理|下载媒体文件列表|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|分片下载媒体文件列表|✓|✓|✓|✓|✓|\-|✓|\-|\-|\-|\-|✓|✓|✓|✓|
|下载媒体文件|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|删除媒体文件|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|设置和获取自定义文件夹的扩展名|✓|✓|✓|✓|✓|✓|✓|\-|\-|\-|\-|✓|✓|✓|✓|
|设置和获取自定义文件的扩展名称|✓|✓|✓|✓|✓|\-|✓|\-|\-|\-|\-|✓|✓|✓|✓|
|获取相机 SD 卡的存储信息|✓|✓|✓|✓|✓|\-|\-|✓|✓|✓|✓|✓|✓|✓|✓|
|格式化存储|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|
|激光测距功能|获取激光测距信息|✓|✓|✓|✓|✓|\-|\-|\-|\-|\-|\-|\-|✓|\-|\-|
|红外相机功能|设置红外测温坐标点
获取红外测温坐标点的数据|\-|\-|\-|\-|\-|\-|\-|\-|\-|\-|\-|\-|✓|\-|✓|
|设置红外测温区域
获取红外测温区域的数据|\-|\-|\-|\-|\-|\-|\-|\-|\-|\-|\-|\-|✓|\-|✓|
|设置红外变焦参数|\-|✓|\-|✓|✓|\-|\-|\-|\-|\-|\-|\-|\-|\-|\-|
|设置和获取增益模式|\-|✓|\-|✓|✓|\-|\-|\-|\-|✓|\-|\-|✓|\-|✓|
|获取不同增益模式的温度范围|\-|✓|\-|✓|✓|\-|\-|\-|\-|✓|\-|\-|✓|\-|✓|
|设置 FFC 校准模式|\-|✓|\-|✓|✓|\-|\-|\-|\-|✓|\-|\-|✓|\-|✓|
|手动触发一次 FFC 校准模式|\-|✓|\-|✓|✓|\-|\-|\-|\-|✓|\-|\-|✓|\-|✓|
|开启或者关闭联动变焦功能|\-|✓|\-|\-|✓|\-|\-|\-|\-|\-|\-|\-|✓|\-|✓|
|测光功能|设置相机镜头的测光模式|✓|✓|✓|✓|✓|✓|✓|\-|\-|\-|✓|✓|✓|✓|✓|
|相机镜头点测光|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|✓|

表. 相机支持功能差异概览

## 使用相机管理功能

### 1\. 相机管理模块初始化

使用 PSDK 开发的负载设备如需控制相机功能，需要先调用`GduCameraManager_Init()`接口初始化相机管理模块。

```
USER_LOG_INFO("--> Step 1: Init camera manager module");
GduTest_WidgetLogAppend("--> Step 1: Init camera manager module");
returnCode = GduCameraManager_Init();
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("Init camera manager failed, error code: 0x%08X\r\n", returnCode);
    goto exitCameraModule;
}
```

### 2\. 获取相机类型和版本

开发者可通过分别调用`GduCameraManager_GetCameraType`和`GduCameraManager_GetFirmwareVersion`接口来获取无人机上指定挂载位置的相机类型和版本。

```
    USER_LOG_INFO("--> Step 2: Get camera type and version");
    GduTest_WidgetLogAppend("--> Step 2: Get camera type and version");
    returnCode = GduCameraManager_GetCameraType(mountPosition, &cameraType);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get mounted position %d camera's type failed, error code: 0x%08X\r\n",
                       mountPosition, returnCode);
        goto exitCameraModule;
    }
    USER_LOG_INFO("Mounted position %d camera's type is %s",
                  mountPosition,
                  s_cameraTypeStrList[GduTest_CameraManagerGetCameraTypeIndex(cameraType)].cameraTypeStr);

    returnCode = GduCameraManager_GetFirmwareVersion(mountPosition, &firmwareVersion);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get mounted position %d camera's firmware version failed, error code: 0x%08X\r\n",
                       mountPosition, returnCode);
        goto exitCameraModule;
    }
    USER_LOG_INFO("Mounted position %d camera's firmware is V%d.%d.%d.%d\r\n", mountPosition,
                  firmwareVersion.firmware_version[0], firmwareVersion.firmware_version[1],
                  firmwareVersion.firmware_version[2], firmwareVersion.firmware_version[3]);
```

### 3\. 设置或获取相机参数

相机管理提供一系列的 Set 或者 Get 接口，可以设置或获取指定挂载位置的相机参数，如可以通过分别调用`GduCameraManager_GetISO`和`GduCameraManager_SetISO`接口来获取和设置相机的 ISO 参数。

```
T_GduReturnCode returnCode;
E_GduCameraManagerISO isoDataTemp;

returnCode = GduCameraManager_GetISO(position, &isoDataTemp);
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
    returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
    USER_LOG_ERROR("Get mounted position %d camera's iso failed, error code: 0x%08X.",
                   position, returnCode);
    return returnCode;
}

if (isoDataTemp == isoData) {
    USER_LOG_INFO("The mounted position %d camera's iso is already what you expected.",
                  position);
    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

returnCode = GduCameraManager_SetISO(position, isoData);
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS &&
    returnCode != GDU_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND) {
    USER_LOG_ERROR("Set mounted position %d camera's iso %d failed, "
                   "error code: 0x%08X.", position, isoData, returnCode);
}

return returnCode;
```

### 4\. 控制相机执行指定的动作

开发者可通过调用对应的接口来执行指定的相机动作，如调用`GduCameraManager_StartShootPhoto`接口可以控制指定位置的相机进行拍照的操作。

```
/*!< start to shoot single photo */
USER_LOG_INFO("Mounted position %d camera start to shoot photo", position);
returnCode = GduCameraManager_StartShootPhoto(position, GDU_CAMERA_MANAGER_SHOOT_PHOTO_MODE_SINGLE);
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("Mounted position %d camera shoot photo failed, "
                   "error code :0x%08X", position, returnCode);
}
```

### 6\. 下载和删除相机原始媒体文件

开发者可以通过调用以下 Sample 来下载和删除相机拍摄的媒体文件，支持下载的媒体文件格式包括：JPG、DNG、MOV、MP4

```
GduTest_CameraManagerMediaDownloadAndDeleteMediaFile(mountPosition);
```