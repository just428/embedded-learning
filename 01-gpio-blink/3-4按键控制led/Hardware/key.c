#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
}
uint8_t key_getnum(void)
{
	uint8_t keynum =0;
	
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1 )==0)
	{
	Delay_ms(20);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1 )==0);
	Delay_ms(20);
	keynum =1;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11 )==0)
	{
	Delay_ms(20);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11 )==0);
	Delay_ms(20);
	keynum =2;
	}
	
	return  keynum;
}