/*
 * miganie.c
 *
 * Created: 22.12.2016 17:14:11
 * Author : Grzegorz
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	//DDRC  |= (1<<PB0) | (1<<PB1); //pin 0 i 1 jako wyjscie
	DDRB = 0xFF;
	//PORTC |= (1<<PB0);
    while (1) 
    {
		
		PORTB = 0xFF;
		_delay_ms(500);
		PORTB = 0;
		_delay_ms(100);
		
    }
}

