# Profibus_DP_slave
This is a Keil MDK-ARM project, containing a Profibus DP firmware wich is tailored to compile with Keil uVision. It is a firmware to create Profibus DP slave devices with the VPC3+S network controller, provided by the vendor Profichip. 

The firmware source code is modified such that is can be compiled in Keil. 

The project generates code for STM32F103C8T6 microcontroller.

The STM32 and the VPC3+S can be used as a Profibus DP interface.

Microcontroller and network controller communicates via SPI. Routines  implemented in vpc3.c

In main() lies a test application sending some irrelevant data to a master device hanging on the same DP network.
