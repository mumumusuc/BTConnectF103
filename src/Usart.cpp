#ifndef USART_CPP_
#define USART_CPP_

#include "Usart.h"

GPIO_InitTypeDef GPIO_InitStructure;
USART_ClockInitTypeDef USART_ClockInitStructure;

void Usart::init() {
	/* PA9 USART1_Tx */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //���������TX��
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* PA10 USART1_Rx  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������루RX��
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1, &USART_ClockInitStructure);

	USART_InitTypeDef USART_InitStructure; //����һ��USART��ʼ���ṹ�壨�̼�����ò�ƺ�ϲ�������ַ������ʼ���豸��
	USART_InitStructure.USART_BaudRate = 115200;  //��ʼ��������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�������ݳ���Ϊ8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //ֹͣλΪ1
	USART_InitStructure.USART_Parity = USART_Parity_No;  //��У��λ
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;  //����������Ϊnone

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //���պͷ���ģʽ����
	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);  //��ʼ��1��
	/* Configure USART2 */
//	USART_Init(USART2, &USART_InitStructure);  //��ʼ��2��
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);  //��1��
	/* Enable the USART2 */
//	USART_Cmd(USART2, ENABLE);  //��2��
}

void Usart::send(unsigned char ch) {
	USART_SendData(USART1, ch);   // USART1 ���Ի��� USART2 ��
	while (!(USART1->SR & USART_FLAG_TXE))
		;
}
