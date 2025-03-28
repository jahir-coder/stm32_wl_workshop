/**
  @page LoRaWAN_Sigfox_send_on_timerReadme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2020 STMicroelectronics *******************
  * @file    Applications/LoRaWAN_Sigfox/LoRaWAN_Sigfox_send_on_timer/readme.txt 
  * @author  MCD Application Team
  * @brief   This application is a simple demo application software of a LoRa
  *          modem connecting to Network server. Data sent can be checked on 
  *          Network server for eg Loriot. Traces are displayed over UART
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Description

This directory contains a set of source files that implements a LoRa application
and sigfox application device sending sensors data to LoRa or Sigfox Network server
every 10 seconds. To swicth between lora and Sigfox simply psuh the user button 1

This application is targeting the STM32WLxx Nucleo board embedding the STM32WLxx.
  ******************************************************************************

@par Keywords

Applications, SubGHz_Phy, LoRaWAN, Sigfox, End_Node, SingleCore

@par Directory contents 


  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/adc.h                       This file contains all the function prototypes for
                                                                      the adc.c file
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/adc_if.h                    Header for ADC interface configuration
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/board_resources.h           Header for driver at.c module
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/dma.h                       This file contains all the function prototypes for
                                                                      the dma.c file
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/flash_if.h                  This file provides interface to low level driver
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/lptim.h                     This file contains all the function prototypes for
                                                                      the lptim.c file
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/main.h                      : Header for main.c file.
                                                                      This file contains the common defines of the application.
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/mn_lptim_if.h               Header for between Sigfox monarch and lptim
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/platform.h                  Header for General HW instances configuration
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/rtc.h                       This file contains all the function prototypes for
                                                                      the rtc.c file
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/stm32wlxx_hal_conf.h        HAL configuration file.
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/stm32wlxx_it.h              This file contains the headers of the interrupt handlers.
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/stm32wlxx_nucleo_conf.h     STM32WLxx_Nucleo board configuration file.
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/stm32_lpm_if.h              Header for Low Power Manager interface configuration
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/subghz.h                    This file contains all the function prototypes for
                                                                      the subghz.c file
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/sys_app.h                   Function prototypes for sys_app.c file
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/sys_conf.h                  Applicative configuration, e.g. : debug, trace, low power, sensors
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/sys_debug.h                 Configuration of the debug.c instances
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/sys_sensors.h               Header for sensors application
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/timer_if.h                  configuration of the timer_if.c instances
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/usart.h                     This file contains all the function prototypes for
                                                                      the usart.c file
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/usart_if.h                  Header for USART interface configuration
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/utilities_conf.h            Header for configuration file to utilities
  - LoRaWAN_Sigfox_send_on_timer/Core/Inc/utilities_def.h             Definitions for modules requiring utilities
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/App/app_lorawan.h            Header of application of the LRWAN Middleware
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/App/CayenneLpp.h             Implements the Cayenne Low Power Protocol
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/App/Commissioning.h          End-device commissioning parameters
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/App/lora_app.h               Header of application of the LRWAN Middleware
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/App/lora_app_version.h       Definition the version of the application
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/App/lora_info.h              To give info to the application about LoRaWAN configuration
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/App/se-identity.h            Secure Element identity and keys
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/Target/lorawan_conf.h        Header for LoRaWAN middleware instances
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/Target/mw_log_conf.h         Configure (enable/disable) traces for CM0
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/Target/radio_board_if.h      Header for Radio interface configuration
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/Target/radio_conf.h          Header of Radio configuration
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/Target/systime.h             Map middleware systime
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/Target/timer.h               Wrapper to timer server
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/app_sigfox.h              Header of application of the Sigfox Middleware
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/ee.h                      Header of the EEPROM emulator
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/sgfx_app.h                provides code for the application of the SIGFOX Middleware
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/sgfx_app_version.h        Definition the version of the application
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/sgfx_cstimer.h            Interface for  cstimer.c driver
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/sgfx_eeprom_if.h          eeprom interface to the upper module.
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/sigfox_data.h             provides encrypted data
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/Target/mcu_api.h              defines the interfaace to mcu_api.c
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/Target/mn_api.h               monarch library interface
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/Target/rf_api.h               interface to rf_api.c
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/Target/se_nvm.h               header to emulated SE nvm data
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/Target/sgfx_credentials.h     interface to key manager and encryption algorithm

  - LoRaWAN_Sigfox_send_on_timer/Core/Src/adc.c                       This file provides code for the configuration
                                                                      of the ADC instances.
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/adc_if.c                    Read status related to the chip (battery level, VREF, chip temperature)
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/board_resources.c           Source file
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/dma.c                       This file provides code for the configuration
                                                                      of all the requested memory to memory DMA transfers.
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/flash_if.c                  This file contains the FLASH driver
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/lptim.c                     This file provides code for the configuration
                                                                      of the LPTIM instances.
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/main.c                      : Main program body
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/mn_lptim_if.c               Interface between Sigfox monarch and lptim
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/rtc.c                       This file provides code for the configuration
                                                                      of the RTC instances.
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/stm32wlxx_hal_msp.c         This file provides code for the MSP Initialization
                                                                      and de-Initialization codes.
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/stm32wlxx_it.c              Interrupt Service Routines.
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/stm32_lpm_if.c              Low layer function to enter/exit low power modes (stop, sleep)
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/subghz.c                    This file provides code for the configuration
                                                                      of the SUBGHZ instances.
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/system_stm32wlxx.c          CMSIS Cortex Device Peripheral Access Layer System Source File
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/sys_app.c                   Initializes HW and SW system entities (not related to the radio)
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/sys_debug.c                 Enables 4 debug pins for internal signals RealTime debugging
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/sys_sensors.c               Manages the sensors on the application
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/timer_if.c                  Configure RTC Alarm, Tick and Calendar manager
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/usart.c                     This file provides code for the configuration
                                                                      of the USART instances.
  - LoRaWAN_Sigfox_send_on_timer/Core/Src/usart_if.c                  Configuration of UART MX driver interface for hyperterminal communication
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/App/app_lorawan.c            Application of the LRWAN Middleware
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/App/CayenneLpp.c             Implements the Cayenne Low Power Protocol
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/App/lora_app.c               Application of the LRWAN Middleware
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/App/lora_info.c              To give info to the application about LoRaWAN configuration
  - LoRaWAN_Sigfox_send_on_timer/LoRaWAN/Target/radio_board_if.c      This file provides an interface layer between MW and Radio Board
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/app_sigfox.c              Application of the Sigfox Middleware
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/ee.c                      Implementation of the EEPROM emulator
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/sgfx_app.c                provides code for the application of the sigfox Middleware
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/sgfx_cstimer.c            manages carrier sense timer.
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/App/sgfx_eeprom_if.c          eeprom interface to the sigfox component
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/Target/mcu_api.c              mcu library interface
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/Target/mn_api.c               monarch library interface implementation
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/Target/rf_api.c               rf library interface
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/Target/se_nvm.c               manages SE nvm data
  - LoRaWAN_Sigfox_send_on_timer/Sigfox/Target/sgfx_credentials.c     manages keys and encryption algorithm


@par Hardware and Software environment 

  - This example runs on the STM32WLxx Nucleo board.

  - STM32WLxx Nucleo board Set-up    
    - Connect the Nucleo board to your PC with a USB cable type A to micro-B 
      to ST-LINK connector.
    - Please ensure that the ST-LINK connector jumpers are fitted.

  - Configure the software via the configuration files:
    - sys_conf.h, lorawan_conf.h, sigfox_conf.h, lora_app.h, Commissioning.h, se-identity.h
    - Careful: 
        - the region and class chosen on LoRaWAN/App/lora_app.h shall be compatible with LoRaWAN/Target/lorawan_conf.h list
        - STM32WLxx Nucleo requires IS_TCXO_SUPPORTED=1

  -Set Up:

             --------------------------  V    V  --------------------------
             |           Object       |  |    |  |           Network      |
             |                        |  |    |  |                        |
   ComPort<--|                        |--|    |--|                        |-->Web Server
             |                        |          |                        |
             --------------------------          --------------------------

@par How to use it ? 
In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Open a Terminal, connected the Object
  - UART Config = 115200, 8b, 1 stopbit, no parity, no flow control

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */