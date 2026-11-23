# Canary

Canary is a small survey probe that a ship drops onto a planet before anyone lands. Like a canary in a coal mine, its one job is to tell the crew whether the surface is safe. This repo is the probe's firmware: bare-metal C running directly on an STM32 microcontroller, with no operating system underneath.

The probe grows phase by phase from a silent board into a working field instrument. It starts by blinking its landing beacon through hand-written register addresses, then gains status lights, an arming button, a serial uplink to the ship, sensors for temperature, pressure and radiation, precise timers and interrupts. Each phase introduces a real need on the probe, and the hardware feature introduced in that phase is the answer to it.

Canary sits in the same universe as the other learning repos: Calypso flies the ship, Xenolog logs what the crew finds, and Canary is the first thing on the ground.

See [docs/PRD.md](docs/PRD.md) for the full product brief.

---

## 🔧 Prerequisites

**Hardware**
- STM32 **Nucleo-F411RE** development board
- USB cable for the board (Mini-B on the Nucleo-64) — this powers the board, flashes it and carries the serial uplink
- From phase 2 onwards: a breadboard, jumper wires, a few LEDs with resistors (220–330 Ω), a potentiometer (10 kΩ) and a temperature sensor. Each phase lists exactly what it needs.
- Recommended: a USB logic analyzer (8-channel, 24 MHz) for watching signals on the pins

**Software**
- **STM32CubeIDE** — builds, flashes and debugs the firmware
- **VS Code** (optional) — for editing; build and flash still happen in CubeIDE
- A **serial terminal** (RealTerm, PuTTY or similar) — needed from phase 4, when the probe starts talking to the ship
- **PulseView** (optional) — software for the logic analyzer

**Documentation** — keep these open; you will use them in every phase
- STM32F411 **datasheet** — pinout and memory map
- STM32F411 **reference manual (RM0383)** — every peripheral register, bit by bit
- Nucleo-64 **user manual (UM1724)** — which pins the board's LED and button are wired to

No electronics background is assumed. Whenever a phase adds a circuit, its README explains the circuit before the code.

---

## 🗂️ What's in the blank project

`main` is a freshly generated STM32CubeIDE project for the STM32F411RE with nothing added. Everything here runs before or around your code:

| File | What it does |
|---|---|
| `Src/main.c` | Your code. For now, an empty `while (1)` loop — bare-metal firmware has no operating system to return to, so `main` never ends |
| `Startup/startup_stm32f411retx.s` | The first code that runs after reset: sets up memory and calls `main`. Also holds the interrupt vector table (phase 12) |
| `STM32F411RETX_FLASH.ld` | Linker script: where code and data live in the chip's Flash and RAM |
| `Src/syscalls.c`, `Src/sysmem.c` | Small stubs the C library needs on a system without an OS (used when `printf` arrives in phase 4) |

When you build, CubeIDE reports one warning: *"FPU is not initialized…"*. That is expected — the chip has a floating-point unit we are not using yet — and it does not stop the firmware from running.

---

## ▶️ Build, flash and debug

**Open the project**
1. Clone this repo.
2. In STM32CubeIDE: **File → Open Projects from File System…**, select the cloned folder, **Finish**.

**Build**
- Select the `canary` project and click the **hammer** (Build). The output appears in `Debug/`.

**Flash and run**
1. Connect the board over USB.
2. Click **Run** (green play button). The first time, choose **STM32 C/C++ Application** and accept the default debugger (ST-LINK).
3. CubeIDE builds, flashes the board and starts the firmware. On `main` you will see nothing happen — that is correct.

**Debug**
- Click **Debug** (the bug icon). Execution stops at the start of `main`; use **Step Over** (F6), **Step Into** (F5) and **Resume** (F8). From phase 1 you will also use the **SFRs** view to watch peripheral registers change.

**Editing in VS Code**
- Edit the files in VS Code, then switch to CubeIDE to build and flash. If CubeIDE does not notice a change, select the project and press **F5** (Refresh).

---

## 📍 Branch sequence

| Branch | What it introduces | Abstraction level |
|---|---|---|
| `📌 main` | Blank STM32CubeIDE project — builds, flashes, runs an empty superloop | Scaffold only |
| `phase-01_first-light_registers` | Datasheet and memory map · registers via hand-written macros · RCC clock enable · blinking the landing beacon | Raw register addresses |
| `phase-02_status-lights_cmsis-bsrr` | Register structs · vendor device header · SAFE / CAUTION / HAZARD lights · atomic set/reset (BSRR) | Register structs |
| `phase-03_arming-button_gpio-input` | Button input · pull-ups · active-low · edge detection · debounce | — |
| `phase-04_uplink-telemetry_uart-tx` | Serial communication · UART framing · UART transmit driver · `printf` over the uplink | — |
| `phase-05_uplink-commands_uart-rx` | UART receive · command interpreter | — |
| `phase-06_sensor-bus_spi-i2c` | SPI · I²C *(to be designed)* | — |
| `phase-07_surface-sensors_adc` | ADC concepts · ADC driver · temperature and pressure · first safety verdict | — |
| `phase-08_precise-timing_systick` | SysTick · exact millisecond delays · precise beacon pattern | Calibrated delays |
| `phase-09_mission-clock_timers` | General-purpose timer · prescaler and auto-reload · 1 Hz telemetry | — |
| `phase-10_hardware-beacon_output-compare-pwm` | Output compare · PWM · night-mode dimming | Hardware-driven output |
| `phase-11_radiation-counter_input-capture` | Input capture · measuring a pulse rate | — |
| `phase-12_hazard-button_interrupts` | Interrupts · NVIC · vector table · what runs before `main` · EXTI button | Interrupt-driven |
| `phase-13_uplink-interrupts_ring-buffer` | Interrupt-driven UART · ring buffer · sharing data safely | — |
| `phase-14_mission-tick_nonblocking` | SysTick, timer and ADC interrupts · non-blocking timing | Tick-based scheduling |
| `phase-15_driver-layers` | Driver APIs · register / driver / application layers | Driver layer |
| `phase-16_vendor-hal` | Vendor HAL · clock tree and PLL · tracing HAL calls to registers | Vendor HAL |
| `phase-17_probe-modes_state-machines` | Finite state machines · probe modes · the limits of the superloop | State machines |

Each phase branch builds on the previous one. Start every phase by reading its `README.md` — it explains the problem the phase solves, what changed in the code and what to try next.
