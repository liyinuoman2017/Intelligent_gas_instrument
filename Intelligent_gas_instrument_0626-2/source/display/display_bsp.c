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
#include "display_bsp.h"

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



/***********************************************************************************************************
* @ÃèÊö	: 
***********************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////LCD
LCD_HandleTypeDef LCDHandle;

/*
A1  SG0
A2 	SG1
A3	SG2
A6	SG3
A7 	SG4

B0	SG5
B1 	SG6
B3 	SG7
B4 	SG8
B5 	SG9


A8	CM0
A9	CM1
A10	CM2
B9	CM3
*/
static void Error_Handler(void)
{
  /* Infinite loop */
  while(1)
  {
  }
}

void HAL_LCD_MspInit(LCD_HandleTypeDef *hlcd)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_OscInitTypeDef  RCC_OscInitStruct;
 
	RCC_PeriphCLKInitTypeDef periph_clock; 
	
    if(hlcd->Instance == LCD)
    {
        __HAL_RCC_PWR_CLK_ENABLE();
        
        HAL_PWR_EnableBkUpAccess();
        
        __HAL_RCC_BACKUPRESET_FORCE();
        __HAL_RCC_BACKUPRESET_RELEASE();
		__HAL_RCC_LCD_CLK_ENABLE(); 
		
        /** Enable LCD LSE Clock*/
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
        RCC_OscInitStruct.LSEState = RCC_LSI_ON;
        if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        {
            Error_Handler();
        }
		__HAL_RCC_RTC_CONFIG(RCC_OSCILLATORTYPE_LSI);
		periph_clock.PeriphClockSelection = RCC_PERIPHCLK_RTC;
		periph_clock.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
		HAL_RCCEx_PeriphCLKConfig(&periph_clock);
  

		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
        /** Configure peripheral GPIO*/
        GPIO_InitStruct.Pin         = GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3| GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull        = GPIO_NOPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate   = GPIO_AF1_LCD;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin         = GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_3| GPIO_PIN_4 | GPIO_PIN_5;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
        
        
        GPIO_InitStruct.Pin         = GPIO_PIN_8 | GPIO_PIN_9| GPIO_PIN_10;
        //GPIO_InitStruct.Alternate   = GPIO_AF1_LCD;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin         = GPIO_PIN_9;
        //GPIO_InitStruct.Alternate   = GPIO_AF1_LCD;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  

        GPIO_InitStruct.Pin         = GPIO_PIN_10;
        //GPIO_InitStruct.Alternate   = GPIO_AF1_LCD;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin         = GPIO_PIN_9;
        //GPIO_InitStruct.Alternate   = GPIO_AF1_LCD;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        //LCD_CLK_ENABLE();
    }
}

void lcd_init(void)
{
    LCDHandle.Instance                  = LCD;
    LCDHandle.Init.Prescaler            = LCD_PRESCALER_4;
    LCDHandle.Init.Divider              = LCD_DIVIDER_16;
    LCDHandle.Init.Duty                 = LCD_DUTY_1_4;
    LCDHandle.Init.Bias                 = LCD_BIAS_1_3;
    LCDHandle.Init.VoltageSource        = LCD_VOLTAGESOURCE_INTERNAL;
    LCDHandle.Init.Contrast             = LCD_CONTRASTLEVEL_5;
    LCDHandle.Init.DeadTime             = LCD_DEADTIME_0;
    LCDHandle.Init.PulseOnDuration      = LCD_PULSEONDURATION_7;
    LCDHandle.Init.BlinkMode            = LCD_BLINKMODE_OFF;
    LCDHandle.Init.BlinkFrequency       = LCD_BLINKFREQUENCY_DIV8;
    LCDHandle.Init.MuxSegment           = LCD_MUXSEGMENT_DISABLE;
    /* YR1433 LCD glass need open high drive*/
    LCDHandle.Init.HighDrive            = LCD_HIGHDRIVE_1;
    
    __HAL_LCD_RESET_HANDLE_STATE(&LCDHandle);
    
    HAL_LCD_MspInit(&LCDHandle);
    
    HAL_LCD_Init(&LCDHandle);
    
    HAL_LCD_Clear(&LCDHandle);
}


/***********************************************END*********************************************************/
