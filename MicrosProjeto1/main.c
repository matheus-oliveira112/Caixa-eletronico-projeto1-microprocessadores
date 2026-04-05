/*
 * MicrosProjeto1.c
 *
 * Created: 08/05/2025 21:14:24
 * Author : vinic
 */ 
#include "header.h"
#include <avr/interrupt.h>
#include <avr/io.h>


char recebendo = 0;
char recebe_horario = 0;
char recebe_dados = 0;
char resposta_saque = 0;
char resposta_saldo = 0;
char n;							//Variavel que recebe o numero de bytes seguintes
char campo_dados[100];					//Variavel utilizado para armazenar o campo de dados
char saldo [100];				//Variavel utilizado para armazenar o saldo
char aux = 0;	
//char variavel_saldo;		

char contador = 0;
char str[4] = {1, 1, 10, 0};  // {dia, mês, hora, minuto}
char vetor_auxiliar[4];
char alternador = 1;
char variavel_inatividade;
char inatividade;
char tempo_expirado;
char variavelK;

// Tabela de dias por mês (índice 0 = janeiro)
const char dias_por_mes[12] = {
	31, // Janeiro
	28, // Fevereiro
	31, // Março
	30, // Abril
	31, // Maio
	30, // Junho
	31, // Julho
	31, // Agosto
	30, // Setembro
	31, // Outubro
	30, // Novembro
	31  // Dezembro
};


int main(void)
{
	unsigned char usuario[13];
	char tecla;
	char tecla_pagamento;
	//char teste[32] = {"0060.00"};
	char j;
	INIT_DISPLAY();	
	config_teclado();
	timer1_init();
	sei();
	INIT_SERIAL();
	

	
    while (1) 
    {
		//tecla = le_teclas();
		//
		//if(tecla=='1'){
			//UDR0 = 'A';
			//cmd_DISPLAY(0x01);
			//cmd_DISPLAY(0x80);
			//str_DISPLAY("R$");
		//}
		//if(tecla=='2'){
			//UDR0 = 'B';
			//str_DISPLAY(teste);
		//}
		//else if(tecla=='3'){
			//UDR0 = 'C';
			//cmd_DISPLAY(0x01);
		//}
		
		
		//			   //
		//             //			PARTE QUE LE O LOGIN/SENHA
		if (recebendo == 3) {
			recebendo = 0;
			variavel_inatividade = 0;
			limpa_display();
			str_DISPLAY("BANRISUFRGS");
			//variavel que tera valor = 0 fazendo com que a sessao não feche por inatividade
			tecla = le_teclas();
			if(tecla=='#') {
				goto trava_terminal;
			}
			variavel_inatividade = 1;
			//variavel que tera valor = 1 fazendo com que a sessao possa ser fechada por inatividade
			limpa_display();
			str_DISPLAY("LOGIN: ");
			for (j = 0; j < 6; j++) {
				usuario[j] = le_teclas();
				if(usuario[j]=='$') {
					limpa_display();
					str_DISPLAY("VOLTEI");
					//goto trava_terminal;
				}
				//if(variavelK==1) {
					//limpa_display();
					//str_DISPLAY("VOLTEI");
				//}
				char_DISPLAY(usuario[j]);
				//A cada vez que um digito é digito pelo usuario o tempo de interacao sera zerado
			}
			cmd_DISPLAY(0xC0);
			str_DISPLAY("SENHA: ");
			for (j = 6; j < 12; j++) {
				usuario[j] = le_teclas();
				if(usuario[j]==1) {
					goto trava_terminal;
				}
				char_DISPLAY('*');
			}
			comunica_entrada();
			for (j = 0; j < 12; j++) {
				UDR0 = usuario[j];
				while(!(UCSR0A & (1<<TXC0)));
			}
			trava_terminal:
			;	
		}

		//			   //
		//             //			PARTE QUE ESCOLHE A OPERAÇÃO DESEJADA E A REALIZA NOS PASSOS SEGUINTES
		
		if (recebendo == 4) {
			do {
				recebendo = 0;
				limpa_display();
				str_DISPLAY("[1]Saque[2]Saldo[3]Pagam.[4]Sair");
				
				tecla = le_teclas();
				if(tecla==1) {
					goto trava_terminal;
				}
				
				if(tecla == '1') {				// Saque
					saque_func();
				}
				
				else if(tecla == '2') {				// Saldo
					comunica_saldo();
				}
				else if(tecla == '3') {					//Pagamento
					do 
					{
						limpa_display();
						str_DISPLAY("[1]Boleto");
						cmd_DISPLAY(0xC0);
						str_DISPLAY("[2]Digitacao");
						tecla_pagamento = le_teclas();
						
						if(tecla==1) {
							goto trava_terminal;
						}
						
						if(tecla_pagamento == '1') {	
							limpa_display();
							str_DISPLAY("ESCANEIE O BOLETO");						
						}
						else if(tecla_pagamento == '2') {				
							
						}
						else {
							limpa_display();
							str_DISPLAY("Invalido,");
							cmd_DISPLAY(0xC0);
							str_DISPLAY("digite novamente");
							for(j = 0; j <13; j++) {
								delay_ms(200);				//Delay de 1s
							}
						}
					} while ((tecla_pagamento != '1') && (tecla_pagamento != '2'));
				}
				else if(tecla == '4') {				// Sair/finalizar sessão
					comunica_fechamento();
				}
				else {
					limpa_display();
					str_DISPLAY("Invalido,");
					cmd_DISPLAY(0xC0);
					str_DISPLAY("digite novamente");
					for(j = 0; j <13; j++) {
						delay_ms(200);				//Delay de 1s
					}
				}
				
			} while ((tecla != '1') && (tecla != '2') && (tecla != '3') && (tecla != '4'));		//Caso o digito nao seja uma das opcoes (1, 2, 3 ou 4), o usuario deve digitar novamente
			
		}
		
    }
}

