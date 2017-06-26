

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"


class Usart {
public:
	Usart() = default;

	void init();

	void send(USART_TypeDef*,uint16_t);

	void send(USART_TypeDef*,const char*);

private:
	void NVIC_Configuration(void);
} ;

