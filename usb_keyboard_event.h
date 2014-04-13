// Converts USB key events into modifier bitfield and key list of report structure

#include <stdint.h>
#include <stdbool.h>

// Calls usb_keyboard_send() if report is dirty
void usb_keyboard_update( void );

// Marks report as dirty
void usb_keyboard_touch( void );

// Adjusts report structure to reflect pressed/released key
void report_usb_event( uint8_t code, bool pressed );


//// Code

#include "usb_keyboard.h"
#include "keycode.h"
#include "config.h"
#include <stdbool.h>

#ifndef DEBUG
	#define DEBUG( e )
#endif

static bool usb_report_dirty;

enum { max_keys = 6 };

// Marks report as dirty so it'll be sent on the next update
void usb_keyboard_touch( void )
{
	usb_report_dirty = true;
}

// Calls usb_keyboard_send() only if changes have been made to report via report_usb_event()
void usb_keyboard_update( void )
{
	if ( usb_report_dirty )
	{
		usb_report_dirty = false;
		usb_keyboard_send();
	}
}

// Adds key press/release to current report
void usb_keyboard_event( uint8_t code, bool pressed )
{
	if ( !code )
		return;
	
	usb_report_dirty = true;
	
	if ( KC_LCTRL <= code && code <= KC_RGUI )
	{
		// Modifier; route to mask rather than keys list
		uint8_t mask = 1 << (code - KC_LCTRL);
		keyboard_modifier_keys |= mask;
		if ( !pressed )
			keyboard_modifier_keys ^= mask;
	}
	else
	{
		// Find key
		uint8_t* p = keyboard_keys + max_keys;
		do
		{
			if ( *--p == code )
				break;
		}
		while ( p != keyboard_keys );

		if ( !pressed )
		{
			// Remove key
			if ( *p == code )
			{
				*p = 0;
				return;
			}
			
			DEBUG(debug_byte( code ));
			DEBUG(debug_str( "released key not in list\n" ));
		}
		else
		{
			// Don't add if already there (sometimes keyboard gives multiple
			// key down events when pressing lots of keys)
			if ( *p == code )
			{
				DEBUG(debug_str( "pressed key already in list\n" ));
				return;
			}
			
			// Add to first empty entry
			p = keyboard_keys + max_keys;
			do
			{
				if ( *--p == 0 )
				{
					*p = code;
					return;
				}
			}
			while ( p != keyboard_keys );
			
			DEBUG(debug_str( "too many keys pressed\n" ));
		}
	}
}