ISR(USART0_RX_vect) 
{
	char recebido = UDR0;
	//char tecla;
	//unsigned char saque[8] = {0};
	//unsigned char usuario[13];
	char j;
	
	
		//             //			PARTE QUE RECEBE DA SERIAL OS DADOS PARA O PROSSEGUIMENTO DO PROGRAMA
	
	if ((recebido == 'S') && (recebendo == 0))
	{
		recebendo = 1;
	}
	
	else if(recebendo ==1)
	{
		//PARTE QUE FINALIZA A SESSÃO
		if((recebido == 'F') && (recebe_dados==0))
		{
			limpa_display();
			str_DISPLAY("Sessao encerrada");
			recebendo = 3;
			for(j = 0; j <10; j++) {
				delay_ms(200);				//Delay de 2s 
			}
		}
		
		//PARTE QUE RECEBE O 'OK' DA IMPRESSAO
		if((recebido == 'I') && (recebe_dados==0) && (resposta_saque==0))
		{
			UDR0 = 'B';
			
			//if(variavel_saldo ==1) {
				//limpa_display();
				//str_DISPLAY("Sessao encerrada");
				//recebendo = 3;
				//for(j = 0; j <10; j++) {
					//delay_ms(200);				//Delay de 2s
				//}
				//goto sessao_encerrada;
			//}
			recebendo = 0;
			comunica_fechamento();
			
			//sessao_encerrada:
			//;
		}
		
		//PARTE QUE DIZ SE O SAQUE FOI APROVADO OU NÃO
		if((recebido == 'S') && (recebe_dados==0))
		{
			resposta_saque++;
		}
		else if((recebido == 'O') && (recebe_dados==0) && (resposta_saque==1))
		{
			resposta_saque=0;
			limpa_display();
			str_DISPLAY("Saque aprovado!");
			for(j = 0; j <15; j++) {
				delay_ms(200);				//Delay de 3s
			}
			recebendo = 0;
			comunica_fechamento();
		}
		else if((recebido == 'I') && (recebe_dados==0) && (resposta_saque==1))
		{
			resposta_saque=0;
			limpa_display();
			str_DISPLAY("Saldo insufic.!");
			for(j = 0; j <15; j++) {
				delay_ms(200);				//Delay de 3s
			}
			recebendo = 0;
			comunica_fechamento();
		}
		
		//PARTE QUE DIZ O SALDO
		if((recebido == 'V') && (recebe_dados==0) && (resposta_saldo==0))
		{
			resposta_saldo++;
		}
		else if(resposta_saldo==1)
		{
			resposta_saldo++;
			n = recebido;
		}
		else if(resposta_saldo==2)
		{
			saldo[aux] = recebido;
			aux++;
			if(aux == n) {		
				limpa_display();
				str_DISPLAY("Saldo:");
				cmd_DISPLAY(0xC0);
				str_DISPLAY("R$");
				for(j=0; j<n; j++){
					if(j == (n-2)) {
						char_DISPLAY('.');
					}
					char_DISPLAY(saldo[j]);
				}
				for(j = 0; j <15; j++) {
					//delay_ms(200);				//Delay de 3s
				}
				aux = 0;
				resposta_saldo=0;
				
				//CI + variavel_saldo = 1, goto
				//variavel_saldo ==1
				recebendo = 0;
				comunica_fechamento();
			}
		}
		
		
		//PARTE QUE LIBERA O TERMINAL
		if((recebido == 'L') && (recebe_dados==0))
		{
			comunica_liberacao();
			recebendo = 3;		
		}
		
		//PARTE QUE TRAVA O TERMINAL
		if ((recebido == 'T') && (recebe_dados==0))
		{
			comunica_travamento();
			recebendo = 2;		
		}
		
		//PARTE QUE RECEBE O HORARIO DO SERVIDOR
		if ((recebido == 'H') && (recebe_dados==0) && (recebe_horario==0)) 
		{
			recebe_horario++; 
		}
		else if (recebe_horario==1) {
			vetor_auxiliar[0] = recebido;
			recebe_horario++;
		}
		else if (recebe_horario==2) {
			vetor_auxiliar[1] = recebido;
			recebe_horario++;
		}
		else if (recebe_horario==3) {
			vetor_auxiliar[2] = recebido;
			recebe_horario++;
		}
		else if (recebe_horario==4) {
			recebe_horario = 0;
			recebendo = 0;		
			str[3] = recebido;
			comunica_horario();
			str[2] = vetor_auxiliar[2];
			str[1] = vetor_auxiliar[1];
			str[0] = vetor_auxiliar[0];	
			contador = 0;
		}
		
		//PARTE QUE RECEBE OS DADOS DO CLIENTE APOS O MESMO SE REGISTRAR (podem ser validados ou nao)
		if ((recebido == 'E') && (recebe_dados==0)) 
		{
			recebe_dados++;
		}
		else if (recebe_dados == 1) {
			n = recebido;					//Numero de bytes do campo de dados do cliente
			recebe_dados++;
		}
		else if (recebe_dados == 2) {
			campo_dados[aux] = recebido;
			aux++;
			if((aux==n) && (campo_dados[0]=='N') && (campo_dados[1]=='a') && (campo_dados[2]=='o') && (campo_dados[3]==' ') && (campo_dados[4]=='a') && (campo_dados[5]=='u') &&
			(campo_dados[6]=='t') && (campo_dados[7]=='o') && (campo_dados[8]=='r') && (campo_dados[9]=='i') && (campo_dados[10]=='z') && (campo_dados[11]=='a') &&
			(campo_dados[12]=='d') && (campo_dados[13]=='o')) {
				limpa_display();
				str_DISPLAY("Nao autorizado");
				for(j = 0; j <15; j++) {
					delay_ms(200);				//Delay de 3s
				}
				recebe_dados = 0;
				aux = 0;
				recebendo = 3;
			}
			else if(aux == n) {		
				limpa_display();
				for(j=0; j<n; j++) {
					if(j==(n-3)) {
						cmd_DISPLAY(0xC0);
					}
					char_DISPLAY(campo_dados[j]);
				}
				for(j = 0; j <15; j++) {
					delay_ms(200);				//Delay de 3s
				}
				recebe_dados = 0;
				aux = 0;
				recebendo = 4;			//Vai para tela onde o usuario escolhe a operação que deseja fazer
			}
		}
		
	}
	
		//			   //
		//             //			PARTE QUE TRAVA O TERMINAL 
	if (recebendo == 2)	{
		limpa_display();
		str_DISPLAY("Caixa bloqueado");
		//variavel que tera valor = 0 fazendo com que a sessao não feche por inatividade 
		recebendo = 0;
	}

}


