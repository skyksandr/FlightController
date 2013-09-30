#ifndef PWM_HPP
#define PWM_HPP

#include "stm32f4xx.h"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <misc.h>

const uint8_t MAX_PORTS = 2;
const uint16_t PERIOD = 2000;
const uint8_t PRESCALER = 15; // Timer freq = 16 Mhz

enum { PWM1 = 0, PWM2 = 1, PWM3 = 2, PWM4 = 3, PWM5 = 4, PWM6 = 5, PWM7 = 6, PWM8 = 7 };

struct Hardware {
	// timers
	uint32_t RCC_Timer;
	TIM_TypeDef* timer;
	uint16_t channel;
	uint16_t timIT;
	uint8_t IRQChannel;

	// gpio
	uint32_t RCC_GPIO;
	GPIO_TypeDef* gpioPort;
	uint16_t gpioPin;
	uint8_t gpioPinSource;
	uint8_t gpioAF;
};

class PwmPorts
{
private:
	// values
	uint16_t rise[MAX_PORTS];
	uint16_t state[MAX_PORTS];
	uint16_t capture[MAX_PORTS];

	Hardware pwmHardware[MAX_PORTS];

	//void hwConfig();
	void gpioInit(uint32_t RCC_Periph, GPIO_TypeDef* gpioPort, uint16_t gpioPin, uint8_t gpioPinSource, uint8_t gpioAF);
	void timerInit(uint32_t RCC_Periph, TIM_TypeDef* timer, uint16_t channel, uint16_t timIT, uint8_t IRQChannel);
	void timICConfig(TIM_TypeDef* tim, uint16_t channel, uint16_t polarity);

	void callback(uint8_t port, uint16_t capture);

public:
	PwmPorts();
	void init();
	void hwConfig();

	void timHandler(TIM_TypeDef* tim, uint8_t port);
	uint16_t read(uint8_t port);
	void transmit(uint8_t port, uint16_t value);
};

#endif
