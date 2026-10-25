#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "key.h"

uint8_t keynum ;
int main(void)
{
	LED_Init();
	key_Init();
	while(1)
	{
		keynum =key_getnum();
		if(keynum==1)
		{
			LED0_Turn();
		}
		if(keynum==2){
		
			LED2_Turn();
		}
	}

}