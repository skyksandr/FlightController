#include "stm32f4xx.h"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <misc.h>
#include "pwm.hpp"

extern PwmPorts pwmPorts;

PwmPorts::PwmPorts()
{
	hwConfig();
	init();
}

void PwmPorts::init()
{
	for (uint8_t i = 0; i < MAX_PORTS; i++)
	{
		gpioInit(pwmHardware[i].RCC_GPIO,
				pwmHardware[i].gpioPort,
				pwmHardware[i].gpioPin,
				pwmHardware[i].gpioPinSource,
				pwmHardware[i].gpioAF);

		timerInit(pwmHardware[i].RCC_Timer,
				pwmHardware[i].timer,
				pwmHardware[i].channel,
				pwmHardware[i].timIT,
				pwmHardware[i].IRQChannel);
	}
}

void PwmPorts::hwConfig()
{
	// PWM1
	pwmHardware[PWM1].RCC_GPIO = RCC_AHB1Periph_GPIOA;
	pwmHardware[PWM1].gpioPort = GPIOA;
	pwmHardware[PWM1].gpioPin = GPIO_Pin_6;
	pwmHardware[PWM1].gpioPinSource = GPIO_PinSource6;
	pwmHardware[PWM1].gpioAF = GPIO_AF_TIM3;

	pwmHardware[PWM1].RCC_Timer = RCC_APB1Periph_TIM3;
	pwmHardware[PWM1].timer = TIM3;
	pwmHardware[PWM1].channel = TIM_Channel_1;
	pwmHardware[PWM1].timIT = TIM_IT_CC1;
	pwmHardware[PWM1].IRQChannel = TIM3_IRQn;

	// PWM2
	pwmHardware[PWM2].RCC_GPIO = RCC_AHB1Periph_GPIOA;
	pwmHardware[PWM2].gpioPort = GPIOA;
	pwmHardware[PWM2].gpioPin = GPIO_Pin_7;
	pwmHardware[PWM2].gpioPinSource = GPIO_PinSource7;
	pwmHardware[PWM2].gpioAF = GPIO_AF_TIM3;

	pwmHardware[PWM2].RCC_Timer = RCC_APB1Periph_TIM3;
	pwmHardware[PWM2].timer = TIM3;
	pwmHardware[PWM2].channel = TIM_Channel_2;
	pwmHardware[PWM2].timIT = TIM_IT_CC2;
	pwmHardware[PWM2].IRQChannel = TIM3_IRQn;

}

void PwmPorts::gpioInit(uint32_t RCC_Periph, GPIO_TypeDef* gpioPort, uint16_t gpioPin, uint8_t gpioPinSource, uint8_t gpioAF)
{

	RCC_AHB1PeriphClockCmd(RCC_Periph, ENABLE);

	GPIO_InitTypeDef gpioStruct;
	gpioStruct.GPIO_Mode = GPIO_Mode_AF;
	gpioStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;
	gpioStruct.GPIO_Pin = gpioPin;

	GPIO_Init(gpioPort, &gpioStruct);
	GPIO_PinAFConfig(gpioPort, gpioPinSource, gpioAF);

}

void PwmPorts::timerInit(uint32_t RCC_Periph, TIM_TypeDef* timer, uint16_t channel, uint16_t timIT, uint8_t IRQChannel)
{
	RCC_APB1PeriphClockCmd(RCC_Periph, ENABLE);

	TIM_TimeBaseInitTypeDef base_timer;
	TIM_TimeBaseStructInit(&base_timer);
	base_timer.TIM_Prescaler = PRESCALER;
	base_timer.TIM_Period = PERIOD;
	base_timer.TIM_ClockDivision = 0;
	base_timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(timer, &base_timer);

	timICConfig(timer, channel, TIM_ICPolarity_Rising);

	// TIM enable counter
  	TIM_Cmd(timer, ENABLE);

	// Enable the CC2 Interrupt Request
	TIM_ITConfig(timer, timIT, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void PwmPorts::timICConfig(TIM_TypeDef* tim, uint16_t channel, uint16_t polarity)
{
	TIM_ICInitTypeDef timICStruct;

	timICStruct.TIM_Channel = channel;
	timICStruct.TIM_ICPolarity = polarity;
	timICStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	timICStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	timICStruct.TIM_ICFilter = 0x0;

	TIM_ICInit(tim, &timICStruct);
}

void PwmPorts::callback(uint8_t port, uint16_t value)
{
	if (state[port] == 0)
	{
		rise[port] = value;
		state[port] = 1;
		timICConfig(pwmHardware[port].timer, pwmHardware[port].channel, TIM_ICPolarity_Falling);
	} else
	{
		capture[port] = (value > rise[port]) ? (value - rise[port]) : ((PERIOD - rise[port]) + value);
		state[port] = 0;
		timICConfig(pwmHardware[port].timer, pwmHardware[port].channel, TIM_ICPolarity_Rising);
	}
}

void PwmPorts::timHandler(TIM_TypeDef* timer, uint8_t port)
{
	if (TIM_GetITStatus(timer, TIM_IT_CC1) == SET)
	{
		TIM_ClearITPendingBit(timer, TIM_IT_CC1);
		callback(port, TIM_GetCapture1(timer));
	} else if (TIM_GetITStatus(timer, TIM_IT_CC2) == SET)
	{
		port += 1;
		TIM_ClearITPendingBit(timer, TIM_IT_CC2);
		callback(port, TIM_GetCapture2(timer));
	} else if (TIM_GetITStatus(timer, TIM_IT_CC3) == SET)
	{
		port += 2;
		TIM_ClearITPendingBit(timer, TIM_IT_CC3);
		callback(port, TIM_GetCapture3(timer));
	} else if (TIM_GetITStatus(timer, TIM_IT_CC4) == SET)
	{
		port += 3;
		TIM_ClearITPendingBit(timer, TIM_IT_CC4);
		callback(port, TIM_GetCapture4(timer));
	}
}

uint16_t PwmPorts::read(uint8_t port)
{
	return capture[port];
}

extern "C" {
	void TIM3_IRQHandler()
	{
		pwmPorts.timHandler(TIM3, PWM1);
	}
}
