#include "button.h"
#include "stm32_regs.h"

#define DEBOUNCE_MS 20U

static bool candidate_pressed;
static bool stable_pressed;
static uint32_t candidate_since_ms;

static bool button_read_raw(void)
{
    return (GPIOA_IDR & 1UL) != 0U;
}

void button_init(void)
{
    /* Enable GPIOA clock. */
    RCC_AHB1ENR |= 1UL;
    (void)RCC_AHB1ENR;

    /* Configure PA0 as an input with a pull-down. */
    GPIOA_MODER &= ~3UL;
    GPIOA_PUPDR = (GPIOA_PUPDR & ~3UL) | 2UL;

    candidate_pressed = button_read_raw();
    stable_pressed = false;
    candidate_since_ms = 0U;
}

void button_update(uint32_t now_ms)
{
    bool raw_pressed = button_read_raw();

    if (raw_pressed != candidate_pressed)
    {
        candidate_pressed = raw_pressed;
        candidate_since_ms = now_ms;
    }

    if ((uint32_t)(now_ms - candidate_since_ms) >= DEBOUNCE_MS)
    {
        stable_pressed = candidate_pressed;
    }
}

bool button_is_pressed(void)
{
    return stable_pressed;
}