
## 文件说明
请在立创eda中导入 epro文件
请使用platformIO 导入工程ElectricDog

## 视频链接：
[B站视频--功能演示及介绍](https://www.bilibili.com/)
## 项目简介

本项目是基于ESP8266设计、基于arduino开发的电子小狗，项目同时运行了ESP8266网络服务器、WS2812驱动、舵机驱动、oled显示等功能。可以实现Wi-Fi控制
## 基本实现效果
1.webserver 控制

2.基本的前进 后退 转弯 还有一些动作

3.oled 表情显示

4.ws2812 灯效

5.无客户端连接小狗 Wi-Fi 一段时间后 小狗趴下，再过一段时间后仍然无客户端连接小狗进入睡眠状态 （功耗极低）

6.在充电时禁止控制

7.服务器 舵机 oled 等效同时运行（伪）

8.电量低时显示低电量信息，禁止控制


## 项目材料

本项目使用的原件在eda的epro文件可以看到 另外说明的是充电模块（charging module）是 http://e.tb.cn/h.gr9VPKEFwmDm6h5?tk=g0wm37w5mOG ，当然 你可以其他模块 或者设计充电保护电路
