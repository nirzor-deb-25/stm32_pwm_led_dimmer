#include "stm32_regs.h"

#define SCB_VTOR REG32(0xE000ED08UL)

void SystemInit(void)
{
    /* Use the interrupt vector table at the start of flash. */
    SCB_VTOR = 0x08000000UL;

    /*
     * Keep the reset clock configuration:
     * internal HSI, 16 MHz, without the PLL.
     */
}


/* Initialization hook called by the C library startup. */
void _init(void)
{
}