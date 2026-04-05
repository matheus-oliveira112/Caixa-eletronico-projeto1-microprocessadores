/*
 * operacoes.c
 *
 * Created: 27/05/2025 00:19:08
 *  Author: mathe
 */ 

#include <avr/io.h>

extern char tempo_expirado; 

void saque_func() {
	
	char j, i, k;
	unsigned char saque[8] = {0};
		
	do {
		for(i=0; i<6; i++) {
			saque[i]='0';
		}
		limpa_display();
		str_DISPLAY("Valor do saque:");
		for(i=0; i<6; i++) {
			k = le_teclas();
			if(k==1) {
				goto sessao_encerrada;
			}
			if(k=='#') {			//Confirma o valor digitado pelo usuario
				break;
			}
			if(i>0) {
				for(j=0; j<5; j++) {		//Desloca o numero que se tem no vetor para direita
					saque[j] = saque[j+1];
				}
			}
			saque[5] = k;
			
			//limpa
			//mensagem "R$ saque[]"
			limpa_display();
			str_DISPLAY("Valor do saque:");
			cmd_DISPLAY(0xC0);
			str_DISPLAY("R$");
			for(j=0; j<=5; j++) {	
				if(j==4) {
				str_DISPLAY(".");	
				}	
				char_DISPLAY(saque[j]);
			}
			
		}
		if ((saque[0] - '0' > 1) ||
		(saque[0] - '0' == 1 && saque[1] - '0' > 2) ||
		(saque[0] - '0' == 1 && saque[1] - '0' == 2 && saque[2] - '0' > 0) ||
		(saque[0] - '0' == 1 && saque[1] - '0' == 2 && saque[2] - '0' == 0 && saque[3] - '0' > 0) ||
		(saque[0] - '0' == 1 && saque[1] - '0' == 2 && saque[2] - '0' == 0 && saque[3] - '0' == 0 && saque[4] - '0' > 0) ||
		(saque[0] - '0' == 1 && saque[1] - '0' == 2 && saque[2] - '0' == 0 && saque[3] - '0' == 0 && saque[4] - '0' == 0 && saque[5] - '0' > 0)) {
			//Valor >1200, digite novamente
			limpa_display();
			str_DISPLAY("Valor >1200");
			cmd_DISPLAY(0xC0);
			str_DISPLAY("Digite novamente");
			for(j = 0; j <10; j++) {
				delay_ms(200);				//Delay de 2s
			}
		}
		
} while((saque[0] - '0' > 1) ||
(saque[0] - '0' == 1 && saque[1] - '0' > 2) ||
(saque[0] - '0' == 1 && saque[1] - '0' == 2 && saque[2] - '0' > 0) ||
(saque[0] - '0' == 1 && saque[1] - '0' == 2 && saque[2] - '0' == 0 && saque[3] - '0' > 0) ||
(saque[0] - '0' == 1 && saque[1] - '0' == 2 && saque[2] - '0' == 0 && saque[3] - '0' == 0 && saque[4] - '0' > 0) ||
(saque[0] - '0' == 1 && saque[1] - '0' == 2 && saque[2] - '0' == 0 && saque[3] - '0' == 0 && saque[4] - '0' == 0 && saque[5] - '0' > 0));
	
	comunica_saque();
	
	for(i=0; i<=5; i++) {					//Valor do saque sendo enviado pela serial
		UDR0 = saque[i];
		while(!(UCSR0A & (1<<TXC0)));
	}
	sessao_encerrada:
	;
}

