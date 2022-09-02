/* 
 * File:   Main.h
 * Author: July Chassot
 *
 * Created on 15 de Setembro de 2021, 03:16
 */

#ifndef MAIN_H
#define	MAIN_H

#define _XTAL_FREQ			20000000	//Frequencia do clock
#define PRESCALER			0b110       //1:128 para o WDT, +- 1 segundo
#define QUANTUM				5           //Numero de flags para contagem 1 seg
#define TMR1_PRESCALER		0b11        //1:8 para o Timer 1
#define TMR1L_START_TIME	220         //Bits menos significativos do Timer 1
#define TMR1H_START_TIME	11          //Bits mais significativos do Timer 1
#define ADC_CLOCK			0b00        //Clock de conversão A/D setado para Fosc/2

//Sensores do projeto
#define LED_INSTAVEL PORTBbits.RB1
#define LED_ESTAVEL  PORTBbits.RB0
#define BUZZER       PORTDbits.RD0

//LCD
#define RS  PORTDbits.RD2
#define EN	PORTDbits.RD3
#define D4	PORTDbits.RD4
#define D5	PORTDbits.RD5
#define D6	PORTDbits.RD6
#define D7	PORTDbits.RD7

#endif	/* MAIN_H */

