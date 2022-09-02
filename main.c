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

// subrotina para tratar a interrrupção 
int conversorADC(int canal){
	CLRWDT();
	ADCON0bits.CHS = 0b000;	//analisar porta AN0
	__delay_ms(10);
	ADCON0bits.GO = 1;		//1 inicia conversão
	while(ADCON0bits.GO == 1){
		NOP();
	}
	CLRWDT();
	return (int)ADRESH;
}

void main()
{//Registrador de configurações basicas
	OPTION_REGbits.nRBPU	= 0;			//Pull-up ativados
	OPTION_REGbits.INTEDG	= 0;			//Interrupção RB0 em falling edge
	OPTION_REGbits.T0CS		= 0;			//Clock interno
	OPTION_REGbits.PSA		= 1;			//Prescaler atribuido ao WDT
	OPTION_REGbits.PS		= PRESCALER;	//Valor do prescaler
	
	//Registrador de interrupções basicas
	INTCONbits.GIE		= 1;	//Interrupções globais ativadas
	INTCONbits.PEIE		= 1;	//Interrupções de perifericos ativadas
	INTCONbits.TMR0IE	= 0;	//Timer 0 desativado
	INTCONbits.INTE		= 1;	//Interrupção RB0 ativada
	INTCONbits.TMR0IF	= 0;	//Flag de interrupaçvoid main()
{//Registrador de configurações basicas
	OPTION_REGbits.nRBPU	= 0;			//Pull-up ativados
	OPTION_REGbits.INTEDG	= 0;			//Interrupção RB0 em falling edge
	OPTION_REGbits.T0CS		= 0;			//Clock interno
	OPTION_REGbits.PSA		= 1;			//Prescaler atribuido ao WDT
	OPTION_REGbits.PS		= PRESCALER;	//Valor do prescaler
	
	//Registrador de interrupções basicas
	INTCONbits.GIE		= 1;	//Interrupções globais ativadas
	INTCONbits.PEIE		= 1;	//Interrupções de perifericos ativadas
	INTCONbits.TMR0IE	= 0;	//Timer 0 desativado
	INTCONbits.INTE		= 1;	//Interrupção RB0 ativada
	INTCONbits.TMR0IF	= 0;	//Flag de interrupação do Timer 0
	INTCONbits.INTF		= 0;	//Flag de interrupação do RB0
	
	//Registrador de interrupções de perifericos
	PIE1bits.ADIE	= 0;	//Desativa interrupção ao finalizar conversão A/D
	PIE1bits.TMR1IE = 1;	//Ativa Timer 1
	
	//Registrador de flags de interrupções de perifericos
	//PIR1bits.ADIF	= 0;	//Flag de interrupação do conversor A/D
	PIR1bits.TMR1IF = 0;	//Flag de interrupação do Timer 1
	
	//Registrador de configuração do Timer 1
	T1CONbits.T1CKPS = TMR1_PRESCALER;	//Prescaler Timer 1
	T1CONbits.TMR1CS = 0;				//Timer 1 usa clock interno
	T1CONbits.TMR1ON = 1;				//Ativação Timer 1
	
	//Registradores de contagem do Timer 1
	TMR1L = TMR1L_START_TIME;	//Valor de Inicio dos bits menos significativos
	TMR1H = TMR1H_START_TIME;	//Valor de Inicio dos bits mais significativos
	
	//Registrador 0 de configuração do conversor A/D
	ADCON0bits.ADCS = ADC_CLOCK;	//Clock de conversão A/D
	//ADCON0bits.CHS = 0b000;		//Qual a porta que o conversor analisa
	//ADCON0bits.GO = 0;			//1 inicia conversão, 0 esta pronta
	ADCON0bits.ADON = 1;			//Ativa conversor A/D
	
	//Registrador 1 de configuração do conversor A/D
	ADCON1bits.ADFM  = 0;		//Usando bits mais significativos da conversão
	ADCON1bits.ADCS2 = 0;		//Clock depende apenas de ADCON0
	ADCON1bits.PCFG  = 0b0000;	//Definição de quais RA são analogicas
	//Configuração escolhida
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
        lcd_write_string("tensão =");
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