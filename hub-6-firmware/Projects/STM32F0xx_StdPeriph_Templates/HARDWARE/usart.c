#include "usart.h"
#include "string.h"
#include "stm32f0xx.h"
#include "stdio.h"
#include "crc.h"
#include "main.h"
#include "delay.h"

volatile uint8_t USB_EX1_Flag;
volatile uint8_t USB_EX2_Flag;
volatile uint8_t USB_EX3_Flag;
volatile uint8_t USB_EX4_Flag;
volatile uint8_t USB_EX5_Flag;
volatile uint8_t USB_EX6_Flag;
enum { ST_BIT=0,CMD_BIT,DATA_BIT};
QUEUE_USART_t Q_usart;

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{  
  USART1->TDR = (uint8_t) ch; 
	while((USART1->ISR&0X40)==0);//ѭ������,ֱ���������   
 // USART1->TDR = (uint8_t) ch; 
	USART_ClearFlag(USART1, USART_FLAG_TC);	
	return ch;
}
#endif 


void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
      
        USART_SendData(pUSARTx,ch);
            
        while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET); 
				USART_ClearFlag(USART1, USART_FLAG_TC);
				
}

void Usart_SendString(USART_TypeDef * pUSARTx, uint8_t* p,uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		Usart_SendByte( pUSARTx, *p++);
	}
} 
  
void usart_init(uint32_t bound)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 ,ENABLE);	//ʹ��USART1��GPIOAʱ��
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA,ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ClearITPendingBit(USART1, USART_IT_TC); 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	 USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}


void usart_queue_init(void)
{
	uint8_t i;
	for(i=0;i<Q_usart.rx_len;i++)
	{
		Q_usart.queue[i]=0;
	}
  Q_usart.queue[0] = 0x00;
  Q_usart.count = 0;
	Q_usart.frame_ready=0;
	Q_usart.rx_flag=1;
	Q_usart.rx_len=0;
}
void USART1_IRQHandler(void)             
	{
	uint8_t Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)   //RX Interrupt
		{
			
			Res =USART_ReceiveData(USART1);	
		
			if(Q_usart.rx_flag==1)     //first data flag
			{
				Q_usart.rx_flag=0;
				Q_usart.count=0;
				Q_usart.frame_ready=0;
				Q_usart.rx_len=0;
			}
						
			Q_usart.queue[Q_usart.count]=Res; 
			Q_usart.count++;
			
			if(Q_usart.count>= RXBUF_MAX)
			{
					usart_queue_init();	
			}
     } 
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)    //IDLE interrupt  
	{
	
			USART_ClearFlag(USART1, USART_FLAG_IDLE);
			Q_usart.rx_flag=1;
			Q_usart.frame_ready=1;
			Q_usart.rx_len=Q_usart.count;
			Q_usart.count=0;
	}
}
	
void u485_send_crc(uint32_t crcdata,uint8_t *p,uint8_t len)
{
	U485_OUT;
  Usart_SendString(USART1,p,len);
  Usart_SendByte( USART1,(uint8_t)(crcdata>>24));
  Usart_SendByte( USART1,((uint8_t)(crcdata>>16))&0xff);
	Usart_SendByte( USART1,((uint8_t)(crcdata>>8))&0xff);
	Usart_SendByte( USART1,(uint8_t)(crcdata&0xff));
	printf("\r\n");
	U485_IN;	
}	

uint8_t rcv_tick()
{  
	uint8_t i;
	uint16_t sum=0;
	if(Q_usart.frame_ready==1)
	{		
		Q_usart.frame_ready=0;
		
		if(Q_usart.queue[ST_BIT]!=0x53)
		{
				return 1;
		}
		
		for(i=0;i<(Q_usart.rx_len-1);i++)
		{
			sum+=Q_usart.queue[i];
		}
		if(sum!=Q_usart.queue[Q_usart.rx_len-1])
		{
			return 1;
		}
		
	
		switch(Q_usart.queue[CMD_BIT])
		{
			case 1:
				printf("hello");
				break;
			case 2:
				Usart_SendString(USART1, Q_usart.queue,4);
				switch(Q_usart.queue[2])
				{
					case 1:
						USB_EX1_L;
						USB_EX1_Flag=1;
						break;
					case 2:
						USB_EX2_L;
						USB_EX2_Flag=1;
						break;
					case 3:
						USB_EX3_L;
						USB_EX3_Flag=1;
						break;
					case 4:
						USB_EX4_L;
						USB_EX4_Flag=1;
						break;
					case 5:
						USB_EX5_L;
						USB_EX5_Flag=1;
						break;
					case 6:	
						USB_EX6_L;
						USB_EX6_Flag=1;
						break;
					case 0:
						USB_RESETN_L;
						delay_ms(1);
						USB_RESETN_H;
						break;		
				}		
				break;
			case 3:
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);
					delay_ms(20);
	        GPIO_SetBits(GPIOA,GPIO_Pin_7);
				  Usart_SendString(USART1, Q_usart.queue,4);
					break;
			case 4:
					GPIO_ResetBits(GPIOF,GPIO_Pin_1);
					delay_ms(1000);
					IWDG_Feed();
					delay_ms(1000);
					IWDG_Feed();
					delay_ms(1000);
					IWDG_Feed();
					delay_ms(1000);
					IWDG_Feed();
					delay_ms(1000);
					IWDG_Feed();
					delay_ms(1000);
					IWDG_Feed();
	        GPIO_SetBits(GPIOF,GPIO_Pin_1);
					Usart_SendString(USART1, Q_usart.queue,4);
					break;
		}
		
		for(i=0;i<Q_usart.rx_len;i++)
		{
			Q_usart.queue[i]=0;
		}
		
	}
	return 0;
}

