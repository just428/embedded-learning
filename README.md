# embedded-learning

汕头大学 · 通信工程 | 嵌入式学习记录

目标：2027 暑假嵌入式实习（STM32 / RTOS / BLE）

## 进度打卡

| 模块 | 内容 | 状态 |
|------|------|------|
| GPIO | 点灯 / 按键 / 蜂鸣器 | ✅ 完成 |
| 定时器 | 定时中断 / PWM 呼吸灯 | 进行中 |
| UART | printf 重定向 / 环形缓冲接收 | 待开始 |
| I2C | OLED 显示 | 待开始 |
| SPI | Flash / TFT | 待开始 |
| ADC+DMA | 电压采集 | 待开始 |
| FreeRTOS | 任务 / 队列 / 信号量 | 待开始 |
| ESP32 BLE | GAP / GATT / 自定义服务 | 待开始 |

## 目录说明

- `01-gpio-blink/` GPIO 基础实验（LED流水灯 / 3-4按键控制led / 蜂鸣器），每个子目录含 Keil5 工程与源码
- `notes/` 每周学习笔记，记录卡住的地方和解法
- `c-practice/` C 语言手写题，每题一个独立 `.c` + 预期输出

## C 练手写题

- [x] 字符串函数 strlen / strcpy / strcmp / strcat
- [x] 位操作宏 SET_BIT / CLR_BIT
- [ ] memcpy / memmove
- [ ] 环形缓冲区
- [ ] 单链表反转
- [ ] 联合体大小端转换

## 硬件

- STM32F103 系列开发板（大容量，`startup_stm32f10x_hd.s`）
- 编译环境：Keil MDK5 + 标准外设库
