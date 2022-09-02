/*
 * File:   Main.c
 * Author: July Chassot
 *
 * Created on 15 de Setembro de 2021, 03:16
 */

#include <xc.h>
#include <stdio.h>
#include <proc/pic16f877a.h>
#include "Config_BITs.h"
#include "Main.h"
#include "lcd.h"

//Variaveis 
float temperatura, tensao, corrente;
int i;
int Timer;
int flag;
int ADC_temp;
int ADC_tensao;
int ADC_corrente;
char buffer[5];
#define TIMER 1

// subrotina conversorAD 
int conversorADC(int canal){
    ADCON0= (canal<<3);
    ADON = 1;
    __delay_us(10);
   
    GO=1;                                   
    while(GO_DONE==1);                      
                                         
    return((ADRESH<<8) + ADRESL);
}

// subrotina para tratar a interrrupção 
void __interrupt() Interrupcao(){
	CLRWDT();
	INTCONbits.GIE = 0;//Interrupções desativadas
	if (PIR1bits.TMR1IF){
		if(Timer == TIMER*QUANTUM){
            ADC_tensao = conversorADC(1);
            ADC_temp = conversorADC(2);
            ADC_corrente = conversorADC(0);
 
            Timer = 0;
            flag = 1;
		}
		else{
			Timer++;
		}
		TMR1L = TMR1L_START_TIME;
		TMR1H = TMR1H_START_TIME;
		PIR1bits.TMR1IF = 0;
	}
	CLRWDT();
	INTCONbits.GIE = 1;//Interrupções ativadas
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
	INTCONbits.INTE		= 0;	//Interrupção RB0 ativada
	INTCONbits.TMR0IF	= 0;	//Flag de interrupaçvoid main()
	
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
	
	ADCON0 = 0x00;
    ADCON1 = 0x80;
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
	Lcd_Init();

    char msg[20];
    char msg1[20];
    
    while (1) {
		CLRWDT();
		if (flag == 1){
			Lcd_Clear();
			Lcd_Set_Cursor(1, 1);
			Lcd_Write_String("Atualizando...");
			__delay_ms(150);
			flag = 0;
		}
        
        temperatura = (ADC_temp/2)-1;
        tensao = ADC_tensao;
        if (temperatura <= -1 && tensao <= 0){
            sprintf (msg, "Inicializando");
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String(msg);    
        }
        else {
        
        sprintf (msg1,"Temp = %.1f   ",temperatura);
        Lcd_Set_Cursor(1, 1);
		Lcd_Write_String(msg1);

        sprintf (msg,"Tensao = %.1f   ",tensao);
        Lcd_Set_Cursor(2, 1);
		Lcd_Write_String(msg);

        corrente = ADC_corrente;
		if (corrente > 250){
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
}

