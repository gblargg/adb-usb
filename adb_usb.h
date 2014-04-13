// Conversion from ADB to USB, including locking caps to momentary caps USB uses

#include <stdint.h>

// Init ADB reading and initialize keyboard
void adb_usb_init( void );

// Handle an ADB key press/release byte and update keyboard_modifiers and keyboard_keys.
void adb_usb_handle( uint8_t raw );

// Reads new ADB event pair from keyboard and releases caps if necessary
uint16_t adb_usb_read( void );

// Call tens of times a second
void adb_usb_update_leds( void );


//// Source

#include "keymap.h"
#include "usb_keyboard_event.h"
#include "adb.h"
#include "usb_keyboard.h"

static void adb_usb_handle_( uint8_t raw )
{
	usb_keyboard_event( keymap_to_usb( raw & 0x7f ), ~raw & 0x80 );
}

// Locking caps to USB momentary caps

enum { released_mask = 0x80 };
enum { caps_mask = 0x02 };
enum { adb_caps = 0x39 };

static uint8_t caps_pressed;
static uint8_t caps_on;

static void caps_release( void )
{
	if ( caps_pressed )
	{
		caps_pressed = false;
		adb_usb_handle_( adb_caps | released_mask );
	}
}

static void caps_set_leds( uint8_t leds )
{
	// only update our flag when USB host changes caps LED
	static uint8_t prev_caps_led;
	uint8_t caps_led = leds & caps_mask;
	if ( prev_caps_led != caps_led )
	{
		prev_caps_led = caps_led;
		caps_on = caps_led;
	}
}

static void caps_event( uint8_t raw )
{
	uint8_t new_caps = (raw & released_mask) ? 0 : caps_mask;
	if ( caps_on != new_caps )
	{
		// in case host doesn't handle LED, keep ours updated
		caps_on = new_caps;
		caps_pressed = 1;
		adb_usb_handle_( adb_caps );
	}
}


void adb_usb_init( void )
{
	adb_host_init();
	_delay_ms( 300 ); // keyboard needs at least 250ms or it'll ignore host_listen below
	
	uint16_t id = adb_host_talk( adb_cmd_read + 3 );
	if ( id == adb_host_nothing || id == adb_host_error )
		id = 0;
	keymap_init( id & 0xff );
	
	// Enable separate key codes for left/right shift/control/option keys
	// on Apple Extended Keyboard.
	adb_host_listen( adb_cmd_write + 3, 0x02, 0x03 );
	
	usb_init();
	while ( !usb_configured() )
		{ }
}

uint16_t adb_usb_read( void )
{
	cli(); // don't let anything upset ADB timing
	uint16_t keys = adb_host_kbd_recv();
	sei();
	
	caps_release();
	
	return keys;
}

void adb_usb_update_leds( void )
{
	static uint8_t leds = -1;
	uint8_t new_leds = keyboard_leds;
	if ( leds != new_leds )
	{
		leds = new_leds;
		caps_set_leds( new_leds );
		
		cli();
		adb_host_kbd_led( ~new_leds & 0x07 );
		sei();
	}
}

void adb_usb_handle( uint8_t raw )
{
	#if !UNLOCKED_CAPS
		if ( (raw & 0x7f) == adb_caps )
		{
			caps_event( raw );
			return;
		}
	#endif
	
	adb_usb_handle_( raw );
}
