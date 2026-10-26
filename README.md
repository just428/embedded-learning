# embedded-learning

汕头大学 · 通信工程 | 嵌入式学习记录
目标：2027 暑假嵌入式实习（STM32 / RTOS / BLE）

## 进度打卡

| 模块 | 内容 | 状态 |
|------|------|------|
| C 手写题 | strlen/strcpy/strcmp/strcat | ✅ 完成（2026-10-26） |
| C 手写题 | 位操作宏 SET_BIT / CLR_BIT | ✅ 完成（2026-10-26） |
| C 手写题 | memcpy / memmove | 待开始 |
| C 手写题 | atoi / itoa | 待开始 |
| GPIO | 点灯 / 按键 / 蜂鸣器 | ✅ 完成 |
| 定时器 | 定时中断 / PWM 呼吸灯 | 进行中 |
| UART | printf 重定向 / 环形缓冲接收 | 待开始 |
| I2C | OLED 显示 | 待开始 |
| SPI | Flash / TFT | 待开始 |
| ADC+DMA | 电压采集 | 待开始 |
| FreeRTOS | 任务 / 队列 / 信号量 | 待开始 |
| ESP32 BLE | GAP / GATT / 自定义服务 | 待开始 |

## C 练手写题清单

- [x] 字符串函数 strlen / strcpy / strcmp / strcat
- [x] 位操作宏 SET_BIT / CLR_BIT
- [ ] memcpy / memmove
- [ ] 环形缓冲区
- [ ] 单链表反转
- [ ] 联合体大小端转换

## 目录说明

- `01-gpio-blink/` GPIO 基础实验（LED流水灯 / 3-4按键控制led / 蜂鸣器），每个子目录含 Keil5 工程与源码
- `c-practice/` —— C 语言手写题，每题一个独立目录 + README + 自己写的 `.c`，含踩坑注释
- `notes/` —— 每周学习笔记（规划中），记录卡住的地方和解法

## 硬件

- STM32F103 系列开发板（大容量，`startup_stm32f10x_hd.s`）
- 编译环境：Keil MDK5 + 标准外设库
- 其他：ST-Link + SSD1306 OLED + AHT20 温湿度

## 参考资料

- 学习路线与计划见 `嵌入式实习学习路线图.md`
- GitHub 上手见 `GitHub从零留痕上手.md`
