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
#include "task_scheduler.h"

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
uint8_t null_buff[5];
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
/*声明业务*/
extern application_template_t display_operation;
extern application_template_t io_manage_operation;
extern application_template_t network_operation;
extern application_template_t senser_operation;

/*关联业务*/
/*业务总数量 注意：数量不能溢出否则程序跑飞*/
#define SCHEDULER_NUM  4
const application_template_t* user_business[SCHEDULER_NUM]=
{
	&display_operation,
	&io_manage_operation,
	&network_operation,
	&senser_operation,	
};

/***********************************************************************************************************
* @描述	: 
***********************************************************************************************************/
uint8_t task_initialization(void)
{
	uint8_t i;
	for(i =0; i < SCHEDULER_NUM; i++)
	{
		user_business[i]->initialization(null_buff);
	}
	return 0;
}
/***********************************************************************************************************
 * @描述	: 后台持续运行业务
***********************************************************************************************************/	
uint8_t task_run(void)
{
	uint8_t i;
	/*依次执行所有业务连续运行函数*/
	for(i =0; i < SCHEDULER_NUM; i++)
	{
		user_business[i]->run(null_buff);
	}	
	return 0;
}
/***********************************************************************************************************
* @描述	: 业务中断 1MS中断
***********************************************************************************************************/
uint8_t task_interrput(void)
{
	uint8_t i;
	/*依次执行所有业务中断*/
	for(i =0; i < SCHEDULER_NUM; i++)
	{
		user_business[i]->interrput(null_buff);
	}	
	return 0;	
}
/***********************************************END*********************************************************/
