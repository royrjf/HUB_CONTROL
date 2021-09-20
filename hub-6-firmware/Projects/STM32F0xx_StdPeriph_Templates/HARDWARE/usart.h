#ifndef __U485_H
#define __U485_H
#include "stm32f0xx.h"


#define U485_OUT		GPIO_SetBits (GPIOA, GPIO_Pin_11);
#define U485_IN	GPIO_ResetBits (GPIOA, GPIO_Pin_11);

#define RXBUF_MAX 100
typedef struct 
{
    uint8_t queue[RXBUF_MAX];
    volatile uint8_t count;    
		volatile uint8_t rx_flag;
		volatile uint8_t rx_len;  
    volatile uint8_t frame_ready; // frame ready flag 
} QUEUE_USART_t;

extern QUEUE_USART_t Q_usart;

extern volatile uint8_t EX_Flag;

extern void usart_init(uint32_t bound);
extern void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
extern uint8_t rcv_tick(void);

extern volatile uint8_t USB_EX1_Flag;
extern volatile uint8_t USB_EX2_Flag;
extern volatile uint8_t USB_EX3_Flag;
extern volatile uint8_t USB_EX4_Flag;
extern volatile uint8_t USB_EX5_Flag;
extern volatile uint8_t USB_EX6_Flag;
#endif
