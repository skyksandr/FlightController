#ifndef BOARD_HPP
#define BOARD_HPP

#include "stm32f4xx.h"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <misc.h>

#include "sensors.hpp"

const uint8_t MAX_RX_PORTS = 8;

class Board
{
private:
public:
	RxPort *rxPwmPort[MAX_RX_PORTS];

	void timHandler(TIM_TypeDef* timer, uint8_t port);
	void rxPwmPortCallback(uint8_t port, uint16_t value);
};

#endif


