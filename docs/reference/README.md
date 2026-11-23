# Reference documents

Every phase sends you to one of these four documents. Download them once and keep them open while you work — in bare-metal programming, the documentation *is* the API.

The PDFs are published by STMicroelectronics and Arm, so they are not stored in this repo. Download each one from the official link and save it in this folder (`docs/reference/`) under the suggested file name; the folder's `.pdf` files are ignored by git.

| Document | What you use it for | Download | Suggested file name |
|---|---|---|---|
| **STM32F411xC/E reference manual (RM0383)** | Every peripheral register, bit by bit — RCC, GPIO, USART, ADC, timers, EXTI. The document you will open most | [st.com](https://www.st.com/resource/en/reference_manual/rm0383-stm32f411xce-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) | `RM0383_STM32F411_reference_manual.pdf` |
| **STM32F411xC/E datasheet** | The memory map, the pinout, the alternate-function table (which pin can do what) and the electrical limits of each pin | [st.com](https://www.st.com/resource/en/datasheet/stm32f411re.pdf) | `STM32F411xCxE_datasheet.pdf` |
| **STM32 Nucleo-64 user manual (UM1724)** | The board itself: which pin the LED and button are on, the USB serial connection, and the header pinouts for wiring | [st.com](https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf) | `UM1724_Nucleo-64_user_manual.pdf` |
| **Cortex-M4 Devices Generic User Guide (DUI0553)** | The Arm core's own peripherals, which ST's manual does not cover: SysTick, the NVIC, exceptions and the vector table | [developer.arm.com](https://developer.arm.com/documentation/dui0553/latest/) | `DUI0553_Cortex-M4_generic_user_guide.pdf` |

Revisions used when writing this course: RM0383 Rev 3, datasheet Rev 7, UM1724 Rev 14. Newer revisions are fine — section and page numbers may shift slightly.

## Which document answers which question

- *"What is at this address?"* → datasheet, memory map
- *"Which pin is the LED / button / USB serial on?"* → Nucleo user manual
- *"Which pins can be UART TX, an ADC input or a timer channel?"* → datasheet, alternate-function table
- *"What does this bit in this register do?"* → reference manual
- *"How do SysTick or interrupt priorities work?"* → Cortex-M4 generic user guide

More documents join this list as the probe grows: the accelerometer datasheet for the sensor-bus phase, and the temperature sensor's datasheet for the surface-sensors phase.
