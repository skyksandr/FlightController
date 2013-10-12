#include "stm32f4xx.h"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <misc.h>

#include "robot.hpp"
#include "board.hpp"
#include "sensors.hpp"

extern Robot robot;

void Board::timHandler(TIM_TypeDef* timer, uint8_t port)
{
	if (TIM_GetITStatus(timer, TIM_IT_CC1) == SET)
	{
		timer->SR = (uint16_t)~TIM_IT_CC1;
		rxPwmPortCallback(port, timer->CCR1);
	} else if (TIM_GetITStatus(timer, TIM_IT_CC2) == SET)
	{
		port += 1;
		timer->SR = (uint16_t)~TIM_IT_CC2;
		rxPwmPortCallback(port, timer->CCR2);
	} else if (TIM_GetITStatus(timer, TIM_IT_CC3) == SET)
	{
		port += 2;
		timer->SR = (uint16_t)~TIM_IT_CC3;
		rxPwmPortCallback(port, timer->CCR3);
	} else if (TIM_GetITStatus(timer, TIM_IT_CC4) == SET)
	{
		port += 3;
		timer->SR = (uint16_t)~TIM_IT_CC1;
		rxPwmPortCallback(port, timer->CCR4);
	}
}

void Board::rxPwmPortCallback(uint8_t port, uint16_t value)
{
	rxPwmPort[port] -> callback(value);
}
