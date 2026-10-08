#include "timing.h"
#include "stm32_regs.h"

static volatile uint32_t milliseconds;

void timing_init(void)
{
    /* Configure SysTick for the default 16 MHz CPU clock. */
    SYSTICK_CTRL = 0U;
    milliseconds = 0U;

    SYSTICK_LOAD = 16000U - 1U;
    SYSTICK_VAL = 0U;

    /*
     * Bit 2: use the processor clock.
     * Bit 1: enable the SysTick interrupt.
     * Bit 0: enable the counter.
     */
    SYSTICK_CTRL = 7U;
}

void SysTick_Handler(void)
{
    milliseconds++;
}

uint32_t timing_millis(void)
{
    return milliseconds;
}