/*
 * atrasos.c
 *
 * Created: 09/05/2025 20:50:39
 *  Author: vinic
 */ 


#include <avr/io.h>





void delay_ms(unsigned char atraso)
{
	TCCR0A |= (1<<WGM01); // Modo CTC
	TCCR0B |= (1<<1)|(1<<0); 
	TCNT0 = 0;	
	
	OCR0A = 250;
	unsigned char count = 0;
	while(count < atraso)
	{
		count++;
		while(!(TIFR0 &(1<<OCF0A)));
		TIFR0 |= (1<<OCF0A);
	}
	TCCR0B = 0;
}

void delay_us(unsigned char atraso)
{
	TCCR0A |= (1<<WGM01); // Modo CTC
	TCCR0B |= (1<<1); 
	TCNT0 = 0;	
	
	OCR0A =2;
	
	unsigned char count = 0;
	while(count < atraso)
	{
		count++;
		while(!(TIFR0 &(1<<OCF0A)));
		TIFR0 |= (1<<OCF0A);
	}
	TCCR0B = 0;
}