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
#include "string.h"
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
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
/* USART1 init function */
uint8_t rx_buff[RX_BUFF_LENGTH+200];
uint8_t rx_length;
uint8_t rx_test[RX_BUFF_LENGTH];
read_frame_t nb_read=
{
	.read_length =0,
};
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
void Error_Handler(void);

/***********************************************************************************************************
* @描述	: 
***********************************************************************************************************/
read_frame_t *read_nb_data(void)
{
	return &nb_read;
}
/***********************************************************************************************************
* @描述	: 
***********************************************************************************************************/
void send_nb_data(uint8_t *buff,uint8_t length)
{
	HAL_UART_Transmit_IT(&huart1, buff,length);
}
/***********************************************************************************************************
* @描述	: 
***********************************************************************************************************/
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
	HAL_UART_Receive_DMA(&huart1,rx_buff,RX_BUFF_LENGTH); 
}
/***********************************************************************************************************
* @描述	: 
***********************************************************************************************************/ 
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
 
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
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
	
    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

  }
}
/***********************************************************************************************************
* @描述	: 
***********************************************************************************************************/
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
 
  if(uartHandle->Instance==USART1)
  {
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
  }
}
/***********************************************************************************************************
* @描述	: 
***********************************************************************************************************/

void USART1_IRQHandler(void)
{
    if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)  // 空闲中断标记被置位
    {
		// 清除中断标记
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);  
		// 清除中断标记
		__HAL_UART_CLEAR_IDLEFLAG(&huart1); 		
		//停止DMA
		HAL_UART_DMAStop(&huart1);
		//读取数据长度
		nb_read.read_length = RX_BUFF_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		//清空数据
		memset(nb_read.read_buff, 0, RX_BUFF_LENGTH);		
		//拷贝数据
		memcpy(nb_read.read_buff, rx_buff, nb_read.read_length);
		memcpy(rx_test, rx_buff, nb_read.read_length);
		
		memset(rx_buff, 0, RX_BUFF_LENGTH);
		
		HAL_UART_DMAResume(&huart1);
		//重新设置DMA
		HAL_UART_Receive_DMA(&huart1, (uint8_t *)rx_buff, RX_BUFF_LENGTH);		

    }
    else 
    {
        HAL_UART_IRQHandler(&huart1);
    }
}


/***********************************************END*********************************************************/
