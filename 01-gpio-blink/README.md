# 01 GPIO 基础实验

STM32F103 系列（大容量，启动文件 `startup_stm32f10x_hd.s`）的 GPIO 上手练习。
使用 Keil5 工程（`.uvprojx`），标准外设库（library1 / system / user1 / strat1）。

> 待补：把每块板子的型号、LED 接的引脚（如 PC13）、晶振频率填进来。

## 子实验

| 目录 | 内容 |
|------|------|
| `LED流水灯/` | GPIO 推挽输出，多个 LED 依次点亮（流水灯） |
| `3-4按键控制led/` | 按键输入 + GPIO 输出，按键控制 LED 亮灭 |
| `蜂鸣器/` | GPIO 驱动有源/无源蜂鸣器 |

## 关键代码（点灯核心）

```c
GPIO_SetBits(GPIOC, GPIO_PIN_13);    // 拉高
GPIO_ResetBits(GPIOC, GPIO_PIN_13);  // 拉低
// 或用翻转：在定时器里翻转电平实现闪烁
```

## 学到的

- GPIO 输出模式：推挽 vs 开漏
- 标准外设库初始化流程：时钟使能 → GPIO 结构体配置 → Init
- Keil 工程结构：library（外设库）/ system（系统时钟）/ user（应用）/ startup（启动文件）

## 踩坑

- 时钟没配置对，LED 不亮（先查 `SystemInit` / 系统时钟配置）
- 编译产物（Objects / Listings / DebugConfig）不传仓库，已在根目录 `.gitignore` 排除
