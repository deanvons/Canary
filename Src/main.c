/*
 * Canary — planetary survey probe firmware
 *
 * Phase 00: pre-flight.
 * The probe is still falling inside its drop pod. The flight computer records
 * the whole descent in one 32-bit status word in SRAM, and the code keeps that
 * word up to date one bit at a time.
 */

#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* ------------------------------------------------------------------------
 * Status word layout — the table in the README, written down once
 * ------------------------------------------------------------------------ */
#define HEAT_SHIELD_JETTISONED  (1U << 0)   // bit 0
#define PARACHUTE_OUT           (1U << 1)   // bit 1
#define LEGS_EXTENDED           (1U << 2)   // bit 2
#define HEARTBEAT               (1U << 7)   // bit 7

#define STAGE_POS               (4U)                  // the descent-stage field starts at bit 4...
#define STAGE_MASK              (3U << STAGE_POS)     // ...and is two bits wide: bits 5:4
#define STAGE_ORBIT             (0U)                  // 00
#define STAGE_ENTRY             (1U)                  // 01
#define STAGE_DESCENT           (2U)                  // 10
#define STAGE_LANDED            (3U)                  // 11

#define POD_ID_POS              (24U)                 // the pod ID fills the top byte: bits 31:24
#define POD_ID_CANARY           (0xCAU)

/*
 * The status word. The linker gives it an address somewhere in SRAM,
 * at or above 0x2000 0000; nothing in this file says where.
 * It starts as 0xCA000000: the pod ID in the top byte, in orbit, no flags set.
 */
volatile uint32_t descent_status = (POD_ID_CANARY << POD_ID_POS) | (STAGE_ORBIT << STAGE_POS);

int main(void)
{
    // Superloop: everything the probe does will happen inside this loop, forever
    while (1)
    {
    }
}
