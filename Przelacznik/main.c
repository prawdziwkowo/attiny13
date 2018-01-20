/*
 * Przelacznik.c
 *
 
 Fuses:
 high: xff
 low: x69
 
 * Created: 05.01.2018 22:35:48
 * Author : Grzegorz
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

//ile czekaæ na wygaszenie
#define clockTickWait 600

volatile short isEnabled = 1;
volatile short isButtonPushed = 0;
volatile short disableAfterUnPush = 0;
volatile int counter = 0;
volatile short buttonCounter = 0;

void inline power_ac_disable() {
	ACSR &= ~(1<<ACIE);
	ACSR |= (1<<ACD);
}


void inline initExtInterrupt(void)
{//the low level of INTx generates an interrupt request
	GIMSK |= (1<<INT0);	  //External Interrupt Request 0 Enable
	//GIFR  |= (1<<INTF0);	  //Clear External Interrupt 0 Flag	
}

void inline disableExtInterrupt(void)
{
	GIMSK &= ~(1<<INT0);
}

void inline Timer0Init()
{
	TCCR0B |= (1<<CS02); //preskaler 256, czas oko³o 0,1 s
	TIMSK0 |= (1<<TOIE0); //odblokowanie przerwania nadmiaru timera 0
}

void sleep()
{
	PORTB &= ~(1<<PB3); //wylaczenie
	counter = 0; //licznik na 0
	
	initExtInterrupt();

	set_sleep_mode(SLEEP_MODE_PWR_DOWN); //ustaw tryb spania
	sleep_mode();
}



int main(void)
{
	power_adc_disable();
	power_ac_disable();
	
	//Pb3 jako wyjscie
	DDRB |= (1<<PB3);
	
	//PB1 jako wejscie z pull-up - tam bêdzie prze³¹cznik
	DDRB &= ~(1<<PB1);
	PORTB |= (1<<PB1);
	
	Timer0Init();
	
	sei();
	
	sleep();
	
    while (1) 
    {
		if (!isButtonPushed) {
			if (counter > clockTickWait || !isEnabled) {
				sleep();
			}
		}		
    }
}


//przerwaniem tylko wybudzenie
ISR(INT0_vect)
{
	disableExtInterrupt();
	PORTB |= (1<<PB3);
	isButtonPushed = 1;
	isEnabled = 1;
	disableAfterUnPush = 0;
}

//przerwanie przepe³nienia timera
ISR(TIM0_OVF_vect) 
{
	counter++;
	
	if (!isButtonPushed && !(PINB & (1<<PB1))) {
		isButtonPushed = 1;
		disableAfterUnPush = 1;
	}

	if (isButtonPushed && (PINB & (1<<PB1))) {
		buttonCounter++;
		if (buttonCounter > 2) {
			isButtonPushed = 0;
			buttonCounter = 0;
			if (disableAfterUnPush) { 
				isEnabled = 0;
			}
		}
	}
}



