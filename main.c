/* File:   main.c
 * Author: July Chassot
 *
 * Created on 15 de Setembro de 2021, 08:14
 */

#include <xc.h>
#include <proc/pic16f877a.h>
#include "lcd.c"

#define LED_INSTAVEL PORTBbits.RB1
#define LED_ESTAVEL  PORTBbits.RB0
#define BUZZER       PORTDbits.RB0
#define _XTAL_FREQ 4000000
#pragma config WDTE = ON  //habilita o uso do WDT

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

//Variaveis 
int variaveis[3];
int conversorADC();

// subrotina para tratar a interrrup��o 
int conversorADC(int canal){
	CLRWDT();
	ADCON0bits.CHS = 0b000;	//analisar porta AN0
	__delay_ms(10);
	ADCON0bits.GO = 1;		//1 inicia convers�o
	while(ADCON0bits.GO == 1){
		NOP();
	}
	CLRWDT();
	return (int)ADRESH;
}

void main()
{//Registrador de configura��es basicas
	OPTION_REGbits.nRBPU	= 0;			//Pull-up ativados
	OPTION_REGbits.INTEDG	= 0;			//Interrup��o RB0 em falling edge
	OPTION_REGbits.T0CS		= 0;			//Clock interno
	OPTION_REGbits.PSA		= 1;			//Prescaler atribuido ao WDT
	OPTION_REGbits.PS		= PRESCALER;	//Valor do prescaler
	
	//Registrador de interrup��es basicas
	INTCONbits.GIE		= 1;	//Interrup��es globais ativadas
	INTCONbits.PEIE		= 1;	//Interrup��es de perifericos ativadas
	INTCONbits.TMR0IE	= 0;	//Timer 0 desativado
	INTCONbits.INTE		= 1;	//Interrup��o RB0 ativada
	INTCONbits.TMR0IF	= 0;	//Flag de interrupa�void main()
{//Registrador de configura��es basicas
	OPTION_REGbits.nRBPU	= 0;			//Pull-up ativados
	OPTION_REGbits.INTEDG	= 0;			//Interrup��o RB0 em falling edge
	OPTION_REGbits.T0CS		= 0;			//Clock interno
	OPTION_REGbits.PSA		= 1;			//Prescaler atribuido ao WDT
	OPTION_REGbits.PS		= PRESCALER;	//Valor do prescaler
	
	//Registrador de interrup��es basicas
	INTCONbits.GIE		= 1;	//Interrup��es globais ativadas
	INTCONbits.PEIE		= 1;	//Interrup��es de perifericos ativadas
	INTCONbits.TMR0IE	= 0;	//Timer 0 desativado
	INTCONbits.INTE		= 1;	//Interrup��o RB0 ativada
	INTCONbits.TMR0IF	= 0;	//Flag de interrupa��o do Timer 0
	INTCONbits.INTF		= 0;	//Flag de interrupa��o do RB0
	
	//Registrador de interrup��es de perifericos
	PIE1bits.ADIE	= 0;	//Desativa interrup��o ao finalizar convers�o A/D
	PIE1bits.TMR1IE = 1;	//Ativa Timer 1
	
	//Registrador de flags de interrup��es de perifericos
	//PIR1bits.ADIF	= 0;	//Flag de interrupa��o do conversor A/D
	PIR1bits.TMR1IF = 0;	//Flag de interrupa��o do Timer 1
	
	//Registrador de configura��o do Timer 1
	T1CONbits.T1CKPS = TMR1_PRESCALER;	//Prescaler Timer 1
	T1CONbits.TMR1CS = 0;				//Timer 1 usa clock interno
	T1CONbits.TMR1ON = 1;				//Ativa��o Timer 1
	
	//Registradores de contagem do Timer 1
	TMR1L = TMR1L_START_TIME;	//Valor de Inicio dos bits menos significativos
	TMR1H = TMR1H_START_TIME;	//Valor de Inicio dos bits mais significativos
	
	//Registrador 0 de configura��o do conversor A/D
	ADCON0bits.ADCS = ADC_CLOCK;	//Clock de convers�o A/D
	//ADCON0bits.CHS = 0b000;		//Qual a porta que o conversor analisa
	//ADCON0bits.GO = 0;			//1 inicia convers�o, 0 esta pronta
	ADCON0bits.ADON = 1;			//Ativa conversor A/D
	
	//Registrador 1 de configura��o do conversor A/D
	ADCON1bits.ADFM  = 0;		//Usando bits mais significativos da convers�o
	ADCON1bits.ADCS2 = 0;		//Clock depende apenas de ADCON0
	ADCON1bits.PCFG  = 0b0000;	//Defini��o de quais RA s�o analogicas
	//Configura��o escolhida
	//AN7 | AN6 | AN5 | AN4 | AN3 | AN2 | AN1 | AN0
	// A  |  A  |  A  |  A  |  A  |  A  |  A  |  A
	ADRESL = 0;	//Zera o valor armazenado no conversor A/D
	ADRESH = 0;
	
	//Registradores TRIS
    TRISA = 0b11111111;       //configura como entrada
    TRISB = 0b00000000;       //configura como saida 
    TRISD = 0b00000000;       //configura como saida  

    //** inicializando saidas
    LED_ESTAVEL = 0;
    LED_INSTAVEL = 0; 
    BUZZER = 0; 

    CLRWT ();
    while (1) {
        for (i = 0 ;i < 3; i++){
            variaveis[i] = conversorADC(i)
        }
        lcd_set_cursor(1, 1);
        lcd_write_string("temperatura =");
        lcd_write_string(variaveis[2]);

        lcd_set_cursor(2, 1);
        lcd_write_string("tens�o =");
        lcd_write_string(variaveis[1]);

        if (variaveis[0] > 60){
            LED_ESTAVEL = 1;
            LED_INSTAVEL = 0;
            BUZZER = 0;
        }
        else{
            LED_ESTAVEL = 0;
            LED_INSTAVEL = 1;
            BUZZER = 1;
        }
    }
}