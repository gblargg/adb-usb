// Basic support for ADB keyboard

#ifndef ADB_H
#define ADB_H

#include <stdint.h>
#include <stdbool.h>

// Sets up ADB bus. Doesn't send anything to keyboard.
void adb_host_init( void );

// Receives key press event from keyboard.
// 0xKKFF: one key changed state
// 0xKKKK: two keys changed state
enum { adb_host_nothing = 0 }; // no keys changed state
enum { adb_host_error = 0xFFFE }; // receive error
uint16_t adb_host_kbd_recv( void );

// Current state of keyboard modifiers and a few other keys
// Returns adb_host_nothing if keyboard didn't respond.
// Returns adb_host_error if error receiving.
uint16_t adb_host_kbd_modifiers( void );

// Sends command and two bytes of data to keyboard
enum { adb_cmd_write = 0x28 };
void adb_host_listen( uint8_t cmd, uint8_t data_h, uint8_t data_l );

// Sends command and receives two bytes of data from keyboard, or adb_host_* if no response/error
enum { adb_cmd_read  = 0x2C };
uint16_t adb_host_talk( uint8_t cmd );

// Sets keyboard LEDs. Note that bits are inverted here, so 1 means off, 0 means on.
void adb_host_kbd_led( uint8_t led );

// State of power switch (false = pressed), or true if unsupported
bool adb_host_psw( void );


#define ADB_POWER       0x7F
#define ADB_CAPS        0x39

#endif
