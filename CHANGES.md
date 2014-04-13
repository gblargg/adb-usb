* Added support for waking computer by pressing power key.
* Reorganized code.
* Added ADB_SWAP_CMD_OPTION to act like Apple ADB keyboard
* Added detection of extended versus compact keyboard, and use of separate keymap.


blargg-adb-usb-3
----------------
* Fixed keypad equals to KP_EQUAL instead of normal equals.
* Fixed to once again distinguish between left/right shift, control, and alt keys. This got broken in previous version.


blargg-adb-usb-2
----------------
* Changed polling to 12ms (83Hz). Works better with M0116. Can still be adjusted back to 8ms (125Hz) in main.c
* Fixed dropped key when it's pressed and released in same ADB event.
* Reorganized code to be in sync with v-usb version.
* Disable interrupts for all ADB transactions.
* Send each ADB event byte as a separate USB report. This ensures things are in order and cleanly handles cases when the same key is pressed then released in a single ADB event.


blargg-adb-usb-1
----------------
* First release.
