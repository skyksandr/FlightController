#ifndef SHIFT_HPP
#define SHIFT_HPP

#include "stm32f4xx.h"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>

class shiftReg {
private:
	uint32_t RCC_AHB1_PeriphGPIOx;
	GPIO_TypeDef* portGPIO;
	uint16_t dataPin;
	uint16_t clockPin;
	uint16_t latchPin;
public:
	shiftReg(uint32_t RCC_GPIOx, GPIO_TypeDef* pGPIO, uint8_t dPin,
			uint8_t cPin, uint8_t lPin) : RCC_AHB1_PeriphGPIOx(RCC_GPIOx), portGPIO(pGPIO), dataPin(dPin), clockPin(cPin), latchPin(lPin)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1_PeriphGPIOx, ENABLE);

		GPIO_InitTypeDef gpioStruct;

		gpioStruct.GPIO_Pin = dataPin | clockPin | latchPin;
		gpioStruct.GPIO_Mode = GPIO_Mode_OUT;
		gpioStruct.GPIO_OType = GPIO_OType_PP;
		gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;

		GPIO_Init(portGPIO, &gpioStruct);
	}
	void set(const uint8_t value) const;
};
#endif
