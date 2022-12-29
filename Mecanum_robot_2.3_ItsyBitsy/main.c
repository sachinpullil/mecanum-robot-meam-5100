//Frequency Detection Circuit

#include "MEAM_general.h"  		       		// includes the resources included in the MEAM_general.h file										
					
static unsigned int oldtime;
static unsigned int tperiod;

ISR(TIMER3_CAPT_vect){
		set(TIFR3, ICF3);		  			// clear flag by writing 1
		if(ICR3>20){
			tperiod = ICR3-oldtime;
			oldtime = ICR3;
		}
		if (21<tperiod && tperiod<24){		// for 700Hz
			set(PORTB,2);
			clear(PORTB,3);
			set(PORTB,4);
		} 
		if (675<tperiod && tperiod<685){	// for 23Hz
			set(PORTB,2);
			clear(PORTB,3);
			clear(PORTB,4);
		} 
		else{								// for any other frequency
			clear(PORTB,2);
		}
}

ISR(TIMER1_CAPT_vect){
		set(TIFR1, ICF1);			  		// clear flag by writing 1
		if(ICR1>20){
			tperiod = ICR1-oldtime;
			oldtime = ICR1;
		}
		if (21<tperiod && tperiod<24){		// for 700Hz
			set(PORTB,3);
			clear(PORTB,2);
			set(PORTB,4);
		} 
		if (675<tperiod && tperiod<685){	// for 23Hz
			set(PORTB,3);
			clear(PORTB,2);
			clear(PORTB,4);
		} 
		else{								// for any other frequency
			clear(PORTB,3);
		}
}
								 
int main(void){
	sei();									// global interrupts

	set(TIMSK3,ICIE3);						// enable timer input capture interrupts
	set(TIMSK1,ICIE1);

	_clockdivide(0);
	set(TCCR3B,CS32);						// turn on timers
	set(TCCR3B,CS30);
	set(TCCR1B,CS12);
	set(TCCR1B,CS10);

	clear(DDRC,7);							// clear input capture pins
	clear(DDRD,4);
	
	set(DDRB,3);							// set outputs
	set(DDRB,2);
	set(DDRB,4);
	
	while(1){
 	  ICR3 = TCNT3;							// keep ICR3 matching TNCT3 until interrupted
 	  ICR1 = TCNT1;							// keep ICR1 matching TNCT1 until interrupted
 	  _delay_ms(1000);						// delay to keep pins on for a second while waiting for interrupts
 	  clear(PORTB,2);
 	  clear(PORTB,3);
  }
}	