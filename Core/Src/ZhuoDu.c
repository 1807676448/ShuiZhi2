#include "ZhuoDu.h"

static ADC_HandleTypeDef *zhuodu_adc;

void ZhuoDu_Init(ADC_HandleTypeDef *hadc)
{
    zhuodu_adc = hadc;
}

static void ZhuoDu_Channel_Config(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    sConfig.OffsetSignedSaturation = DISABLE;
    
    if (HAL_ADC_ConfigChannel(zhuodu_adc, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

float ZhuoDu_Read_Voltage(void)
{
    uint32_t adc_val = 0;
    
    ZhuoDu_Channel_Config();
    
    HAL_ADC_Start(zhuodu_adc);
    if (HAL_ADC_PollForConversion(zhuodu_adc, 100) == HAL_OK)
    {
        adc_val = HAL_ADC_GetValue(zhuodu_adc);
    }
    HAL_ADC_Stop(zhuodu_adc);
    
    return (float)adc_val * 3.3f / 65535.0f;
}

float ZhuoDu_Read_NTU(void)
{
    float voltage = ZhuoDu_Read_Voltage();
    
    float ntu = voltage*(-865.68f)+3291.3f;  

    return ntu;
}
