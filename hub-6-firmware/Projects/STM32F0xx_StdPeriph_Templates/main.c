#include "main.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "wdg.h"
#include "crc.h"
#include "string.h"

void hub_reset_init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
 /* Enable the GPIO_LED Clock */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA,ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	delay_ms(10);

	
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
}

void pc_reset_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF,GPIO_Pin_1);
}

void hub_switch_init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB,ENABLE);

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_9|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	 /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	USB_EX1_L;
	USB_EX2_L;
	USB_EX3_L;
	USB_EX4_L;
	USB_EX5_L;
	USB_EX6_L;
	delay_ms(500);
	USB_EX1_H;
	delay_ms(500);
	USB_EX2_H;
	delay_ms(500);
	USB_EX3_H;
	delay_ms(500);
	USB_EX4_H;
	delay_ms(500);
	USB_EX5_H;
	delay_ms(500);
	USB_EX6_H;

}

int main(void)
{	
	TIM3_Int_Init(4999,47);     //5ms	
	delay_init();
  usart_init(115200);
	LEDInit();
	crc_init();	
	hub_switch_init();
	hub_reset_init();
	pc_reset_init();
	
	IWDG_Config(IWDG_Prescaler_256,500);  //1s 	 

  while(1)
  {		
		if(1)
		{
			rcv_tick();
		}

		if(1)
		{
			IWDG_Feed();
		}
  }
}

