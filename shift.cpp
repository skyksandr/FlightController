#include "stm32f4xx.h"
#include <stm32f4xx_gpio.h>
#include "shift.hpp"

void shiftReg::set(const uint8_t value) const
{
	for (int i = 0; i < 8; i++)
	{
		GPIO_ResetBits(portGPIO, clockPin);
		GPIO_WriteBit(portGPIO, dataPin, (BitAction)((1 << i) & value));
		GPIO_SetBits(portGPIO, clockPin);
	}

	GPIO_SetBits(portGPIO, latchPin);
	GPIO_ResetBits(portGPIO, latchPin);
}
