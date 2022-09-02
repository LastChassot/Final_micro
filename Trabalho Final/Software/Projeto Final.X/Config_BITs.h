/* 
 * File:   Config_BITs.h
 * Author: July Chassot
 *
 * Created on 15 de Setembro de 2021, 03:16
 */

// PIC16F877A Configuration Bit Settings


// CONFIG
#pragma config FOSC = HS	// Oscillator Selection bits (RC oscillator)
#pragma config WDTE = ON	// Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON	// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON	// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF	// Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF	// Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF	// Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF		// Flash Program Memory Code Protection bit (Code protection off)
