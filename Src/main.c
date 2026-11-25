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

// Copies of the landed word, taken half-word by half-word and byte by byte, for the debugger to show
volatile uint16_t landed_halves[2];
volatile uint8_t  landed_bytes[4];

int main(void)
{
    /* An address is just a number. The linker chose it; & tells us what it is. */
    uintptr_t status_address = (uintptr_t)&descent_status;

    /*
     * Turn the number back into a pointer. Every update below goes through it,
     * the same way phase 1 reaches a hardware register that has no name in C.
     */
    volatile uint32_t *status = (volatile uint32_t *)status_address;

    /* The descent: one masked update per moment of the fall. Step over each line and watch the word. */

    // 1. Entry: stage 00 -> 01. Clear the whole field, then write the new stage into it
    *status &= ~STAGE_MASK;
    *status |= (STAGE_ENTRY << STAGE_POS);

    // 2. Parachute opens
    *status |= PARACHUTE_OUT;

    // 3. Heat shield drops away
    *status |= HEAT_SHIELD_JETTISONED;

    // 4. Descent: stage 01 -> 10
    *status &= ~STAGE_MASK;
    *status |= (STAGE_DESCENT << STAGE_POS);

    // 5. Legs out
    *status |= LEGS_EXTENDED;

    // 6. Touchdown: only declare a landing if the legs really are out
    if ((*status & LEGS_EXTENDED) != 0U)
    {
        *status &= ~STAGE_MASK;
        *status |= (STAGE_LANDED << STAGE_POS);
    }

    // 7. Parachute cut away, so it can't drag the probe across the ground
    *status &= ~PARACHUTE_OUT;

    /*
     * The landed word, 0xCA000035, read through two more pointers to the same address.
     * Pointer arithmetic: [1] is one whole item further on, 2 bytes for uint16_t, 1 byte for uint8_t.
     */
    volatile uint16_t *status_halves = (volatile uint16_t *)status_address;
    volatile uint8_t  *status_bytes  = (volatile uint8_t *)status_address;

    landed_halves[0] = status_halves[0];   // 0x0035
    landed_halves[1] = status_halves[1];   // 0xCA00

    landed_bytes[0] = status_bytes[0];     // 0x35: little-endian, so the low byte comes first
    landed_bytes[1] = status_bytes[1];     // 0x00
    landed_bytes[2] = status_bytes[2];     // 0x00
    landed_bytes[3] = status_bytes[3];     // 0xCA: the pod ID sits at the highest address

    // Superloop: the probe has landed, and the heartbeat bit shows the flight computer is still running
    while (1)
    {
        *status ^= HEARTBEAT;
    }
}
