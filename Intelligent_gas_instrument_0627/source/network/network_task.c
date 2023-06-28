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
#include "user_type.h"
#include "network_task.h"
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
uint16_t delay=0;
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
static uint8_t read(uint8_t* stream);
static uint8_t write(uint8_t* stream);
static uint8_t initialization(uint8_t* stream);
static uint8_t interrput(uint8_t* stream);
static uint8_t run(uint8_t* stream);

const application_template_t network_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
	.run=&run,
};

static void nb_run(void);
/***********************************************************************************************************
* @ÃèÊö	: 
***********************************************************************************************************/
static uint8_t read(uint8_t* stream)
{

	return 0;
}
/***********************************************************************************************************
* @ÃèÊö	: 
***********************************************************************************************************/
static uint8_t write(uint8_t* stream)
{

	return 0;
}
/***********************************************************************************************************
* @ÃèÊö	: 
***********************************************************************************************************/
static uint8_t initialization(uint8_t* stream)
{

	uart_init();
	return 0;
}
/***********************************************************************************************************
* @ÃèÊö	: 
***********************************************************************************************************/
static uint8_t interrput(uint8_t* stream)
{
	if(delay >0)
		delay--;
	return 0;
}
/***********************************************************************************************************
* @ÃèÊö	: 
***********************************************************************************************************/
static uint8_t run(uint8_t* stream)
{
	nb_run();	
	return 0;
}


/***********************************************************************************************************
* @ÃèÊö	: 
***********************************************************************************************************/

typedef void (*callback_t)(void);
uint8_t nb_main_state = 0;
uint8_t nb_sub_state = 0;
read_frame_t *nb_data;
#include "string.h"

void test_at(void)
{
	if(nb_sub_state == 0)
	{
		send_nb_data("AT\r\n",sizeof("AT\r\n"));
		
		nb_sub_state =1;
		delay =500;
	}
	else if(nb_sub_state == 1)
	{
		if(delay ==0)
		{
			nb_sub_state =0;
		}
		nb_data =read_nb_data();
		if(nb_data->read_length >0)
		{
			
			if( ( strstr( (char*)nb_data->read_buff ,"OK"  ) ) != NULL )  /*ÅÐ¶Ï·¢²¼ÃûÊÇ·ñÎªethernet*/
			{
				nb_sub_state=0;
				nb_main_state = 1;
			}
			
			nb_data->read_length = 0;
			memset(nb_data->read_buff, 0, RX_BUFF_LENGTH);
		}
	}
}
	
void stop_sleep(void)
{
	if(nb_sub_state == 0)
	{
		send_nb_data("AT+QSCLK=0\r\n",sizeof("AT+QSCLK=0\r\n"));
		
		nb_sub_state =1;
		delay =500;
	}
	else if(nb_sub_state == 1)
	{
		if(delay ==0)
		{
			nb_sub_state =0;
		}
		nb_data =read_nb_data();
		if(nb_data->read_length >0)
		{
			if( ( strstr((char*)nb_data->read_buff ,"OK" ) ) != NULL )  /*ÅÐ¶Ï·¢²¼ÃûÊÇ·ñÎªethernet*/
			{
				nb_sub_state=0;
				nb_main_state = 2;
			}
			nb_data->read_length = 0;
			memset(nb_data->read_buff, 0, RX_BUFF_LENGTH);
		}
	}
}

void detection_signal(void)
{
	if(nb_sub_state == 0)
	{
		send_nb_data("AT+CSQ=?\r\n",sizeof("AT+CSQ=?\r\n"));
		
		nb_sub_state =1;
		delay =500;
	}
	else if(nb_sub_state == 1)
	{
		if(delay ==0)
		{
			nb_sub_state =0;
		}
		nb_data =read_nb_data();
		if(nb_data->read_length >0)
		{
			if( ( strstr((char*)nb_data->read_buff ,"OK" ) ) != NULL )  /*ÅÐ¶Ï·¢²¼ÃûÊÇ·ñÎªethernet*/
			{
				nb_sub_state=0;
				nb_main_state = 3;
			}
			nb_data->read_length = 0;
			memset(nb_data->read_buff, 0, RX_BUFF_LENGTH);
		}
	}
	
}

void connect_to_server(void)
{
	static uint8_t retry_num =0;
	if(nb_sub_state == 0)
	{
		send_nb_data("AT+QMTOPEN=0,\"47.109.92.229\",1883\r\n",sizeof("AT+QMTOPEN=0,\"47.109.92.229\",1883\r\n"));
		
		nb_sub_state =1;
		delay =5000;
		retry_num++;
		if(retry_num> 3)
		{
			retry_num =0;
			
			send_nb_data("AT+QRST=1\r\n",sizeof("AT+QRST=1\r\n"));
			nb_sub_state=0;
			nb_main_state = 0;
		}
	}
	else if(nb_sub_state == 1)
	{
		if(delay ==0)
		{
			nb_sub_state =0;
		}
		nb_data =read_nb_data();
		if(nb_data->read_length >0)
		{
			
			if( ( strstr((char*)nb_data->read_buff ,"+QMTOPEN: 0,0" ) ) != NULL )  /*ÅÐ¶Ï·¢²¼ÃûÊÇ·ñÎªethernet*/
			{
				nb_sub_state=0;
				nb_main_state = 4;
			}
			nb_data->read_length =0;
			memset(nb_data->read_buff, 0, RX_BUFF_LENGTH);
		}
	}
}
void connect_to_mqtt(void)
{
	
}
#define NB_CALLBACK_MAX 5
const callback_t nb_handle[NB_CALLBACK_MAX] = 
{
	test_at,
	stop_sleep,
	detection_signal,
	connect_to_server,
	connect_to_mqtt,
};

static void nb_run(void)
{
	if(nb_main_state < NB_CALLBACK_MAX)
	{
		nb_handle[nb_main_state]();
	}
	else
		nb_handle[0]();
}
/***********************************************END*********************************************************/
