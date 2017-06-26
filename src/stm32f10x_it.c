#include "stm32f10x.h"

void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	//	|| USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET)
			{
		uint16_t data = USART_ReceiveData(USART1);
		USART_SendData(USART2, data);
		while (!(USART1->SR & USART_FLAG_TXE))
			;
		//	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//	USART_ClearFlag(USART1, USART_IT_RXNE);
		//	USART_ClearFlag(USART1, USART_FLAG_ORE);
	}
}

void USART2_IRQHandler(void) {
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	//|| USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET)
			{
		uint16_t data = USART_ReceiveData(USART2);
		USART_SendData(USART1, data);
		while (!(USART2->SR & USART_FLAG_TXE))
			;
		//	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		//	USART_ClearFlag(USART2, USART_IT_RXNE);
		//	USART_ClearFlag(USART2, USART_FLAG_ORE);
	}
}
