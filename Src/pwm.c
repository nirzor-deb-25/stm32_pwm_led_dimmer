#include "pwm.h"
#include "stm32_regs.h"

#define GREEN_PIN       12U
#define TIMER_PERIOD    1000U

void pwm_init(void)
{
    /* Enable GPIOD and TIM4 clocks. */
    RCC_AHB1ENR |= (1UL << 3);
    (void)RCC_AHB1ENR;

    RCC_APB1ENR |= (1UL << 2);
    (void)RCC_APB1ENR;

    /* Reset TIM4 to a known state. */
    RCC_APB1RSTR |= (1UL << 2);
    RCC_APB1RSTR &= ~(1UL << 2);

    /* Configure PD12 as push-pull, low-speed, without pulls. */
    GPIOD_OTYPER &= ~(1UL << GREEN_PIN);
    GPIOD_OSPEEDR &= ~(3UL << (GREEN_PIN * 2U));
    GPIOD_PUPDR &= ~(3UL << (GREEN_PIN * 2U));

    /* Select AF2: TIM4 channel 1 on PD12. */
    GPIOD_AFRH &= ~(15UL << 16);
    GPIOD_AFRH |= (2UL << 16);

    /* Select alternate-function mode for PD12. */
    GPIOD_MODER &= ~(3UL << (GREEN_PIN * 2U));
    GPIOD_MODER |= (2UL << (GREEN_PIN * 2U));

    /*
     * Reset-default TIM4 clock: 16 MHz.
     * Counter clock = 16 MHz / (15 + 1) = 1 MHz.
     * PWM frequency = 1 MHz / 1000 = 1 kHz.
     */
    TIM4_PSC = 15U;
    TIM4_ARR = TIMER_PERIOD - 1U;
    TIM4_CCR1 = 0U;

    /*
     * Channel 1: output mode, PWM mode 1.
     * Enable compare preload for updates at cycle boundaries.
     */
    TIM4_CCMR1 = (6UL << 4) | (1UL << 3);

    /* Enable channel 1 with active-high polarity. */
    TIM4_CCER = 1UL;

    /* Enable auto-reload preload. */
    TIM4_CR1 = (1UL << 7);

    /* Load the prescaler and preload registers. */
    TIM4_EGR = 1UL;

    /* Start the timer. */
    TIM4_CR1 |= 1UL;
}

void pwm_set_percent(uint32_t percent)
{
    if (percent > 100U)
    {
        percent = 100U;
    }

    TIM4_CCR1 = (percent * TIMER_PERIOD) / 100U;
}