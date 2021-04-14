# efm8_flash
Multi platform USB Flash tool for Silicon labs EFM8UB1 MCUs

EFM8 Flash is a simple implementation of the protocol described in section 7 of the Silicon Labs application note AN945 "EFM8 Factory Bootloader User's Guide".
The utility is very simple to use. Use the browse button to select the file to flash. Make sure the file is in binary format. 
To convert it from the Intel Hex format output by Silicon Labs Simplicity Studio use the srec_cat command from the S-record package.
EFM8 can currently flash only firmware less than 15K on the EFM8UB1. It will detect hotplug of the MCU at any time. 
Press the flash button to write the firmware on the MCU.
The archive also contains a command line version of the utility useful for scripting.

# build dependencies
The GUI uses wxwidgets for cross platform compatibility. WXGlade has been used to build the main window.
the hidapi library is used to access the MCU. The makefile supports native linux builds and cross compiling for windows.

