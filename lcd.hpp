#ifndef LCD_HPP
#define LCD_HPP

#include "stm32f4xx.h"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include "shift.hpp"

class LiquidCrystal {
private:
	uint32_t RCC_AHB1_PeriphGPIOx;
	GPIO_TypeDef* portGPIO;
	uint16_t rsPin;
	uint16_t enPin;
	shiftReg shiftRegister;
	void writeData(uint16_t data);
	void writeCmd(uint16_t cmd);
	void printChr(const char* chr);
public:
	LiquidCrystal(uint32_t RCC_Sh_GPIOx, GPIO_TypeDef* sh_pGPIO, uint8_t dPin, uint8_t cPin, uint8_t lPin,
				uint32_t RCC_GPIOx, GPIO_TypeDef* pGPIO, uint8_t rPin, uint8_t ePin) : RCC_AHB1_PeriphGPIOx(RCC_GPIOx), portGPIO(pGPIO), rsPin(rPin), enPin(ePin), shiftRegister(RCC_Sh_GPIOx, sh_pGPIO, dPin, cPin, lPin)
	{
//		shiftReg shift(RCC_Sh_GPIOx, sh_pGPIO, dPin, cPin, lPin);
//		shiftRegister = shift;

		RCC_AHB1PeriphClockCmd(RCC_AHB1_PeriphGPIOx, ENABLE);

		GPIO_InitTypeDef gpioStruct;

		gpioStruct.GPIO_Pin = enPin | rsPin;
		gpioStruct.GPIO_Mode = GPIO_Mode_OUT;
		gpioStruct.GPIO_OType = GPIO_OType_PP;
		gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;

		GPIO_Init(portGPIO, &gpioStruct);

		init();
	}
	void init();
	void clear();
	void setCursor(uint8_t line, uint8_t pos);
	void blink();
	void noBlink();
	void print(const char* str);

};

#endif
