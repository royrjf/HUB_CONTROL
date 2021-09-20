
#ifndef __MAIN_H
#define __MAIN_H
#include "stm32f0xx.h"

#define USB_EX1_H GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define USB_EX1_L GPIO_ResetBits(GPIOA,GPIO_Pin_0)

#define USB_EX2_H GPIO_SetBits(GPIOA,GPIO_Pin_10)
#define USB_EX2_L GPIO_ResetBits(GPIOA,GPIO_Pin_10)

#define USB_EX3_H GPIO_SetBits(GPIOA,GPIO_Pin_9)
#define USB_EX3_L GPIO_ResetBits(GPIOA,GPIO_Pin_9)




#define USB_EX4_H GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define USB_EX4_L GPIO_ResetBits(GPIOB,GPIO_Pin_1)

#define USB_EX5_H GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define USB_EX5_L GPIO_ResetBits(GPIOA,GPIO_Pin_6)

#define USB_EX6_H GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define USB_EX6_L GPIO_ResetBits(GPIOA,GPIO_Pin_5)

#define USB_RESETN_L GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define USB_RESETN_H GPIO_SetBits(GPIOA,GPIO_Pin_7)

#endif /* __MAIN_H */

