---
title: PSDK 教程合集
date: 2026-04-17
---

# PSDK 教程合集文档

---

# 目录

- [版本发布记录](#版本发布记录)
- [文档阅读指引](#文档阅读指引)
- [基础入门](#基础入门)
  - [什么是PSDK](#什么是psdk)
  - [PSDK 功能概览](#psdk-功能概览)
  - [如何使用PSDK](#如何使用psdk)
- [型号说明](#型号说明)
  - [负载开发标准](#负载开发标准)
  - [选择开发平台](#选择开发平台)
  - [选购硬件平台](#选购硬件平台)
- [快速开始](#快速开始)
  - [开发须知](#开发须知)
  - [设备连接](#设备连接)
  - [配置开发环境](#配置开发环境)
  - [运行示例程序](#运行示例程序)
  - [跨平台移植](#跨平台移植)
- [功能集](#功能集)
  - [日志管理](#日志管理)
  - [信息管理](#信息管理)
  - [基础相机功能](#基础相机功能)
  - [云台功能](#云台功能)
  - [电源管理](#电源管理)
  - [飞行控制](#飞行控制)
  - [自定义控件](#自定义控件)
  - [HMS 功能](#hms-功能)
  - [时间同步](#时间同步)
  - [数据传输](#数据传输)
  - [视频流传输](#视频流传输)
  - [本地升级](#本地升级)
  - [精准定位](#精准定位)
  - [运动规划](#运动规划)
  - [相机管理](#相机管理)

---

# 版本发布记录

---
title: 版本发布记录
date: 2026-04-16
---

## PSDK 版本支持信息

| 机型                   | 推荐的SDK版本 | 版本维护 | 新功能支持 |
| ---------------------- | ------------- | -------- | ---------- |
| S400行业系列及后续机型 | PSDK 3.0      | √        | √          |

## Payload SDK v3.0.16 发布记录

### 发布日期
2023.05.03

### 发布功能列表

-支持拍照录像功能
-支持云台控制
-支持消息订阅
-支持时间同步
-支持精准定位
-支持自定义控件
-支持云台控制
-支持电源管理
-支持飞行控制
-支持PPK功能
-支持航迹规划
-支持本地升级
-支持相机码流获取

### 支持产品列表与固件版本

- S400 行业系列

|       产品        |   固件版本   |
| :---------------: | :----------: |
|  GDU Flight II App  |   v1.0.060   |
|  S400 飞控          | v0.0.39  |

---

# 文档阅读指引

---
title: 文档阅读指引
date: 2026-04-10
---

在开始您的阅读前，请先查看本节，了解当前教程文档的结构与内容、获得阅读指引。期望通过本节帮助用户尽快了解与上手PSDK。

## 阅读公告
* API 文档与教程文档分开存放，点击[API Reference](/payload-sdk/api/)或页面右上角"Android API Reference"字样跳转。
* 若您对文档内容有什么疑问，请点击任意页面右下角的"文档反馈"标志，反馈您的问题并留下您的邮箱，我们将与您联系。

---

# 基础入门

---

## 什么是PSDK

---
title: 什么是PSDK
date: 2026-04-16
---

为方便您使用PSDK 提供的新功能开发出安全可靠的负载设备，请持续关注GDU PSDK 的版本发布信息，及时使用最新版本的PSDK 开发包开发负载设备。

> **本文所指**
>
> - "移动端App" 为使用MSDK 开发的移动端App 或GDU Flight II
> - "负载设备" 为使用PSDK 开发的负载设备。
> - "机载计算机" 为运行了基于PSDK 开发的无人机控制程序的计算设备

## PSDK介绍
GDU为了满足不同挂载能快速在GDU无人机上实现适配，开发了第三方通用开发平台payload sdk和airPort
转接环配件，方便开发者接入GDU无人机。开发者根据自身载荷应用场景，使用psdk提供的接口，可快速实现对应的功能

## 主要优势

- **功能丰富且完善**
  通过使用PSDK 提供的如信息获取、数据传输和电源管理等基础功能，以及相机、云台、负载协同和精准定位等高级功能，开发者能够根据行业的应用需求，设计出**功能完善**的负载设备。并且PSDK 提供了丰富的接口，方便开发者使用第三方应用程序和算法框架，使用图像识别、自主巡航及SLAM 等技术开发出专业的应用软件，此外，还方便开发者接入第三方传感器、相机或检测设备，采集所需的数据信息，满足用户个性化的应用功能和控制需求。
- **拓展应用自定义**
  开发者除使用GDU Fight II 控制基于PSDK 开发的负载设备和控制无人机和负载设备**自动执行任务**的控制程序外，通过使用Mobile SDK 也能开发出控制负载设备执行指定动作的**移动端App**
- **兼容广泛的软硬件平台**
  使用PSDK 开发的应用程序能够运行在主流的嵌入式硬件平台上，如STM32等，也可运行在主流的嵌入式操作系统和软件架构中，如Linux、ROS及RTOS，开发者按照跨平台移植中的步骤执行所需的工作后，即可在不同版本的软硬件平台上运行基于PSDK 开发的应用程序。
- **支持服务有保障**
  PSDK 不仅提供了用于开发GDU 无人机负载设备的API 接口和硬件平台，还提供了开发负载设备的**设计标准**，此外，还提供了包括但**不限于**技术支持、负载检测及市场推广等服务，服务开发者使用PSDK 开发出功能完善的无人机负载设备，探索行业应用的无限潜能。

## 负载应用

使用PSDK 开发可挂载在GDU 无人机上的负载设备，能够满足不同行业多样化的应用需求：

| 负载设备     | 安防 | 巡检     | 勘测     | 环保     | 更多行业 |          |          |          |          |      |      |
| ------------ | ---- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | ---- | ---- |
| 治安管理     | 消防 | 应急救援 | 管网巡检 | 厂区巡检 | 地质测绘 | 空间规划 | 资源勘探 | 生态监测 | 生物保护 | ...  |      |
| 可变焦相机   | ✓    | ✓        | ✓        | ✓        | ✓        | ✓        | ✓        | ✓        | ✓        | ✓    | ...  |
| 热成像相机   | ✓    | ✓        | ✓        | ✓        | ✓        | -        | -        | ✓        | ✓        | ✓    | ...  |
| 红外相机     | ✓    | ✓        | ✓        | -        | ✓        | -        | -        | ✓        | ✓        | ✓    | ...  |
| 多目相机     | ✓    | ✓        | ✓        | -        | -        | ✓        | ✓        | ✓        | -        | -    | ...  |
| 激光雷达     | -    | -        | ✓        | ✓        | -        | ✓        | ✓        | -        | -        | -    | ...  |
| 气体检测仪   | -    | -        | ✓        | ✓        | ✓        | -        | -        | -        | ✓        | ✓    | ...  |
| 辐射检测仪   | ✓    | ✓        | -        | -        | -        | -        | -        | ✓        | ✓        | -    | ...  |
| 水质检测仪   | -    | -        | -        | -        | -        | -        | -        | -        | ✓        | -    | ...  |
| 喊话器       | ✓    | ✓        | ✓        | ✓        | ✓        | -        | -        | -        | -        | -    | ...  |
| 探照灯       | ✓    | ✓        | ✓        | -        | ✓        | -        | -        | ✓        | -        | -    | ...  |
| 自动巡检系统 | -    | -        | -        | ✓        | ✓        | -        | -        | -        | -        | -    | ...  |
| 更多负载     | ...  | ...      | ...      | ...      | ...      | ...      | ...      | ...      | ...      | ...  | ...  |

## 使用MSDK

MSDK：使用MSDK 开发的移动端App 能够控制负载设备执行指定的动作和任务

## 免责声明

在使用PSDK 开发的应用程序前，请先查阅飞行地所在区域的法律和规定，**因使用PSDK 而引发的安全问题或法律纠纷均与GDU 无关，GDU 不承担一切因使用PSDK 而导致的任何法律风险和责任**。

---

## PSDK 功能概览

---
title: PSDK 功能概览
date: 2026-04-16
---

### 日志管理 

拥有日志管理功能的负载设备支持用户通过串口、终端等方法输出不同模块的日志信息，使用具有日志颜色显示功能的工具能够以不同的颜色显示不同类型的日志。

### 信息管理

- 无人机系统信息获取：负载设备通过该功能可主动获取GDU移动端App 使用的语言等信息。
- 消息订阅：负载设备能够订阅无人机上各个部件实时产生的传感器数据以及无人机系统状态信息，如飞机速度、融合海拔高度及RTK 位置等。

### 相机功能

- 基础功能：设置相机模式、拍照、录像、获取相机状态
- 进阶功能：指点变焦、测光、对焦
- 高级功能：视频流传输、媒体文件回放与下载

### 云台功能

- 控制云台转动速度和转动方式
- 设置平滑系数和最大速度系数
- 云台校准、复位及角度微调

### 电源管理

具有电源管理功能的负载设备，不仅能够向无人机申请较高的功率，还能接收无人机发送的关机通知正常关机，防止负载设备意外丢失数据信息。

### 飞行控制

- 设置或获取无人机飞行控制器的参数，如返航高度，避障状态等
- 执行基本的飞行任务，如起飞、降落及返航等
- 无人机基础控制功能，如速度控制、姿态控制和位置控制

### 自定义控件

- 负载控制：GDU Flight 能够根据控件配置文件中的配置项，显示开发者为负载设备设计的控件，用户通过该控件能够控制使用PSDK 开发的负载设备。
- 浮窗信息显示：GDU Flight 能够显示基于PSDK 开发的负载设备推送的信息。

### HMS功能

获取HMS信息：获取无人机的健康管理信息，有效监控无人机当前状态

### 时间同步

- 获取PPS数据：获取无人机的硬件触发脉冲信号
- 获取UTC时间：获取统一的UTC时间

### 数据传输

- 数据传输：用户能够在负载设备和移动端App间传输控制命令和数据信息。

### 高级视觉

获取相机码流：获取相机的H.264 码流，用户可对视频流解码进行实时处理。

### 本地升级

本地升级是一个用于升级负载设备固件的功能，开发者使用本地升级功能，能够方便用户通过GDU 上位机 选择本地固件文件更新负载设备的固件。

### 运动规划

航点任务：控制GDU 的无人机按照预设的多个航点实现自主飞行

---

## 如何使用PSDK

---
title: 如何使用PSDK
date: 2026-04-16
---

## 1. 选用开发工具

选购开发负载设备时所需使用的无人机、开发平台和硬件平台：

- 选购无人机
- 选购开发负载设备的转接环
- 选择开发平台

## 2. 开发负载设备

#### 开发前准备

使用PSDK 开发负载设备前，建议先学习开发负载设备所需的基础知识、了解PSDK 的功能以及负载设备所需满足的标准，根据实际的开发需求选购合适的硬件平台，选择可靠的开发平台。

- 学习无人机基础知识和控制原理：如俯仰、偏航、滚转和升降等基础知识
- 了解PSDK 支持的功能
- 选购硬件平台

#### 开始开发负载设备

使用PSDK 开发负载设备时，**请正确地连接**所选购的硬件平台、第三方开发板和GDU 的无人机，**正确地配置**负载设备开发环境，通过运行示例代码编译后的程序，了解使用PSDK 开发负载设备的方法。

## 3. 成为GDU 的合作伙伴

GDU 秉承**开放共赢**的合作理念，为开发者提供技术支持、负载检测、渠道推荐以及GDU 生态推广等服务，诚挚携手广大开发者以及合作伙伴共建可持续发展的应用生态。

- 若合作伙伴开发的负载设备能通过GDU 的负载检测，该负载设备将被GDU 推荐给第三方合作伙伴。
- 若合作伙伴的负载设备能够通过第三方专业机构的检测，在与GDU 签署合作协议后，GDU 将与合作伙伴开展更为深入的合作，如将合作伙伴的负载设备推荐给[全球用户]等，更多合作事宜请[ 联系我们]

## 获取技术支持服务

- 在[GDU 技术支持社区](https://gitee.com/gdu-sdk)中寻求帮助

---

# 型号说明

---

## 负载开发标准

---
title: 负载开发标准
date: 2026-04-16
---

为确保无人机在搭载负载设备执行飞行任务时的安全，请在使用PSDK 开发负载设备时，遵循GDU 的负载设备开发标准。

## 硬件接口标准

## 干扰兼容性要求

## 结构设计标准

## 自定义数据传输标准

##### 使用S400系列

| 通道类型                | 传输方向             | 速率限制                                                |
| ----------------------- | -------------------- | ------------------------------------------------------- |
| 命令通道                | 移动端App ➟ 负载设备 | ≤4096B/s                                                |
| 负载设备 ➟ 移动端App    |                      |                                                         |
| 用户负载设备 ➟ 负载设备 |                      |                                                         |
| 负载设备 ➟ 用户负载设备 |                      |                                                         |

## 视频流传输标准

使用PSDK 开发的**相机类**负载设备，**必须**选用如下的两种**H.264 编码标准**。

**Custom-H264** 标准

#### 特性

1. 较为通用且更易获取的标准H264格式码流。
2. 使用该视频流格式发送码流时，会通过飞机透传到App端解码，且实时码率不得超过反馈值。
3. 使用该格式传输视频流，SEI的内容可传输到App端。

#### 要求

基于PSDK 开发的**相机类**负载设备，在使用**Custom-H264 标准**时，需注意如下关键配置信息：

| 关键项                                 | H.264 标准项        | 取值                                                         |
| -------------------------------------- | ------------------- | ------------------------------------------------------------ |
| 视频分辨率                             | -                   | 最大不超过 1920 × 1080                                       |
| 帧率                                   | -                   | 最大不超过 30 fps                                            |
| 码率                                   | -                   | 最大不超过 8 Mbps                                            |
| 推荐视频长宽比                         | -                   | 4: 3                                                         |
| GOP 编码结构必须选用Period I           | -                   | -                                                            |
| 档次                                   | Level Number        | ＜5.1                                                        |
| profile_idc                            | 7.3.2.1.1           | Baseline=66，Main=77，High=100                               |
| YUV420 格式                            | 7.3.2.1.1           | chroma_format_idc=1                                          |
| 8Bit 视频                              | 7.3.2.1.1           | bit_depth_luma_minus8=0 bit_depth_chroma_minus8=0            |
| 不允许编码器自定义 Scaling Matrix      | 7.3.2.1.1 7.3.2.2   | seq_scaling_matrix_present_flag=0 pic_scaling_matrix_present_flag=0 |
| 仅支持帧格式编码，不支持场编码         | 7.3.2.1.1           | frames_mbs_only_flag=1                                       |
| 仅允许 P 帧和 I 帧，且P 帧有单个参考帧 | 7.3.3 7.3.2.2 7.3.3 | slice_type = 0 或 2 num_ref_idx_l0_default_active_minus1=0 num_ref_idx_active_override_flag=0 |
| 不支持多Slice Group                    | 7.3.2.2             | num_slice_groups_minus1=0                                    |

> **注意：** 使用H.264 编码标准时，GOP 编码结构必须选用Period I（帧内编码帧，每隔1 秒将插入1 个IDR 帧，确保图像数据丢失时可被恢复）。

**GDU-H264** 标准

#### 特性

1. 较为严苛的GDU H264格式码流标准。
2. 使用该视频流格式发送码流时，会通过飞机转码，在最大码率限制内，能够根据实时图传链路的情况，动态调节码率，达到最优的图传显示效果。
3. 使用该格式传输视频流，SEI的内容会被飞机丢弃，无法传输到App端。

#### 要求

基于PSDK 开发的**相机类**负载设备，在使用**GDU-H264 标准**时，需注意如下关键配置信息：

| 关键项                                           | H.264 标准项        | 取值                                                         |
| ------------------------------------------------ | ------------------- | ------------------------------------------------------------ |
| 视频分辨率                                       | -                   | 最大不超过 1920 × 1080                                       |
| 帧率                                             | -                   | 最大不超过 30fps                                             |
| 码率                                             | -                   | 最大不超过 8 Mbps                                            |
| 每一帧结尾必须加上AUD信息                        | -                   | 固定6字节：0x00 0x00 0x00 0x01 0x09 0x10                     |
| 推荐视频长宽比                                   | -                   | 4:3或16: 9                                                   |
| 一帧数据字节数                                   | -                   | 最大不超过 256KBytes                                         |
| 档次                                             | Level Number        | ＜5.1                                                        |
| profile_idc                                      | 7.3.2.1.1           | Baseline=66，Main=77，High=100                               |
| YUV420 格式                                      | 7.3.2.1.1           | chroma_format_idc=1                                          |
| 8Bit 视频                                        | 7.3.2.1.1           | bit_depth_luma_minus8=0 bit_depth_chroma_minus8=0            |
| 不允许编码器自定义 Scaling Matrix                | 7.3.2.1.1 7.3.2.2   | seq_scaling_matrix_present_flag=0 pic_scaling_matrix_present_flag=0 |
| 仅支持帧格式编码，不支持场编码                   | 7.3.2.1.1           | frames_mbs_only_flag=1                                       |
| 仅允许 P 帧和 I 帧，且P 帧有单个参考帧           | 7.3.3 7.3.2.2 7.3.3 | slice_type = 0 或 2 num_ref_idx_l0_default_active_minus1=0 num_ref_idx_active_override_flag=0 |
| 不支持多Slice Group                              | 7.3.2.2             | num_slice_groups_minus1=0                                    |
| 不支持长期参考帧                                 | 7.3.3.3             | long_term_reference_flag=0                                   |
| 要求解码顺序和播放顺序一致                       | E.1.1               | max_num_reorder_frames=0                                     |
| 限制解码的参考帧数量为5                          | E.1.1               | max_dec_frame_buffering=5                                    |
| 支持最大的参考帧个数为1帧，支持最大的参考间隔为1 | 7.3.2.1.1           | max_num_ref_frames=1                                         |

> **注意：**
>
> - 使用H.264 编码标准时，Gop structure 可选，为Period I或者GDR。
> - 如果是Period Ⅰ，要求每个I帧为IDR帧，若不符合要求，则不保证图传能够从丢包错误中正确恢复；在发送完SPS/PPS之后，需要立马传输Ⅰ帧的流，然后再传输AUD，在SPS/PPS和slice之间无AUD信息。Period Ⅰ结构中，建议一秒一个IDR。
> - 如果为GDR，在每个GDR刷新帧需要传输sps/pps。

---

## 选择开发平台

---
title: 选择开发平台
date: 2026-04-16
---

请根据操作系统和开发平台对PSDK 功能的支持差异、负载设备程序的资源占用情况以及PSDK 支持的工具链，选择开发负载设备的操作系统和开发平台。

## 选择飞行平台

## 选择操作系统

#### 平台功能差异

| 功能名称     | 平台支持-Linux | 平台支持-RTOS |
| ------------ | -------------- | ------------- |
| 日志管理     | ✓              | ✓             |
| 信息管理     | ✓              | ✓             |
| 基础相机功能 | ✓              | ✓             |
| 云台功能     | ✓              | ✓             |
| 电源管理     | ✓              | ✓             |
| 飞行控制     | ✓              | ✓             |
| 自定义控件   | ✓              | ✓             |
| HMS 功能     | ✓              | ✓             |
| 时间同步     | ✓              | ✓             |
| 低速数据传输 | ✓              | ✓             |
| 视频流传输   | ✓              | ✓             |
| 回放下载     | ✓              | -             |
| 获取相机码流 | ✓              | -             |
| 本地升级     | ✓              | ✓             |
| 精准定位     | ✓              | ✓             |
| 运动规划     | ✓              | ✓             |

#### 资源占用

##### Linux

使用ai盒子 运行Linux 平台PSDK 示例程序，程序运行时的资源占用情况如下所示：

- 栈：约 12288 字节
- 堆：约 40960 字节
- Text 段：755359 字节
- Data 段：3872 字节
- Bss 段：23848 字节
- CPU 占用：7.2 %

##### RTOS

使用STM32F407IGH6-EVAL 运行RTOS 平台PSDK 示例程序，程序运行时的资源占用情况如下所示：

- Text 段：167140 字节
- Data 段：1740 字节
- Bss 段：117852 字节
- CPU 占用：30 %

## 选择开发平台

PSDK 支持使用如下工具编译基于PSDK 开发的负载设备，请根据选用的**开发平台**正确地选择工具链。

> **说明：** 有关跨平台移植的详细说明请参见[跨平台移植open in new window](https://developer.gdu.com/doc/payload-sdk-tutorial/cn/quick-start/porting.html)。

| 工具链名称                           | 目标平台                         | 典型芯片型号                            | 推荐开发平台                            |
| ------------------------------------ | -------------------------------- | --------------------------------------- | --------------------------------------- |
| aarch64-linux-gnu-gcc                | aarch64-linux-gnu                | NVIDIA Jetson TX2、Rockchip RK3399 pro  | Manifold2-G、瑞芯微Toybrick开发板       |
| x86_64-linux-gnu-gcc                 | x86_64-linux-gnu                 | 64位intel处理器，如 Intel Core i7-8550U | Maniflod2-C                             |
| arm-linux-gnueabi-gcc                | arm-linux-gnueabi                | ZYNQ、I.MX6Q                            | -                                       |
| arm-linux-gnueabihf-gcc              | arm-linux-gnueabihf              | 支持硬件浮点运算的处理器，如OK5718-C等  | -                                       |
| armcc-cortex-m4                      | Cortex M4/M4F系列MCU             | STM32F407IGT6、STM32F405RGT6            | STM32F407-Eval、STM32F407探索者开发板等 |
| arm-none-eabi-gcc                    | Cortex M4/M4F系列MCU             | STM32F407IGT6、STM32F405RGT6            | STM32F407-Eval、STM32F407探索者开发板等 |
| arm-linux-androideabi-gcc            | arm-linux-androideabi            | 高通骁龙系列芯片                        | 安卓平台                                |
| aarch64-linux-android-gcc            | aarch64-linux-android            | 高通骁龙系列芯片                        | 安卓平台                                |
| arm-himix100-linux-gcc               | arm-himix100-linux               | hi3516EV系列芯片                        | -                                       |
| arm-himix200-linux-gcc               | arm-himix200-linux               | hi3516C系列芯片                         | -                                       |
| aarch64-himix100-linux-gcc           | aarch64-himix100-linux           | hi3559C                                 | -                                       |
| arm-hisiv300-linux-uclibcgnueabi-gcc | arm-hisiv300-linux-uclibcgnueabi | hi3516A系列芯片                         | -                                       |
| arm-hisiv400-linux-gnueabi-gcc       | arm-hisiv400-linux-gnueabi       | hi3516A系列芯片                         | -                                       |
| arm-hisiv500-linux-uclibcgnueabi-gcc | arm-hisiv500-linux-uclibcgnueabi | hi3519系列芯片                          | -                                       |
| arm-hisiv600-linux-gnueabi-gcc       | arm-hisiv600-linux-gnueabi       | hi3519系列芯片                          | -                                       |
| xtensa-esp32-elf-gcc                 | xtensa-esp32-elf                 | ESP32系列芯片                           | ESP32-DevkitC                           |

> **说明：** 开发者需根据所使用的开发平台，选择指定编译链的静态库。若开发包中没有所需编译链的静态库，请提供开发负载设备时使用的**开发平台的型号**、**编译链的型号**和**编译链的安装包**给我们的技术支持同事，我们将为您准备相应的工具链。

---

## 选购硬件平台

---
title: 选购硬件平台
date: 2026-04-16
---

## 选购无人机

> **说明：**
>
> - 目前我司的S200系列和S400系列 P300系列均支持psdk 开发。

## 购买开发套件

#### FlyPort V2

借助FlyPort V2开发套件中的接口拓展板，开发者能够使用第三方开发板开发负载设备。
FlyPort V2 外观如下：

> **说明：** 当负载设备进入量产阶段后，请购买 FlyPort V2 转接环套装

## 选购第三方开发平台

GDU PSDK 使用STM3240G-EVAL(STM32F407IG) 开发板开发并调试示例程序，建议开发者参考[STM3240G-EVAL(STM32F407IG)open in new window](https://www.st.com/en/evaluation-tools/stm3240g-eval.html) 的参数信息选购所需使用的开发平台。

---

# 快速开始

---

## 开发须知

---
title: 开发须知
date: 2026-04-16
---

使用PSDK 开发负载设备前，请认真阅读本文档中的内容，避免操作不当意外损毁无人机、硬件平台或负载设备。

## 固件升级

在开发负载设备前，需先使用配套遥控器或者uver平台对无人机进行升级，确保无人机是最新版本固件

## 开发与测试

使用PSDK 开发负载设备时，为保护开发者免受意外，请注意如下事项：

- 在使用PSDK 开发负载设备或测试基于PSDK 开发的负载设备时，请取下无人机上的桨叶。
- 无人机电机在转动时，请勿靠近。
- 请勿向无人机电源输出接口输入大功率电流。

## 免责声明

- 在使用基于PSDK 开发的负载设备前，请检查使用负载设备所在地区的法律法规。**因使用PSDK 引发的安全问题或法律纠纷与GDU 无关，GDU不承担任何连带责任**。
- 使用基于PSDK 开发的负载设备执行飞行任务时，请按照用户手册中的说明操作无人机，确保飞机在执行任务时处于良好的飞行状态，降低无人机潜在的安全隐患和出现安全事故的可能性。
- **GDU不会以任何方式和任何理由获取第三方用户的私人数据。** 开发者应开发出**保护用户隐私数据的功能，例如数据加密和混淆等**。因使用GDU PSDK 造成用户隐私数据泄露的事件，GDU 将不负任何法律责任。

---

## 设备连接

---
title: 设备连接
date: 2026-04-16
---

使用FlyPort V2或者SDK 同轴线开发负载设备时，请按本文引导连接硬件平台、负载设备开发板、接口拓展板和GDU 的无人机。

### 基于TypeC口的负载开发（PSDK接口）

#### PSDK接口定义

表 PSDK 接口引脚介绍

| 编号     | A1   | A2   | A3   | A4   | A5       | A6     | A7       | A8      | A9   | A10  | A11  | A12  |
| -------- | ---- | ---- | ---- | ---- | -------- | ------ | -------- | ------- | ---- | ---- | ---- | ---- |
| 接口名称 | GND  | NC   | NC   | VCC  | SYNC_PPS | USB_ID | USB_VBUS | ON_DET  | VCC  | NC   | NC   | GND  |
| 编号     | B12  | B11  | B10  | B9   | B8       | B7     | B6       | B5      | B4   | B3   | B2   | B1   |
| 接口名称 | GND  | NC   | NC   | VCC  | UART_RX  | USB_DM | USB_DP   | UART_TX | VCC  | NC   | NC   | GND  |

##### 接口功能说明

> **说明：**
>
> - 不同机型之间在供电接口、USB_VBUS和USB_ID接口上存在差异，在软硬件设计上需要考虑兼容性。
> - NC引脚为飞机端预留信号引脚。为避免开发或使用中第三方设备损坏飞机端SDK接口，请将NC引脚置空，不要与设备端自定义接口连接。

- 供电接口：
  - 如需为用户负载设备供电，请使用VCC引脚。
    > **说明：**
    > - 该接口的额定电压为12-17.6V（随电池电压降低），电流限制3A，无人机输出的总功率36-52.8W。
    > - 该接口的VCC引脚在同一网络下，开发者可根据实际的使用需求连接VCC引脚数量。但为保证无人机能够持续稳定地输出电流，请同时连接4 个VCC引脚。
  - 飞行器无电压输出能力，只能检测外部电压的有无，USB_VBUS引脚（A7）无电压输出能力。
  - ON_DET 引脚（A8）为S400 行业系列外接PSDK设备检测引脚，接入ON_DET 引脚（A8）的对端必须为GND，请勿串联任何阻值电阻，此时才会有12-17.6V电压输出。

- 数据通信接口：
  - 如需使用USB 2.0或者RNDIS（TCP/IP over USB） 的功能，请使用USB_DP 引脚（B6）和USB_DM 引脚（B7）。
  - 如需通过无人机上的串口，实现用户负载设备和无人机间的通信，请使用UART_RX 引脚（B8）和UART_TX 引脚（B5）。UART_RX 引脚（B8）需连接PSDK设备的TX端，UART_TX 引脚（B5）需连接PSDK设备的RX端。
  > **说明：** S400 机型所支持的串口波特率为 921600。

- 时间同步：如需使用无人机对外输出的同步信号，实现时间同步功能，请使用SYNC_PPS 引脚（A5）。

## 使用 S400 系列

S400 有两种设备连接方式：
1、通过云台挂载接口，使用Flyport V2 转接环与第三方开发板相连。通信波特率为460800
2、通过飞机顶部12v typec接口，使用定制usb线与第三方开发板相连。通信波特率为230400

## 使用 S200 系列
S200 只有一种设备连接方式：
通过飞机顶部typec接口，使用定制usb线与第三方开发板相连。通信波特率为230400

### 基于TypeC口的负载开发（PSDK接口）

表 转接板连接线接口

| 编号     | A1   | A2   | A3   | A4   | A5       | A6     | A7       | A8      | A9   | A10  | A11  | A12  |
| -------- | ---- | ---- | ---- | ---- | -------- | ------ | -------- | ------- | ---- | ---- | ---- | ---- |
| 接口名称 | GND  | NC   | NC   | VCC  | SYNC_PPS | USB_ID | USB_VBUS | ON_DET  | VCC  | NC   | NC   | GND  |
| 编号     | B12  | B11  | B10  | B9   | B8       | B7     | B6       | B5      | B4   | B3   | B2   | B1   |
| 接口名称 | GND  | NC   | NC   | VCC  | UART_RX  | USB_DM | USB_DP   | UART_TX | VCC  | NC   | NC   | GND  |

##### 接口功能说明

> **说明：**
>
> - NC引脚为飞机端预留信号引脚。为避免开发或使用中第三方设备损坏飞机端SDK接口，请将NC引脚置空，不要与设备端自定义接口连接。

- 供电接口：
  - 如需为用户负载设备供电，请使用VCC引脚。
    > **说明：**
    > - 该接口的额定电压为19.2-26.1V（随电池电压降低），电流限制4A，且无人机输出的总功率为76.8-104.4W。
    > - 该接口的VCC引脚在同一网络下，开发者可根据实际的使用需求连接VCC引脚数量。但为保证无人机能够持续稳定地输出电流，请同时连接4 个VCC引脚。

- 数据通信接口：
  - 如需通过无人机上的串口，实现用户负载设备和无人机间的通信，请使用UART_RX 引脚（B8）和UART_TX 引脚（B5）。UART_RX 引脚（B8）需连接PSDK设备的TX端，UART_TX 引脚（B5）需连接PSDK设备的RX端。

- 时间同步：如需使用无人机对外输出的同步信号，实现时间同步功能，请使用SYNC_PPS 引脚（A5）。

#### 使用SDK同轴线

开发者根据同轴线的引脚定义，将SDK 同轴线集成至负载中，并通过该线材将用户定制的负载设备与对应的飞机进行连接。

### 基于云台口的负载开发

#### 使用FlyPort V2

FlyPort V2 提供了排线接口和同轴线接口，使用排线通过Port 1 接口可将FlyPort V2 连接至接口拓展板或第三方开发板。使用同轴线通过Port 2 接口可将FlyPort V2 连接至FlyPort 负载设备开发板。FlyPort V2 的引脚如 图2.FlyPort V2 引脚接口 所示。

> **注意**
>
> - 请勿同时使用Port 1 和Port 2。
> - 使用排线连接接口拓展板和FlyPort V2 上的Port 1 接口时，请务必使红色线缆与圆点标记位对齐插入，避免接反线缆烧毁负载设备。

##### Port 1

- 使用FlyPort V2 的供电功能，请使用引脚：1～６。
- 为实现使用FlyPort V2 开发的负载设备与GDU 的无人机通信，**必须**连接引脚：11和12。
- 使用PSDK 提供的时间同步功能，需要连接PPS 引脚（14）与**具有RTK 功能的无人机**间同步时间。
- 使用高功率功能的负载设备通过引脚15 和16 向GDU 的无人机申请高功率。
- 使用网口高速数据传输功能，需连接引脚17～20。

##### Port 2

- 使用FlyPort V2 的供电功能，请使用引脚：1～17。
- 为实现使用FlyPort V2 开发的负载设备与GDU 的无人机通信，**必须**连接引脚：37和39。
- 使用PSDK 提供的时间同步功能，需要连接PPS 引脚（19）与**具有RTK 功能的无人机**间同步时间。
- 使用高功率功能的负载设备通过HIGH_POWER_APPLY 引脚（22） 向GDU 的无人机申请高功率。
- 使用高速数据传输功能，需连接引脚23、25、29 和31。

##### 安装负载设备

1. 安装主轴臂：使用四颗M2×12 的螺丝锁定负载设备，负载设备上对应螺纹孔的深度不小于5.3 mm。
2. 安装辅轴臂：使用一颗M3 螺丝、辅轴臂套筒和辅轴臂胶塞锁定辅轴臂。确保M3螺丝穿过俯仰轴中心轴线。
3. 负载设备质心位置
   - 质心位置不变的负载，需保证负载设备的质心位于俯仰轴中心轴线上。
   - 质心位置可变的负载设备如变焦相机，需保证当变焦镜头在最大变焦倍数时，相机的质心位于P轴的轴线上。

---

## 配置开发环境

---
title: 配置开发环境
date: 2026-04-16
---

## 获取基础软件

- [Payload SDK 软件开发工具包open in new window]([GitHub - GduDeveloper/Payload-SDK: GDU Payload SDK Official Repository](https://github.com/GduDeveloper/Payload-SDK))
- [Mobile SDK 软件开发工具包open in new window](https://developer.gdu.com/mobile-sdk/downloads/) （可选）

## 配置RTOS 开发环境

- 安装[Keil MDKopen in new window](http://www2.keil.com/mdk5/)和相关开发工具
  - C Compiler: Armcc.exe V5.06 update 6 (build 750)
  - Assembler: Armasm.exe V5.06 update 6 (build 750)
  - Linker/Locator: ArmLink.exe V5.06 update 6 (build 750)
  - Library Manager: ArmAr.exe V5.06 update 6 (build 750)
  - Hex Converter: FromElf.exe V5.06 update 6 (build 750)
- 使用Keil软件
  **激活Keil MDK 软件**后，使用Keil Pack Installer 或[手动下载](http://www.keil.com/dd2/Pack/)最新的STM32F4xx_DFP.2.x.x 驱动包

## 配置Linux 开发环境

使用Linux 开发环境时，请安装如下开发工具：

- C编译器： GCC 5.4.0/5.5.0 版本
- CMake：2.8 及以上版本
- FFmpeg：版本需要高于或等于 4.1.3，低于5.0.0

---

## 运行示例程序

---
title: 运行示例程序
date: 2026-04-16
---

## 获取示例代码

下载[psdk开发包]((https://github.com/GduDeveloper/Payload-SDK)), 在 PSDK 开发包中获取 GDU 提供的示例代码，借助示例代码了解使用 PSDK 开发负载设备的方法，使用示例代码快速开发出功能完善的出负载设备。

## 运行 RTOS 示例代码

> **说明：** 本文以**STM32F4 Discovery 开发板**为例，介绍运行 RTOS 示例代码的步骤和方法。

### 烧录 Bootloader

1. 使用 Keil MDK IDE 打开位于`sample/sample_c/platform/rtos_freertos/stm32f4_discovery/project/mdk_bootloader/`目录下的工程文件`mdk_bootloader.uvprojx`。
2. 使用 Keil MDK IDE 编译工程为示例程序。
3. 将编译后的示例程序**烧录**到负载设备中（如 STM32F4_discovery 开发板）。

> **相关参考**
>
> - 实现 Bootloader：`platform/rtos_freertos/stm32f4_discovery/bootloader`
> - Bootloader 工程目录：`platform/rtos_freertos/stm32f4_discovery/project/mdk_bootloader`

### 补充应用信息

1. 使用 Keil IDE 打开位于`sample/sample_c/platform/rtos_freertos/stm32f4_discovery/project/mdk/`目录下的工程文件`mdk_app.uvprojx`。
2. 在 `sample/sample_c/platform/rtos_freertos/application/gdu_sdk_app_info.h` 文件中补充应用的名称、ID、Key、License、开发者账号和指定波特率。 重申：如果三方挂载接在S400的云台接口位置，USER_BAUD_RATE应设置为460800，如果三方挂载接在S400/S200的顶部typec接口位置，USER_BAUD_RATE应设置为230400;

```text
#define USER_APP_NAME               "your_app_name"
#define USER_APP_ID                 "your_app_id"
#define USER_APP_KEY                "your_app_key"
#define USER_APP_LICENSE            "your_app_license"
#define USER_DEVELOPER_ACCOUNT      "your_developer_account"
#define USER_BAUD_RATE              "460800"
```

### 编译并烧录

- 使用 Keil MDK IDE 编译示例代码为示例程序。
- 编译示例代码后，将编译后的程序**烧录**到负载设备中（如 STM32F4_discovery 开发板）。
- 如需调试示例程序，请将串口调试工具的波特率设置为：`921600`。

## 运行 Linux 示例代码

> **说明：** 本文以 Manifold 2-C 为例，介绍运行 Linux 示例代码的步骤和方法。

### 补充应用信息

- 在 `samples/sample_c/platform/linux/manifold2/application/gdu_sdk_app_info.h` 文件中替换应用的名称、ID、Key、License、开发者账号和指定波特率。重申：如果三方挂载接在S400的云台接口位置，USER_BAUD_RATE应设置为460800，如果三方挂载接在S400/S200的顶部typec接口位置，USER_BAUD_RATE应设置为230400;

```text
#define USER_APP_NAME               "your_app_name"
#define USER_APP_ID                 "your_app_id"
#define USER_APP_KEY                "your_app_key"
#define USER_APP_LICENSE            "your_app_license"
#define USER_DEVELOPER_ACCOUNT      "your_developer_account"
#define USER_BAUD_RATE              "460800"
```

- 在 `samples/sample_c/platform/linux/manifold2/hal/hal_uart.h` 文件的 `LINUX_UART_DEV1`和 `LINUX_UART_DEV2` 宏中填写对应的串口名称。

```text
#define LINUX_UART_DEV1    "/dev/your_com"
#define LINUX_UART_DEV2    "/dev/your_com"
```

- 通过`ifconfig`命令，查看当前与无人机通讯的网口设备名称，并填写到`samples/sample_c/platform/linux/manifold2/hal/hal_network.h` 文件的 `LINUX_NETWORK_DEV` 宏中。

```text
#define LINUX_NETWORK_DEV    "your_network_name"
```

### 编译示例程序

- **编译示例代码**
  进入示例代码工程的根目录下：`Payload-SDK/`，使用如下命令将示例代码编译为示例程序。
  1. `mkdir build`
  2. `cd build`
  3. `cmake ..`
  4. `make`

  根目录位置说明：

```text
Payload-SDK/
├── CMakeLists.txt
├── EULA.txt
├── LICENSE.txt
├── README.md
├── build/
├── doc/
├── psdk_lib/
├── samples/
└── tools/
```

- **执行 C 语言示例程序**
  - 进入示例程序的目录： `cd build/bin/`
  - 使用`sudo ./gdu_sdk_demo_linux`命令运行示例程序
- **执行 C++ 语言示例程序**
  - 进入示例程序的目录： `cd build/bin/`
  - 使用`sudo ./gdu_sdk_demo_linux_cxx`命令运行示例程序

## 绑定故障排查

### 1. FlyPort V2 故障排查

| 错误代码 | 错误说明         | 解决方案                                                     |
| -------- | ---------------- | ------------------------------------------------------------ |
| 1        | 无法获取负载信息 | 1. 确认硬件平台正常连接 2. 确认基于 PSDK 开发的负载设备控制程序已正常运行 |
| 2        | 开发者账号非法   | 请确认负载设备控制程序中的用户信息与所绑定的转接环的用户信息相同 |
| 3        | 无法获取认证信息 | 请将负载设备中的日志信息提交给 GDU 技术支持团队排查相应的问题 |
| 4        | FlyPort 校验失败 |                                                              |
| 5        | 认证信息错误     |                                                              |
| 6        | 存储绑定信息失败 |                                                              |
| 7        | 无法获取 SN 号   |                                                              |

## 编译故障排查

### 使用 Keil MDK 编译示例代码时报错："error：L6050U"

- 故障原因：Keil MDK 未激活。
- 解决方法：使用 Keil MDK 编译示例代码前请先激活 Keil MDK。

---

## 跨平台移植

---
title: 跨平台移植
date: 2026-04-16
---

将基于PSDK 开发的负载设备控制程序移植到不同版本的软硬件平台上时，需要先初始化Hal 和Osal 层，注册关键的配置信息。通过加载静态库、引用指定的资源文件并声明结构体，设置负载设备控制程序跨平台移植所需的配置信息。最后使用指定的接口将Platform 模块注册到负载设备的控制程序中，获取硬件资源和操作系统资源，实现负载设备控制程序的跨平台移植。

## 示例代码

- Linux
  - Hal 层适配：`samples/sample_c/platform/linux/aibox/hal`
  - Osal 层适配：`samples/sample_c/platform/linux/common/osal/`
- FreeRTOS
  - Hal 层适配：`samples/sample_c/platform/rtos_freertos/stm32f4_discovery/hal`
  - Osal 层适配：`samples/sample_c/platform/rtos_freertos/common/osal/`

> **说明：** PSDK Platform 模块的API 接口，在`psdk_lib/include/gdu_platform.h`文件中。

## 概述

为能使基于PSDK 开发的负载设备控制程序移植到不同的软硬件平台，需要通过Hal（Hardware Abstraction Layer，硬件接口层）适配不同的硬件平台，通过Osal（Operating System Abstraction Layer，操作系统抽象层）实现与不同操作系统的兼容。

## 基础概念

### Hal 层

Hal（Hardware Abstraction Layer，硬件接口层）是PSDK 硬件接口抽象层，位于操作系统、负载设备控制程序和硬件接口间。开发者需要按照`GduPlatform_RegHalUartHandler()`, `GduPlatform_RegHalUsbBulkHandler`与`GduPlatform_RegHalNetworkHandler()`接口中的函数原型，实现并将适配Hal 层的函数注册到负载设备控制程序中，使基于PSDK 开发的负载设备控制程序，通过Hal 层即可直接访问负载设备硬件的底层资源，控制负载设备执行相应的动作，使负载设备控制程序能够适配不同的硬件平台，如STM32F407IGH6-EVAL 或AI盒子 等。

#### 网口设备

需要使用网口的设备适配Hal 层函数需要执行如下操作：

- 实现适配Hal 层网口操作函数
  - 本地网卡初始化：`T_GduReturnCode (*NetworkInit)(const char *ipAddr, const char *netMask, T_GduNetworkHandle *networkHandle)`
  - 本地网卡反初始化：`T_GduReturnCode (*NetworkDeInit)(T_GduNetworkHandle networkHandle)`
  - 获取网卡信息：`T_GduReturnCode (*NetworkGetDeviceInfo)(T_GduHalNetworkDeviceInfo *deviceInfo)`
- 使用`GduPlatform_RegHalNetworkHandler()`接口注册网口操作函数

#### 串口设备

使用串口通信的设备适配Hal 层函数需要执行如下操作：

- 实现适配Hal 层UART 操作函数
  - 串口初始化：`T_GduReturnCode (*UartInit)(E_GduHalUartNum uartNum, uint32_t baudRate, T_GduUartHandle *uartHandle)`
  - 串口反初始化：`T_GduReturnCode (*UartDeInit)(T_GduUartHandle uartHandle)`
  - 发送数据：`T_GduReturnCode (*UartWriteData)(T_GduUartHandle uartHandle, const uint8_t *buf, uint32_t len, uint32_t *realLen)`
  - 接收数据：`T_GduReturnCode (*UartReadData)(T_GduUartHandle uartHandle, uint8_t *buf, uint32_t len, uint32_t *realLen)`
  - 获取串口状态： `T_GduReturnCode (*UartGetStatus)(E_GduHalUartNum uartNum, T_GduUartStatus *status)`
- 使用`GduPlatform_RegHalUartHandler()`接口注册串口操作函数

> **说明**
>
> - 负载设备串口的参数
>   - 波特率：460800(FlyPort V2/X-Port　固件版本低于且包含V01.01.0100)或者GDU Assistant 2 软件设置值(FlyPort V2/X-Port　固件版本高于V01.01.0100)
>   - 停止位：1
>   - 数据位：8
>   - 奇偶校验：无
> - 可通过GDU Assistant 2 软件设置FlyPort V2/X-Port 与PSDK 负载设备的通信波特率（PSDK 设备通信参数须与FlyPort V2/X-Port 保持一致），完美适配不同的硬件平台（某些硬件平台不支持默认通信参数或存在功能缺陷）且适应不同的应用场景（例如订阅大量数据时需适当提高通信波特率）。

### Osal 层

Osal（Operating System Abstraction Layer，操作系统抽象层）是PSDK 的操作系统抽象层，位于负载设备控制程序和操作系统间。开发者需要按照`GduPlatform_RegOsalHandler()`接口中的函数原型，实现并将适配不同操作系统的函数注册到负载设备控制程序中，使用PSDK 开发的负载设备控制程序即可直接访问操作系统以及操作系统内核的资源，将负载设备控制程序移植到不同的操作系统上。

#### 线程函数

使用线程机制管理负载设备控制程序执行相应的任务，开发者需要实现创建线程、销毁线程和线程睡眠的函数。

- 创建线程：`T_GduReturnCode (*TaskCreate)(const char *name, void *(*taskFunc)(void *), uint32_t stackSize, void *arg, T_GduTaskHandle *task)`
- 销毁线程：`T_GduReturnCode (*TaskDestroy)(T_GduTaskHandle task)`
- 线程睡眠：`T_GduReturnCode (*TaskSleepMs)(uint32_t timeMs)`

#### 互斥锁

互斥锁是一种用于防止多个线程同时对同一队列、计数器和中断处理程序等公共资源（如共享内存等）执行读写操作的机制，能够有效避免进程死锁或长时间的等待。使用互斥锁机制，需要开发者实现创建互斥锁、销毁互斥锁、互斥锁上锁和互斥锁解锁。

- 创建互斥锁：`T_GduReturnCode (*MutexCreate)(T_GduMutexHandle *mutex)`
- 销毁互斥锁：`T_GduReturnCode (*MutexDestroy)(T_GduMutexHandle mutex)`
- 互斥锁上锁：`T_GduReturnCode (*MutexLock)(T_GduMutexHandle mutex)`
- 互斥锁解锁：`T_GduReturnCode (*MutexUnlock)(T_GduMutexHandle mutex)`

#### 信号量

信号量是一种用于防止多线程同时操作相同代码段的机制。开发者使用该机制时，需要实现创建信号量、销毁信号量、等待信号量、释放信号量和等待超时信号量函数。

- 创建信号量：`T_GduReturnCode (*SemaphoreCreate)(uint32_t initValue, T_GduSemaHandle *semaphore)`

> **说明：** 使用该接口时，请设置`initValue`信号量的初始值。

- 销毁信号量：`T_GduReturnCode (*SemaphoreDestroy)(T_GduSemaHandle semaphore)`
- 等待信号量：`T_GduReturnCode (*SemaphoreWait)(T_GduSemaHandle semaphore)`

> **说明：** 等待信号量接口等待时间的**最大值**为**32767 ms**。

- 等待超时信号量：`T_GduReturnCode (*SemaphoreTimedWait)(T_GduSemaHandle semaphore, uint32_t waitTimeMs)`
- 释放信号量：`T_GduReturnCode (*SemaphorePost)(T_GduSemaHandle semaphore)`

#### 时间接口

- 获取当前系统的时间（ms）：`T_GduReturnCode (*GetTimeMs)(uint32_t *ms)`
- 获取当前系统的时间（us）：`T_GduReturnCode (*GetTimeUs)(uint64_t *us)`

#### 内存管理接口

- 申请内存：`void *(*Malloc)(uint32_t size)`
- 释放内存：`void (*Free)(void *ptr)`

## 实现跨平台移植

### 1. 跨平台接口适配

| 跨平台接口适配 | 适配方案                                                     |                                                              |                                                              |
| -------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Hal 层适配     | 串口                                                         | Linux                                                        | 请根据硬件连接，配置对应的串口设备名称，如`ttyUSB0`, 并实现串口初始化、串口读数据和串口写数据的回调函数。详细实现方法请参见：/samples/sample_c/platform/linux/manifold2/hal/hal_uart.c |
| RTOS           | 请根据MCU 的型号配置对应的串口管脚，并实现串口初始化、串口读数据和串口写数据的回调函数。 详细实现方法请参见：samples/sample_c/platform/rtos_freertos/stm32f4_discovery/hal/hal_uart.c |                                                              |                                                              |
| 网口           | Linux                                                        | 使用网口将第三方开发平台连接至GDU无人机后，需要实现并注册配置网络的回调函数，当系统初始化时，会自动完成负载网络参数的配置，配置完成后，可以使用网口相关的功能。 详细实现方法请参见：/samples/sample_c/platform/linux/manifold2/hal/hal_network.c |                                                              |
| RTOS           | 对于RTOS系统，可以通过注册配置网络参数的回调函数，获取到当前负载应配置的网络参数，根据实际需要告知其他子系统模块，完成网口相关的功能。 详细实现方法请参见：/samples/sample_c/platform/rtos_freertos/stm32f4_discovery/application/main.c |                                                              |                                                              |
| Osal 层适配    | Linux                                                        | 使用标准库 `pthread` 封装 `T_GduOsalHandler`中的线程函数、互斥锁、信号量以及时间接口等接口。详细实现方法请参见：samples/sample_c/platform/linux/common/osal/osal.c |                                                              |
| RTOS           | 使用 CMSIS 封装的`thread`接口，封装 `T_GduOsalHandler`中的线程函数、互斥锁、信号量以及时间接口等接口。 详细实现方法请参见：samples/sample_c/platform/rtos_freertos/common/osal/osal.c |                                                              |                                                              |

### 2. 注册跨平台适配接口

#### 结构体声明

请完整地填充 `T_GduHalUartHandler`、`T_GduHalNetworkHandler` 、`T_GduHalUsbBulkHandler`和 `T_GduOsalHandler` 中的接口内容，确保所注册的接口能够正常使用。

- T_GduHalUartHandler uartHandler

```c
T_GduHalUartHandler uartHandler = {
    .UartInit = HalUart_Init,
    .UartDeInit = HalUart_DeInit,
    .UartWriteData = HalUart_WriteData,
    .UartReadData = HalUart_ReadData,
    .UartGetStatus = HalUart_GetStatus,
};
```

- T_GduHalNetworkHandler networkHandler

```c
 T_GduHalNetworkHandler networkHandler = {
     .NetworkInit = HalNetWork_Init,
     .NetworkDeInit = HalNetWork_DeInit,
     .NetworkGetDeviceInfo = HalNetWork_GetDeviceInfo,
};
```

- T_GduOsalHandler osalHandler

```c
T_GduOsalHandler osalHandler = {
    .TaskCreate = Osal_TaskCreate,
    .TaskDestroy = Osal_TaskDestroy,
    .TaskSleepMs = Osal_TaskSleepMs,
    .MutexCreate= Osal_MutexCreate,
    .MutexDestroy = Osal_MutexDestroy,
    .MutexLock = Osal_MutexLock,
    .MutexUnlock = Osal_MutexUnlock,
    .SemaphoreCreate = Osal_SemaphoreCreate,
    .SemaphoreDestroy = Osal_SemaphoreDestroy,
    .SemaphoreWait = Osal_SemaphoreWait,
    .SemaphoreTimedWait = Osal_SemaphoreTimedWait,
    .SemaphorePost = Osal_SemaphorePost,
    .Malloc = Osal_Malloc,
    .Free = Osal_Free,
    .GetTimeMs = Osal_GetTimeMs,
    .GetTimeUs = Osal_GetTimeUs,
};
```

请依次调用 `GduPlatform_RegOsalHandler()`、`GduPlatform_RegHalUartHandler()`函数注册基础Hal 层和Osal 层，若接口注册不成功，请根据返回码和日志信息排查错误问题。若使用依赖USB或者网卡的功能，还需要根据实际情况调用`GduPlatform_RegHalUsbBulkHandler`函数注册USB Bulk设备的相关方法或者调用`GduPlatform_RegHalNetworkHandler`函数注册网卡的相关方法。

> **说明：** 跨平台移植模块必须在其他PSDK 功能模块前被注册，若Platform 模块注册失败或未注册，开发者将无法使用基于PSDK 开发的负载设备。

```c
    returnCode = GduPlatform_RegOsalHandler(&osalHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register osal handler error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    returnCode = GduPlatform_RegHalUartHandler(&uartHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register hal uart handler error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

#if (CONFIG_HARDWARE_CONNECTION == GDU_USE_UART_AND_USB_BULK_DEVICE)
    returnCode = GduPlatform_RegHalUsbBulkHandler(&usbBulkHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register hal usb bulk handler error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
#elif (CONFIG_HARDWARE_CONNECTION == GDU_USE_UART_AND_NETWORK_DEVICE)
    returnCode = GduPlatform_RegHalNetworkHandler(&networkHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register hal network handler error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    //Attention: if you want to use camera stream view function, please uncomment it.
    returnCode = GduPlatform_RegSocketHandler(&socketHandler);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register osal socket handler error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
#elif (CONFIG_HARDWARE_CONNECTION == GDU_USE_ONLY_UART)
    /*!< Attention: Only use uart hardware connection.
     */
#endif
```

---

# 功能集

---

## 日志管理

---
title: 日志管理
date: 2026-04-16
---

## 概述

PSDK 的日志管理功能支持通过如串口、终端或USB 等日志输出方法，输出Debug、Info、Warn和Error 四种类型的日志；使用能够显示日志颜色的终端工具，如Putty 等能够以不同的颜色显示不同类型的日志。
基于PSDK 开发的负载设备输出的日志结构如下：日志颜色起始符 + 系统时间 + 模块名称 + 日志等级标识 + 日志内容 + 日志颜色结束符

## 基础概念

### 日志标识符

使用**不支持**显示日志颜色的调试工具查看日志时，会显示日志的颜色标识符。

- 日志颜色：不同类型的日志，其标识颜色也不同。XShell、SecureCRT及Putty 等工具能够根据日志的等级，以不同的颜色显示不同类型的日志。
- 日志颜色起始标识符：
  - 黑色：`\033[30m`、红色：`\033[31m`、绿色：`\033[32m`
  - 黄色：`\033[33m`、蓝色：`\033[34m`、紫色：`\033[35m`
  - 青色：`\033[36m`、白色：`\033[37m`
- 日志颜色结束标识符：`\033[0m`

### 日志信息

- 系统时间：负载设备上电时，负载设备的时间为负载设备系统的时间，当负载设备与无人机完成时间同步后，负载设备的时间将与无人机的时间同步(ms)。

- 模块名称：PSDK 模块的名称（该名称无法被修改），用户打印接口的模块名称为"user"。

- 日志内容：单条日志最多不超过500 个字节（bytes）。

- 日志等级：日志的等级从高到低为Debug、Info、Warn和Error，日志管理功能模块可打印不高于指定等级的所有日志。

  表1. 日志等级说明

  | 日志等级  | 日志内容 | 输出接口       | 日志颜色 |
  | --------- | -------- | -------------- | -------- |
  | Debug - 4 | 调试信息 | USER_LOG_DEBUG | White    |
  | Info - 3  | 关键信息 | USER_LOG_INFO  | Green    |
  | Warn - 2  | 警告信息 | USER_LOG_WARN  | Yellow   |
  | Error - 1 | 系统错误 | USER_LOG_ERROR | Red      |

## 使用日志管理功能

使用PSDK 日志管理功能，需要先初始化日志打印接口，设置日志等级，通过注册日志打印函数，实现日志管理功能。

> **说明：** 本教程以"使用STM32 在RTOS 系统上通过串口打印日志信息"为例，介绍使用日志管理功能的方法和步骤。

### 1. 初始化串口并注册日志输出方法

在RTOS 系统上使用PSDK 开发负载设备的日志管理功能时，建议使用串口打印日志信息。

```c
static T_GduReturnCode GduUser_PrintConsole(const uint8_t *data, uint16_t dataLen)
{
    UART_Write(GDU_CONSOLE_UART_NUM, (uint8_t *) data, dataLen);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

UART_Init(GDU_CONSOLE_UART_NUM, GDU_CONSOLE_UART_BAUD);
```

### 2. 注册日志打印接口

使用PSDK 的日志管理功能，需要初始化日志打印方法`printConsole`，设置所需打印的日志等级、是否启动颜色显示和该日志对应的打印方法，并通过`GduLogger_AddConsole()`注册到负载设备控制程序中。

> **注意：** 使用PSDK 开发的负载设备最多支持同时使用**8种**日志打印方法。

```c
T_GduLoggerConsole printConsole = {
    .func = GduUser_PrintConsole,
    .consoleLevel = GDU_LOGGER_CONSOLE_LOG_LEVEL_INFO,
    .isSupportColor = true,
};

returnCode = GduLogger_AddConsole(&printConsole);
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    printf("add printf console error");
    goto out;
}
```

### 3. 输出日志信息

调用日志打印函数输出日志信息，日志输出结果如 图1.日志信息 所示。

```text
USER_LOG_ERROR("psdk log console test.");
USER_LOG_WARN("psdk log console test.");
USER_LOG_INFO("psdk log console test.");
USER_LOG_DEBUG("psdk log console test.");
```

> **说明：** 日志的等级为Info，因此负载设备中类型为Info、Warn 及Error 的日志将被输出在终端上，类型为Debug 的日志将不会被打印出来。

## 日志查看

- Linux：
  - 在Clion IDE 上使用 [Grep Consoleopen in new window](https://plugins.jetbrains.com/plugin/7125-grep-console/)可过滤负载设备的日志文档
  - 使用grep命令：在终端中执行`./demo_linux_ubuntu | grep 'Info'`过滤所需的日志信息，更多使用方法请使用`man grep`查询
- RTOS：使用串口查看工具如SecureCRT、XShell或Putty 等工具查看日志信息

---

## 信息管理

---
title: 信息管理
date: 2026-04-16
---

## 概述

PSDK 的信息管理功能包含信息获取和消息订阅功能，基于PSDK 开发的负载设备具有信息获取功能，能够主动获取到无人机的型号、负载设备挂载的位置以及用户使用的移动端App 等信息，加载不同的配置文件，方便用户使用负载设备；具有消息订阅功能的负载设备，能够记录用户订阅的数据信息，方便用户实现更广泛的应用。

## 基础概念

#### 信息获取

信息获取是指负载设备能够**主动获取并记录**无人机上如无人机型号、硬件平台类型和负载设备挂载位置等数据信息。

> **说明：** 将使用PSDK 开发的负载设备安装到无人机上，在开机初始化5s 后才能够获取到无人机正确的数据信息。

使用PSDK 开发的负载设备在初始化后，即可获取到如下信息：

- 基本信息：无人机型号、硬件平台类型和负载挂载位置
- 移动端App 信息：App 的系统语言和App 的屏幕类型

#### 消息订阅

无人机上的各个部件根据无人机实际的飞行状况，会实时产生大量的数据信息并被无人机推送给其他模块，用户使用具有消息订阅功能的负载设备，能够指定所需订阅的数据信息。

#### 订阅项

使用PSDK 消息订阅功能可订阅的数据信息如 表1.无人机订阅项 所示。

表1.无人机订阅项

| 数据类型       | 订阅项（Topic） | 最大订阅频率（Hz） |
| -------------- | --------------- | ------------------ |
| 基础信息           | 速度         |  10               |
|               | 融合海拔高度     |  10               |
|               | 相对高度         |  10               |
|               | 融合位置         |  10               |
|               | 飞行状态         |  10               |
|               | 电池信息         |  10               |
| GPS 信息       | GPS 信息         | 10               |
|               | GPS 时间         |  5               |
|               | GPS 位置         |  5                |
|               | GPS 速度         |  5                |
|               | GPS 信息         |  5                |
|               | GPS 信号等级     |  10               |
| RTK 信息       | RTK 信息         |  5                |
|                | RTK 速度         |  5                |
|                | RTK 航向角       |  5                |
|                | RTK 位置属性     |  5                 |
|                | RTK 航向角属性   |  5                 |

#### 订阅规则

- 消息订阅功能最多支持订阅**3类频率**，如最多支持订阅1Hz、5Hz、10Hz，3类频率，每个订阅项只能被订阅一次。
- 指定订阅频率时，任何参数的订阅频率不能小于或等于0 ，相同订阅频率的主题的数据长度总和须小于或等于242。

## 使用消息订阅功能

PSDK 支持通过注册回调和接口调用两种方式订阅无人机对外推送的数据信息：

- 通过调用`GduFcSubscription_GetLatestValueOfTopic()`获取无人机最新产生的订阅项的数据信息及其对应的时间。
- 通过调用`GduFcSubscription_SubscribeTopic`接口指定订阅频率和订阅项，通过构造并注册回调函数，获取无人机最新产生的订阅项的数据信息及其对应的时间。

#### 消息订阅功能模块初始化

使用PSDK 开发的负载设备如需订阅无人机上的状态信息，需要先调用`GduFcSubscription_Init()`初始化消息订阅模块。

```c
gduStat = GduFcSubscription_Init();
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("init data subscription module error.");
    return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
}
```

#### 通过构造回调函数获取无人机上的信息

1. 构造回调函数
   通过构造回调函数接收无人机推送的信息。

> **注意：** 为避免出现内存踩踏事件，须将数据地址的类型强制转换为订阅项数据结构中的指针类型。

```c
static T_GduReturnCode GduTest_FcSubscriptionReceiveQuaternionCallback(const uint8_t *data, uint16_t dataSize, const T_GduDataTimestamp *timestamp)
{
    T_GduFcSubscriptionQuaternion *quaternion = (T_GduFcSubscriptionQuaternion *) data;
    gdu_f64_t pitch, yaw, roll;

    USER_UTIL_UNUSED(dataSize);

    pitch = (gdu_f64_t) asinf(-2 * quaternion->q1 * quaternion->q3 + 2 * quaternion->q0 * quaternion->q2) * 57.3;
    roll = (gdu_f64_t) atan2f(2 * quaternion->q1 * quaternion->q2 + 2 * quaternion->q0 * quaternion->q3, -2 * quaternion->q2 * quaternion->q2 - 2 * quaternion->q3 * quaternion->q3 + 1) * 57.3;
    yaw = (gdu_f64_t) atan2f(2 * quaternion->q2 * quaternion->q3 + 2 * quaternion->q0 * quaternion->q1, -2 * quaternion->q1 * quaternion->q1 - 2 * quaternion->q2 * quaternion->q2 + 1) * 57.3;

    if (s_userFcSubscriptionDataShow == true) {
        USER_LOG_INFO("receive quaternion data.");

        USER_LOG_INFO("timestamp: millisecond %u microsecond %u.", timestamp->millisecond,
                      timestamp->microsecond);
        USER_LOG_INFO("quaternion: %f %f %f %f.\r\n", quaternion->q0, quaternion->q1, quaternion->q2, quaternion->q3);
        USER_LOG_INFO("euler angles: pitch = %.2f roll = %.2f yaw = %.2f.", pitch, yaw, roll);
        GduTest_WidgetLogAppend("pitch = %.2f roll = %.2f yaw = %.2f.", pitch, yaw, roll);
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
```

1. 注册回调函数
   注册回调函数接收无人机产生并对外推送的数据信息，下述代码以1Hz 的频率订阅无人机"无人机飞行速度"和"无人机GPS坐标"，如 图2. 订阅结果（1） 所示。

> **说明：** 使用订阅功能订阅无人机上的数据信息时，订阅频率只能为"最大订阅频率"的约数。

```c
gduStat = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_VELOCITY, GDU_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                           NULL);
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("Subscribe topic velocity error.");
    return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
}

gduStat = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_GPS_POSITION, GDU_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                           NULL);
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("Subscribe topic gps position error.");
    return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
}
```

#### 在线程函数中获取无人机上的信息

通过数据订阅线程函数获取无人机推送的信息并打印在终端上。下述代码以1Hz 的频率，订阅无人机最新产生的无人机飞行速度和无人机GPS坐标，以及该数据对应的时间，如 图3. 订阅结果（2） 所示。

```c
gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_VELOCITY,
                                                  (uint8_t *) &velocity,
                                                  sizeof(T_GduFcSubscriptionVelocity),
                                                  &timestamp);
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("get value of topic velocity error.");
} else {
    USER_LOG_INFO("velocity: x = %f y = %f z = %f healthFlag = %d.", velocity.data.x, velocity.data.y,
                  velocity.data.z, velocity.health);
}

gduStat = GduFcSubscription_GetLatestValueOfTopic(GDU_FC_SUBSCRIPTION_TOPIC_GPS_POSITION,
                                                  (uint8_t *) &gpsPosition,
                                                  sizeof(T_GduFcSubscriptionGpsPosition),
                                                  &timestamp);
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("get value of topic gps position error.");
} else {
    USER_LOG_INFO("gps position: x = %d y = %d z = %d.", gpsPosition.x, gpsPosition.y, gpsPosition.z);
}
```

---

## 基础相机功能

---
title: 基础相机功能
date: 2026-04-16
---

## 概述

为满足开发者对相机类负载设备的控制需求，PSDK 提供了**控制**相机执行拍照、录像、变焦及对焦等功能的接口，开发者需**先实现**相机拍照、录像以及测光等功能，再通过注册PSDK 相机类的接口，开发出功能完善的相机类负载设备；通过使用GDU Flight 以及基于MSDK 开发的移动端App，用户能够控制使用PSDK 开发的相机类负载设备执行指定的动作，获取负载设备中的信息和资源。

- 基础功能：设置相机模式、拍照、录像、获取相机状态、SD 卡管理
- 高级功能：变焦、测光、对焦、视频流传输、回放下载、媒体库管理

## 基础概念介绍

### 相机模式

使用相机类功能前，需要先设置相机类负载设备的模式，不同的模式指定了相机类负载设备在执行某个任务时的工作逻辑。

- 拍照：在该模式下，用户能够触发相机类负载设备拍摄照片。
- 录像：在该模式下，用户能够触发相机类负载设备录制影像。
- 视频回放：在该模式下，用户可以在移动端App 上播放或下载负载设备上的媒体文件。

> **注意：** 相机只能在一种模式中执行相应的操作，如在录像模式下**仅能录像无法拍照**。

### 拍照模式

使用PSDK 开发的相机类负载设备支持以下拍照模式：

- 单拍：下发拍照命令后，相机拍摄单张照片。
- 连拍：下发拍照命令后，相机将连续拍摄指定数量的照片；当前支持2/3/5/7/10/14张连拍。
- 定时拍照：下发拍照命令时，相机按照指定的时间间隔，拍摄指定数量的照片。
  - 当前支持2/3/5/7/10秒时间间隔；
  - 当前最大支持指定拍摄254 张照片，当拍照张数为255的时候，相机将不间断地拍摄照片。

### 对焦模式

- 自动对焦：Auto Focus，简称AF。在自动对焦模式下，相机类负载设备根据光电传感器获取的影像状态（漫发射），计算出对焦结果实现对焦功能，获取清晰的影像。
- 手动对焦：Manual Focus，简称MF。在手动对焦模式下，用户通过调节对焦环能够获得清晰的影像。

### 对焦点

> **说明：** 相机类负载设备的对焦点默认为传感器的中心位置。

在控制相机类负载设备对焦，需要先设置对焦点，该对焦点的值为当前对焦点在相机画面中的横纵比系数，如 图1.对焦点 所示。

- 在自动对焦模式下，开发者需要制定相机对焦的策略，设置对焦点（该对焦点也称"目标点"）。
- 在手动对焦模式下，用户可根据实际需要调整对焦点，获得所需的影像。

### 对焦环

使用PSDK 开发的具有变焦环（光学变焦）的相机类负载设备通过调用`SetFocusRingValue`接口，设置对焦环的值：

- 对焦环的值默认为0，表示无穷大和最接近的可能焦距。
- 对焦环的值不为0时，用户可根据相机的实际参数设置对焦环的值。

### 对焦助手

在AF 和 MF 模式下，对焦助手通过数字变焦的方式，能够放大用户指定的对焦区域，调用`SetFocusAssistantSettings`接口可设置对焦助手的状态，使用对焦助手，用户能够查看当前相机类负载设备的对焦质量。

### 变焦模式

- 光学变焦，通过改变光学镜头的结构实现变焦，光学变焦倍数越大，能拍摄的景物就越远，反之则近；
- 数码变焦，处理器使用特定的算法，通过改变传感器上每个像素的面积，实现数码变焦；
- 连续变焦，相机类负载设备控制镜头以指定的速度沿指定的方向运动，相机类负载设备先控制镜头执行光学变焦，当光学变焦达到上限后，相机类负载设备再执行数码变焦，以此实现连续变焦的功能。当前变焦倍数=当前光学变焦倍数 × 当前数码变焦倍数；
- 指点变焦，用户指定某一目标点后，基于PSDK 开发的相机类负载设备能够控制云台转动，使指定的目标处于画面中心，控制相机类负载设备按照预设的变焦倍数放大图像。

### 变焦方向

- ZOOM_IN ：变焦倍数减小，图像由远到近
- ZOOM_OUT ：变焦倍数增大，图像由近到远

### 变焦速度

- SLOWEST：以最慢的速度变焦
- SLOW：以较慢的速度变焦
- MODERATELY_SLOW：以比正常速度稍慢的速度变焦
- NORMAL：镜头以正常的速度变焦
- MODERATELY_FAST：以比正常速度稍快的速度变焦
- FAST：以较快的速度变焦
- FASTEST：以最快的速度变焦

### 测光模式

- 平均测光，通过对画面整体亮度的分析，计算出画面的平均亮度的值，适合光照均匀的拍照场景；
- 中央重点测光，仅对图像传感器中间区域测光，适合拍摄框架式构图的照片；
- 点测光，对在以"指定的点"为中心的范围内测光，如 图1.点测光区域 所示，通过该方式能获得准确的测光结果，确保指定的对象能够曝光正确，适合光照复杂的拍摄场景。

图像传感器被分为12列8行定义的96个点区域。行索引范围是[0,7]，其中值在图像上从上到下递增；列索引范围是[0，11]，其中值从左到右增加。

### 媒体文件管理

使用PSDK 开发的相机类负载设备能够根据用户的指令，执行文件删除或下载等操作。

### 媒体文件预览功能

使用PSDK 开发的相机类负载设备支持用户使用GDU Flight 或基于MSDK 开发的移动端App 预览负载设备中的媒体文件。

- 静态预览：预览单个文件或文件列表
  - 缩略图，预览文件列表
    - 图像：负载设备按照文件的原始比例生成缩略图，请将预览图的宽度设置为100 像素
    - 视频：截取视频某一帧的画面
  - 截屏图，预览单个文件
    - 图像：按原始比例，建议缩放图像成宽为600 像素的预览图
    - 视频：截取视频某一帧的画面
  - 原始文件，如需获得相机类负载设备中原始的媒体文件，请使用下载功能获取指定的媒体文件。
- 动态预览（视频预览)：播放、暂停、停止、跳转（快进、快退和进度拖动）
  > **说明：** 支持动态预览的文件格式：MP4、JPG、DNG 和MOV

## 使用相机类基础功能

请开发者根据选用的**开发平台**以及行业应用实际的使用需求，按照PSDK 中的结构体`T_GduCameraCommonHandler `构造实现相机类负载设备设置相机模式、拍照和录像等功能的函数，将相机功能的函数注册到PSDK 中指定的接口后，用户通过使用GDU Flight 或基于MSDK 开发的移动端App 能够控制基于PSDK 开发的相机类负载设备执行相应的动作。

```c
    // 获取负载设备系统当前的状态
    s_commonHandler.GetSystemState = GetSystemState;
    // 实现设置相机类负载设备模式的功能
    s_commonHandler.SetMode = SetMode;
    s_commonHandler.GetMode = GduTest_CameraGetMode;
    // 实现开始或停止录像的功能
    s_commonHandler.StartRecordVideo = StartRecordVideo;
    s_commonHandler.StopRecordVideo = StopRecordVideo;
    // 实现开始或停止拍照的功能
    s_commonHandler.StartShootPhoto = StartShootPhoto;
    s_commonHandler.StopShootPhoto = StopShootPhoto;
    // 实现设置相机类负载设备的拍照功能
    s_commonHandler.SetShootPhotoMode = SetShootPhotoMode;
    s_commonHandler.GetShootPhotoMode = GetShootPhotoMode;
    s_commonHandler.SetPhotoBurstCount = SetPhotoBurstCount;
    s_commonHandler.GetPhotoBurstCount = GetPhotoBurstCount;
    s_commonHandler.SetPhotoTimeIntervalSettings = SetPhotoTimeIntervalSettings;
    s_commonHandler.GetPhotoTimeIntervalSettings = GetPhotoTimeIntervalSettings;
    // 实现SD 卡管理功能
    s_commonHandler.GetSDCardState = GetSDCardState;
    s_commonHandler.FormatSDCard = FormatSDCard;
```

---

## 云台功能

---
title: 云台功能
date: 2026-04-16
---

云台功能用于控制负载设备的云台执行转动、平滑控制以及微调等动作。

## 云台控制模式

### 角度控制

通过云台角度控制接口，能够控制云台按照指定的角度执行转动。

### 角速度控制

通过云台角速度控制接口，能够控制云台按照指定的角速度执行转动。

## 云台参数

### 平滑系数

平滑系数用于控制云台转动时的平滑程度。

### 最大速度系数

最大速度系数用于控制云台转动的最大速度。

---

## 电源管理

---
title: 电源管理
date: 2026-04-16
---

## 概述

将基于PSDK 开发的负载设备安装在无人机的云台上后，负载设备通过功率申请能获得较高的功率，因此，使用高功率的负载设备必须同时支持使用低功率；为防止负载设备未执行关机准备而丢失数据或意外损毁，PSDK 还提供了下发关机通知的功能。

## 使用电源管理功能

#### 1. 引脚初始化

需要使用高功率的负载设备在申请高功率前，需要先初始化负载设备上用于申请高功率的引脚。

```c
static T_PsdkReturnCode PsdkTest_HighPowerApplyPinInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(HIGH_POWER_APPLY_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = HIGH_POWER_APPLY_PIN;
    HAL_GPIO_Init(HIGH_POWER_APPLY_Port, &GPIO_InitStructure);

    return PSDK_RETURN_CODE_OK;
}
```

> **说明：** 请根据所选用的开发平台支持的库，实现初始化申请高功率引脚的功能。

#### 2. 开发并注册改变负载设备引脚电平状态的函数

1.开发改变负载设备引脚电平状态的函数
开发者开发出负载设备改变引脚电平状态的功能后，需要将改变负载设备引脚电平状态的函数注册到指定的接口中。

```c
T_GduReturnCode GduTest_WriteHighPowerApplyPin(E_GduPowerManagementPinState pinState)
{
    GPIO_PinState state;

    switch (pinState) {
        case GDU_POWER_MANAGEMENT_PIN_STATE_RESET:
            state = GPIO_PIN_RESET;
            break;
        case GDU_POWER_MANAGEMENT_PIN_STATE_SET:
            state = GPIO_PIN_SET;
            break;
        default:
            USER_LOG_ERROR("pin state unknown: %d.", pinState);
            return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    HAL_GPIO_WritePin(HIGH_POWER_APPLY_PORT, HIGH_POWER_APPLY_PIN, state);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
```

2.注册改变引脚电平状态的函数
注册改变负载设备引脚电平状态的函数后，用户能够控制负载设备改变引脚的电平状态。

```c
returnCode = GduPowerManagement_RegWriteHighPowerApplyPinCallback(s_applyHighPowerHandler.pinWrite);
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("register WriteHighPowerApplyPinCallback error.");
    return returnCode;
}
```

#### 3.申请高功率

负载设备调用`GduPowerManagement_ApplyHighPowerSync()`接口后，负载设备能够向无人机申请高功率，高功率申请完成后，可使用电压表等仪器测量AirPort指定接口的电压。

```c
returnCode = GduPowerManagement_ApplyHighPowerSync();
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("apply high power error");
    return returnCode;
}
```

## 使用关机通知功能

无人机下发关机通知的流程如下所示：

- 当无人机获取到用户发送的关机通知时，无人机将向使用PSDK 开发的负载设备发送关机通知；
- 当负载设备接收到无人机发送的关机通知后，负载设备将完成关机前需执行的操作，并更改关机准备状态；
- 当无人机下发关机通知之后3s，无人机将会关机。

#### 1.构造关机状态回调函数

开发者开发出负载设备执行关机准备的功能后，需要将负载设备执行关机准备功能的函数注册到指定的接口中。

```c
static T_GduReturnCode GduTest_PowerOffNotificationCallback(bool *powerOffPreparationFlag)
{
    USER_LOG_INFO("aircraft will power off soon.");

    *powerOffPreparationFlag = true;

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
```

#### 2.注册回调函数

注册负载设备关机准备功能的函数后，负载设备在接收到关机通知后，将执行关机准备动作，并更改负载设备的关机准备状态。

```c
// register power off notification callback function
returnCode = GduPowerManagement_RegPowerOffNotificationCallback(GduTest_PowerOffNotificationCallback);
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("register power off notification callback function error");
    return returnCode;
}
```

---

## 飞行控制

---
title: 飞行控制
date: 2026-04-16
---

## 概述

使用PSDK 的无人机飞行控制功能，能够设置并获取无人机各项基础参数，控制无人机执行基础飞行动作

## 基础概念

#### 无人机基础信息

PSDK 开放了设置无人机基础参数的接口，开发者通过设置并获取无人机的基础参数，能够实现对无人机的精准控制：

- 开启或关闭避障功能（水平避障和顶部避障）
- 设置无人机的返航点和返航高度

具有信息管理功能的应用程序，能够方便用户获取无人机基础参数的信息：

- 获取避障功能的状态（水平避障和顶部避障）
- 获取无人机返航点和返航高度

#### 无人机基础飞行动作

无人机的基础飞行动作主要包含无人机锁定、无人机起飞降落和无人机返航三类，开发者使用PSDK 提供的接口，即可根据实际的控制需求，控制无人机执行指定的飞行动作。

表1. 基础飞行动作

| 功能类型     | 基本功能                                                     | 功能说明                                               |
| ------------ | ------------------------------------------------------------ | ------------------------------------------------------ |
| 起飞和降落   | 自动起飞                                                     | 使用该功能时，无人机会自动起飞2 米（该高度不可调整） |
| 自动降落     | 使用该功能时，无人机会自动降落                               |                                                        |
| 取消自动降落 | 使用该功能时，无人机在下降多过程中会停止降落，并悬停在空中   |                                                        |
| 强制降落     | 同自动降落             |                                                        |
| 无人机返航   | 返航                                                         | 使用该功能时，无人机会自动返航                         |
| 取消返航     | 使用该功能时，无人机会悬停在空中                             |                                                        |

说明： 无人机控制无需获取控制权，但是在使用飞行控制接口前，需要先调用GduFlightController_joyStickCtrlSwitch接口使能无人机控制，不然可能会出现无法控制的情况。退出无人机控制状态时，需要调用GduFlightController_joyStickCtrlSwitch接口失能无人机控制。

---

## 自定义控件

---
title: 自定义控件
date: 2026-04-16
---

## 概述

"自定义控件"是一个将"负载设备的功能"封装为按钮、开关以及范围条等控件的功能。使用GDU Flight 或基于MSDK 开发的移动端App 能够识别负载设备中控件的配置信息并生成UI 控件，方便用户快速设置负载设备的参数并控制负载设备执行指定的动作。同时，GDU Flight 或基于MSDK 开发的移动端App 还能以浮窗的形式显示负载设备的状态信息。此外，用户还能根据使用需求，将负载设备的功能映射到遥控器上的预留按键上，通过使用遥控器上的预留按键，以更便捷的方式控制负载设备。

## 基础概念

### 控件

#### 主界面的控件

- 动作栏控件：动作栏支持按钮、开关、范围条和选择列表四个控件类型，最多支持设置8个自定义控件。
- 浮窗：实时显示负载设备的状态信息。

#### 配置界面的控件

用户在配置界面能够操作配置界面中的控件，如按钮、开关、范围条、选择列表、文本输入框、整型值输入框。

> **说明**
>
> - 配置界面仅支持显示一个"文本输入框"控件，该文本框最多支持输入128个字节的字符。
> - 使用PSDK 开发的负载设备通过`GduLowSpeedDataChannel_RegRecvDataCallback(ReceiveDataFromMobile)`函数能够获取用户在GDU Flight **文本输入框控件**中向负载设备发送的信息（编码格式为UTF-8）。
> - 受日志长度的影响，用户所输入的字符可能不会被全部显示出来。

#### 控件配置文件

> **说明**
>
> - 控件配置文件的路径：`sample/sample_c/module_sample/widget/widget_file`
> - GDU Flight系统语言为中文时，控件配置文件为`cn_big_screen`
> - GDU Flight系统语言为英文时，控件配置文件为`en_big_screen`
> - 不同语言下的配置信息如控件编号、数量与类型等需要保持一致。

> **注意：** 控件配置文件包含**静态配置文件**和**控件UI 图标**，建议先在**静态配置文件**中配置控件属性，再设计控件图标。

#### 配置控件属性

`widget_config.json`是一个用于配置控件静态属性的文件，修改`widget_config.json`文件时，请务必严格遵守[JSONopen in new window](https://www.json.org/)的语法规则，否则配置文件将无法使用。

> **提示：**
>
> - JSON文件中的配置项被包裹在一个{}中，通过key-value的方式表达数据；
> - JSON的Key 必须包裹在一个双引号中，请勿丢失Key 值的双引号；
> - JSON的值只支持数字（含浮点数和整数）、字符串、Bool值（如true 和 false)、数组（需要包裹在[]中)和对象（需要包裹在{}中 ）
>
> **注意：** 下述代码中 "//" 后的内容为代码注释，在实际的JSON配置文件请勿添加该内容。

```json
{
    "version":  { // 自定义控件配置文件版本,用户不可更改此版本号
        "major" :  1,
        "minor" :  0
    },
    "main_interface": { // GDU Flight 主界面控件设置
        "floating_window": {// 浮窗配置
            "is_enable": true // 浮窗是否显示，true ：显示，false ：隐藏
        },
        "widget_list": [ // 主界面动作栏控件列表
            {
                "widget_index": 0, // 控件编号
                "widget_type": "button", // 控件类型，主界面Action控件支持 "button" : 按钮，"switch" : 开关，"range" : 范围条，"list" : 选择列表
                "widget_name": "Button_1", // 控件名称
                "icon_file_set":  { // 控件图标文件集
                    "icon_file_name_selected" :  "icon_button1.png", // 选中状态下控件图标文件名称
                    "icon_file_name_unselected" :  "icon_button1.png" // 未选中状态下控件图标文件名称
                }
            }
        ]
    },
    "config_interface": {
        "text_input_box": { // 文本输入框
            "widget_name":"TextInputBox", //文本输入框名称
            "placeholder_text":"Please input message", //文本输入框占位符文本
            "is_enable":false // 文本输入框是否显示，false : 不显示，true ： 显示
        },
        "widget_list": [
            {
                "widget_index": 5,
                "widget_type": "button",
                "widget_name": "Button 5"
            }
        ]
    }
}
```

### 使用自定义控件功能

开发负载设备的自定义控件功能，需要先完成自定义控件的初始化，获取控件配置文件所在的目录，配置控件在不同系统语言下显示的配置文件，通过设置控件处理函数列表，应用自定义控件功能，最终实现自定义控件功能。

#### 1. 控件初始化

使用"自定义控件"功能前，需要使用如下代码初始化负载设备的控件。

```text
gduStat = GduWidget_Init();
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("Gdu test widget init error, stat = 0x%08llX", gduStat);
    return gduStat;
}
```

#### 2. 设置控件配置信息

在Linux 和RTOS 系统上开发负载设备时，需要设置控件的配置信息，如控件默认的配置文件和不同系统语言对应的控件配置文件。确保GDU Flight 能够获取控件的配置信息并正确地显示在GDU Flight上。

##### 设置负载设备的控件参数（Linux）

```text
    //Step 2 : Set UI Config (Linux environment)
    char curFileDirPath[WIDGET_DIR_PATH_LEN_MAX];
    char tempPath[WIDGET_DIR_PATH_LEN_MAX];
    gduStat = GduUserUtil_GetCurrentFileDirPath(__FILE__, WIDGET_DIR_PATH_LEN_MAX, curFileDirPath);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file current path error, stat = 0x%08llX", gduStat);
        return gduStat;
    }

    if (s_isWidgetFileDirPathConfigured == true) {
        snprintf(tempPath, WIDGET_DIR_PATH_LEN_MAX, "%swidget_file/en_big_screen", s_widgetFileDirPath);
    } else {
        snprintf(tempPath, WIDGET_DIR_PATH_LEN_MAX, "%swidget_file/en_big_screen", curFileDirPath);
    }

    //set default ui config path
    gduStat = GduWidget_RegDefaultUiConfigByDirPath(tempPath);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add default widget ui config error, stat = 0x%08llX", gduStat);
        return gduStat;
    }

    //set ui config for English language
    gduStat = GduWidget_RegUiConfigByDirPath(GDU_MOBILE_APP_LANGUAGE_ENGLISH,
                                             GDU_MOBILE_APP_SCREEN_TYPE_BIG_SCREEN,
                                             tempPath);
```

#### 3.注册控件处理函数列表

通过`GduWidget_RegHandlerList`接口注册负载设备的某项功能和对应的控件参数，将负载设备的功能绑定到指定的控件上。

```text
    gduStat = GduWidget_RegHandlerList(s_widgetHandlerList, s_widgetHandlerListCount);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set widget handler list error, stat = 0x%08llX", gduStat);
        return gduStat;
    }
```

---

## HMS 功能

---
title: HMS 功能
date: 2026-04-16
---

## 概述

HMS（健康管理系统）是对飞机各模块的运行健康状态的一个监控系统。在GDU Flight上已经提供了交互界面查看各模块的工作状态是否有异常。PSDK则提供了基本接口来获取各模块异常的错误信息，便于PSDK程序获知飞机各模块的工作状态。

## 基本概念

### HMS错误信息

单条HMS的错误信息包括一个错误ID，错误的Index和一个错误的等级，在订阅HMS信息后，这些错误信息将会打包发送给PSDK。PSDK根据这些错误信息的数据解析成字符串，并从终端打印出来。

## 使用HMS功能

### 1. 初始化HMS功能模块

在使用HMS功能的相关接口之前，需要先调用`GduTest_HmsInit` 接口初始化HMS功能模块。

```c
returnCode = GduTest_HmsInit();
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("Hms sample init error, error code:0x%08llX", returnCode);
    goto out;
}
```

### 2. 订阅飞行状态信息

在HMS模块内会根据TOPIC_STATUS_FLIGHT的订阅数据来判断飞机是否在空中，从而区分HMS信息是在飞行过程中接收到的还是在地上接收到的。

```c++
T_GduReturnCode returnCode;

returnCode = GduFcSubscription_Init();
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("Hms sample init data subscription module failed, error code:0x%08llX", returnCode);
    return returnCode;
}

/*! subscribe fc data */
returnCode = GduFcSubscription_SubscribeTopic(GDU_FC_SUBSCRIPTION_TOPIC_STATUS_FLIGHT,
                                              GDU_DATA_SUBSCRIPTION_TOPIC_10_HZ,
                                              NULL);

if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("HMS sample subscribe topic flight status error, error code:0x%08llX", returnCode);
    return returnCode;
}
```

### 3. 注册HMS信息回调

开发者可通过调用`GduHms_RegHmsInfoCallback` 接口注册HMS信息回调的方式获取到实时的HMS错误信息推送，当HMS错误发生时，用户可以在`GduTest_HmsInfoCallback`回调函数中获取到具体的HMS信息。

```c++
osalHandler = GduPlatform_GetOsalHandler();
USER_LOG_INFO("--> Step 2: Register callback function of push HMS information");
GduTest_WidgetLogAppend("--> Step 2: Register callback function of push HMS information");
returnCode = GduHms_RegHmsInfoCallback(GduTest_HmsInfoCallback);
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("Register callback function of push HMS information failed, error code:0x%08llX", returnCode);
    goto out;
}

osalHandler->TaskSleepMs(30000);
```

---

## 时间同步

---
title: 时间同步
date: 2026-04-16
---

#### 概述

时间同步是一个用于同步负载设备时间和无人机时间的功能，PSDK 通过PPS 信号（周期性的脉冲）同步负载设备和**具有RTK 功能的无人机**的时间。具有"时间同步"功能的负载设备，能够方便用户顺利地使用日志排查无人机飞行过程中的各类故障、分析传感器采样的数据以及获取精准的定位信息等功能。

> **本文所指**
>
> - 无人机时间：无人机系统的时间。
> - 本地时间：负载设备上的时间。

#### 时间同步

安装在无人机上的使用PSDK 开发的负载设备在上电后，将初始化时间同步功能模块，消除负载设备和无人机的时钟差，同步负载设备和无人机的时间。

> **说明：** 使用时间同步功能前，请通过移动端App 确认无人机与RTK 卫星间保持良好的通信状态；该移动端App 可为GDU 发布的App，如GDU Flight，也可为基于MSDK 开发的移动端App.

1. 将负载设备安装在无人机的云台上，在无人机上电后，使用PSDK 开发的负载设备将接收到无人机发送的PPS 硬件脉冲信号；

2. 当负载设备检测到PPS 信号的上升沿时，负载设备需要记录负载设备上的本地时间；

3. PSDK 的底层处理程序将获取与PPS 信号同步的无人机系统上的时间。

   > **注意**
   >
   > - 请确保PPS 信号上升沿到达负载至负载记录本地时间之间的延迟低于1ms。
   > - 请使用硬件中断的形式实现 PPS 信号的响应。

4. PSDK 的底层处理程序将计算负载设备本地的时间与无人机系统时间的时钟差，实现负载设备与无人机系统时间的同步。

5. 负载设备通过`GduTimeSync_TransferToAircraftTime`接口将负载设备本地的时间转换为无人机系统上的时间。

#### 使用时间同步功能

#### 1. 配置PPS 引脚参数

为了使PPS 引脚能够正确接收PPS 信号，需要设置PPS 引脚的各项参数，并开启PPS 硬件引脚的功能，接收PPS 时间同步信号。

```c
T_GduReturnCode GduTest_PpsSignalResponseInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOD clock */
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* Configure pin as input floating */
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = PPS_PIN;
    HAL_GPIO_Init(PPS_PORT, &GPIO_InitStructure);

    /* Enable and set EXTI Line Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(PPS_IRQn, PPS_IRQ_PRIO_PRE, PPS_IRQ_PRIO_SUB);
    HAL_NVIC_EnableIRQ(PPS_IRQn);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
```

#### 2. 时间同步功能模块初始化

使用时间同步功能，需要初始化时间同步模块。

```c
gduStat = GduTimeSync_Init();
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("time synchronization module init error.");
    return gduStat;
}
```

#### 3. 开发并注册获取负载设备上本地时间的函数

基于PSDK 开发的负载设备在使用时间同步功能时，需要以硬件中断的方式响应无人机推送的PPS 时间同步信号；当最新的PPS 信号被触发时，基于PSDK 开发的负载设备控制程序，需要获取到负载设备本地的时间。

1. 开发并注册硬件中断处理函数
   开发者需要实现硬件中断处理功能，并将处理硬件中断的函数注册到指定的接口中，当负载设备接收到PPS 信号的上升沿时，硬件中断处理函数能够处理PPS 时间同步信号。

```c
void GduTest_PpsIrqHandler(void)
{
    T_GduReturnCode psdkStat;
    uint32_t timeMs = 0;

    /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_IT(PPS_PIN) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(PPS_PIN);
        psdkStat = Osal_GetTimeMs(&timeMs);
        if (psdkStat == GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
            s_ppsNewestTriggerLocalTimeMs = timeMs;
    }
}
```

2. 注册获取PPS 信号被触发时负载设备本地时间的函数
   注册获取PPS 信号被触发时负载设备本地时间功能的函数后，需要将该函数注册到负载设备控制程序中，当负载设备接收到PPS 信号的上升沿时，基于PSDK 开发的负载设备控制程序，能够记录PPS 信号被触发时负载设备的本地时间。

```c
// users must register getNewestPpsTriggerTime callback function
gduStat = GduTimeSync_RegGetNewestPpsTriggerTimeCallback(s_timeSyncHandler.GetNewestPpsTriggerLocalTimeUs);
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("register GetNewestPpsTriggerLocalTimeUsCallback error.");
    return gduStat;
}
```

#### 4. 时间同步

使用PSDK 开发的负载设备控制程序通过`PsdkOsal_GetTimeMs`获取负载设备上的本地时间并将负载设备上的时间转换为无人机系统的时间。

- 获取负载设备本地的时间

```c
gduStat = osalHandler->GetTimeMs(&currentTimeMs);
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("get current time error: 0x%08llX.", gduStat);
    continue;
}
```

- 时间转换
  将负载设备本地的时间转换为无人机系统上的时间。

```c
gduStat = GduTimeSync_TransferToAircraftTime(currentTimeMs * 1000, &aircraftTime);
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("transfer to aircraft time error: 0x%08llX.", gduStat);
    continue;
}

USER_LOG_DEBUG("current aircraft time is %d.%d.%d %d:%d %d %d.", aircraftTime.year, aircraftTime.month, aircraftTime.day, aircraftTime.hour, aircraftTime.minute, aircraftTime.second, aircraftTime.microsecond);
```

#### 适配产品

S400 RTK（PPS 信号频率：1Hz）

---

## 数据传输

---
title: 数据传输
date: 2026-04-16
---

#### 概述

负载设备和无人机使用数据传输模块，在控制命令传输通道上以透传的方式在PSDK和MSDK间传输控制指令

> **说明:** PSDK 的数据传输模块以透传的方式在负载设备、机载计算机和无人机间传输数据，支持用户根据实际的使用需求设计数据传输协议，使基于PSDK 开发的负载设备能够与移动端App 或机载计算机间实现复杂通信。

#### 基础概念

#### 通道的分类

目前GDU PSDK 的数据传输通道支持命令信号传输通道。命令信号传输通道：可靠性强，专用于传输对可靠性要求较高的信号，如控制指令和状态信息等。

#### 通道的带宽

- 通道带宽：数据通道在**理论状态下**可传输的数据量
- 实时带宽：数据通道在**工作状态下理论上**可传输的数据量
- 实际带宽：数据通道实际传输的数据量

#### 通道带宽限制

数据传输通道的带宽限制，是指在单位时间内，通道**最大**能够传输的数据量（byte/s）。
受制于接口部件的物理特性和实际的使用环境，命令信号传输通道带宽为**静态带宽限制**。

#### 静态带宽限制

静态带宽限制是指命令信号传输通道**固定的**带宽大小，受制于材料的物理特性和固有的电气特性而**无法被改变**。

#### 流量控制

PSDK 的数据传输模块通过使用流量阈值和缓冲区，限制负载设备向移动端App 或机载计算机发送的数据量，实现流量控制功能。

PSDK 的流量控制过程如下所示：

1. 负载设备通过数据通道向机载计算机或移动端App 传输数据信息；
2. 将负载设备发送的数据暂存入缓冲区中；
3. 当缓冲区数据超过了**流量阈值**时，负载设备发送的数据将被丢弃；

> **说明**
>
> - 数据传输模块的流量控制周期为 1s，在每个控制周期内，负载设备传输的数据量应小于用户设置的**流量阈值**。
> - 流量阈值决定当前数据传输通道最大可传输的数据，该阈值由静态带宽限制因素决定。

#### 通道状态

使用PSDK 开发的负载设备能够获取到通道的繁忙状态：当对应通道的数据被存入缓冲区或被丢弃时，数据传输模块的通道状态为"繁忙"。

#### 使用数据传输功能

使用PSDK 的数据传输功能，需要在创建工程文件并完成PSDK 的初始化后，初始化数据传输模块再实现数据传输功能和监控数据通道状态的功能。

##### 1. 数据传输功能模块初始化

使用"数据传输"功能前，需要使用如下代码初始化数据传输功能模块，确保负载设备能够正常传输数据。

```c
gduStat = GduLowSpeedDataChannel_Init();
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("init data transmission module error.");
    return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
}
```

##### 2. 实现数据发送功能

使用PSDK 开发的负载设备通过命令信号传输通道向移动端App 或机载计算机发送测试数据。

- 向移动端App 发送控制命令
  使用PSDK 开发的负载设备在命令信号传输通道上向移动端App 发送**控制命令**。

```c
        channelAddress = GDU_CHANNEL_ADDRESS_MASTER_RC_APP;
        gduStat = GduLowSpeedDataChannel_SendData(channelAddress, dataToBeSent, sizeof(dataToBeSent));
        if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
            USER_LOG_ERROR("send data to mobile error.");
```

##### 4. 实现数据接收功能

构造并注册数据接收函数后，使用PSDK 开发的负载设备能够通过命令信号传输通道接收从移动端设备和机载计算机发送的控制命令。

###### 4.1 构造回调函数

构造回调函数获取并打印从移动端App 和机载计算机上发送的控制命令。

```c
static T_GduReturnCode ReceiveDataFromMobile(const uint8_t *data, uint16_t len)
{
    char *printData = NULL;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    printData = osalHandler->Malloc(len + 1);
    if (printData == NULL) {
        USER_LOG_ERROR("malloc memory for printData fail.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    strncpy(printData, (const char *) data, len);
    printData[len] = '\0';
    USER_LOG_INFO("receive data from mobile: %s, len:%d.", printData, len);
    GduTest_WidgetLogAppend("receive data: %s, len:%d.", printData, len);

    osalHandler->Free(printData);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_GduReturnCode ReceiveDataFromOnboardComputer(const uint8_t *data, uint16_t len)
{
    char *printData = NULL;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

    printData = osalHandler->Malloc(len + 1);
    if (printData == NULL) {
        USER_LOG_ERROR("malloc memory for printData fail.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    strncpy(printData, (const char *) data, len);
    printData[len] = '\0';
    USER_LOG_INFO("receive data from onboard computer: %s, len:%d.", printData, len);
    GduTest_WidgetLogAppend("receive data: %s, len:%d.", printData, len);

    osalHandler->Free(printData);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
```

###### 4.2 注册回调函数

通过注册回调函数，负载设备能够接收从移动端App 和机载计算机发送的控制命令。

```c
channelAddress = GDU_CHANNEL_ADDRESS_MASTER_RC_APP;
gduStat = GduLowSpeedDataChannel_RegRecvDataCallback(channelAddress, ReceiveDataFromMobile);
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("register receive data from mobile error.");
    return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
}
```

##### 5. 监控数据传输通道的状态

在数据传输线程中获取数据通道的状态并打印到终端。

- 获取命令信号传输通道的状态信息
  获取负载设备向移动端App 和机载计算机发送控制命令的命令信号传输通道的状态。

```c
gduStat = GduLowSpeedDataChannel_GetSendDataState(channelAddress, &state);
if (gduStat == GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_DEBUG(
    "send to onboard computer state: realtimeBandwidthBeforeFlowController: %d, realtimeBandwidthAfterFlowController: %d, busyState: %d.",
    state.realtimeBandwidthBeforeFlowController, state.realtimeBandwidthAfterFlowController,
    state.busyState);
} else {
	USER_LOG_ERROR("get send to onboard computer channel state error.");
}
```

---

## 视频流传输

---
title: 视频流传输
date: 2026-04-16
---

#### 概述

使用PSDK 提供的视频流传输控制功能，开发者**需要先实现**获取视频流文件码流的功能。

对视频流编码，并结合视频的帧率等参数，调用指定的接口发送视频流数据。用户通过使用GDU Flight 以及基于MSDK 开发的移动端App，能够获取相机类负载设备上实时的视频数据。

> **相关参考**
> - 仅基于Linux 开发的相机类负载设备支持视频流传输功能。

#### 使用视频流传输功能

#### 1. 配置网络参数

- 以自动的方式设置网络参数
   为方便开发者使用**具有网口的开发平台**实现更广泛的应用，PSDK 推荐开发者使用`GduPlatform_RegHalNetworkHandler()`接口注册设置负载设备网络参数的回调函数，以自动的方式设置网络参数;

设置网络参数时，开发者须使用`GduPlatform_RegHalNetworkHandler()`接口注册设置负载设备网络参数的回调函数。

```c
T_GduReturnCode HalNetWork_Init(const char *ipAddr, const char *netMask, T_GduNetworkHandle *halObj)
{
    int32_t ret;
    char cmdStr[LINUX_CMD_STR_MAX_SIZE];

    if (ipAddr == NULL || netMask == NULL) {
        USER_LOG_ERROR("hal network config param error");
        return GDU_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    memset(cmdStr, 0, sizeof(cmdStr));

    snprintf(cmdStr, sizeof(cmdStr), "ifconfig %s up", LINUX_NETWORK_DEV);
    ret = system(cmdStr);
    if (ret != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Can't open the network.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    snprintf(cmdStr, sizeof(cmdStr), "ifconfig %s %s netmask %s", LINUX_NETWORK_DEV, ipAddr, netMask);
    ret = system(cmdStr);
    if (ret != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Can't config the ip address of network.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
```

#### 2. 创建视频流处理线程

为避免因其他任务阻塞视频流处理线程，导致视频流传输时出现花屏和绿屏的问题，请在使用PSDK 开发相机类负载设备时，单独创建视频流处理线程。

```c
    returnCode = osalHandler->TaskCreate("user_camera_media_task", UserCameraMedia_SendVideoTask, 2048, NULL, &s_userSendVideoThread);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("user send video task create error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }
```

#### 3. 获取视频流文件信息

使用PSDK 开发的相机类负载设备在发送视频流文件前，须读取相机类负载设备本地的H.264 文件，获取视频流文件的信息。

```c
// video send preprocess
returnCode = GduPlayback_VideoFileTranscode(videoFilePath, "h264", transcodedFilePath,
                                            GDU_FILE_PATH_SIZE_MAX);
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("transcode video file error: 0x%08llX.", returnCode);
    continue;
}

returnCode = GduPlayback_GetFrameRateOfVideoFile(transcodedFilePath, &frameRate);
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("get frame rate of video error: 0x%08llX.", returnCode);
    continue;
}
```

#### 5. 发送视频流文件

基于PSDK 开发的相机类负载设备在解析视频流文件并识别视频流文件的帧头后，将以逐帧的方式发送视频流文件。

> **注意:** 基于PSDK 开发的相机类负载设备最长可发送的帧为65K，若帧的长度超过65k，则需拆解该帧发送。

```c
lengthOfDataHaveBeenSent = 0;
while (dataLength - lengthOfDataHaveBeenSent) {
    lengthOfDataToBeSent = USER_UTIL_MIN(DATA_SEND_FROM_VIDEO_STREAM_MAX_LEN,
                                         dataLength - lengthOfDataHaveBeenSent);
    returnCode = GduPayloadCamera_SendVideoStream((const uint8_t *) dataBuffer + lengthOfDataHaveBeenSent,
                                                  lengthOfDataToBeSent);
    if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("send video stream error: 0x%08llX.", returnCode);
    }
    lengthOfDataHaveBeenSent += lengthOfDataToBeSent;
}
```

使用GDU Flight 或基于MSDK 开发的移动端App 向相机类负载设备发送视频流传输命令后，移动端App 将能接收并循环播放相机类负载设备中的媒体文件。

---

## 本地升级

---
title: 本地升级
date: 2026-04-16
---

#### 概述

本地升级是一个用于升级负载设备固件的功能，开发者使用本地升级功能，能够方便用户通过GDU Flight 2 选择本地固件文件更新负载设备的固件。

#### 固件传输协议

开发在开发本地升级功能时，可调用接口`GduUpgrade_Init`指定传输协议并配置传输设置信息。

#### 实现本地升级功能

##### 1. 本地升级模块初始化

在使用本地升级功能前，请先初始化升级模块，并指定升级固件时所使用的传输协议。

```c
returnCode = GduUpgrade_Init(&upgradeConfig);
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("GduUpgrade_Init error, return code = %d", returnCode);
    return returnCode;
}
```

##### 2. 开启本地升级功能

用户在使用本地升级功能前，需是使能本地升级功能，使能本地升级功能后，GDU Assistant 2 将会显示负载设备的升级界面，用户可选择所需的固件文件升级负载设备。

```c
returnCode = GduUpgrade_EnableLocalUpgrade();
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("GduUpgrade_EnableLocalUpgrade error, return code = %d", returnCode);
    return returnCode;
}
```

##### 3. 升级流程

开发者根据结构体`T_GduUpgradeHandler`实现负载设备本地升级的函数，并通过`GduUpgrade_RegHandler()`接口注册负载设备本地升级的回调函数；通过函数`GduUpgrade_PushUpgradeState()`推送负载设备升级状态。

#### 编译固件

1. 修改固件版本号
   在`T_GduTestUpgradeConfig`结构定义中修改固件的版本号。

```c
    T_GduTestUpgradeConfig testUpgradeConfig = {
        .firmwareVersion = {1, 0, 0, 0},
        .transferType = GDU_FIRMWARE_TRANSFER_TYPE_DCFTP,
        .needReplaceProgramBeforeReboot = true
    };
```

1. 修改固件文件名
   重新编译固件文件后，需修改编译后的固件的名称，如`PSDK_APPALIAS_V02.01.00.04.bin`。 文件名称格式：`Product Name_Vaa.bb.cc.dd.bin`
   - Product Name：负载产品名（若开发者设置了负载别名，该名称则为负载别名）
   - Vaa.bb.cc.dd：为负载四位版本号

##### 使用GDU Assistant 2 升级固件

打开GDU Assistant 2选择编译生成的固件升级文件（PSDK_APPALIAS_V02.01.00.04.bin），进行固件升级。

#### Sample 升级原理介绍

##### Linux 程序文件升级原理

利用升级功能可以获取到需要更新的固件文件，将固件文件替换掉原先运行的程序文件，系统重启后完成固件升级。

##### STM32F4单片机程序升级原理

使用STM32F4 开发的负载设备从Flash 中直接读取固件文件并运行负载设备程序，因此开发者需先烧录Bootloader 再烧录应用固件.

##### STM32F4 固件升级流程

1. STM32F4 平台上电后，将启动MCU
2. 运行Bootloader 并判断升级状态：
   - 若无需升级固件，Bootloader 将跳转至应用固件区，并运行应用固件
   - 若需升级负载设备的固件，Bootloader 将在替换固件文件后，跳转至应用固件区，并运行应用固件

---

## 精准定位

---
title: 精准定位
date: 2026-04-16
---

> **提示：** 在运行"精准定位"示例代码前，请使用GDU Flight 或MSDK开发的APP 查看无人机与RTK 卫星间保持良好的通信状态，确保负载设备可获取精准的定位结果.

#### 概述

为满足使用PSDK 开发的负载设备对厘米级精度的定位需求，GDU 支持开发者使用基准站（如D-RTK 2）和移动站（S400 RTK），借助RTK（Real Time Kinematic，实时动态载波相位差分技术）获取无人机飞行姿态和高精度的定位信息。

#### 基础概念

##### 术语解释

- 目标点：实际获取到定位信息的位置，如云台口中心点。
- 兴趣点：由用户任意指定的负载设备上某一器件的位置，如相机图像传感器的中心点，该目标点也可以为兴趣点。
- 任务：多个连续的飞行动作集合称为一个任务，如对某个区域执行一次测绘任务。根据实际使用需要，用户可创建多个任务。
- 定位事件：触发定位请求的事件，如相机曝光时触发定位请求，则"相机曝光"是一个定位事件；多个事件的集合为事件集合，使用PSDK 开发的负载设备可同时请求多个定位事件发生时的位置信息，如相机协同曝光。

##### 获取精准定位

> **说明：** 获取精准定位时，需使用时间同步功能将负载设备的本地时间同步为无人机时间。

1. 定位事件发生时，负载设备需要会记录本地时间（该时间为负载设备上的时间）；
2. 负载设备通过时间转换功能，将负载设备上的时间转换为无人机上的时间；
3. 负载设备使用定位事件发生时的无人机时间（无人机系统的时间）请求位置。

> **说明:** 定位事件发生时的无人机时间（无人机系统的时间）应早于最新的PPS 信号上升沿时间，且时间间隔须在1～2s内。

获取目标点的位置后，根据目标点的位置，使用目标点与无人机RTK 主天线位置的偏移量、无人机的姿态、负载设备的结构等信息，能够计算兴趣点的位置。

#### 使用精准定位功能

##### 1. 定位功能模块初始化

使用"精准定位"功能前，需要先初始化精准定位功能模块，确保精准定位功能可正常运行。

```c
gduStat = GduPositioning_Init();
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("positioning module init error.");
    return gduStat;
}
```

##### 2. 请求并打印位置信息

用户触发精准定位功能后，负载设备将根据定位事件发生时的时间获取精准的定位信息。

1. 获取本地时间
   用户触发精准定位功能后，负载设备获取PPS 信号触发时负载设备上的本地时间。

```c
gduStat = GduTest_TimeSyncGetNewestPpsTriggerLocalTimeUs(&ppsNewestTriggerTimeUs);
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("get newest pps trigger time error: 0x%08llX.", gduStat);
    continue;
}
```

2. 时间转换
   将指定的定位事件发生时的负载设备上的本地时间转换为无人机系统的时间。

```c
for (i = 0; i < GDU_TEST_POSITIONING_EVENT_COUNT; ++i) {
    eventInfo[i].eventSetIndex = s_eventIndex;
    eventInfo[i].targetPointIndex = i;

    gduStat = GduTimeSync_TransferToAircraftTime(
        ppsNewestTriggerTimeUs - 1000000 - i * GDU_TEST_TIME_INTERVAL_AMONG_EVENTS_US, &aircraftTime);
    if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("transfer to aircraft time error: 0x%08llX.", gduStat);
        continue;
    }

    eventInfo[i].eventTime = aircraftTime;
}
```

3. 获取精准的定位信息
   完成时间同步后，用户即可获取并打印负载设备在某一时刻的精准位置。

```c
gduStat = GduPositioning_GetPositionInformationSync(GDU_TEST_POSITIONING_EVENT_COUNT, eventInfo, positionInfo);
if (gduStat != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("get position information error.");
    continue;
}

USER_LOG_DEBUG("request position of target points success.");
```

#### 适配产品

S400 RTK

---

## 运动规划

---
title: 运动规划
date: 2026-04-16
---

## 概述

GDU PSDK 为满足用户对控制无人机飞行自动化的需求，提供了运动规划功能；开发者使用运动规划功能，可根据实际的使用需求设计相应的航点任务和热点任务，制定控制无人机自动化飞行的控制逻辑。

## 航点任务

航点规划是一个控制无人机按照指定的航线飞行，实现无人机飞行自动化的控制功能。开发者通过调用GDU PSDK 的接口，能够控制无人机以**指定的高度**飞往**指定位置**并执行相应动作，根据实际的使用需求，还可控制无人机重复多次执行指定的任务，实现自动化巡航等功能。

## 使用航点任务 - Waypoint 3.0

Waypoint 3.0功能提供了上传航线任务、监控航线状态和控制航线执行动作等功能。

#### 1. 航点任务初始化

在使用航线任务-Waypoint 3.0功能之前，需要先调用接口`GduWaypointV3_Init` 接口进行初始化操作。

```c
returnCode = GduWaypointV3_Init();
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("Waypoint v3 init failed.");
    return returnCode;
}
```

#### 2. 上传航点任务

用户可以通过调用`GduWaypointV3_UploadXmlFile` 直接传入xml文件数据，执行航线任务。xml文件可以在GDU Flight提前规划好航线导出。

```c
T_GduReturnCode GduWaypointV3_UploadXmlFile(const uint8_t *data, uint32_t dataLen);
```

#### 3. 控制无人机执行航点任务

当航线上传完成之后，即可调用`GduWaypointV3_Action` 执行航线任务，包括开始、停止、暂停和继续执行航线任务。

```c
T_GduReturnCode GduWaypointV3_Action(E_GduWaypointV3Action action);
```

#### 4. 监测航线状态

在用户开始执行航线任务之前，建议注册航线状态回调接口，在执行航线过程中，可以通过注册的回调函数实时接收航线状态信息，保证飞行安全。

```c
returnCode = GduWaypointV3_RegMissionStateCallback(GduTest_WaypointV3StateCallback);
if (returnCode != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    USER_LOG_ERROR("Register waypoint v3 state callback failed.");
    goto out;
}

static T_GduReturnCode GduTest_WaypointV3StateCallback(T_GduWaypointV3MissionState missionState)
{
    USER_LOG_INFO("Waypoint v3 mission state: %d", missionState.state);

    return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
```

---

## 相机管理

---
title: 相机管理
date: 2026-04-16
---

## 概述

为方便开发者在 PSDK 的基础上快速开发出控制 GDU 无人机上相机的功能，GDU PSDK 提供了相机管理功能，开发者基于相机管理的接口能够同时设置并获取无人机上多个相机的感光度、光圈、快门和曝光等参数的值，控制相机实现拍照、录像及指点变焦等功能。

## 相机管理功能

在使用相机管理功能时，开发者需要先**初始化** PSDK 中的相机管理功能模块，再根据实际的使用需要**设置相机的模式**，最后根据用户的使用逻辑实现所需使用的功能，如设置相机的参数或检查功能的状态等。

## 使用相机管理功能

### 1. 相机管理模块初始化

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

### 2. 获取相机类型和版本

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

### 3. 设置或获取相机参数

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

### 4. 控制相机执行指定的动作

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

### 6. 下载和删除相机原始媒体文件

开发者可以通过调用以下 Sample 来下载和删除相机拍摄的媒体文件，支持下载的媒体文件格式包括：JPG、DNG、MOV、MP4

```
GduTest_CameraManagerMediaDownloadAndDeleteMediaFile(mountPosition);
```

---
