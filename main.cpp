
#include "stm32f4xx.h"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <misc.h>
#include "lcd.hpp"
#include "stdio.h"
#include "pwm.hpp"

PwmPorts pwmPorts;

int main(void)
{

	LiquidCrystal lcd(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2,
			RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_5, GPIO_Pin_4);

	pwmPorts.hwConfig();
	pwmPorts.init();

	char buffer[20];
	char *bufPtr;
	bufPtr = (char*)buffer;
	int count = 0;

	while(1)
	{
		count++;
		if (count > 500000)
		{
			sprintf(bufPtr, "Ch1: %d Ch2: %d", pwmPorts.read(PWM1), pwmPorts.read(PWM2));
			lcd.setCursor(0,0);
			lcd.clear();
			lcd.print(bufPtr);
			count = 0;
		}
	}
}
