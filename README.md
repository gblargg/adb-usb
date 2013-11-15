ADB-to-USB adapter using Teensy
===============================
This is a simple Mac ADB keyboard to ADB converter using a Teensy/Teensy++ 2.0 AVR development board. The ADB code is based on the t.m.k. Keyboard Firmware Collection, with some timing improvements to make it more robust. It uses PJRC's usb_keyboard driver for USB. I wrote this while debugging some problems with the ADB code and because the TMK collection was too big for me to understand clearly.

This hasn't undergone much testing by others.

See keymap.h to customize layout.


Features
--------
* Tested on M3501 (Apple Extended Keyboard II) and M0116 (Apple Keyboard).
* Works through host suspend/wake.
* Adjustable ADB polling rate; comes configured for 12ms (83Hz).


Construction
------------
* ADB +5V and GND go to Teensy +5V and GND. ADB Data goes to Teensy D0. A 1K resistor must also be connected between ADB Data and +5V. See pinout below.

* Extract sources and change to their directory. If you've unlocked the caps lock key on your keyboard so that it's momentary like all the other keys, open config.h and uncomment the #define UNLOCKED_CAPS line.

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
* Wake host when power key is pressed. No idea how to do this.


-- 
Shay Green <gblargg@gmail.com>
