# ShuiZhi2 水质监测系统

基于 STM32H723 的多参数水质监测固件系统。

## 项目简介
ShuiZhi2 是一个单线程嵌入式系统，通过 ADC 和 I2C 接口采集多种环境与水质参数，并以此计算水质状态。系统每 1 秒通过 USART1 串口打印一次传感器数据。

## 支持参数
- **pH 值**: 采集自 ADC1_INP2 (PF11)，量程 0-14。
- **浊度 (Turbidity)**: 采集自 ADC1_INP3 (PA6)，输出单位为 NTU。
- **TDS (总溶解固体)**: 采集自 ADC1_INP4 (PC4)，支持基于 DS18B20 的温度自动校正。
- **水温 (Water Temperature)**: 采集自 DS18B20 传感器 (PB0)。
- **空气温湿度 (AHT20)**: 通过 I2C1 采集。
- **大气压/海拔 (BMP280)**: 通过 I2C1 采集。

## 硬件映射
| 传感器 | 引脚/接口 | 说明 |
| :--- | :--- | :--- |
| pH 传感器 | ADC1_INP2 (PF11) | 采样前重新配置通道 |
| 浊度传感器 | ADC1_INP3 (PA6) | 线性 NTU 映射 |
| TDS 传感器 | ADC1_INP4 (PC4) | 0-1400 ppm，带温度补偿 |
| DS18B20 | PB0 | 单总线模式，开漏输出 |
| AHT20 | I2C1 (0x38) | 温湿度采集 |
| BMP280 | I2C1 (0x77) | 气压与估算海拔 |
| 辅助定时器 | TIM7 | 提供微秒级精确延时 |
| 日志输出 | USART1 | printf 目标接口 |

## 开发环境
- **IDE**: VS Code (推荐) 或 Keil MDK-ARM (兼容)
- **工程管理**: EIDE (Embedded IDE for VS Code)
- **编译工具链**: Arm Compiler 6 (AC6)
- **调试连接**: CMSIS-DAP/OpenOCD

## 构建与烧录 (VS Code EIDE)
在工作区根目录下执行以下任务：
- `build`: 增量编译项目。
- `rebuild`: 重新编译整个工程。
- `flash`: 将固件上传至设备。
- `build and flash`: 编译并烧录。

输出产物位于 `Vscode/build/ShuiZhi2/ShuiZhi2.axf`。

## 代码结构
- `Core/Src`: 核心驱动与逻辑。
    - `main.c`: 外设初始化与主监控循环。
    - `PH.c`/`ZhuoDu.c`/`TDS.c`: 各项参数的具体采集逻辑。
    - `Ds18B20.c`: 单总线驱动实现。
    - `aht20_bmp280.c`: I2C 传感器驱动。
- `Vscode/`: EIDE 工程配置及构建参数。
- `MDK-ARM/`: Keil 启动代码与传统工程文件。

## TDS 校准方法
1. 连接 TDS 探针与 DS18B20 传感器。
2. 使用标准液测得当前未校准 PPM 值。
3. 计算 `K = 标准值 / 测量值`。
4. 调用 `TDS_Set_K_Value(K)` 设置校准系数（范围 0.1 - 10.0）。

## 注意事项
- **精确延时**: TIM7 为 AHT20 和 DS18B20 共有，请勿在其他模块重配。
- **ADC 时序**: 采样时间设置为 ~64.5 cycles，以保证模拟量采集的稳定性。
- **LCD/显示**: 目前仅支持串口日志输出 (`printf`)。

---
**版本**: v2.0 (Water Temperature Edition)
**更新日期**: 2026-01-20
