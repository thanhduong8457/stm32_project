# STM32 F1 GPIO Demo Project

## Project Overview
This project demonstrates the use of GPIO pins on the STM32 F1 microcontroller. It includes examples of configuring and using GPIO pins for input and output operations.

## Table of Contents
- [Introduction](#introduction)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Setup Instructions](#setup-instructions)
- [Usage](#usage)
- [Troubleshooting](#troubleshooting)
- [References](#references)

## Introduction
Provide a brief introduction to the project, its purpose, and what it aims to achieve.

## Hardware Requirements
List all the hardware components required for this project:
- STM32 F1 development board
- LEDs
- Push buttons
- Breadboard and jumper wires

## Software Requirements
List all the software tools and libraries required for this project:
- STM32CubeMX
- Keil MDK or STM32CubeIDE
- STM32 HAL Library

## Setup Instructions
Step-by-step instructions to set up the project:
1. Clone the repository.
2. Open the project in STM32CubeMX and generate the initialization code.
3. Open the generated project in Keil MDK or STM32CubeIDE.
4. Build and flash the firmware to the STM32 F1 development board.

## Usage
Instructions on how to use the project:
1. Connect the LEDs and push buttons to the specified GPIO pins.
2. Power on the STM32 F1 development board.
3. Observe the behavior of the LEDs when the push buttons are pressed.

## Troubleshooting
Common issues and their solutions:
- **Issue:** LED does not light up.
    - **Solution:** Check the GPIO pin configuration and connections.
- **Issue:** Push button does not respond.
    - **Solution:** Verify the button connections and debounce logic.

## References
List any references or documentation used in the project:
- [STM32F1 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [STM32 HAL Library Documentation](https://www.st.com/en/embedded-software/stm32cube-mcu-packages.html)
