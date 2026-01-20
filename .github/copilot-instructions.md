## ShuiZhi2 AI 指南
- 概览: STM32H723 (Cube/HAL) 单线程固件，采集 pH、浊度、湿度/温度 (AHT20)、气压/海拔 (BMP280)。主循环在 [Core/Src/main.c](Core/Src/main.c) 初始化外设后每 1 秒读取传感器并经 USART1 打印。
- 硬件映射: ADC1_INP2 (PF11)=pH，ADC1_INP3 (PA6)=浊度；采样前都会重新配置 ADC1。I2C1 连接 AHT20(0x38)/BMP280(0x77)。TIM7 提供微秒延时；UART1 是 printf 目标。
- pH 采集: [Core/Src/PH.c](Core/Src/PH.c) 每次配置 ADC 通道 2，取 7 样本（1 ms 间隔），冒泡排序取中值，16 位/3.3 V 转电压，公式 `-5.7541*V + 16.654`，夹紧 0–14。
- 浊度: [Core/Src/ZhuoDu.c](Core/Src/ZhuoDu.c) 每次配置 ADC 通道 3，单次转换，转电压，线性 NTU 映射 `ntu = voltage*(-865.68)+3291.3`，无滤波；先 `ZhuoDu_Init(&hadc1)` 绑定 ADC。
- AHT20: [Core/Src/aht20_bmp280.c](Core/Src/aht20_bmp280.c) 软复位+初始化，`AHT20_Read_CTdata` 发送测量命令、等待 80 ms、读 6 字节、解析 20 位湿度/温度；主循环把原始值转定点整数。用 TIM7 忙等待微秒。
- BMP280: 同文件读取校准块，配置过采样 (T x16, P x8) 和 IIR 滤波，持有 `bmp280_calib`。`BMP280GetData` 至少 100 ms 间隔，补偿后取最近 5 次压力均值并按固定温度估算海拔。
- 时序: TIM7 由 AHT20 和 `HAL_Delay_Us` ([Core/Src/My_Debug.c](Core/Src/My_Debug.c)) 共享；不要在其他模块重配 TIM7。毫秒延时用 `HAL_Delay`。
- 串口/日志: [Core/Src/My_Debug.c](Core/Src/My_Debug.c) 的 `fputc`/`fgetc` 重定向 USART1，阻塞发送；首选 `printf`。`My_Usart_Send*` 只是轻量包装。
- 时钟/电源: 系统时钟 HSI+PLL1（SYSCLK）与 PLL2（ADC），见 [Core/Src/main.c](Core/Src/main.c)、[Core/Src/adc.c](Core/Src/adc.c)；MPU 默认区域禁空指针访问。
- 构建/烧录: 用 VS Code 任务（根目录 Vscode/）`build`、`rebuild`、`flash`、`build and flash`（EIDE 命令）。工作区根在 `Vscode/`；产物位于 `Vscode/build/ShuiZhi2/`。无独立 README，依赖任务。
- 错误处理: 传感器初始化失败进入 `Error_Handler`（关中断、死循环）。多数读取失败返回上次数据而非阻塞；若加日志注意调用点。
- 约定: 每个测量函数内重新配置 ADC 通道；采样时间 ~64.5 cycles；HAL 轮询超时短（10–100 ms）；沿用现有标定系数。
- 扩展: 复用 I2C1/TIM7 辅助函数，避免直接寄存器延时；新增 ADC1 传感器时确保通道不冲突。保持 `printf` 浮点输出；主循环除 1 秒节奏外尽量非阻塞。
- 关键文件: 传感器驱动 [Core/Src](Core/Src)，引脚/外设 [Core/Src/gpio.c](Core/Src/gpio.c)、[Core/Src/adc.c](Core/Src/adc.c)，启动 [startup_stm32h723xx.s](startup_stm32h723xx.s)，工程配置在 `Vscode/`（EIDE）与 `MDK-ARM/`（Keil）。
- 测试: 无自动化测试；运行 build 任务并观测 USART1 日志。BMP/AHT 变更需保持时间约束（BMP 间隔 >=100 ms，AHT 测量后等待 ~80 ms）。

如需补充（如引脚细节、时钟树、日志规范）请告知。## ShuiZhi2 AI Guide
