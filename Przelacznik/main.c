/*
 * Przelacznik.c
 *
 * Created: 05.01.2018 22:35:48
 * Author : Grzegorz
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

volatile short isEnabled = 0;
volatile short isButtonPushed = 0;
volatile int counter = 0;
volatile short buttonCounter = 0;

void inline power_ac_disable() {
	ACSR &= ~(1<<ACIE);
	ACSR |= (1<<ACD);
}


void inline initExtInterrupt(void)
{//the low level of INTx generates an interrupt request
	GIMSK |= (1<<INT0);	  //External Interrupt Request 0 Enable
	GIFR  |= (1<<INTF0);	  //Clear External Interrupt 0 Flag	
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
	isEnabled = 0;
	initExtInterrupt(); //wlaczenie zewnetrznych przerwan
	//set_sleep_mode(SLEEP_MODE_PWR_DOWN); //ustaw tryb spania
	//sleep_mode();
}


//TODO: zablokowaæ usypianie gdy przycisk wlaczony
int main(void)
{
	power_adc_disable();
	power_ac_disable();
	
	
	//Pb3 jako wyjscie
	DDRB |= (1<<PB3);
	
	DDRB |= (1<<PB4);
	
	
	//PB1 jako wejscie z pull-up - tam bêdzie prze³¹cznik
	DDRB &= ~(1<<PB1);
	PORTB |= (1<<PB1);
	
	Timer0Init();
	
	sei();
	//TODO: od razu sleep
	sleep();
	
    while (1) 
    {
		
		if (counter > 50 && !isButtonPushed) {//TODO: zrobic na 600
			sleep();
		}
		
		//Je¿eli wy³¹czyliœmy i mamy pewnoœæ, ¿e button zosta³ wciœniêty
		if (!isButtonPushed && !isEnabled) {
			sleep();
		}
		
		//wlaczamy zewnetrzne przerwanie gdy jestesmy pewni, ¿e przycisk zosta³ odcisniety	
		if (!isButtonPushed) {
			initExtInterrupt();
		}
    }
}

//TODO: nie wy³¹cza siê poprawnie przyciskiem - zobaczyæ co jest nie tak - w tej chwili nie mam zielonego pojêcia

ISR(INT0_vect)
{
	disableExtInterrupt();
	isButtonPushed = 1;
	
	if (!isEnabled) {
		isEnabled = 1;
		PORTB |= (1<<PB3);
	} else {
		isEnabled = 0;
	}
}

ISR(TIM0_OVF_vect) //przerwanie przepe³nienia timera
{
	counter++;
	if (isButtonPushed && (PINB & (1<<PB1))) {
		buttonCounter++;
		if (buttonCounter > 2) {
			isButtonPushed = 0;
			buttonCounter = 0;
		}
	}
	
	if (isButtonPushed) {
		PORTB |= (1<<PB4);
		} else {
		PORTB &= ~(1<<PB4);
	}
	
	
}



