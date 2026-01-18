#ifndef __ZHUODU_H
#define __ZHUODU_H

#include "main.h"

// 浊度传感器初始化
void ZhuoDu_Init(ADC_HandleTypeDef *hadc);

// 读取浊度电压值
float ZhuoDu_Read_Voltage(void);

// 读取浊度值 (NTU)
float ZhuoDu_Read_NTU(void);

#endif
