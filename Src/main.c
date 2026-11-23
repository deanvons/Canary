/*
 * Canary — planetary survey probe firmware
 *
 * main branch: the blank scaffold every phase starts from.
 * It builds, flashes and runs on the Nucleo-F411RE, but does nothing yet:
 * there is no operating system to return to, so main never ends.
 */

#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
    // Superloop: everything the probe does will happen inside this loop, forever
    while (1)
    {
    }
}
