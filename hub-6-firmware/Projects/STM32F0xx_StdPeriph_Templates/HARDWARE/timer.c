#include "timer.h"
#include "main.h"
#include "usart.h"

void TIM3_Int_Init(uint16_t arr,uint16_t psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;  //先占优先级0级

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}


void TIM3_IRQHandler(void)   //TIM3中断
{
	static uint8_t i;
	static uint8_t usb_cnt_1,usb_cnt_2,usb_cnt_3,usb_cnt_4,usb_cnt_5,usb_cnt_6;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 	
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		i++;
		if(i==200)
		{
			i=0;
			GPIOA->ODR ^= GPIO_Pin_1;
		}
	
		if(USB_EX1_Flag==1)
		{
			usb_cnt_1++;
			if(usb_cnt_1==100)
			{
				usb_cnt_1=0;
				USB_EX1_Flag=0;
				USB_EX1_H;
			}
		}
		
		if(USB_EX2_Flag==1)
		{
			usb_cnt_2++;
			if(usb_cnt_2==100)
			{
				usb_cnt_2=0;
				USB_EX2_Flag=0;
				USB_EX2_H;
			}
		}
		
		if(USB_EX3_Flag==1)
		{
			usb_cnt_3++;
			if(usb_cnt_3==100)
			{
				usb_cnt_3=0;
				USB_EX3_Flag=0;
				USB_EX3_H;
			}
		}
		
		if(USB_EX4_Flag==1)
		{
			usb_cnt_4++;
			if(usb_cnt_4==100)
			{
				usb_cnt_4=0;
				USB_EX4_Flag=0;
				USB_EX4_H;
			}
		}
		
		if(USB_EX5_Flag==1)
		{
			usb_cnt_5++;
			if(usb_cnt_5==100)
			{
				usb_cnt_5=0;
				USB_EX5_Flag=0;
				USB_EX5_H;
			}
		}
		
		if(USB_EX6_Flag==1)
		{
			usb_cnt_6++;
			if(usb_cnt_6==100)
			{
				usb_cnt_6=0;
				USB_EX6_Flag=0;
				USB_EX6_H;
			}
		}
		
	}
}
