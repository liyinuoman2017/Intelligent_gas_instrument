/**
*********************************************************************************************************
*                                        
*                                      (c) Copyright 2023-2033
*                                         All Rights Reserved
*
* @File    : 
* @By      : liwei
* @Version : V0.01
* 
*********************************************************************************************************
**/

/*
*********************************************************************************************************
Includes 
*********************************************************************************************************
*/


/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/



#include "stm32l0xx_hal.h"
#include "senser_bsp.h"
/*
*********************************************************************************************************
Typedef
*********************************************************************************************************
*/

/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/

/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/

ADC_HandleTypeDef             AdcHandle;

void DMA1_Channel1_IRQHandler(void)
{

}

static void Error_Handler(void)
{
  /* Infinite loop */
  while(1)
  {
  }
}


/* ADC channel configuration structure declaration */
ADC_ChannelConfTypeDef        sConfig;

/* Converted value declaration */
uint32_t                      aResultDMA[20];

/***********************************************************************************************************
* @√Ë ˆ	: 
***********************************************************************************************************/
ADC_HandleTypeDef adc1;
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef                 GPIO_InitStruct;
  static DMA_HandleTypeDef         DmaHandle;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();	
  /* ADC1 Periph clock enable */
  __HAL_RCC_ADC1_CLK_ENABLE();
  /* Enable DMA1 clock */
  __HAL_RCC_DMA1_CLK_ENABLE();

  
  /*##- 2- Configure peripheral GPIO #########################################*/
  /* ADC3 Channel8 GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
  
  /*##- 3- Configure DMA #####################################################*/ 

  /*********************** Configure DMA parameters ***************************/
  DmaHandle.Instance                 = DMA1_Channel1;
  DmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
  DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  DmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  DmaHandle.Init.Mode                = DMA_CIRCULAR;
  DmaHandle.Init.Priority            = DMA_PRIORITY_MEDIUM;
  DmaHandle.Init.Request             = DMA_REQUEST_0;
  
  /* Deinitialize  & Initialize the DMA for new transfer */
  HAL_DMA_DeInit(&DmaHandle);  
  HAL_DMA_Init(&DmaHandle);
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);
  
  /* NVIC configuration for DMA Input data interrupt */
 // HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
  //HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void adc_init(void)
{
	HAL_ADC_MspInit(&adc1);
  AdcHandle.Instance = ADC1;
  
  AdcHandle.Init.OversamplingMode      = DISABLE;
  
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV1;
  AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
  AdcHandle.Init.LowPowerFrequencyMode = ENABLE;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
    
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.SamplingTime          = ADC_SAMPLETIME_7CYCLES_5;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ContinuousConvMode    = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.DMAContinuousRequests = ENABLE;

  /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  
  /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ### - 3 - Channel configuration ######################################## */
  /* Select Channel 0 to be converted */
  sConfig.Channel = ADC_CHANNEL_4;    
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* Select Channel 0 to be converted */
  sConfig.Channel = ADC_CHANNEL_5;    
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }  
  /* Select Channel 0 to be converted */
  sConfig.Channel = ADC_CHANNEL_10;    
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }  
  /* Select Channel 0 to be converted */
  sConfig.Channel = ADC_CHANNEL_11;    
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }    
  
  /* ### - 4 - Start conversion in DMA mode ################################# */
  if (HAL_ADC_Start_DMA(&AdcHandle, aResultDMA, 20) != HAL_OK)
  {
    Error_Handler();
  }
}


/***********************************************END*********************************************************/
