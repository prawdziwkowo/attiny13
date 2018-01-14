/*
 * MrBlinky.c
 * MrBlinky Watchdog wof wof edition
 * Using the watchdog timer as a timed interrupt 
 *
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

int main(void)
{
//#### SETUP ####
	// Disable all interrupts
	cli();
	
	// Clear MCU Status Register
	// Not really needed here as we don't need to know why the MCU got reset. P.44
	MCUSR = 0;
	
	// Disable and clear all Watchdog settings P.46
	// Not really sure this is needed as we never set the WDE, but nice to be thorough  
	WDTCR = (1<<WDCE)|(1<<WDE);
	WDTCR = 0;
	
	// Setup Watchdog for interrupt and not reset, and a approximately 500ms timeout P.45
	WDTCR = (1<<WDTIE)|(1<<WDP2)|(1<<WDP0);
	
	// Enable all interrupts.
	sei();
			
	// Set  PORTB pin 3 as output
	DDRB = _BV(3);
	
//#### END OF SETUP ####
		
	// Do nothing.
    while(1){
		PORTB ^= (1<<3);
		_delay_ms(50);
		PORTB ^= (1<<3);
		_delay_ms(50);
		PORTB ^= (1<<3);
		_delay_ms(50);
		PORTB ^= (1<<3);
		_delay_ms(50);
		PORTB ^= (1<<3);
		_delay_ms(50);
		PORTB ^= (1<<3);
		_delay_ms(50);
		
		set_sleep_mode(SLEEP_MODE_PWR_DOWN); //ustaw tryb spania
		sleep_mode();
	};
}

ISR(WDT_vect) {
	// Flip bit 3 of PORTB
	PORTB ^= (1<<3);
		
	// ReEnable the watchdog interrupt, as this gets reset when entering this ISR and 
	// automatically enables the WDE signal that resets the MCU the next time the 
	// timer overflows
	WDTCR |= (1<<WDTIE);
}