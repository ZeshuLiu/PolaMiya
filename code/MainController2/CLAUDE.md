# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

STM32G474RETx 微控制器项目，使用 STM32CubeMX 生成配置，通过 Keil MDK-ARM (CMSIS 6) 构建。这是一个主控设备项目，包含 LCD 显示、USB 通信、电机控制、电量计量等功能。

## Build & Debug

### 构建命令
- 使用 **Keil MDK-ARM** 打开 `MDK-ARM/MainController2.uvprojx` 进行编译
- 解决方案配置：`MDK-ARM/MainController2.csolution.yml`
- 目标设备：STM32G474RETx
- 编译器：ARM Compiler 6 (AC6)

### 调试配置
- 调试器：ST-Link via pyOCD
- 接口：SWD，时钟 4MHz
- 使用 VS Code CMake 调试配置（见 `.vscode/launch.json`）

## Code Architecture

### 目录结构
```
├── Core/           # STM32CubeMX 生成的核心代码 (main, 系统初始化，外设驱动)
├── Drivers/        # STM32G4xx HAL 库和 CMSIS
├── HARDWARE/LCD/   # LCD 显示屏驱动 (280x240 分辨率)
├── Libs/           # 自定义功能库
│   ├── ZUI/        # 自研 UI 框架 (轻量级嵌入式 GUI)
│   ├── TMP102/     # 温度传感器驱动
│   ├── SDM18/      # 电量计量芯片驱动
│   ├── Motors/     # 电机控制 (TIM16 PWM)
│   └── EEPROM/     # 数据存储 (DataStore/eeprom)
├── USB_Device/     # USB CDC 虚拟串口
└── MDK-ARM/        # Keil 项目配置
```

### 关键模块

**ZUI 框架** (`Libs/ZUI/`)
- 基于 UI_Element 链表和 UI_Layer 的轻量级 GUI
- 双缓冲渲染 (280x12 行 x2 字节 = 6720 字节)
- 支持按键响应 (短按/长按)
- 主要 API: `zui_init()`, `zui_set_current_layer()`, `zui_render_current_layer()`

**主循环架构** (`main.c`)
- TIM2 定时器中断提供时间基准 (10ms/200ms/300ms/1s)
- 状态机处理：按键扫描、ADC 采样、UI 刷新、电量计量
- 电机控制状态机：`motor_start()` → 运行 → `motor_end()`

**外设配置**
- ADC1: DMA 采样 (Vbat, 温度传感器，VREFINT)
- I2C2: TMP102 温度传感器
- I2C3: 备用
- SPI1/SPI3: LCD/其他外设
- USART2/USART3/LPUART1: 串行通信
- TIM1/TIM2/TIM16: 电机 PWM 和定时中断
- USB Device: CDC 类虚拟串口

### 命名约定
- HAL 风格：`MX_` 前缀为初始化函数，`HAL_` 前缀为 HAL 库函数
- 自定义模块：小写模块名 + 下划线，如 `zui_init`, `motor_start`
- 全局变量：大写 (`USR_KEYs`, `normal_layer`) 或带模块前缀

## 开发注意事项

1. **修改外设后**：需在 STM32CubeMX (`.ioc` 文件) 中重新生成代码
2. **添加新模块**：在 `Libs/` 下创建，更新 `.cproject.yml` 的 include 路径和文件列表
3. **UI 开发**：继承 ZUI 框架，定义 UI_Element 并实现 `render()` 回调
4. **内存敏感**：嵌入式环境，注意栈/堆使用和 DMA 缓冲区对齐

## 相关文档

- STM32G474 参考手册：https://www.st.com/en/microcontrollers-microprocessors/stm32g474re.html
- CMSIS 6 文档：https://github.com/ARM-software/CMSIS_6
- Keil MDK: https://www.keil.com/download/
