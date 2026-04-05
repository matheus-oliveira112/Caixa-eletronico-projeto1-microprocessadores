/*
 * DataHora.c
 *
 * Created: 23/05/2025 17:31:07
 *  Author: vinic
 */ 
#include <avr/io.h>
//#include <avr/interrupt.h>



void timer1_init(void) {
	// Configure Timer1 para modo CTC (Clear Timer on Compare Match)
	TCCR1B |= (1 << WGM12);
	// Prescaler de 1024
	TCCR1B |= (1 << CS12) | (1 << CS10);
	// Compare value para gerar uma interrupção a cada 1s (com 16MHz)
	OCR1A = 15624; // 1 segundo
	// Habilita interrupção por compare match A
	TIMSK1 |= (1 << OCIE1A);	
}

