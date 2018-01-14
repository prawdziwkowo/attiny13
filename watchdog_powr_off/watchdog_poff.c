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
#include <avr/power.h>


void WDT_init() {
	// Clear MCU Status Register
	// Not really needed here as we don't need to know why the MCU got reset. P.44
	MCUSR = 0;
	
	// Disable and clear all Watchdog settings P.46
	// Not really sure this is needed as we never set the WDE, but nice to be thorough
	WDTCR = (1<<WDCE)|(1<<WDE);
	WDTCR = 0;
	
	// Setup Watchdog for interrupt and not reset, and a approximately 500ms timeout P.45
	//WDTCR = (1<<WDTIE)|(1<<WDP2)|(1<<WDP0); //0.5s
	WDTCR = (1<<WDTIE)|(1<<WDP3); //4s
}

void power_ac_disable() {
	ACSR &= ~(1<<ACIE);
	ACSR |= (1<<ACD);
}

void my_power_adc_disable() {
	PRR |= (1<<PRADC);
}

void power_vref_disable()
{
	
}

//w power down mode pobiera pr¹d jakiêœ 8uA
//popatrzeæ jeszcze jak sie zachowuje to wszystko, co mo¿na jeszcze powy³¹czaæ

int main(void)
{
	
	//wylaczam komparator
	power_adc_disable(); //jakieœ 10uA mniej
	//my_power_adc_disable();
	power_ac_disable(); //jakieœ 50uA mniej
	//power_adc_enable2();
	//power_adc_enable();
//#### SETUP ####
	// Disable all interrupts
	//cli();
	
	WDT_init();
	
	// Enable all interrupts.
	sei();
			
	// Set  PORTB pin 3 as output
	DDRB = (1<<PB3);
	PORTB |= (1<<PB3);
	
	//port b jako wejscia z pullupem
	PORTB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB4);
	
//#### END OF SETUP ####
		//set_sleep_mode(SLEEP_MODE_PWR_DOWN); //ustaw tryb spania
		
		//sleep_mode();
	// Do nothing.
    while(1){
		
		//PORTB ^= (1<<3);
		//_delay_ms(50);
		//PORTB ^= (1<<3);
		//_delay_ms(50);
		//PORTB ^= (1<<3);
		//_delay_ms(50);
		//PORTB ^= (1<<3);
		//_delay_ms(50);
		//PORTB ^= (1<<3);
		//_delay_ms(50);
		//PORTB ^= (1<<3);
		//_delay_ms(50);
		//jak zrobiê coœ do mierzenia, to przetestowaæ wy³¹czanie ró¿nych akcesoriów
		//sprawdziæ jakoœ czy wchodzi w ten tryb
		//set_sleep_mode(SLEEP_MODE_PWR_DOWN); //ustaw tryb spania
		
		//sleep_mode();
	};
}

ISR(WDT_vect) {
	// Flip bit 3 of PORTB
	//PORTB ^= (1<<3);
		
	// ReEnable the watchdog interrupt, as this gets reset when entering this ISR and 
	// automatically enables the WDE signal that resets the MCU the next time the 
	// timer overflows
	WDTCR |= (1<<WDTIE);
}