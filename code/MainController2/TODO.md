# 代码问题待办清单

生成时间：2026-03-20

---

## 优先级 P0 - 严重问题（建议立即修复）

### [x] P0-01 `main.c:60` - `tim2_int_mask` 缺少 volatile
### [x] P0-02 `main.c:93-94` - 未使用的调试变量
### [x] P0-03 `motor.h:6-7` - 宏定义运算符优先级问题
### [x] P0-04 `main.c:68` - 未使用的全局变量
### [x] P0-05 `main.c:69` - 未使用的全局变量

## 优先级 P1 - 中等问题（建议尽快修复）

### [ ] P1-01 `sdm18.c:9` - 使用 `_Bool` 而非 `bool`

**位置**: `Libs/SDM18/sdm18.c:9`

**问题**: C99 标准建议使用 `<stdbool.h>` 中的 `bool` 类型。

**当前代码**:
```c
_Bool single_meter = 0;
```

**修复建议**:
```c
#include <stdbool.h>
bool single_meter = false;
```

---

### [ ] P1-02 `sdm18.c:10` - UART 返回值未检查

**位置**: `Libs/SDM18/sdm18.c:10`, `sdm18_start_meter()`, `sdm18_single_meter()`

**问题**: `sdm_tmp` 被赋值但从未检查，UART 操作失败时无法发现。

**当前代码**:
```c
volatile HAL_StatusTypeDef sdm_tmp;
sdm_tmp = HAL_UART_Transmit(...);  // 返回值未检查
```

**修复建议**: 添加返回值检查或使用 `(void)` 显式忽略

---

### [ ] P1-03 `zui.c:16` - 隐式依赖全局变量

**位置**: `Libs/ZUI/zui.c:16`

**问题**: 直接使用 `hspi1` 全局变量，与 `main.h` 耦合严重，移植性差。

**当前代码**:
```c
LCD_Init(hspi1);
```

**修复建议**: 通过参数传递 SPI 句柄，或在使用文件中初始化

---

### [ ] P1-04 `usart.c:400-403` - UART 错误回调为空

**位置**: `Core/Src/usart.c:400-403`

**问题**: UART 错误（帧错误、溢出等）被静默忽略，不利于调试。

**当前代码**:
```c
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    return;
}
```

**修复建议**: 添加错误日志或至少保留空函数体注释说明

---

### [ ] P1-05 `main.c:180-187` - 电池电压逻辑可能误判

**位置**: `Core/Src/main.c:180-187`

**问题**: `BAT_V` 是滤波后的值，若 ADC 采样异常可能导致背光频繁开关。

**当前代码**:
```c
if (BAT_V > 3.0){
    HAL_GPIO_TogglePin(KEY3_GPIO_Port, KEY3_Pin);
    LCD_BLK_Set();
}
else{
    HAL_GPIO_WritePin(KEY3_GPIO_Port, KEY3_Pin, GPIO_PIN_SET);
    LCD_BLK_Clr();
}
```

**修复建议**: 添加迟滞比较（如 2.8V/3.2V）或增加异常判断

---

## 优先级 P2 - 轻微问题（可选修复）

### [ ] P2-01 `motor.c:27` - 八进制字面量混淆

**位置**: `Libs/Motors/motor.c:27`

**问题**: `000` 是八进制写法，虽然值为 0 但易混淆。

**当前代码**:
```c
motor_pwm_width = 000;
```

**修复建议**:
```c
motor_pwm_width = 0;
```

---

### [ ] P2-02 `DataStore.c:39` - EEPROM 频繁写入

**位置**: `Libs/EEPROM/DataStore.c:39`

**问题**: 每次重启都写入 EEPROM，若系统频繁重启可能耗尽寿命（通常 100 万次）。

**当前代码**:
```c
void data_update_rbc()
{
    EpData.reboot_cnt += 1;
    AT24C08C_Write2KB(..., &EpData.reboot_cnt, ...);  // 每次都写
}
```

**修复建议**: 考虑延迟写入或累积一定次数后写入

---

### [ ] P2-03 `tim.c:314` - 计数器永不复位

**位置**: `Core/Src/tim.c:314`

**问题**: `int2_cnt` 会持续增长直到溢出（约 497 天），被注释的代码表明这是有意为之，建议确认。

**当前代码**:
```c
static uint32_t int2_cnt = 1;
// if (int2_cnt == TIM_INTCNT_MAX){  // 被注释掉
//   int2_cnt = 1;
// }
```

**修复建议**: 确认是否为预期行为，若是则添加注释说明

---

### [ ] P2-04 `zui.c:126, 151, 177` - 缓冲区计算可能越界

**位置**: `Libs/ZUI/zui.c:126, 151, 177`

**问题**: 若 `usr_data_len` 设置不正确，会导致显示截断或计算错误。

**当前代码**:
```c
uint16_t sizex = (el->usr_data_len-1) * 7;  // 假设 usr_data_len 是 strlen+1
```

**修复建议**: 添加边界检查或使用 `strlen` 计算

---

## 修复记录

| 日期 | 修复项 | 状态 |
|------|--------|------|
| 2026-03-20 | P0-01 `tim2_int_mask` 缺少 volatile | 已修复 |
| 2026-03-20 | P0-02 未使用的调试变量 | 已修复 |
| 2026-03-20 | P0-03 宏定义运算符优先级 | 已修复 |
| 2026-03-20 | P0-04 未使用的全局变量 test_pin | 已修复 |
| 2026-03-20 | P0-05 未使用的全局变量 tmp | 已修复 |

---

## 备注

- 共发现 **14** 个问题
- P0 严重问题：**5** 个（已全部修复）
- P1 中等问题：**5** 个
- P2 轻微问题：**4** 个
