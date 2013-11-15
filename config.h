#ifndef CONFIG_H
#define CONFIG_H

// Uncomment if you have removed the locking mechanism from your caps lock
//#define UNLOCKED_CAPS 1

// Which port and pin to run ADB through. Also needs a 1K pull-up to +5V.
#define ADB_PORT        PORTD
#define ADB_PIN         PIND
#define ADB_DDR         DDRD
#define ADB_DATA_BIT    0

#endif
