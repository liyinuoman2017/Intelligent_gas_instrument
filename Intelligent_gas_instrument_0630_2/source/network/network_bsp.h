/**
*********************************************************************************************************
*                                        
*                                      (c) Copyright 2023-2032
*                                         All Rights Reserved
*
* @File    : 
* @By      : liwei
* @Version : V0.01
* 
*********************************************************************************************************
**/
#ifndef __NETWORK_BSP_H__
#define __NETWORK_BSP_H__

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
#define RX_BUFF_LENGTH (100)

/*
*********************************************************************************************************
Typedef
*********************************************************************************************************
*/
typedef struct read_frame_def
{
uint8_t read_buff[RX_BUFF_LENGTH];
uint8_t read_length;

}read_frame_t;

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

void uart_init(void);
void send_nb_data(uint8_t *buff,uint8_t length);
read_frame_t *read_nb_data(void);
#endif
/***********************************************END*****************************************************/
