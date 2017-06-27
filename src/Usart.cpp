#include "Usart.h"

void Usart::init() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA |
	RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	/* PA9 USART1_Tx */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //���������TX��
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* PA10 USART1_Rx  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������루RX��
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1, &USART_ClockInitStructure);
	USART_ClockInit(USART2, &USART_ClockInitStructure);

	USART_InitTypeDef USART_InitStructure; //����һ��USART��ʼ���ṹ�壨�̼�����ò�ƺ�ϲ�������ַ������ʼ���豸��
	USART_InitStructure.USART_BaudRate = 38400;  //��ʼ��������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�������ݳ���Ϊ8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //ֹͣλΪ1
	USART_InitStructure.USART_Parity = USART_Parity_No;  //��У��λ
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;  //����������Ϊnone

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //���պͷ���ģʽ����

	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);  //��ʼ��1��
	/* Configure USART2 */
	USART_Init(USART2, &USART_InitStructure);  //��ʼ��2��
	/* Enable the USART1 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);  //��1��
	/* Enable the USART2 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);  //��2��
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	NVIC_Configuration();
}

void Usart::send(USART_TypeDef* USARTx, uint16_t ch) {
	USART_SendData(USARTx, ch);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
		;
}

void Usart::send(USART_TypeDef* USARTx, const char* str) {
	while (*str) {
		USART_SendData(USARTx, *str++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
			;
	};
}

void Usart::NVIC_Configuration(void) {
#ifdef  VECT_TAB_RAM

	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else

	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_Init(&NVIC_InitStructure);
//	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
//	NVIC_Init(&NVIC_InitStructure);

}

