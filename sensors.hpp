#ifndef SENSORS_HPP
#define SENSORS_HPP

#include "stm32f4xx.h"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <misc.h>

#include "robot.hpp"

const uint16_t PERIOD = 30000;
const uint8_t PRESCALER = 15;

// Абстрактный класс, общий для всех сенсоров
class Sensor
{
protected:
	uint16_t value;
public:
	virtual uint16_t getValue() = 0;
};

// Класс для работы с приемником радио сигнала
class RxPort : public Sensor
{
private:
	// TIM
	uint32_t RCC_Timer;
	TIM_TypeDef* timer;
	uint16_t channel;
	uint16_t timIT;
	uint8_t IRQChannel;
	// GPIO
	uint32_t RCC_GPIO;
	GPIO_TypeDef* gpioPort;
	uint16_t gpioPin;
	uint8_t gpioPinSource;
	uint8_t gpioAF;

	uint16_t rise;
	uint16_t state;

	void gpioInit(uint32_t RCC_Periph, GPIO_TypeDef* gpioPort, uint16_t gpioPin, uint8_t gpioPinSource, uint8_t gpioAF);
	void timerInit(uint32_t RCC_Periph, TIM_TypeDef* timer, uint16_t channel, uint16_t timIT, uint8_t IRQChannel);
	void timICConfig(TIM_TypeDef* tim, uint16_t channel, uint16_t polarity);

	void callback(uint8_t port, uint16_t capture);

public:

	void init();

	void setTimerParameters(uint32_t _RCC_Timer, TIM_TypeDef* _timer, uint16_t _channel, uint16_t _timIT, uint8_t _IRQChannel);
	void setGPIOParameters(uint32_t _RCC_GPIO, GPIO_TypeDef* _gpioPort, uint16_t _gpioPin, uint8_t _gpioPinSource, uint8_t _gpioAF);

	uint16_t getValue();
	void callback(uint16_t capture);
};

// class for ultrasonic sensor
class UltraSonic : public Sensor
{
public:
	uint16_t getValue();
};

#endif
