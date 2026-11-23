## Product Requirement Document

### What it is

Canary is a battery-powered survey probe deployed to a planet's surface ahead of a crewed landing. Once a crew member arms it, it flashes a landing beacon in a recognisable pattern, continuously measures surface temperature, atmospheric pressure and radiation, and reports a SAFE / CAUTION / HAZARD verdict on its status lights and over a serial uplink to the ship. The crew can query and control the probe from the ship — request status, switch the beacon or its night mode, acknowledge an alert — and the probe raises a hazard alert on its own the moment readings cross safety thresholds. The finished firmware runs on an STM32 Nucleo-F411RE with no operating system: everything the probe does, it does by talking to the hardware directly.

### Core use cases

1. A crew member can arm the probe with its button so that it only starts surveying once it is deployed.
2. A crew member on the ship can read live telemetry over the uplink so that the landing decision is based on current surface conditions.
3. A crew member can send commands (`STATUS`, `BEACON ON` / `BEACON OFF`, `NIGHT`, `ACK`) over the uplink so that the probe can be controlled without anyone on the surface.
4. A landing crew can find the probe by its beacon, which flashes a fixed pattern and dims at night, so that it is visible without blinding them.
5. The ship's crew can rely on the probe to switch to a hazard alert as soon as temperature, pressure or radiation crosses a threshold, so that no one lands into danger.

### Out of scope

- Real radiation and pressure sensing hardware — a potentiometer and a pulse source stand in for them; temperature uses a real sensor
- Wireless communication — the "uplink" is the board's serial port over the USB connection
- Power management, sleep modes and battery monitoring
- Persistent storage of readings
- Multitasking with an operating system — the probe runs a single loop; that limitation is what the final phase exposes

### Why this project

A probe that must look after itself on an alien surface needs every bare-metal building block for a concrete reason — lights, a button, sensors, an uplink and precise timing — and the growing pile of jobs on one CPU makes the move from polling to interrupts to state machines feel necessary rather than taught.
