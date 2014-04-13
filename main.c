// Teensy setup and main loop

// Minimum time between ADB polls
enum { min_adb_ms = 12 }; // (83Hz) best for everything, and what Macs use

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DEBUG( e )
#include "config.h"

#include "adb_usb.h"

static void init( void )
{
	clock_prescale_set( clock_div_1 );
	
	// Reduce power usage
	// Avoid floating inputs on unused pins
	#ifdef PORTA
		DDRA  = 0;
		PORTA = 0xFF;
	#endif
	
	DDRB  = 0;
	PORTB = 0xFF;
	DDRC  = 0;
	PORTC = 0xFF;
	DDRD  = 0;
	PORTD = 0xFF;
	DDRE  = 0;
	PORTE = 0xFF;
	DDRF  = 0;
	PORTF = 0xFF;
	
	power_all_disable();
	power_usb_enable();
	power_timer1_enable();
	
	PORTD &= ~(1<<6); // don't leave LED on Teensy glowing dimly
	
	adb_usb_init();
	
	TCCR1B = 5<<CS10; // 1024 prescaler
}

int main( void )
{
	init();
	
	enum { tcnt1_hz = F_CPU / 1024 };
	
	unsigned prev_time = TCNT1;
	for ( ;; )
	{
		// Limit ADB poll rate
		enum { min_period = (long) tcnt1_hz * min_adb_ms / 1000 };
		while ( TCNT1 - prev_time < min_period )
			{ }
		prev_time = TCNT1;
		
		uint16_t keys = adb_usb_read();
		if ( keys != adb_host_nothing && keys != adb_host_error )
		{
			if ( (keys >> 8 & 0xFF) == 0x7F )
			{
				// USB DETACH to wake host
				UDCON |= 1<<DETACH;
				_delay_ms( 10 );
				UDCON &= ~(1<<DETACH);
			}
			
			// Send the two events received in separate USB transfers,
			// so that keys stay in order pressed. We've got plenty of
			// USB bandwidth so use it. This avoids all the fine points
			// with an ADB event having the same key in both bytes.
			adb_usb_handle( keys >> 8 );
			
			// Ignore duplicates or non-event
			if ( (keys >> 8) != (keys & 0xFF) && (keys & 0x7F) != 0x7F )
			{
				usb_keyboard_update();
				adb_usb_handle( keys & 0xFF );
			}
		}
		
		usb_keyboard_update();
		adb_usb_update_leds();
	}
}
