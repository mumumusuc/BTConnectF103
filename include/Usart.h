#ifndef USART_H_
#define USART_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"


class Usart {
public:
	Usart() = default;

	void init();

	void send(unsigned char ch);
};
