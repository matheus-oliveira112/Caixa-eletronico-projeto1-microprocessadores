/*
 * CFile1.c
 *
 * Created: 16/05/2025 02:35:33
 *  Author: mathe
 */ 

#include <avr/io.h>
#define BOUNCE 7
char debounce(char d);

extern char variavelK;
extern char inatividade;

void config_teclado(void) {
	DDRB = 0x07; // 0000 0111 (P0-P2 pinos de saida [colunas], P3-P6 pinos de entrada [linhas])
	//PORTC |= 0x78;		habilita pull ups?
	PINB = 0x78;
}

char le_teclas(void) {
	char col;
	//char cadeado = 0;
	char lin;
	char teclas[4][3] = {
		{'1','2','3'},
		{'4','5','6'},
		{'7','8','9'},
		{'*','0','#'}
	};
	
	while(1) {
		if(variavelK==1) {
			//tempo_expirado = 0;
			inatividade = 0;
			return ('$');
		}
	for (col = 0; col < 3; col++) {
		// Coloca todas colunas em 1
		PORTB |= 0x07;

		// Coloca apenas a coluna atual em 0
		PORTB &= ~(1 << col);

		delay_ms(1);
		
		// Verifica se alguma linha est  em 0
		for (lin = 0; lin < 4; lin++) {
			if (!(debounce(lin + 3))) {  // PC3-PC6 -> linhas
				while (!(debounce(lin + 3))); // Espera soltar para o programa seguir
				//Comeca a ler a proxima tecla somente depois da tecla atual ser solta
				inatividade = 0;		//variavel = 0 para resetar a contagem de 30 segundos de inatividade
				return teclas[lin][col];
				//cadeado = 1;
			}
		}
	}
	}
//return('$');
}


char debounce(char d) {

	char key_last = 0;
	char key_now;
	char count = 0;
	
	while(count<BOUNCE) {

		delay_ms(1);
		key_now = (PINB&(1<<d));			//apenas uma porta qualquer selecionada
		if(key_now==key_last){
			count++;
		}
		else {
			count = 0;
		}
		key_last = key_now;
	}
	

	return key_now;
}
