/*
 * header.h
 *
 * Created: 09/05/2025 17:20:48
 *  Author: vinic
 */ 


#ifndef HEADER_H_
#define HEADER_H_

#include <avr/io.h>

extern char variavelK;
extern char inatividade;

void delay_ms(char atraso);
void INIT_TIMER(char ms_us);

void delay_us(char atraso);
void INIT_DISPLAY(void);
void cmd_DISPLAY(unsigned char cmd);
void char_DISPLAY(unsigned char cmd);
void str_DISPLAY(char *frase);
void limpa_display();

void timer1_init(void);

void saque_func();

void comunica_saque();
void comunica_saldo();
void comunica_fechamento();
void comunica_liberacao();
void comunica_travamento();
void comunica_horario();
void comunica_entrada();
void comunica_operando();

void config_teclado(void);
char le_teclas(void);
char debounce(char d);

void INIT_SERIAL(void);
#endif /* HEADER_H_ */