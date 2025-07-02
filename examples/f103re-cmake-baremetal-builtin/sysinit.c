#include <stm32f103xe.h>

#define SYS_FREQUENCY 72000000

void spin(volatile uint32_t count)
{
    while (count--)
        (void) 0;
}

uint32_t SystemCoreClock = SYS_FREQUENCY;
void     SystemInit(void)
{
    // Enable FPU is not applicable for STM32F1 series (no FPU)
    // Reset RCC clock configuration to default reset state
    RCC->CR |= RCC_CR_HSION;                                   // Enable HSI
    RCC->CFGR = 0x00000000;                                    // Reset CFGR
    RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON);  // Disable HSE, CSS, PLL
    RCC->CR &= ~RCC_CR_HSEBYP;                                 // Disable HSE bypass
    RCC->CIR = 0x00000000;                                     // Disable all interrupts

    // Configure Flash prefetch and latency
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;  // 1 wait state, prefetch enable

    // Configure PLL (example: HSE as source, 8MHz * 9 = 72MHz)
    RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9;  // HSE as PLL source, PLL x9
    RCC->CR |= RCC_CR_HSEON;                           // Enable HSE
    while ((RCC->CR & RCC_CR_HSERDY) == 0)
        spin(1);  // Wait for HSE ready

    RCC->CR |= RCC_CR_PLLON;  // Enable PLL
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
        spin(1);  // Wait for PLL ready

    // Set prescalers: APB1 max 36MHz, APB2 max 72MHz
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;  // APB1 = HCLK/2
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;  // APB2 = HCLK

    // Select PLL as system clock source
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
        spin(1);

    SystemCoreClock = 72000000;  // Update core clock variable

    SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}
