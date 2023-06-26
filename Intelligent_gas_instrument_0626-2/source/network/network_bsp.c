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
#include "stm32l0xx_hal.h"
#include "network_bsp.h"

/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/


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

static void Error_Handler(void)
{
  /* Infinite loop */
  while(1)
  {
  }
}

/***********************************************************************************************************
* @描述	: 
***********************************************************************************************************/
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
/* USART1 init function */
 uint8_t test_buff[20];

void uart_init(void)
{
 
  huart1.Instance        = USART1;
  huart1.Init.BaudRate   = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits   = UART_STOPBITS_1;
  huart1.Init.Parity     = UART_PARITY_NONE;
  huart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  huart1.Init.Mode       = UART_MODE_TX_RX;
  
  if(HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  
	//开启空闲中断
	__HAL_UART_CLEAR_IDLEFLAG(&huart1);
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
	 
	//设置DMA接收缓冲区及长度，接收长度应大于最大报文长度
	HAL_UART_Receive_DMA(&huart1,test_buff,20); 
}
 
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
 
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */
 
  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
   /* Enable DMA1 clock */
	__HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
 
    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel3;
    hdma_usart1_rx.Init.Request = DMA_REQUEST_3;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    //hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }
 
    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);
	
/*
  DmaHandle.Instance                 = DMA1_Channel1;
  DmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
  DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  DmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  DmaHandle.Init.Mode                = DMA_CIRCULAR;
  DmaHandle.Init.Priority            = DMA_PRIORITY_MEDIUM;
  DmaHandle.Init.Request             = DMA_REQUEST_0;	
	*/	

 
//    /* USART1_TX Init */
//    hdma_usart1_tx.Instance = DMA1_Stream1;
//    hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
//    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
//    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
//    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
//    {
//      Error_Handler();
//    }
// 
//    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);
 
    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
 
  /* USER CODE END USART1_MspInit 1 */
  }
}


void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
 
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */
 
  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
 
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
 
    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
 
    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */
 
  /* USER CODE END USART1_MspDeInit 1 */
  }
}

uint8_t test_length=0;
void USART1_IRQHandler(void)
{
    if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)  // 空闲中断标记被置位
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);  // 清除中断标记


		
			test_length = 20 - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
			 
			HAL_UART_DMAStop(&huart1);//停止DMA，为了重新设置DMA发送多少数据
			
			//HAL_UART_Transmit_DMA(&huart1, (uint8_t *)pData, Len);
			HAL_UART_Receive_DMA(&huart1, (uint8_t *)test_buff, 20);		
		
    }
    else 
    {
        HAL_UART_IRQHandler(&huart1);
    }
}


/***********************************************END*********************************************************/
