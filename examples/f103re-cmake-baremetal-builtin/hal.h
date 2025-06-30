#pragma once

#include <stm32f103xe.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define LED1 PIN('B', 0)   // On-board LED pin (green)
#define LED2 PIN('B', 7)   // On-board LED pin (blue)
#define LED3 PIN('B', 14)  // On-board LED pin (red)

#define LED LED2              // Use blue LED for blinking

// System clock
// 6.3.3: APB1 clock <= 45MHz; APB2 clock <= 90MHz
// 3.5.1, Table 11: configure flash latency (WS) in accordance to clock freq
// 33.4: The AHB clock must be at least 25 MHz when Ethernet is used
enum { APB1_PRE = 5 /* AHB clock / 4 */, APB2_PRE = 4 /* AHB clock / 2 */ };
enum { PLL_HSI = 16, PLL_M = 8, PLL_N = 168, PLL_P = 2, PLL_Q = 7 };
#define FLASH_LATENCY 5
#define SYS_FREQUENCY ((PLL_HSI * PLL_N / PLL_M / PLL_P) * 1000000) // Core 168 MHz, USB 48 MHz
#define APB2_FREQUENCY (SYS_FREQUENCY / (BIT(APB2_PRE - 3)))
#define APB1_FREQUENCY (SYS_FREQUENCY / (BIT(APB1_PRE - 3)))


static inline void spin(volatile uint32_t count) {
  while (count--) (void) 0;
}
