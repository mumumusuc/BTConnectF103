#ifndef USART_CPP_
#define USART_CPP_

#include "Usart.h"

GPIO_InitTypeDef GPIO_InitStructure;
USART_ClockInitTypeDef USART_ClockInitStructure;

void Usart::init() {
	/* PA9 USART1_Tx */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //推挽输出（TX）
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* PA10 USART1_Rx  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入（RX）
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1, &USART_ClockInitStructure);

	USART_InitTypeDef USART_InitStructure; //声明一个USART初始化结构体（固件库中貌似很喜欢用这种风格来初始化设备）
	USART_InitStructure.USART_BaudRate = 115200;  //初始化波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //设置数据长度为8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //停止位为1
	USART_InitStructure.USART_Parity = USART_Parity_No;  //无校验位
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;  //数据流控制为none

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收和发送模式都打开
	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);  //初始化1口
	/* Configure USART2 */
//	USART_Init(USART2, &USART_InitStructure);  //初始化2口
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);  //打开1口
	/* Enable the USART2 */
//	USART_Cmd(USART2, ENABLE);  //打开2口
}

void Usart::send(unsigned char ch) {
	USART_SendData(USART1, ch);   // USART1 可以换成 USART2 等
	while (!(USART1->SR & USART_FLAG_TXE))
		;
}
