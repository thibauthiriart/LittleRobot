/*
 * main.c:
 * Copyright (c) 2014-2020 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 * use keil5 for arm.
 */
 
#ifndef __cplusplus /* In C++, 'bool', 'true' and 'false' and keywords */
    #define bool _Bool
    #define true 1
    #define TRUE true
    #define false 0
    #define FALSE false
#else
    #ifdef __GNUC__
    /* GNU C++ supports direct inclusion of stdbool.h to provide C99
        compatibility by defining _Bool */
        #define _Bool bool
    #endif
#endif
 
#include "stm32f10x.h"

bool wait_flag=true;
unsigned char c[16];
int num=0;

void Usart1_Send(char *str)
{
	while(*str != '\0')
	{		
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
		USART_SendData(USART1 , *str);
		str++;
	}
}

void Usart1_Config(void)
{		
	GPIO_InitTypeDef GPIO_Uart;
	USART_InitTypeDef USART_Uart;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
	
 	GPIO_Uart.GPIO_Pin = GPIO_Pin_9;
  	GPIO_Uart.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Uart.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_Uart);
	
	GPIO_Uart.GPIO_Pin = GPIO_Pin_10;
	GPIO_Uart.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_Uart);
	
	USART_Uart.USART_BaudRate = 9600;
	USART_Uart.USART_WordLength = USART_WordLength_8b;
	USART_Uart.USART_StopBits = USART_StopBits_1;
	USART_Uart.USART_Parity = USART_Parity_No ;
	USART_Uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Uart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_Uart);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);																	
	USART_Cmd(USART1, ENABLE);
}

void Nvic_Usart1_Config(void)
{
	NVIC_InitTypeDef NVIC_Uart1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	NVIC_Uart1.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Uart1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Uart1.NVIC_IRQChannelSubPriority = 1;
	NVIC_Uart1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Uart1);
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        c[num]=USART1->DR;
        if(c[num-1]=='O'&&c[num]=='K')
        {
            int i;
            wait_flag=false;
            for(i=0;i<15;i++)
                c[i]='\0';
			num=0;
        }
        num++;
        if(num>=15)
            num=0;
    }
}

int main(void)
{	
    Usart1_Config();
    Nvic_Usart1_Config();
	while(1)
	{
        Usart1_Send("#3P500T2000\r\n");
		//Change wait_flag value
        wait_flag=true;
		//Waiting to return OK
        while(wait_flag);
        
        Usart1_Send("#2P1723#3P1500T2000\r\n");
        wait_flag=true;
        while(wait_flag);
        
        Usart1_Send("#3P2500T2000D1555\r\n");
        wait_flag=true;
        while(wait_flag);
        
        Usart1_Send("#3P1500T2000\r\n");
        wait_flag=true;
        while(wait_flag);
		
		//Test for send wrong instruction
		Usart1_Send("#3P2500T9000D9000\r\n");
		
		//Stop wrong instruction
		Usart1_Send("~ST");
		
		//Change right instruction
		Usart1_Send("#2P2500T9000D9000\r\n");
		wait_flag=true;
        while(wait_flag);
		
		//Test for action groups,Group 1 run 3 times
		Usart1_Send("G1F3\r\n");
		wait_flag=true;
        while(wait_flag);
	}
}
