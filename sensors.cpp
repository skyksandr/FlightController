#include "robot.hpp"
#include "sensors.hpp"

extern Robot robot;

///////////////////////////////////////////////////////////////////////
// pwm ports

void RxPort::setTimerParameters(uint32_t _RCC_Timer, TIM_TypeDef* _timer, uint16_t _channel, uint16_t _timIT, uint8_t _IRQChannel)
{
	this->RCC_Timer     = _RCC_Timer;
	this->timer         = _timer;
	this->channel       = _channel;
	this->timIT         = _timIT;
	this->IRQChannel    = _IRQChannel;
}

void RxPort::setGPIOParameters(uint32_t _RCC_GPIO, GPIO_TypeDef* _gpioPort, uint16_t _gpioPin, uint8_t _gpioPinSource, uint8_t _gpioAF)
{
	this->RCC_GPIO      = _RCC_GPIO;
	this->gpioPort      = _gpioPort;
	this->gpioPin       = _gpioPin;
	this->gpioPinSource = _gpioPinSource;
	this->gpioAF        = _gpioAF;
}

// private

void RxPort::gpioInit(uint32_t RCC_Periph, GPIO_TypeDef* gpioPort, uint16_t gpioPin, uint8_t gpioPinSource, uint8_t gpioAF)
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

void RxPort::timerInit(uint32_t RCC_Periph, TIM_TypeDef* timer, uint16_t channel, uint16_t timIT, uint8_t IRQChannel)
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

	// Enable the Interrupt Request
	TIM_ITConfig(timer, timIT, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void RxPort::timICConfig(TIM_TypeDef* tim, uint16_t channel, uint16_t polarity)
{
	TIM_ICInitTypeDef timICStruct;

	timICStruct.TIM_Channel = channel;
	timICStruct.TIM_ICPolarity = polarity;
	timICStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	timICStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	timICStruct.TIM_ICFilter = 0x0;

	TIM_ICInit(tim, &timICStruct);
}

void RxPort::callback(uint16_t capture)
{
	if (state == 0)
	{
		rise = capture;
		state = 1;
		timICConfig(timer, channel, TIM_ICPolarity_Falling);
	} else
	{
		value = (capture > rise) ? (capture - rise) : ((PERIOD - rise) + capture);
		state = 0;
		timICConfig(timer, channel, TIM_ICPolarity_Rising);
	}
}

// public

void RxPort::init()
{
	gpioInit(RCC_GPIO, gpioPort, gpioPin, gpioPinSource, gpioAF);
	timerInit(RCC_Timer, timer, channel, timIT, IRQChannel);
}

uint16_t RxPort::getValue()
{
	return value;
}

///////////////////////////////////////////////////////////////////////
// ultrasonic sensors

uint16_t UltraSonic::getValue()
{
	return value;
}

///////////////////////////////////////////////////////////////////////
extern "C" {
	void TIM3_IRQHandler()
	{
		robot.board.timHandler(TIM3, 0);
	}

	void TIM4_IRQHandler()
	{
		robot.board.timHandler(TIM4, 4);
	}
}
