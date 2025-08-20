# ble-pay-controller
基于国民技术BLE5.1芯片的扫码支付控制器

项目基于 BLE（低功耗蓝牙）技术，开发了一套可与微信小程序进行通信的智能设备控制系统，实现了设备绑定、启停控制、倒计时同步显示等核心功能。系统通过蓝牙实现从微信小程序到嵌入式设备的实时命令交互与外设控制，适用于共享设备控制等多种智能场景。

**项目架构：**

<img width="541" height="361" alt="image" src="https://github.com/user-attachments/assets/bf7ad1ec-5afc-40b5-b05f-d1fb0f0ef766" />

**项目框架：**

<img width="1257" height="538" alt="image" src="https://github.com/user-attachments/assets/ac11dc1c-cacb-4cc5-a987-9206ddd5d0d8" />


**选择的MCU为入门级的cortex-M0内核架构蓝牙类MCU，N32WB031：**
<img width="1266" height="823" alt="image" src="https://github.com/user-attachments/assets/7b7db5d5-fca3-46d3-a0c4-80c343a6c36e" />

