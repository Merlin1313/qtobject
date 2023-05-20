#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;

void CountSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //上拉输入  默认高电平输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	//AFIO   PB14 中断引脚
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;         //14号线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;         //使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   //中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   //下降 沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	
	//NVIC  
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //NVIC 分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;   //NVIC  中断 	通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;       //使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;     //相应
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}

void EXTI15_10_IRQHandler(void)      //中断函数
{  
	if (EXTI_GetITStatus(EXTI_Line14) == SET)    //标志位判断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)   //程序
		{
			CountSensor_Count ++;                           //程序
		}
		EXTI_ClearITPendingBit(EXTI_Line14);      //清楚标志位，不然一直申请中断
	}
}
