/*
 * This file is part of the 碌OS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "Timer.h"
#include "BlinkLed.h"
#include "Usart.h"
#include <string>
#include <cstring>
#include "stm32f10x_adc.h"

namespace {
// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
constexpr Timer::ticks_t BLINK_ON_TICKS = Timer::FREQUENCY_HZ * 3 / 4;
constexpr Timer::ticks_t BLINK_OFF_TICKS = Timer::FREQUENCY_HZ - BLINK_ON_TICKS;
}

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

Usart usart;
std::string str = "hello!\n";

int main(int argc, char* argv[]) {

	Timer timer;
	timer.start();

	BlinkLed blinkLed;

	// Perform all necessary initialisations for the LED.
	blinkLed.powerUp();

	uint32_t seconds = 0;

	usart.init();
	GPIO_InitTypeDef m_GPIO_InitTypeDef;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // 打开ADC1的APB2时钟
	m_GPIO_InitTypeDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	m_GPIO_InitTypeDef.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &m_GPIO_InitTypeDef);               //GPIO初始化

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;       //ADC工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  //ADC工作在单次模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	ADC_Init(ADC1, &ADC_InitStructure);

//	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);               //开启转换结束中断
	ADC_Cmd(ADC1, ENABLE);            //使能ADC1转换器
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1))	;
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));
	usart.send(USART1, std::string("ADC1 init OK\n\r").data());
	char buff[12];
	uint16_t ADCConvertedValue;
	// Infinite loop
	while (1) {
		blinkLed.turnOn();
//		timer.sleep(seconds == 0 ? Timer::FREQUENCY_HZ : BLINK_ON_TICKS);

		ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1,ADC_SampleTime_71Cycles5);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);      //使能ADC1的软件转换启动功能
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)	; //等待转换结束
		ADCConvertedValue = ADC_GetConversionValue(ADC1);
		memset(buff,sizeof(char)*12,0);
		sprintf(buff, "%04d", ADCConvertedValue);
		buff[4] = ',';

		blinkLed.turnOff();
//		timer.sleep(BLINK_OFF_TICKS);
//		++seconds;

		ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1,ADC_SampleTime_71Cycles5);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);      //使能ADC1的软件转换启动功能
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //等待转换结束
		ADCConvertedValue = ADC_GetConversionValue(ADC1);
		sprintf(buff+5, "%04d", ADCConvertedValue);
		buff[9] = '\r';
		buff[10] = '\n';
		buff[11] = '\0';
		usart.send(USART2, buff);
	}
	// Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
