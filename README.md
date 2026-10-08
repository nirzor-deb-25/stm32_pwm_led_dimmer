# STM32 PWM LED Dimmer



A hardware PWM LED dimmer for the STM32F407G-DISC1 development board, implemented in C using direct register access without HAL.



The USER button cycles the onboard green LED through six duty-cycle levels. TIM4 generates the PWM signal independently of the main loop.



## Features



- Hardware PWM using TIM4 channel 1

- Six selectable duty cycles

- Debounced button input

- Blue LED button indicator

- SysTick-based millisecond timing

- Nonblocking application logic

- Multi-file C project

- Handwritten Makefile with automatic header dependencies

- Command-line programming using `make flash`



## Hardware and Tools



- STM32F407G-DISC1 development board

- USB connection through the onboard ST-LINK

- Windows PowerShell

- GNU Arm toolchain, GNU Make, and STM32CubeProgrammer CLI supplied with STM32CubeIDE



No external components or wiring are required.



## Pin Mapping



| Component | MCU pin | Configuration |

|---|---|---|

| Green LED — LD4 | PD12 | AF2, TIM4 channel 1 |

| Blue LED — LD6 | PD15 | GPIO output |

| USER button — B1 | PA0 | Input with pull-down |



The LEDs are active high. The USER button reads high when pressed.



## Controls



The green LED starts off after reset.



Each debounced USER button press advances one level:



| Presses after reset | Duty cycle |

|---|---:|

| 0 | 0% — off |

| 1 | 10% |

| 2 | 25% |

| 3 | 50% |

| 4 | 75% |

| 5 | 100% — continuously on |

| 6 | 0% — off |



The sequence repeats.



Holding the button advances the level once. The blue LED remains on while the debounced button state is pressed.



## How PWM Controls Brightness



PWM switches the LED on and off rapidly.



Duty cycle describes the fraction of each cycle spent on. For example, a 10% duty cycle keeps the output high for one tenth of each cycle.



Perceived brightness is not proportional to duty cycle. Differences between higher levels may look smaller than expected.



## Timer Configuration



The firmware keeps the reset-default clock configuration, with a nominal 16 MHz HSI clock and an APB1 prescaler of 1.



| Parameter | Value |

|---|---:|

| TIM4 input clock | 16 MHz nominal |

| Prescaler register — PSC | 15 |

| Counter clock | 1 MHz nominal |

| Auto-reload register — ARR | 999 |

| Counts per PWM cycle | 1000 |

| PWM frequency | 1 kHz nominal |

| Output mode | PWM mode 1, active high |



Frequency calculation:



```text

PWM frequency = timer clock / ((PSC + 1) × (ARR + 1))

&#x20;             = 16,000,000 / (16 × 1000)

&#x20;             = 1000 Hz

```



Duty-cycle calculation:



```text

CCR1 = duty_percent × 1000 / 100

```



In up-counting PWM mode 1, the output is active while the counter is below CCR1.



CCR1 is 0 for 0% and 1000 for 100%. Since the counter ranges from 0 to 999, these values produce continuously low and continuously high outputs respectively.



Compare preload applies duty-cycle changes at the next timer update boundary. Auto-reload preload is also enabled.



## Software Modules



| File | Purpose |

|---|---|

| `Src/main.c` | Cycles through duty-cycle levels |

| `Src/pwm.c` | Configures TIM4 and updates CCR1 |

| `Src/led.c` | Controls the blue LED |

| `Src/button.c` | Reads and debounces the USER button |

| `Src/timing.c` | Maintains a millisecond counter using SysTick |

| `Src/system.c` | Provides startup initialization |

| `Inc/` | Module interfaces and register definitions |

| `Startup/` | MCU startup assembly and interrupt vectors |

| `STM32F407VGTX_FLASH.ld` | Flash and RAM linker layout |

| `Makefile` | Build, clean, size, and flash targets |

| `setup.ps1` | Configures tool paths for PowerShell |



TIM4 controls the green LED directly. The LED module configures only the blue LED, preserving PD12's alternate-function configuration.



The button debounce interval is 20 ms.



## Setup and Build



Open PowerShell in the project folder:



```powershell

. .\setup.ps1

make -j4

```



The setup script defaults to:



```text

C:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE

```



For another installation:



```powershell

. .\setup.ps1 -CubeIdeRoot 'C:\path\to\STM32CubeIDE'

```



The script finds the compiler, Make, and programmer in the CubeIDE plugins directory. It changes PATH only for the current session.



If multiple matching tool versions exist, it selects the first path after descending alphabetical sorting.



## Build Commands



| Command | Action |

|---|---|

| `make -j4` | Builds ELF, BIN, and HEX |

| `make size` | Reports firmware memory usage |

| `make clean` | Removes the generated build directory |

| `make flash` | Builds the ELF if needed, programs, verifies, and resets |



For a clean rebuild, run sequentially:



```powershell

make clean

make -j4

```



The Makefile uses Windows `cmd.exe` commands and `>` recipe prefixes.



Generated `.d` files track header dependencies. Changes rebuild the affected objects and regenerate dependent firmware files.



## Program the Board



1. Connect USB to the onboard ST-LINK connector.

2. Stop any active debugger session using that ST-LINK.

3. Run:



```powershell

make flash

```



This replaces the firmware on the board.



To specify the programmer path:



```powershell

make flash PROGRAMMER="C:/path/to/STM32_Programmer_CLI.exe"

```



The flash target updates the ELF. Run `make` to also update BIN and HEX after source changes.



## Generated Files



Build outputs are stored in `build/` and excluded from Git:



- `stm32_pwm_led_dimmer.elf`

- `stm32_pwm_led_dimmer.bin`

- `stm32_pwm_led_dimmer.hex`

- `stm32_pwm_led_dimmer.map`

- Object files and header dependency files



## Verification



| Test | Result |

|---|---|

| Clean build without errors or warnings | Passed |

| Unchanged build reports nothing to do | Passed |

| Firmware programming and verification | Reported passed during hardware testing |

| Green LED starts off | Passed |

| Six presses cycle back to off | Passed |

| Holding USER advances only once | Passed |

| Blue LED follows button state | Passed |

| Power reconnection restores initial off state | Passed |

| 100% appears brighter than 10% | Passed |



Hardware tests were performed manually.



## Limitations



- PWM frequency depends on the internal oscillator's accuracy.

- Timer settings assume the reset-default clock configuration.

- Exact PWM frequency and duty cycles were not measured with an oscilloscope or logic analyzer.

- Visual inspection confirms dimming but does not measure optical brightness.

- The Makefile and setup script target Windows.

- This implementation controls an onboard LED; it does not include a motor driver or motor-control hardware.



## Learning Outcomes



- Configuring GPIO alternate functions

- Setting timer prescalers and periods

- Generating hardware PWM

- Adjusting duty cycle through a compare register

- Using preload registers

- Debouncing button input

- Organizing embedded code into modules

- Building and programming firmware from PowerShell

