/*
 * Canary — planetary survey probe firmware
 *
 * Phase 01: first light.
 * The probe proves it is alive by blinking its landing beacon,
 * using nothing but register addresses taken from the documentation.
 */

#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/*
 * Where is the beacon?
 * Nucleo-64 user manual (UM1724): the green user LED LD2 is wired to PA5 — port A, pin 5.
 */

/* ------------------------------------------------------------------------
 * Address tree — base addresses from the datasheet's memory map
 * ------------------------------------------------------------------------ */
#define PERIPH_BASE           (0x40000000UL)                    // all peripherals start here
#define AHB1PERIPH_OFFSET     (0x00020000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + AHB1PERIPH_OFFSET) // 0x4002 0000: the AHB1 bus

#define GPIOA_OFFSET          (0x0000UL)
#define GPIOA_BASE            (AHB1PERIPH_BASE + GPIOA_OFFSET)  // 0x4002 0000: port A

#define RCC_OFFSET            (0x3800UL)
#define RCC_BASE              (AHB1PERIPH_BASE + RCC_OFFSET)    // 0x4002 3800: reset and clock control

/* ------------------------------------------------------------------------
 * Registers — offsets from the reference manual (RM0383)
 * Each macro casts an address to a pointer to a volatile 32-bit value,
 * then dereferences it, so the macro can be read and written like a variable.
 * ------------------------------------------------------------------------ */
#define RCC_AHB1ENR_OFFSET    (0x30UL)
#define RCC_AHB1ENR           (*(volatile uint32_t *)(RCC_BASE + RCC_AHB1ENR_OFFSET))   // AHB1 clock enables

#define GPIOA_MODER_OFFSET    (0x00UL)
#define GPIOA_MODER           (*(volatile uint32_t *)(GPIOA_BASE + GPIOA_MODER_OFFSET)) // pin modes

#define GPIOA_ODR_OFFSET      (0x14UL)
#define GPIOA_ODR             (*(volatile uint32_t *)(GPIOA_BASE + GPIOA_ODR_OFFSET))   // output data

/* ------------------------------------------------------------------------
 * Bit masks — the individual bits this phase changes
 * ------------------------------------------------------------------------ */
#define GPIOAEN               (1U << 0)   // RCC_AHB1ENR bit 0: clock for port A

#define BEACON_PIN            (1U << 5)   // PA5 in GPIOA_ODR
#define BEACON_MODE_BIT0      (1U << 10)  // pin 5's mode field is bits 11:10 of GPIOA_MODER
#define BEACON_MODE_BIT1      (1U << 11)

int main(void)
{
    // Superloop: everything the probe does will happen inside this loop, forever
    while (1)
    {
    }
}
