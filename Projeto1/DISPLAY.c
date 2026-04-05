/*
 * DISPLAY.c
 *
 * Created: 09/05/2025 17:13:25
 *  Author: vinic
 */ 

#include <avr/io.h>
#define RS PK0
#define EN PK1

void cmd_DISPLAY(unsigned char cmd)
{
	PORTK &= ~(1<<RS); //RS=0
	PORTF &= 0x0F; //limpa a parte alta do barramento de dados
	PORTF |= (cmd & 0xF0); //transfere a parte alta do comando
	
	
	PORTK |= (1<<EN);  //EN=1
	PORTK &= ~(1<<EN); //EN=0

	//delay_us(50);
	PORTF &= 0x0F; //limpa a parte alta do barramento de dados
	PORTF |= ((cmd << 4) & 0xF0); //transfere a parte baixa do comando pro barramento de dados
	PORTK |= (1<<EN);  //EN=1
	PORTK &= ~(1<<EN); //EN=0
	
	
	delay_us(50);
	//PORTK &= ~(1<<EN); //EN=0
	
}

void	INIT_DISPLAY(){
	DDRF |= 0xF0;
	DDRK |= (1<<RS);
	DDRK |= (1<<EN);
	
	PORTK &= ~(1<<RS); //RS=0
	
	//PORTK &= ~(1<<EN); //EN=0
	PORTF &= 0x0F;
	PORTF |= 0x20;
	PORTK |= (1<<EN);  //EN=1
	//delay_us(50);
	PORTK &= ~(1<<EN); //EN=0
	delay_us(50);
	cmd_DISPLAY(0x28);
	cmd_DISPLAY(0x0C);
	cmd_DISPLAY(0x06);
	//delay_us(50);	
}


void char_DISPLAY(unsigned char cmd)
{
	PORTK |= (1<<RS); //RS=1
	PORTF &= 0x0F; //limpa a parte alta do barramento de dados
	PORTF |= (cmd & 0xF0); //transfere a parte alta do comando
	
		
	PORTK |= (1<<EN);  //EN=1
	PORTK &= ~(1<<EN); //EN=0

	//delay_us(50);
	PORTF &= 0x0F; //limpa a parte alta do barramento de dados
	PORTF |= ((cmd << 4) & 0xF0); //transfere a parte baixa do comando pro barramento de dados
	PORTK |= (1<<EN);  //EN=1
	PORTK &= ~(1<<EN); //EN=0	
	
	
	delay_us(50);
	//PORTK &= ~(1<<EN); //EN=0
	
}



void str_DISPLAY(char *frase)
{
	char index = 0;
	while (frase[index] != '\0') {
		
		if (index == 16)
		{
			cmd_DISPLAY(0xC0);
			char_DISPLAY(frase[index]);
		}
		else
		{
			char_DISPLAY(frase[index]);
		}
		index++;
	}
}

void limpa_display() {
	cmd_DISPLAY(0x01);
	cmd_DISPLAY(0x80);
}