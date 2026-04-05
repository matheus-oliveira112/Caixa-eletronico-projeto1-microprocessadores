/*
 * comunicacao.c
 *
 * Created: 27/05/2025 01:04:28
 *  Author: mathe
 */ 

#include <avr/io.h>

void comunica_saque() {
	UDR0 = 'C';
	while(!(UCSR0A & (1<<TXC0)));
	UDR0 = 'S';
	while(!(UCSR0A & (1<<TXC0)));
	UDR0 = 6;								//Numero de byts do valor do saque
	while(!(UCSR0A & (1<<TXC0)));
}


void comunica_saldo() {
	UDR0 = 'C';
	while(!(UCSR0A & (1<<TXC0)));
	UDR0 = 'V';
	while(!(UCSR0A & (1<<TXC0)));
}

void comunica_fechamento() {
	UDR0 = 'C';
	while(!(UCSR0A & (1<<TXC0)));
	UDR0 = 'F';
	while(!(UCSR0A & (1<<TXC0)));
}

void comunica_liberacao() {
	UDR0 = 'C';
	while(!(UCSR0A & (1<<TXC0)));
	UDR0 = 'L';
	while(!(UCSR0A & (1<<TXC0)));
}

void comunica_travamento() {
	UDR0 = 'C';
	while(!(UCSR0A & (1<<TXC0)));
	UDR0 = 'T';
	while(!(UCSR0A & (1<<TXC0)));
}

void comunica_horario() {
	UDR0 = 'C';
	while(!(UCSR0A & (1<<TXC0)));
	UDR0 = 'H';
	while(!(UCSR0A & (1<<TXC0)));
}

void comunica_entrada() {
	UDR0 = 'C';
	while(!(UCSR0A & (1<<TXC0)));
	UDR0 = 'E';
	while(!(UCSR0A & (1<<TXC0)));
}

void comunica_operando() {
	UDR0 = 'C';
	while(!(UCSR0A & (1<<TXC0)));
	UDR0 = 'O';
	while(!(UCSR0A & (1<<TXC0)));
}