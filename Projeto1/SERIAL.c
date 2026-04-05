/*
 * SERIAL.c
 *
 * Created: 23/05/2025 15:27:41
 *  Author: aluno
 */ 
 #include <avr/io.h>
 
 void INIT_SERIAL(void)
{
	UCSR0C = 38;
	UCSR0B |= (1<<3)|(1<<4)|(1<<7);
	UBRR0 = 51;
}