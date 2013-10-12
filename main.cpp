
#include "robot.hpp"

Robot robot;

int main(void)
{
	// Инициализация

	// Канал 1
	RxPort rxPort1;
	rxPort1.setTimerParameters(RCC_APB1Periph_TIM3, TIM3, TIM_Channel_1, TIM_IT_CC1, TIM3_IRQn);
	rxPort1.setGPIOParameters(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_6, GPIO_PinSource6, GPIO_AF_TIM3);
	rxPort1.init();

	robot.board.rxPwmPort[0] = &rxPort1;

	// Канал 2
	RxPort rxPort2;
	rxPort2.setTimerParameters(RCC_APB1Periph_TIM3, TIM3, TIM_Channel_2, TIM_IT_CC2, TIM3_IRQn);
	rxPort2.setGPIOParameters(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_7, GPIO_PinSource7, GPIO_AF_TIM3);
	rxPort2.init();

	robot.board.rxPwmPort[1] = &rxPort2;

	// Канал 3
	RxPort rxPort3;
	rxPort3.setTimerParameters(RCC_APB1Periph_TIM3, TIM3, TIM_Channel_3, TIM_IT_CC3, TIM3_IRQn);
	rxPort3.setGPIOParameters(RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_0, GPIO_PinSource0, GPIO_AF_TIM3);
	rxPort3.init();

	robot.board.rxPwmPort[2] = &rxPort3;

	// Канал 4
	RxPort rxPort4;
	rxPort4.setTimerParameters(RCC_APB1Periph_TIM3, TIM3, TIM_Channel_4, TIM_IT_CC4, TIM3_IRQn);
	rxPort4.setGPIOParameters(RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_1, GPIO_PinSource1, GPIO_AF_TIM3);
	rxPort4.init();

	robot.board.rxPwmPort[3] = &rxPort4;

	// Канал 5
	RxPort rxPort5;
	rxPort5.setTimerParameters(RCC_APB1Periph_TIM4, TIM4, TIM_Channel_1, TIM_IT_CC1, TIM4_IRQn);
	rxPort5.setGPIOParameters(RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_6, GPIO_PinSource6, GPIO_AF_TIM4);
	rxPort5.init();

	robot.board.rxPwmPort[4] = &rxPort5;

	// Канал 6
	RxPort rxPort6;
	rxPort6.setTimerParameters(RCC_APB1Periph_TIM4, TIM4, TIM_Channel_2, TIM_IT_CC2, TIM4_IRQn);
	rxPort6.setGPIOParameters(RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_7, GPIO_PinSource7, GPIO_AF_TIM4);
	rxPort6.init();

	robot.board.rxPwmPort[5] = &rxPort6;

	// Канал 7
	RxPort rxPort7;
	rxPort7.setTimerParameters(RCC_APB1Periph_TIM4, TIM4, TIM_Channel_3, TIM_IT_CC3, TIM4_IRQn);
	rxPort7.setGPIOParameters(RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_8, GPIO_PinSource8, GPIO_AF_TIM4);
	rxPort7.init();

	robot.board.rxPwmPort[6] = &rxPort7;

	// Канал 8
	RxPort rxPort8;
	rxPort8.setTimerParameters(RCC_APB1Periph_TIM4, TIM4, TIM_Channel_4, TIM_IT_CC4, TIM4_IRQn);
	rxPort8.setGPIOParameters(RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_9, GPIO_PinSource9, GPIO_AF_TIM4);
	rxPort8.init();

	robot.board.rxPwmPort[7] = &rxPort8;

	robot.operate();
}
