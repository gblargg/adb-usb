ADB-to-USB adapter using Teensy
===============================
This is a simple Mac ADB keyboard to ADB converter using a Teensy/Teensy++ 2.0/Pro Micro (5V 16MHz) AVR development board. The ADB code is based on the t.m.k. Keyboard Firmware Collection, with some timing improvements to make it more robust. It uses PJRC's usb_keyboard driver for USB. I wrote this while debugging some problems with the ADB code and because the TMK collection was too big for me to understand clearly.

This hasn't undergone much testing by others.

See keymap.h to customize layout.


Files
-----
	usb_keyboard.c			Keyboard HID implementation
	usb_keyboard.h	
	usb_keyboard_event.h	Turns key press/release events into report structure
	adb.c					ADB protocol driver
	adb.h			
	adb_usb.h				ADB locking caps lock, misc
	keycode.h				
	keymap.h				ADB to USB key code conversion
	user_keymap.h			Key layouts for extended and compact ADB keyboards. Modify as needed.
	main.c					Main loop, ADB polling, suspend handling, boot protocol
	config.h				Configuration. Modify as needed.
	Makefile				Builds program
	README.md				Documentation
	CHANGES.md		
	LICENSE.txt		


Features
--------
* Tested on M3501 (Apple Extended Keyboard II) and M0116 (Apple Keyboard).
* Power key wakes host from sleep/suspend.
* Adjustable ADB polling rate; comes configured for 12ms (83Hz).


Customization
-------------
config.h sets some keyboard options and how ADB is connected.

user_keymap.h to customizes keyboard layout. There are separate layouts for the extended and compact keyboard models.


Construction
------------
* ADB +5V and GND go to Teensy +5V and GND. ADB Data goes to Teensy PD0 (Pin "3" on the Pro Micro). A 1K resistor must also be connected between ADB Data and +5V. See pinout below.

* Extract sources and change to their directory. Modify config.h as desired.

* Connect the Teensy and press its reset button. Execute "make flash". This will build the program and flash it to the Teensy, then run it.

* Wait a few seconds and try typing on the ADB keyboard.


Pinouts
-------
ADB cable (male):

	       ,-.,-.
	  +5V / 3  4 \ GND
	Data | 1    2 | Power key
	      \  ==  /
	       ------


To do
-----
* Add media key support (volume, play/pause, etc.)


-- 
Shay Green <gblargg@gmail.com>