ISR(TIMER1_COMPA_vect) {
	char j;
	
	contador++;
	
	//PARTE QUE CHECA A INATIVIDADE DO USUARIO
	if(variavel_inatividade==1) {
		inatividade++;
		
		//if(inatividade==1) {
			//cmd_DISPLAY(0x0C);
		//}
		if (inatividade==19) {
			limpa_display();
			//cmd_DISPLAY(0x0D);
			str_DISPLAY("INATIVIDADE");
			cmd_DISPLAY(0xC0);
			str_DISPLAY("DETECTADA");
			variavelK = 1;
		}
		else if(inatividade==31) {
			limpa_display();
			//cmd_DISPLAY(0x0C);
			str_DISPLAY("Sessao encerrada");
			for(j=0; j<15; j++) {
			delay_ms(200);
			}						//delay 3s
			tempo_expirado = 1;
			inatividade = 0;
			variavel_inatividade = 0;
			recebendo = 3;
		}
	}
	
	//PARTE QUE CHECA SE O HORARIO ESTA ENTRE AS 8 E AS 20
	if ((recebendo!=3 && recebendo!=4) && ((str[2]>20) || (str[2]==20 && str[3]>0) || (str[2]<8))) {
		recebendo = 0;
		limpa_display();
		str_DISPLAY("Caixa bloqueadoA");
		alternador = 1;
		
	}
	else if((alternador==1) && ((str[2]>8 && str[2]<20) || (str[2]==8 && str[3]>=0) || (str[2]==20 && str[3]==0))) {
		recebendo = 0;
		alternador = 0;
		limpa_display();
		str_DISPLAY("==BANRISUFRGS==");
	}
	
	//PARTE QUE ENVIA QUE O CAIXA ESTA VIVO
	if((contador==20) || (contador == 40) || (contador == 60)) {
		comunica_operando();
	}
	
	//RELOGIO INTERNO DO CAIXA
	if (contador >= 60) {
		contador = 0;
		str[3]++; // minutos

		if (str[3] >= 60) {
			str[3] = 0;
			str[2]++; // horas

			if (str[2] >= 24) {
				str[2] = 0;
				str[0]++; // dias

				char dias_no_mes = dias_por_mes[str[1] - 1];

				if (str[0] > dias_no_mes) {
					str[0] = 1;
					str[1]++; // meses

					if (str[1] > 12) {
						str[1] = 1;
					}
				}
			}
		}
	}
}
