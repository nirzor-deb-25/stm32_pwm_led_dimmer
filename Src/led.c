#include "led.h"
#include "stm32_regs.h"

#define BLUE_PIN 15U

void led_init(void)
{
    RCC_AHB1ENR |= (1UL << 3);
    (void)RCC_AHB1ENR;

    /* Start with the blue LED off. */
    GPIOD_BSRR = (1UL << (BLUE_PIN + 16U));

    /* Configure only PD15 as a push-pull output. */
    GPIOD_OTYPER &= ~(1UL << BLUE_PIN);
    GPIOD_OSPEEDR &= ~(3UL << (BLUE_PIN * 2U));
    GPIOD_PUPDR &= ~(3UL << (BLUE_PIN * 2U));

    GPIOD_MODER &= ~(3UL << (BLUE_PIN * 2U));
    GPIOD_MODER |= (1UL << (BLUE_PIN * 2U));
}

void led_blue_set(bool on)
{
    GPIOD_BSRR = on ? (1UL << BLUE_PIN)
                   : (1UL << (BLUE_PIN + 16U));
}