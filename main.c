
#include "stm32f4xx.h"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <misc.h>

void timerInit();
void SysTick_Handler();
volatile unsigned long millis;
unsigned long lastMillis = 0;

class shiftReg {
private:
	uint32_t RCC_AHB1_PeriphGPIOx;
	GPIO_TypeDef* portGPIO;
	uint8_t dataPin;
	uint8_t clockPin;
	uint8_t latchPin;
public:
	shiftReg(uint32_t RCC_GPIOx,
			GPIO_TypeDef* pGPIO, uint8_t dPin,
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
	void set(uint8_t value);
};

void shiftReg::set(uint8_t value)
{
	GPIO_ResetBits(portGPIO, latchPin);

	for (int i = 0; i < 8; i++)
	{
		GPIO_ResetBits(portGPIO, clockPin);
		GPIO_WriteBit(portGPIO, dataPin, (1 << i) & value);
		GPIO_SetBits(portGPIO, clockPin);
	}

	GPIO_SetBits(portGPIO, latchPin);
}

int main(void)
{

	SysTick_Config(SystemCoreClock / 10500);

	shiftReg shiftRegister(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_pin_0, GPIO_Pin_1, GPIO_Pin_2);

	if (millis - lastMillis > 1000)
	{
		lastMillis = millis;
		uint8_t value = millis / 1000;
		shiftRegister.set(value);
	}

}

void SysTick_Handler()
{
	millis++;
}
